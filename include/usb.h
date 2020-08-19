#ifndef __usb_h__
#define __usb_h__
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;


//00h CAPLENGTH Capabilities Registers Length 20h RO
//02每03h HCIVERSION Host Controller Interface Version Number 0100h RO
//04每07h HCSPARAMS Host Controller Structural Parameters 00104208h R/W (special),RO
//08每0Bh HCCPARAMS Host Controller Capability Parameters 00006871h RO

typedef struct _def_ehci
{
	struct S
	{
	unsigned short  cap_length;
	unsigned short  hci_version;
	unsigned long  hcs_param;
	unsigned long  hcc_param; 
	}s;
	unsigned long dd[1];
	unsigned char db[0x400];
}EHCI;


typedef union _def_ehci_reg
{
	struct S
	{
	unsigned long usb2_cmd;//00每03h USB2.0_CMD USB 2.0 Command 00080000h R/W, RO
	unsigned long usb2_sts;//04每07h USB2.0_STS USB 2.0 Status 00001000h R/WC, RO
	unsigned long usb2_intr;//08每0Bh USB2.0_INTR USB 2.0 Interrupt Enable 00000000h R/W
	unsigned long usb2_frindex;//0C每0Fh FRINDEX USB 2.0 Frame Index 00000000h R/W,
	unsigned long ctrld_seg;//10每13h CTRLDSSEGMENTControl Data Structure Segment 00000000h R/W, RO
	unsigned long perodical_list_base;//14每17h PERODICLISTBASE Period Frame List Base Address 00000000h R/W
	unsigned long async_list_addr;//18每1Bh ASYNCLISTADDR Current Asynchronous List Address 00000000h R/W
	unsigned long reserved[9];//1C每3Fh 〞 Reserved 0h RO
	unsigned long config_flag;//40每43h CONFIGGLAG Configure Flag 00000000h Suspend R/W
	
	unsigned long portsc[8];
	//44每47h PORT0SC Port 0 Status and Control 00003000h Suspend R/W,R/WC, RO
	//48每4Bh PORT1SC Port 1 Status and Control 00003000h Suspend R/W,R/WC, RO
	//4C每4Fh PORT2SC Port 2 Status and Control 00003000h Suspend R/W,R/WC, RO
	//50每53h PORT3SC Port 3 Status and Control 00003000h Suspend R/W,R/WC, RO
	//54每57h PORT4SC Port 4 Status and Control 00003000h Suspend R/W,R/WC, RO
	//58每5Bh PORT5SC Port 5 Status and Control 00003000h Suspend R/W,R/WC, RO
	//5C每5Fh PORT6SC Port 6 Status and Control 00003000h Suspend R/W,R/WC, RO
	//60每63h PORT7SC Port 7 Status and Control 00003000h Suspend R/W,R/WC, RO
	unsigned long reserved1[7];//64每7Fh 〞 Reserved Undefined RO
	unsigned long dbg_port;//80每93h 〞 Debug Port Registers Undefined
	unsigned long reserved2[1];//94每3FFh 〞 Reserved Undefined RO
	}s;
	unsigned long dd[1];
	unsigned long de[1];
	unsigned char db[0x400];
}EHCI_REG;


void usb_read( unsigned long address,unsigned char * data,unsigned long dwBytes,unsigned long no);
void usb_write(unsigned long address,unsigned char * data,unsigned long dwBytes,unsigned long no);


typedef struct usb_cmd_def
{
	unsigned short rs:1;//0 Run/Stop (RS) 〞 R/W. When set to 1, the ICH5 proceeds with execution of the schedule. The ICH5
		//continues execution as long as this bit is set. When this bit is cleared, the ICH5 completes the
		//current transaction on the USB and then halts. The HC Halted bit in the status register indicates
		//when the host controller has finished the transaction and has entered the stopped state. The host
		//controller clears this bit when the following fatal errors occur: consistency check failure, PCI Bus
		//errors.
		//0 = Stop
		//1 = Run
		//NOTE: This bit should only be cleared if there are no active Transaction Descriptors in the
		//executable schedule or software will reset the host controller prior to setting this bit again.
	unsigned short hreset:1;//1 Host Controller Reset (HCRESET) 〞 R/W. The effects of HCRESET on Hub registers are slightly
		//different from Chip Hardware Reset and Global USB Reset. The HCRESET affects bits [8,3:0] of the
		//Port Status and Control Register (PORTSC) of each port. HCRESET resets the state machines of
		//the host controller including the Connect/Disconnect state machine (one for each port). When the
		//Connect/Disconnect state machine is reset, the output that signals connect/disconnect are negated
		//to 0, effectively signaling a disconnect, even if a device is attached to the port. This virtual
		//disconnect causes the port to be disabled. This disconnect and disabling of the port causes bit 1
		//(connect status change) and bit 3 (port enable/disable change) of the PORTSC to get set. The
		//disconnect also causes bit 8 of PORTSC to reset. About 64 bit times after HCRESET goes to 0, the
		//connect and low-speed detect will take place, and bits 0 and 8 of the PORTSC will change
		//accordingly.
		//0 = Reset by the host controller when the reset process is complete.
		//1 = Reset. When this bit is set, the host controller module resets its internal timers, counters, state
		//machines, etc. to their initial value. Any transaction currently in progress on USB is immediately
		//terminated.
	unsigned short greset:1;//2 Global Reset (GRESET) 〞 R/W.
		//0 = This bit is reset by the software after a minimum of 10 ms has elapsed as specified in Chapter 7
		//of the Universal Serial Bus Revision 2.0 Specification.
		//1 = Global Reset. The host controller sends the global reset signal on the USB and then resets all
		//its logic, including the internal hub registers. The hub registers are reset to their power on state.
		//Chip Hardware Reset has the same effect as Global Reset (bit 2), except that the host
		//controller does not send the Global Reset on USB.


	unsigned short egrsm:1;//3    Enter Global Suspend Mode (EGSM) ?R/W.
		//0 = Software resets this bit to 0 to come out of Global Suspend mode. Software writes this bit to 0 at
		//the same time that Force Global Resume (bit 4) is written to 0 or after writing bit 4 to 0.
		//1 = Host controller enters the Global Suspend mode. No USB transactions occur during this time.
		//The Host controller is able to receive resume signals from USB and interrupt the system.
		//Software must ensure that the Run/Stop bit (bit 0) is cleared prior to setting this bit.

	unsigned short fgr:1;//4    Force Global Resume (FGR) ?R/W.
		//0 = Software resets this bit to 0 after 20 ms has elapsed to stop sending the Global Resume signal.
		//At that time all USB devices should be ready for bus activity. The 1 to 0 transition causes the
		//port to send a low speed EOP signal. This bit will remain a 1 until the EOP has completed.
		//1 = Host controller sends the Global Resume signal on the USB, and sets this bit to 1 when a
		//resume event (connect, disconnect, or K-state) is detected while in global suspend mode.

	unsigned short sdebug:1;//5     Software Debug (SWDBG) ?R/W. The SWDBG bit must only be manipulated when the controller
		//is in the stopped state. This can be determined by checking the HCHalted bit in the USBSTS
		//register.
		//0 = Normal Mode.
		//1 = Debug mode. In SW Debug mode, the host controller clears the Run/Stop bit after the
		//completion of each USB transaction. The next transaction is executed when software sets the
		//Run/Stop bit back to 1.
	unsigned short cf:1;//6     Configure Flag (CF) ?R/W. This bit has no effect on the hardware. It is provided only as a
		//semaphore service for software.
		//0 = Indicates that software has not completed host controller configuration.
		//1 = HCD software sets this bit as the last action in its process of configuring the host controller.


	unsigned short max_p:1;//7    Max Packet (MAXP) ?R/W. This bit selects the maximum packet size that can be used for full
		//speed bandwidth reclamation at the end of a frame. This value is used by the host controller to
		//determine whether it should initiate another transaction based on the time remaining in the SOF
		//counter. Use of reclamation packets larger than the programmed size will cause a Babble error if
		//executed during the critical window at frame end. The Babble error results in the offending endpoint
		//being stalled. Software is responsible for ensuring that any packet which could be executed under
		//bandwidth reclamation be within this size limit.
		//0 = 32 bytes
		//1 = 64 bytes
	unsigned short loop_back_test_mode:1;//8    Loop Back Test Mode ?R/W.
		//0 = Disable loop back test mode.
		//1 = Intel ICH5 is in loop back test mode. 
		//When both ports are connected together, a write to oneport will be seen on the other port and 
		// the data will be stored in I/O offset 18h.
	unsigned short reserved:9;          //15:7 Reserved
} USBCMD;

typedef struct usb_sts_def
{
	unsigned short 	usb_int:1;//0 USB Interrupt (USBINT) 〞 R/WC.
		//0 = Software clears this bit by writing a 1 to it.
		//1 = The host controller sets this bit when the cause of an interrupt is a completion of a USB
		//transaction whose Transfer Descriptor had its IOC bit set. Also set when a short packet is
		//detected (actual length field in TD is less than maximum length field in TD), and short packet
		//detection is enabled in that TD.
	unsigned short err_int:1;	//1 USB Error Interrupt 〞 R/WC.
		//0 = Software clears this bit by writing a 1 to it.
		//1 = Completion of a USB transaction resulted in an error condition (e.g., error counter underflow). If
		//the TD on which the error interrupt occurred also had its IOC bit set, both this bit and Bit 0 are
		//set.
	unsigned short rsm_det:1;	//2 Resume Detect (RSM_DET) 〞 R/WC.
		//0 = Software clears this bit by writing a 1 to it.
		//1 = The host controller received a ※RESUME§ signal from a USB device. This is only valid if the
		//Host controller is in a global suspend state (bit 3 of Command register = 1).
	unsigned short hserr:1;//3Host System Error 〞 R/WC.
		//0 = Software clears this bit by writing a 1 to it.
		//1 = A serious error occurred during a host system access involving the host controller module. In a
		//PCI system, conditions that set this bit to 1 include PCI Parity error, PCI Master Abort, and PCI
		//Target Abort. When this error occurs, the host controller clears the Run/Stop bit in the
		//Command register to prevent further execution of the scheduled TDs. A hardware interrupt is
		//generated to the system.
	unsigned short hcerr:1;//4 Host Controller Process Error 〞 R/WC.
		//0 = Software clears this bit by writing a 1 to it.
		//1 = The host controller has detected a fatal error. This indicates that the host controller suffered a
		//consistency check failure while processing a Transfer Descriptor. An example of a consistency
		//check failure would be finding an illegal PID field while processing the packet header portion of
		//the TD. When this error occurs, the host controller clears the Run/Stop bit in the Command
		//register to prevent further schedule execution. A hardware interrupt is generated to the system.

	unsigned short hchalted:1;//5 HCHalted 〞 R/WC.
		//0 = Software clears this bit by writing a 1 to it.
		//1 = The host controller has stopped executing as a result of the Run/Stop bit being set to 0, either
		//by software or by the host controller hardware (debug mode or an internal error). Default.

	unsigned short reerved:10;//15:6 Reserved
}USBSTS;


//00每01 USBCMD USB Command 0000h R/W
//02每03 USBSTS USB Status 0020h R/WC
//04每05 USBINTR USB Interrupt Enable 0000h R/W
//06每07 FRNUM Frame Number 0000h R/W (see Note 1)
//08每0B FRBASEADD Frame List Base Address Undefined R/W
//0C SOFMOD Start of Frame Modify 40h R/W
//0D每0F 〞 Reserved 〞 〞
//10每11 PORTSC0 Port 0 Status/Control 0080h R/WC, RO, R/W
//(see Note 1)
//12每13 PORTSC1 Port 1 Status/Control 0080h R/WC, RO, R/W
//(see Note 1)
//14每17 〞 Reserved 〞 〞
//18h LOOPDATA Loop Back Test Data 00h RO

typedef union _def_uhci
{
	struct S
	{
	USBCMD  usbcmd;
	USBSTS  usbsts;
	unsigned short  usbintr;
	unsigned short  frnum; 
	unsigned long  frbaseaddr:24;
	unsigned long  sofmod:8;
	unsigned long  reserved1;
	unsigned short  portsc0; 
	unsigned short  portsc1;
	unsigned long  reserved2;
	unsigned char  loopdata;
	}s;
	unsigned long dd[1];
	unsigned long dw[1];
	unsigned char db[0x20];
}UHCI;

//
// frame list entry
//

typedef struct _def_uhci_fl_entry
{
	unsigned long  t:1;      //0 Terminate (T). 1=Empty Frame (pointer is invalid). 0=Pointer is valid (points to a QH or TD). This bit
					         //indicates to the Host Controller whether the schedule for this frame has valid entries in it.
	unsigned long qh_td:1;   //1 QH/TD Select (Q). 1=QH. 0=TD. This bit indicates to the hardware whether the item referenced by
							 //the link pointer is a TD or a QH. This allows the Host Controller to perform the proper type of
							 //processing on the item after it is fetched.
	unsigned long reserved:2;//3:2 Reserved. These bits must be written as 0s.
	unsigned long flp:28;    //31:4 Frame List Pointer (FLP). This field contains the address of the first data object to be processed in
							 //the frame and corresponds to memory address signals [31:4], respectively.
}FL_ENTRY;



typedef struct _def_uhci_td_linker
{
	unsigned long T:1;//0 Terminate (T). 1=Link Pointer field not valid. 0=Link Pointer field is valid. This bit informs the Host
			//Controller that the link pointer in this TD does not point to another valid entry. When encountered in a
			//queue context, this bit indicates to the Host Controller that there are no more valid entries in the
			//queue. A TD encountered outside of a queue context with the T bit set informs the Host Controller
			//that this is the last TD in the frame.
	unsigned long qh_td:1;//1 QH/TD Select (Q). 1=QH. 0=TD. This bit informs the Host Controller whether the item referenced by
					//the link pointer is another TD or a QH. This allows the Host Controller to perform the proper type of
					//processing on the item after it is fetched
	unsigned long vf:1;//2 Depth/Breadth Select (Vf). 1=Depth first. 0=Breadth first. This bit is only valid for queued TDs and
					//indicates to the hardware whether it should process in a depth first or breadth first fashion. When set
					//to depth first, it informs the Host Controller to process the next transaction in the queue rather than
					//starting a new queue.
	unsigned long reserved:1;//3 Reserved. Must be 0 when writing this field.
	unsigned long lp:28;//31:4 Link Pointer (LP). Bits [31:4] Correspond to memory address signals [31:4], respectively. This field
					//points to another TD or QH.
}UHCI_LINKER;

typedef struct _def_uhci_td_ctrl_status
{

unsigned long actlen:11;//10:0 Actual Length (ActLen). The Actual Length field is written by the Host Controller at the conclusion
						//of a USB transaction to indicate the actual number of bytes that were transferred. It can be used by
						//the software to maintain data integrity. The value programmed in this register is encoded as n-1 (see
						//Maximum Length field description in the TD Token, Dword 2).
unsigned long reserved0:5;//15:11 Reserved (R).

//23:16 Status. This field is used by the Host Controller to communicate individual command execution
//states back to HCD. This field contains the status of the last transaction performed on this TD. For
//Isochronous TDs, this is always the completion status (no retries). For the other transfer types this
//field is updated each time the TD is executed. The bit encodings are:
//Bit Status Field Description
unsigned long reserved1:1;//16 Reserved (R).
unsigned long bitstuff_err:1;//17 Bitstuff Error. This bit is set to a 1 by the Host Controller during status update to indicate that
						//the receive data stream contained a sequence of more than 6 ones in a row.
unsigned long crc_timeout_err:1;//18 CRC/Time Out Error. Set to a 1 by the Host Controller during status update in the case that
						//no response is received from the target device/endpoint within the time specified by the
						//protocol chapter of the USB specification. This bit is also set to a 1 by the Host Controller
						//during status update when a CRC error is detected during the transaction associated with this
						//transfer descriptor. In the transmit case (OUT or SETUP Command), this is in response to the
						//Host Controller detecting a timeout from the target device/endpoint. In the receive case (IN
						//Command), this is in response to the Host Controller＊s CRC checker circuitry detecting an
						//error on the data received from the device/endpoint or a NAK or STALL handshake being
						//received in response to a SETUP transaction.
unsigned long nak_recved:1;//19 NAK Received. Set to a 1 by the Host Controller during status update when the Host
						//Controller receives a ※NAK§ packet during the transaction generated by this descriptor. If a
						//NAK handshake is received from a SETUP transaction, a Time Out Error will also be
						//reported.
unsigned long babble_det:1;//20 Babble Detected. Set to a 1 by the Host Controller during status update when a ※babble§ is
					//detected during the transaction generated by this descriptor. In addition to setting this bit, the
					//Host Controller also sets the ※STALLED§ bit (bit 22) to a 1. Since ※babble§ is considered a fatal
					//error for that transfer, setting the ※STALLED§ bit to a 1 insures that no more transactions
					//occur as a result of this descriptor. Detection of babble causes immediate termination of the
					//current frame. No further TDs in the frame are executed. Execution resumes with the next
					//frame list index.
unsigned long data_buff_err:1;//21 Data Buffer Error. Set to a 1 by the Host Controller during status update to indicate that the
					//Host Controller is unable to keep up with the reception of incoming data (overrun) or is unable
					//to supply data fast enough during transmission (underrun). When this occurs, the actual
					//length and Max Length field of the TD will not match. In the case of an underrun, the Host
					//Controller will transmit an incorrect CRC (thus invalidating the data at the endpoint) and leave
					//the TD active (unless error count reached zero). If a overrun condition occurs, the Host
					//Controller will force a timeout condition on the USB, invalidating the transaction at the source.

unsigned long stalled:1;//22 Stalled. Set to a 1 by the Host Controller during status updates to indicate that a serious error
					//has occurred at the device/endpoint addressed by this TD. This can be caused by babble, the
					//error counter counting down to zero, or reception of the STALL handshake from the device
					//during the transaction. Any time that a transaction results in the Stalled bit being set, the
					//Active bit is also cleared (set to 0). If a STALL handshake is received from a SETUP
					//transaction, a Time Out Error will also be reported.
unsigned long active:1;//23 Active. Set to 1 by software to enable the execution of a message transaction by the Host
					//Controller. When the transaction associated with this descriptor is completed, the Host
					//Controller sets this bit to 0 indicating that the descriptor should not be executed when it is next
					//encountered in the schedule. The Active bit is also set to 0 if a stall handshake is received
					//from the endpoint. For Host Controller schedule execution operations, see the Script and Data
					//Transfer Primitives section.

unsigned long ioc:1;//24 Interrupt on Complete (IOC). 1=Issue IOC. This bit specifies that the Host Controller should issue
					//an interrupt on completion of the frame in which this Transfer Descriptor is executed. Even if the
					//Active bit in the TD is already cleared when the TD is fetched (no transaction will occur on USB), an
					//IOC interrupt is generated at the end of the frame.
unsigned long ios:1;//25 Isochronous Select (IOS). 1=Isochronous Transfer Descriptor. 0=Non-isochronous Transfer
					//Descriptor. The field specifies the type of the data structure. If this bit is set to a 1, the TD is an
					//isochronous transfer. Isochronous TDs are always marked inactive by the hardware after execution,
					//regardless of the results of the transaction.
unsigned long ls:1;//26 Low Speed Device (LS). 1=Low Speed Device. 0=Full Speed Device. This bit indicates that the
					//target device (USB data source or sink) is a low speed device, running at 1.5 Mb/s, instead of at full
					//speed. There are special restrictions on schedule placement for low speed TDs. See section 1.3,
					//Scheduling, for more information on low speed TD schedule placement. If a Host Controller root hub
					//port is connected to a full speed device and this bit is set to a 1 for a low speed transaction, the Host
					//Controller sends out a low speed preamble on that port before sending the PID. No preamble is sent
					//if a Host Controller root hub port is connected to a low speed device.
unsigned long num_err:2;//28:27 This field is a 2-bit down counter that keeps track of the number of Errors
					//detected while executing this TD. If this field is programmed with a non zero value during setup, the
					//Host Controller decrements the count and writes it back to the TD if the transaction fails. If the
					//counter counts from one to zero, the Host Controller marks the TD inactive, sets the ※STALLED§ and
					//error status bit for the error that caused the transition to zero in the TD. An interrupt will be
					//generated to HCD if the decrement to zero was caused by Data Buffer error, Bitstuff error, or if
					//enabled, a CRC or Timeout error. If HCD programs this field to zero during setup, the Host Controller
					//will not count errors for this TD and there will be no limit on the retries of this TD.
					//Bits[28:27] Interrupt After
					//00 No Error Limit
					//01 1 error
					//10 2 errors
					//11 3 errors
					//Error Decrement Counter Error Decrement Counter
					//CRC Error Yes Data Buffer Error Yes
					//Timeout Error Yes Stalled No*
					//NAK Received No Bitstuff Error Yes
					//Babble Detected No*
					//* Detection of Babble or Stall automatically deactivates the TD. Thus, count is not decremented.

unsigned long spd:1;//29 Short Packet Detect (SPD). 1=Enable. 0=Disable. When a packet has this bit set to 1 and the
					//packet:
					//1. is an input packet;
					//2. is in a queue; and
					//3 successfully completes with an actual length less than the maximum length;
					//then the TD is marked inactive, the Queue Header is not updated and the USBINT status bit (Status
					//Register) is set at the end of the frame. In addition, if the interrupt is enabled, the interrupt will be
					//sent at the end of the frame.
					//Note that any error (e.g., babble or FIFO error) prevents the short packet from being reported. The
					//behavior is undefined when this bit is set with output packets or packets outside of queues.

unsigned long reserved2:3;//Reserved (R).
} TD_CTRL_STATUS;



typedef struct _def_uhci_pid
{
unsigned long pid:8;//7:0 Packet Identification (PID). This field contains the Packet ID to be used for this transaction. Only
			//the IN (69h), OUT (E1h), and SETUP (2Dh) tokens are allowed. Any other value in this field causes
			//a consistency check failure
unsigned long dev:7;//14:8 Device Address. This field identifies the specific device serving as the data source or sink.
unsigned long endpt:4;//18:15 Endpoint (EndPt). This 4-bit field extends the addressing, internal to a particular device by providing
//16 endpoints. This permits more flexible addressing of devices in which more than one sub-channel
//is required.
unsigned long toggle:1;//19 Data Toggle (D). This bit is used to synchronize data transfers between a USB endpoint and the
			//host. This bit determines which data PID is sent or expected (0=DATA0 and 1=DATA1). The Data
			//Toggle bit provides a 1-bit sequence number to check whether the previous packet completed. This
			//bit must always be 0 for Isochronous TDs. See section 8.6 of the USB specification for a more
			//detailed description of Data Toggle Synchronization.
unsigned long reserved:1;//20 Reserved (R).
unsigned long maxlen:11;//31:21 Maximum Length (MaxLen). The Maximum Length field specifies the maximum number of data
//bytes allowed for the transfer. The Maximum Length value does not include protocol bytes, such as
//PID and CRC. The maximum data packet is 1280 bytes. The 1280 packet length is the longest
//packet theoretically guaranteed to fit into a frame. Actual packet maximum lengths are set by HCD
//according to the type and speed of the transfer. Note that the maximum length allowed by the USB
//specification is 1023 bytes. The valid encodings for this field are:
//Bits [31:21] Length
//0x000 1 byte
//0x001 2 bytes
//.....
//0x3FE 1023 bytes
//0x3FF 1024 bytes
//.....
//0x4FF 1280 bytes
//0x7FF 0 bytes (Null Data packet.)
//Note that values from 500h to 7FEh are illegal and cause a consistency check failure.
//In the transmit case, the Host Controller uses this value as a terminal count for the number of bytes
//it fetches from host memory. In most cases, this is the number of bytes it will actually transmit. In
//rare cases the Host Controller may be unable to access memory (e.g., due to excessive latency) in
//time to avoid underrunning the transmitter. In this instance the Host Controller would transmit fewer
//bytes than specified in the Maximum Length field.
//In the receive case, this field represents the maximum number of bytes that the device should send
//to the Host Controller. If the device continues to send after the Host Controller has received Max
//Length bytes, a BABBLE error is generated. See the Interrupt section for further information.

} TD_PID;


typedef struct _def_uhci_td
{
	UHCI_LINKER td_linker;
	TD_CTRL_STATUS ctrl_status;
	TD_PID pid;
	unsigned long  buffer;
}UHCI_TD;

typedef struct _def_uhci_qh
{
	UHCI_LINKER head_linker;
	UHCI_LINKER element_linker;

}UHCI_QH;

typedef UHCI_LINKER UHCI_FRAME_POINTER;

void usb_schedule();
#endif //__usb_h__
