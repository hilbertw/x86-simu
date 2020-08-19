#ifndef __8253_h__
#define __8253_h__

//Port 43h, 8253 Mode Control Register, data format:

//|7|6|5|4|3|2|1|0|  Mode Control Register
// | | | | | | | `---- 0=16 binary counter, 1=4 decade BCD counter
// | | | | `--------- counter mode bits
// | | `------------ read/write/latch format bits
// `--------------- counter select bits (also 8254 read back command)
typedef struct mode_ctrl_8253_def
{
	unsigned char bin_or_bcd:1;
	unsigned char mode:3;
	unsigned char wrl_fmt:2;
	unsigned char counter_sel:2;
}MODE_CTRL_8253;


//Bits
// 76 Counter Select Bits
// 00  select counter 0
// 01  select counter 1
// 10  select counter 2
// 11  read back command (8254 only, illegal on 8253, see below)
#define SEL_COUNTER_0 0
#define SEL_COUNTER_1 1
#define SEL_COUNTER_2 2
#define SEL_RD_BACK   3

//Bits
// 54  Read/Write/Latch Format Bits
// 00  latch present counter value
// 01  read/write of MSB only
// 10  read/write of LSB only
// 11  read/write LSB, followed by write of MSB
#define LATCH_PRESENT_COUNTER 0
#define MSB_ONLY              1
#define LSB_ONLY              2
#define LSB_MSB               3



typedef struct rd_back_cmd_def
{
	unsigned char z0:1;
	unsigned char sel_cnt0:1;
	unsigned char sel_cnt1:1;
	unsigned char sel_cnt2:1;
	unsigned char latch_status:1;
	unsigned char latch_count:1;
	unsigned char one_one:2;
} RDBACK_CMD;

//Read Back Command Format  (8254 only)
//|7|6|5|4|3|2|1|0| Read Back Command (written to Mode Control Reg)
// | | | | | | | `--- must be zero
// | | | | | | `---- select counter 0
// | | | | | `----- select counter 1
// | | | | `------ select counter 2
// | | | `------- 0 = latch status of selected counters
// | | `-------- 0 = latch count of selected counters
// `----------- 11 = read back command
typedef struct rd_back_cmd_status_def
{
	unsigned char bin_or_bcd:1;
	unsigned char mode:3;
	unsigned char wrl_fmt:2;
	unsigned char out_pin_states:2;
} RDBACK_CMD_STATUS;
//Read Back Command Status (8254 only, read from counter register)

//|7|6|5|4|3|2|1|0|  Read Back Command Status
// | | | | | | | `--- 0=16 binary counter, 1=4 decade BCD counter
// | | | | `-------- counter mode bits (see Mode Control Reg above)
// | | `----------- read/write/latch format (see Mode Control Reg)
// | `------------ 1=null count (no count set), 0=count available
// `------------- state of OUT pin (1=high, 0=low)


static union
{
unsigned char port43h;
MODE_CTRL_8253 mode_ctrl;
RDBACK_CMD rd_back_cmd;
};


//Port 40h, 8253 Counter 0 Time of Day Clock (normally mode 3)
//Port 41h, 8253 Counter 1 RAM Refresh Counter (normally mode 2)
//Port 42h, 8253 Counter 2 Cassette and Speaker Functions

typedef struct timer_8253_def
{
	union
	{
		unsigned short count;
		struct  {char lsb,msb; } w;
	}u,u_to_reload;
	
	MODE_CTRL_8253 mode;
	char rw_state:5;
	char pin_state:2;
	char run:1;//0:stops.

} STATE_8253;


#endif //__8253_h__