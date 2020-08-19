#ifndef __smbus_h__
#define __smbus_h__ 

#include "config.h"

#define HST_STS        0x00 //Host Status  00  R/WC,RO
#define HST_CNT        0x02 //Host Control 00  R/W, WO
#define HST_CMD        0x03 //Host Command 00  R/W
#define XMIT_SLVA      0x04 //Transmit Slave Address 00  R/W
#define HST_D0         0x05 //Host Data 0 00  R/W
#define HST_D1         0x06 //Host Data 1 00  R/W
#define HOST_BLOCK_DB  0x07 //Host Block Data Byte 00  R/W
#define PEC            0x08 //Packet Error Check 00  R/W
#define RCV_SLVA       0x09 //Receive Slave Address 44  R/W
#define SLV_DATA       0x0A //Slave Data 0000  R/W
#define AUX_STS        0x0C //Auxiliary Status   00  R/WC
#define AUX_CTL        0x0D //Auxiliary Control  00  R/W
#define SMLINK_PIN_CTL 0x0E //SMLink Pin Control 04  R/W, RO
#define SMBUS_PIN_CTL  0x0F //SMBus Pin Control 04  R/W, RO
#define SLV_STS        0x10 //Slave Status  00  R/WC
#define SLV_CMD        0x11 //Slave Command 00  R/W
#define NOTIFY_DADDR   0x14 //Notify Device Address  00  RO
#define NOTIFY_DLOW    0x16 //Notify Data Low Byte   00  RO
#define NOTIFY_DHIGH   0x17 //Notify Data Hig //Byte 00  RO
#define SMBUS_BUFFSIZE 0x20

typedef struct _smbus_node
{
	unsigned char id;
	int (*call_back)();
} SMBUS_NODE;
class CSMBUS
{
public:

	static  unsigned char buffer[SMBUS_BUFFSIZE];
	static  SMBUS_NODE nodes[CFG_MAX_SMBUS_NODES];
	static  char *cmd_type[8];

	void do_command();

};
#endif // __smbus_h__