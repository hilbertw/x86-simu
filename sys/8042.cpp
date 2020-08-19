#include <stdlib.h>
#include <setjmp.h>     

#include "addressmap.h"
#include "sys.h"
#include "log.h"
#include "irq-wire.h"
extern jmp_buf   sysreset_jumper; 
void __cdecl vt_set_led(char led);
void vt_speaker(bool on );
void reset(int t);
void a20_sync();

#define ASSERT_IRQ1() assert_irq(1)
#define DEASSERT_IRQ1() 
//
// 8042 drives kbd and an aux dev;
//
unsigned char  aux_port;


//|7|6|5|4|3-0|  8042 Input Port
// | | | | `---- undefined
// | | | `----- 1=enable 2nd 256K of motherboard RAM, 0=disable
// | | `------ 1=manufacturing jumper not installed, 0=installed
// | `------- 1=primary display is MDA, 0=primary display is CGA
// `-------- 1=keyboard not inhibited, 0=keyboard inhibited

unsigned char  input_port_8042=0x90;
unsigned char  output_port_8042=1;
//|7|6|5|4|3|2|1|0|  8042 Output Port
// | | | | | | | `---- system reset line
// | | | | | | `----- gate A20
// | | | | `-------- undefined
// | | | `--------- output buffer full
// | | `---------- input buffer empty
// | `----------- keyboard clock (output)
// `------------ keyboard data (output)
struct status_8042_def
{
	unsigned char oport_60_has_data:1;
	unsigned char iport_has_data:1;
	unsigned char reset:1;
	unsigned char input_command:1;
	unsigned char enable:1;
	unsigned char tx_timeout:1;
	unsigned char rcv_timeout:1;
	unsigned char even_parity:1;
} status_8042;

//8042 Status Register (port 64h read)

//|7|6|5|4|3|2|1|0|  8042 Status Register
// | | | | | | | `---- output register (60h) has data for system
// | | | | | | `----- input register (60h/64h) has data for 8042
// | | | | | `------ system flag (set to 0 after power on reset)
// | | | | `------- data in input register is command (1) or data (0)
// | | | `-------- 1=keyboard enabled, 0=keyboard disabled (via switch)
// | | `--------- 1=transmit timeout (data transmit not complete)
// | `---------- 1=receive timeout (data transmit not complete)
// `----------- 1=even parity rec'd, 0=odd parity rec'd (should be odd)


struct comand_def
{
	unsigned char en_obf_int:1;
	unsigned char zero:1;
	unsigned char status_sys:1;
	unsigned char disallow_inh:1;
	unsigned char disable_kbd:1;
	unsigned char disable_aux:1;
	unsigned char at_scancode:1;
	unsigned char reserved:1;
}cmd_register;
//    Format:
//	     |7|6|5|4|3|2|1|0|	8042 Command Byte
//	      | | | | | | | `---- 1=enable output register full interrupt
//	      | | | | | | `----- should be 0
//	      | | | | | `------ 1=set status register system, 0=clear
//	      | | | | `------- 1=override keyboard inhibit, 0=allow inhibit
//	      | | | `-------- disable keyboard I/O by driving clock line low
//	      | | `--------- disable auxiliary device, drives clock line low
//	      | `---------- IBM scancode translation 0=AT, 1=PC/XT
//	      `----------- reserved, should be 0

#define STATUS_PORT  0x64
#define KBD_OUT_BUF  0x60
#define KBD_INPT_BUF  0x60
#define KBD_CMD_BUF  0x64
#define KBD_DATA_BUF  0x60
#define PORT_A  0x60
#define PORT_B  0x61
#define OUT_BUF_FULL  0x1
#define INPT_BUF_FULL  0x2
#define SYS_FLAG  0x4
#define CMD_DATA  0x8
#define KEYBD_INH 0x10
#define TRANS_TMOUT 0x20
#define RCV_TMOUT 0x40
#define KB_PARITY_EVEN 0x80
#define INH_KEYBOARD 0x10
#define KBD_ENA  0xAE
#define KBD_DIS  0xAD
// Keyboard Commands
#define KB_MENU  0xF1
#define KB_ENABLE  0xF4
#define KB_MAKEBREAK  0xF7
#define KB_ECHO  0xFE
#define KB_RESET  0xFF
#define KB_LED_CMD  0xED
// Keyboard responses
#define KB_OK  0xAA
#define KB_ACK  0xFA
#define KB_OVERRUN  0xFF
#define KB_RESEND  0xFE
#define KB_BREAK  0xF0
#define KB_FA  0x10
#define KB_FE  0x20
#define KB_PR_LED  0x40

bool en_security;
//
// system interface
//
unsigned char wr_ptr,rd_ptr;
unsigned char ibuf[0x10];

__inline void clear_buf()
{
	wr_ptr=rd_ptr=0;
	status_8042.oport_60_has_data=0;
}
//
//  a 8042/kbd cmd may have data for system.
//
__inline void place_to_ibuf(unsigned char data)
{
	//
	// buffer is not full
	//
	if(rd_ptr==0) {rd_ptr=sizeof(ibuf)-1;}
	else
	rd_ptr--;
	ibuf[rd_ptr]=data;
	//ibuf[wr_ptr++]=data;
	status_8042.oport_60_has_data=1;
	if(cmd_register.en_obf_int)
		ASSERT_IRQ1();  // code for 8259 doesn't clears irq after it is served
}

void key_hit( char scan_code)
{
	if(cmd_register.disable_kbd==1)
		return;
	//
	// buf is too full 
	//
	if((wr_ptr==rd_ptr-1&&rd_ptr!=00)
		||(wr_ptr==sizeof(ibuf)-1&&rd_ptr==0))
		
	{
		//buffer full, assert irq
		status_8042.oport_60_has_data=1;
		//
		// gen int
		//
		if(cmd_register.en_obf_int)
		ASSERT_IRQ1();
		return;
	}
	else
	{
		ibuf[wr_ptr++]=scan_code;
		if(wr_ptr> sizeof(ibuf)-1) {wr_ptr-=sizeof(ibuf);}


		status_8042.oport_60_has_data=1;
		//
		// gen int
		//
		if(cmd_register.en_obf_int)
			ASSERT_IRQ1();
	}
}

unsigned char read_port_60h()
{
	char c=ibuf[rd_ptr];

	// not empty
	if(rd_ptr!=wr_ptr) 
	{
		rd_ptr++;
		if(rd_ptr> sizeof(ibuf)-1) { rd_ptr-=sizeof(ibuf);}

		status_8042.oport_60_has_data=rd_ptr!=wr_ptr;

		if(!status_8042.oport_60_has_data)
		DEASSERT_IRQ1();
		else if(cmd_register.en_obf_int)
		ASSERT_IRQ1();  // code for 8259 doesn't clears irq after it is served
	}

	return c;
}
__inline unsigned char read_port_64h()
{
	status_8042.oport_60_has_data=rd_ptr!=wr_ptr;
	unsigned char c=*(unsigned char*)&status_8042;

	return c;
}

static unsigned char last_command;

void do_8042_command( unsigned char  command)
{
	last_command=command;
	switch(command)
	{
	case 0x20:  //   Read 8042 Command Byte: current 8042 command byte is placed in port 60h.  
		place_to_ibuf(*(unsigned char*)&cmd_register);
		break;
	case 0xA6:  //Enable Security: works only if a password is already loaded
		en_security=true;
		break;
	case 0xA7:  //Disable Auxiliary Interface: sets Bit 5 of command register
	            //stopping auxiliary I/O by driving the clock line low
		cmd_register.disable_aux=1;
		break;
	case 0xA8:  //Enable Auxiliary Interface: clears Bit 5 of command register
		cmd_register.disable_aux=0;

		break;
	case 0xA9:  //Auxiliary Interface Test: clock and data lines are tested;
				//     results placed at port 60h are listed below:
				//00  no error
				//01  keyboard clock line is stuck low
				//02  keyboard clock line is stuck high
				//03  keyboard data line is stuck low
				//04  keyboard data line is stuck high
		place_to_ibuf(0);
		break;
	case 0xAA:  //Self Test: diagnostic result placed at port 60h, 55h=OK
		place_to_ibuf(0x55);
		break;
	case 0xAB:  //Keyboard Interface Test:	clock and data lines are tested;
				//results placed at port 60h are listed above with command A9
		place_to_ibuf(0);
		break;
	case 0xAC:  //Diagnostic Dump: sends 16 bytes of 8042's RAM, current input
				//port state, current output port state and 8042 program status
				//word to port 60h in scan-code format.
		break;
	case 0xAD:  //Disable Keyboard Interface: sets Bit 4 of command register
				//stopping keyboard I/O by driving the clock line low
				//enabling keyboard interface.
		cmd_register.disable_kbd=1;
		status_8042.oport_60_has_data=0;
		//rd_ptr=wr_ptr;
		//place_to_ibuf(0);
		break;
	case 0xAE:  //Enable Keyboard Interface: clears Bit 4 of command register
				//enabling keyboard interface.
 		cmd_register.disable_kbd=0;
		status_8042.oport_60_has_data=rd_ptr!=wr_ptr;
		//place_to_ibuf(0);
		break;
	case 0xC0:  //Read Input Port: data is read from its input port (which is
				//inaccessible to the data bus) and written to output register
				//at port 60h;  output register should be empty before call.
		place_to_ibuf(input_port_8042);
		break;

	case 0xC1:  //Poll Input Port Low Bits: Bits 0-3 of port 1 placed instatus Bits 4-7
		break;

	case 0xC2:  //Poll Input Port High Bits: Bits 4-7 of port 1 placed in status Bits 4-7
		break;

	case 0xD0:  //Read Output Port: data is read from 8042 output port (which is
			    //inaccessible to the data bus) and placed in output register;
			    //the output register should be empty.  (see command D1 below)
	    place_to_ibuf(output_port_8042);
		break;
	case 0xD4:  //Write Auxiliary Device: on PS/2 systems the next data byte
			    //written to input register a port at 60h is sent to the
			    //auxiliary device
		break;
	case 0xE0:  //Read Test Inputs: 8042 reads its T0 and T1 inputs; data is
				//     placed in output register;  Bit 0 is T0, Bit 1 is T1:
				//|1|0|  Test Input Port Bits
				// | `---- keyboard clock
				// `----- keyboard data
		place_to_ibuf(0); 
		break;

	default:
		if((command &0xf0)==0xF0)
		   //Pulse Output Port: Bits 0-3 of the 8042 output port can be
		   //   pulsed low for 6s;  Bits 0-3 of command indicate which
		   //   Bits should be pulsed; 0=pulse, 1=don't pulse; pulsing
		   //   Bit 0 results in CPU reset since it is connected to system
		   //   reset line.
		   if((command&1)==0) 
		   {
			   status_8042.reset=1;  //BAT passed
			   reset(0);
		   }

		break; // do nothing
		   }
}
//
// system to keyboad commands
//
unsigned char last_wr_port60h;
//char indicator;
char scancode_set;
char delay;
char rate;

void do_kbd_command(unsigned char byte)
{
	last_wr_port60h=byte;
	switch(byte)
	{
//Commands System Issues to Keyboard (via 8042 port 60h)
//
//	ED  Set/Reset Mode Indicators, keyboard responds with ACK then
//	    waits for a following option byte.	When the option byte is
//	    received the keyboard again ACK's and then sets the LED's
//	    accordingly.  Scanning is resumed if scanning was enabled.
//	    If another command is received instead of the option byte
//	    (high bit set on) this command is terminated.  Hardware
//	    defaults to these indicators turned off.
//
//	    |7-3|2|1|0| Keyboard Status Indicator Option Byte
//	      |  | | `--- Scroll-Lock indicator  (0=off, 1=on)
//	      |  | `---- Num-Lock indicator  (0=off, 1=on)
//	      |  `----- Caps-Lock indicator  (0=off, 1=on)
//	      `------- reserved (must be zero)
//
		case 0xed:
			break; 
//  
//	EE  Diagnostic Echo, keyboard echoes the EE byte back to the system
//	    without an acknowledgement.
		case 0xee:
			place_to_ibuf(0xee); //  
			return;
//	F0  PS/2 Select/Read Alternate Scan Code Sets, instructs keyboard
//	    to use one of the three make/break scan code sets.	 Keyboard
//	    responds by clearing the output buffer/typematic key and then
//	    transmits an ACK.  The system must follow up by sending an
//	    option byte which will again be ACK'ed by the keyboard:
//
//	      00  return byte indicating scan code set in use
//	      01  select scan code set 1  (used on PC & XT)
//	      02  select scan code set 2
//	      03  select scan code set 3
//
		case 0xf0:
			break; // 

//	F2  PS/2 Read Keyboard ID, keyboard responds with an ACK and a two
//	    byte keyboard ID of 83AB.
		case 0xf2:
			place_to_ibuf(0xAB); //
			place_to_ibuf(0x83); //  
  			//place_to_ibuf(KB_ACK);
			return;
//	F3  Set Typematic Rate/Delay, keyboard responds with ACK and waits
//	    for rate/delay byte.   Upon receipt of the rate/delay byte the
//	    keyboard responds with an ACK, then sets the new typematic
//	    values and scanning continues if scanning was enabled.
//	    
//	    |7|6|5|4|3|2|1|0|  Typematic Rate/Delay Option Byte
//	     | | | |-+-+-+-+---- typematic rate indicator (see INT 16,3)
//	     | | | | | `------- A in period formula (see below)
//	     | | | `---------- B is period formula (see below)
//	     | `------------- typematic delay
//	     `-------------- always zero
//
//	    delay = (rate+1) * 250   (in milliseconds)
//	    rate = (8+A) * (2**B) * 4.17  (in seconds, ?20%)
//
//	    Defaults to 10.9 characters per second and a 500ms delay.  If a
//	    command byte (byte with high bit set) is received instead of an
//	    option byte this command is cancelled.
		case 0xf3:
			break; 
// 
//	F4  Enable Keyboard, cause the keyboard to clear its output buffer
//	    and last typematic key and then respond with an ACK.  The
//	    keyboard then begins scanning.
		case 0xf4:
			//rd_ptr=wr_ptr;
			//goto send_ACK;
//	F5  Default w/Disable, resets keyboard to power-on condition by
//	    clearing the output buffer, resetting typematic rate/delay,
//	    resetting last typematic key and setting default key types.
//	    The keyboard responds with an ACK and waits for the next
//	    instruction.
		case 0xf5:
			//rd_ptr=wr_ptr;
			//goto send_ACK;
//	F6  Set Default, resets to power-on condition by clearing the output
//	    buffer, resetting typematic rate/delay and last typematic key
//	    and sets default key types.  The keyboard responds with an ACK
//	    and continues scanning.
		case 0xf6:
			//rd_ptr=wr_ptr;
			//goto send_ACK;
//	F7  PS/2 Set All Keys to Typematic, keyboard responds by sending an
//	    ACK, clearing its output buffer and setting the key type to
//	    Typematic.	 Scanning continues if scanning was enabled.  This
//	    command may be sent while using any Scan Code Set but only has
//	    effect when Scan Code Set 3 is in use.
		case 0xf7:
			 clear_buf();
			break;
//	F8  PS/2 Set All Keys to Make/Break, keyboard responds by sending an
//	    ACK, clearing its output buffer and setting the key type to
//	    Make/Break.  Scanning continues if scanning was enabled.  This
//	    command may be sent while using any Scan Code Set but only has
//	    effect when Scan Code Set 3 is in use.
//	F9  PS/2 Set All Keys to Make, keyboard responds by sending an ACK,
//	    clearing its output buffer and setting the key type to Make.
//	    Scanning continues if scanning was enabled.  This command may
//	    be sent while using any Scan Code Set but only has effect when
//	    Scan Code Set 3 is in use.
//	FA  PS/2 Set All Keys to Typematic Make/Break, keyboard responds by
//	    sending an ACK, clearing its output buffer and setting the key
//	    type to Typematic Make/Break.  Scanning continues if scanning
//	    was enabled.  This command may be sent while using any Scan Code
//	    Set but only has effect when Scan Code Set 3 is in use.
//	FB  PS/2 Set Key Type to Typematic, keyboard responds by sending an
//	    ACK, clearing its output buffer and then waiting for the key ID
//	    (make code from Scan Code Set 3).  The specified key type is then
//	    set to typematic.	This command may be sent while using any
//	    Scan Code Set but only has effect when Scan Code Set 3 is in use.
//	FC  PS/2 Set Key Type to Make/Break, keyboard responds by sending an
//	    ACK, clearing its output buffer and then waiting for the key ID
//	    (make code from Scan Code Set 3).  The specified key type is then
//	    set to Make/Break.	 This command may be sent while using any Scan
//	    Code Set but only has effect when Scan Code Set 3 is in use.
//	FD  PS/2 Set Key Type to Make, keyboard responds by sending an ACK,
//	    clearing its output buffer and then waiting for the key ID (make
//	    code from Scan Code Set 3).  The specified key type is then set
//	    to Make.  This command may be sent while using any Scan Code Set
//	    but only has effect when Scan Code Set 3 is in use.
//	FE  Resend, should be sent when a transmission error is detected
//	    from the keyboard
//	FF  Reset, Keyboard sends ACK and waits for system to receive it
//	    then begins a program reset and Basic Assurance Test (BAT).
//	    Keyboard returns a one byte completion code then sets default
//	    Scan Code Set 2.
			break;
	case 0xff:
		place_to_ibuf(0xaa);
		status_8042.reset=1;  //BAT passed
		return;

	default:
		break;
	}
	place_to_ibuf(KB_ACK);

	return;
}



void do_option(unsigned char byte)
{

	switch(last_wr_port60h)
	{
//Commands System Issues to Keyboard (via 8042 port 60h)
//
//	ED  Set/Reset Mode Indicators, keyboard responds with ACK then
//	    waits for a following option byte.	When the option byte is
//	    received the keyboard again ACK's and then sets the LED's
//	    accordingly.  Scanning is resumed if scanning was enabled.
//	    If another command is received instead of the option byte
//	    (high bit set on) this command is terminated.  Hardware
//	    defaults to these indicators turned off.
//
//	    |7-3|2|1|0| Keyboard Status Indicator Option Byte
//	      |  | | `--- Scroll-Lock indicator  (0=off, 1=on)
//	      |  | `---- Num-Lock indicator  (0=off, 1=on)
//	      |  `----- Caps-Lock indicator  (0=off, 1=on)
//	      `------- reserved (must be zero)
//
		case 0xed:
			//indicator=byte;
			vt_set_led(byte);
			break; 

//	F0  PS/2 Select/Read Alternate Scan Code Sets, instructs keyboard
//	    to use one of the three make/break scan code sets.	 Keyboard
//	    responds by clearing the output buffer/typematic key and then
//	    transmits an ACK.  The system must follow up by sending an
//	    option byte which will again be ACK'ed by the keyboard:
//
//	      00  return byte indicating scan code set in use
//	      01  select scan code set 1  (used on PC & XT)
//	      02  select scan code set 2
//	      03  select scan code set 3
//
		case 0xf0:
			scancode_set=byte;
			if(byte==0)
			{
				place_to_ibuf(2); //default scan code set
				return;

			}
			break; 
// 
//	F2  PS/2 Read Keyboard ID, keyboard responds with an ACK and a two
//	    byte keyboard ID of 83AB.
//	F3  Set Typematic Rate/Delay, keyboard responds with ACK and waits
//	    for rate/delay byte.   Upon receipt of the rate/delay byte the
//	    keyboard responds with an ACK, then sets the new typematic
//	    values and scanning continues if scanning was enabled.
//	    
//	    |7|6|5|4|3|2|1|0|  Typematic Rate/Delay Option Byte
//	     | | | |-+-+-+-+---- typematic rate indicator (see INT 16,3)
//	     | | | | | `------- A in period formula (see below)
//	     | | | `---------- B is period formula (see below)
//	     | `------------- typematic delay
//	     `-------------- always zero
//
//	    delay = (rate+1) * 250   (in milliseconds)
//	    rate = (8+A) * (2**B) * 4.17  (in seconds, ?20%)
//
//	    Defaults to 10.9 characters per second and a 500ms delay.  If a
//	    command byte (byte with high bit set) is received instead of an
//	    option byte this command is cancelled.
		case 0xf3:
			delay=(rate+1)*250;
			break; 
// 
//	F4  Enable Keyboard, cause the keyboard to clear its output buffer
//	    and last typematic key and then respond with an ACK.  The
//	    keyboard then begins scanning.
//	F5  Default w/Disable, resets keyboard to power-on condition by
//	    clearing the output buffer, resetting typematic rate/delay,
//	    resetting last typematic key and setting default key types.
//	    The keyboard responds with an ACK and waits for the next
//	    instruction.
//	F6  Set Default, resets to power-on condition by clearing the output
//	    buffer, resetting typematic rate/delay and last typematic key
//	    and sets default key types.  The keyboard responds with an ACK
//	    and continues scanning.
//	F7  PS/2 Set All Keys to Typematic, keyboard responds by sending an
//	    ACK, clearing its output buffer and setting the key type to
//	    Typematic.	 Scanning continues if scanning was enabled.  This
//	    command may be sent while using any Scan Code Set but only has
//	    effect when Scan Code Set 3 is in use.
//	F8  PS/2 Set All Keys to Make/Break, keyboard responds by sending an
//	    ACK, clearing its output buffer and setting the key type to
//	    Make/Break.  Scanning continues if scanning was enabled.  This
//	    command may be sent while using any Scan Code Set but only has
//	    effect when Scan Code Set 3 is in use.
//	F9  PS/2 Set All Keys to Make, keyboard responds by sending an ACK,
//	    clearing its output buffer and setting the key type to Make.
//	    Scanning continues if scanning was enabled.  This command may
//	    be sent while using any Scan Code Set but only has effect when
//	    Scan Code Set 3 is in use.
//	FA  PS/2 Set All Keys to Typematic Make/Break, keyboard responds by
//	    sending an ACK, clearing its output buffer and setting the key
//	    type to Typematic Make/Break.  Scanning continues if scanning
//	    was enabled.  This command may be sent while using any Scan Code
//	    Set but only has effect when Scan Code Set 3 is in use.
//	FB  PS/2 Set Key Type to Typematic, keyboard responds by sending an
//	    ACK, clearing its output buffer and then waiting for the key ID
//	    (make code from Scan Code Set 3).  The specified key type is then
//	    set to typematic.	This command may be sent while using any
//	    Scan Code Set but only has effect when Scan Code Set 3 is in use.
//	FC  PS/2 Set Key Type to Make/Break, keyboard responds by sending an
//	    ACK, clearing its output buffer and then waiting for the key ID
//	    (make code from Scan Code Set 3).  The specified key type is then
//	    set to Make/Break.	 This command may be sent while using any Scan
//	    Code Set but only has effect when Scan Code Set 3 is in use.
//	FD  PS/2 Set Key Type to Make, keyboard responds by sending an ACK,
//	    clearing its output buffer and then waiting for the key ID (make
//	    code from Scan Code Set 3).  The specified key type is then set
//	    to Make.  This command may be sent while using any Scan Code Set
//	    but only has effect when Scan Code Set 3 is in use.
//	FE  Resend, should be sent when a transmission error is detected
//	    from the keyboard
//	FF  Reset, Keyboard sends ACK and waits for system to receive it
//	    then begins a program reset and Basic Assurance Test (BAT).
//	    Keyboard returns a one byte completion code then sets default
//	    Scan Code Set 2.

	default:
		break;
}

place_to_ibuf(KB_ACK);

#if LOG_8042
	log0("kbd cmd:%x %x\n",last_wr_port60h,byte);
#endif

return;

}


void write_port_60h( unsigned char  data)
{

	switch(last_command)
	{
	case 0x60:  //   Write 8042 Command Byte: next data byte written to port 60h is placed in 8042 command register.
		place_to_ibuf(*(unsigned char*)&cmd_register);
		*(unsigned char*)&cmd_register=data;	
		break;
	case 0xD1:  //Write 8042 Output Register: on PS/2 systems the next data
			    //byte written to port 60h input register is writeen to controller output port

		output_port_8042=data;

#if  LOG_8042_OPORT
		log("8042 output port:%x\n",data);

#endif
		if((data&1)==0) reset(0);


		a20_sync();
		break;
	case 0xD2:  //Write Keyboard Output Register: on PS/2 systems the next data
			    //byte written to port 60h input register is written to port 60h
			    //output register as if initiated by a device; invokes interrupt if enabled
	case 0xD3:  //Write Auxiliary Output Register: on PS/2 systems the next data
				//byte written to port 60h input register is written to port 60h
				//output register as if initiated by a device; invokes interrupt if enabled
		place_to_ibuf(data);
		//
		// assert irq1
		//
		if(cmd_register.en_obf_int) ASSERT_IRQ1();
		break;
	case 0xD4:  //Write Auxiliary Device: on PS/2 systems the next data byte
			    //written to input register  port at 60h is sent to the
			    //auxiliary device
		if(cmd_register.disable_aux==0)
		aux_port=data;
		place_to_ibuf(0xfe);
		break;
	case 0xA6:  //Enable Security: works only if a password is already loaded
		if(en_security) {} 
		if(0==data) en_security=0;
		break;
	default:
		if(data&0x80) do_kbd_command(data);// if option byte is expected , 
		                                   // but a command byte is received: command cancells
		else do_option(data);

	   }
	last_command=0; //
}
//
// reset when write to 8253 ch 2.
//
char port_61h;

void io_8042(
	  void *lpContext,
	  ADDR_RANGE_CMD op, 
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,                    // optional 
	  unsigned int nBytes )            // in or out buffer
 {	

	 switch(op)
	 {
	 case ADDR_RANGE_READ:  
   	     *pszBuff=(dwAddress==0x60)?read_port_60h():
				(dwAddress==0x64)?read_port_64h():
				(dwAddress==0x61)?(port_61h^=0x10):0xff;
		 break;
	 case ADDR_RANGE_WRITE: if(dwAddress==0x60)write_port_60h(*pszBuff);
							else if (dwAddress==0x64) do_8042_command(*pszBuff);
							else if (dwAddress==0x61) 
							{
								port_61h=*pszBuff&~0x30;
								vt_speaker((port_61h&2)==2); 
							}

		 break;
	 default:
		 return;
	 }	
#if  LOG_8042
	if(dwAddress!=0x61)
	 log4(
		 "8042",
		 op==ADDR_RANGE_READ?'r':'w',
		 dwAddress,	
 		nBytes==1? (unsigned char)pszBuff[0]:
		nBytes==2? *(unsigned short *)pszBuff:
		nBytes==4? *(unsigned long *)pszBuff:0);
#endif


 }

void reset_8042()
{
	*(char *)&cmd_register=
	*(char *)&status_8042=
	wr_ptr=
	rd_ptr=0;


}

void pre_warm_boot_8042()
{
   status_8042.reset=1;  //BAT passed
}