#include "82801-smbus.h"
#include "smbus.h"
#include "log.h"
#include "assert.h"
//
// config space r/w notify
//
 void C82801SMBUS::ConfigCallback(int dwCommand,unsigned int address)
 {
	if((address&0xfc)==0x20)
	{
		 DECL_CONFIG_SPACE_REG
		log2("smbus base",((DWORD*)pConfigSpaceRegs)[8]);
	}
 }
//
// mem/i/o bus operation on function
//
DWORD C82801SMBUS::DoBusOp(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num)
{
	assert(dwBytes==1);
	if(Op==IO_WR)
	{
		assert(ddAddrLo<SMBUS_BUFFSIZE);
		if(ddAddrLo==HST_STS)
			CSMBUS::buffer[ddAddrLo]&=~pBuffer[0];
		else
		CSMBUS::buffer[ddAddrLo]=pBuffer[0];
	}
	else 	if(Op==IO_RD)
	{
		assert(ddAddrLo<SMBUS_BUFFSIZE);

		pBuffer[0]=CSMBUS::buffer[ddAddrLo];
		//INUSE_STS ¡ª R/WC (special). This bit is used as semaphore among various independent
		//software threads that may need to use the ICH4¡¯s SMBus logic, and has no other effect on
		//hardware.
		//0 = After a full PCI reset, a read to this bit returns a 0.
		//1 = After the first read, subsequent reads will return a 1. A write of a 1 to this bit will reset the next
		//read value to 0. Writing a 0 to this bit has no effect. Software can poll this bit until it reads a 0,
		//and will then own the usage of the host controller.
		if(ddAddrLo==HST_STS)
		{
			CSMBUS::buffer[ddAddrLo]|=0x40;
		}
	}

#if LOG_SMBUS	
	log("SMBUS %c,%x,%x\n",(Op==IO_RD)?'r':'w',ddAddrLo,*pBuffer);
#endif
	//
	// start a  address
	//
	if(ddAddrLo==2&&(pBuffer[0]&0x40)!=0) 
	{
#if LOG_SMBUS
		log("SMBUS tx:%x address:%x type (%s)  (%c)\n",
		CSMBUS::buffer[XMIT_SLVA]>>1,
		CSMBUS::buffer[HST_CMD],
		CSMBUS::cmd_type[(CSMBUS::buffer[HST_CNT]>>2)&7],
		(CSMBUS::buffer[XMIT_SLVA]&1)?'r':'w');
#endif
		CSMBUS::buffer[HST_STS]=CSMBUS::buffer[HST_CMD]==0xd?0x88:0x82;
		CSMBUS::buffer[HST_D0]=
			CSMBUS::buffer[HST_CMD]==0x2?(CSMBUS::buffer[XMIT_SLVA]==0xa3?7:0):
			CSMBUS::buffer[HST_CMD]==0x15?0:
			CSMBUS::buffer[HST_CMD]==0xb ?0x2:
			CSMBUS::buffer[HST_CMD]==0x5 ?0:0x1b;
		
	}

	return dwBytes;
}
#if 0
//
// address select
//
int C82801SMBUS::DoDeviceSelect(bool io,DWORD ddAddrLo,DWORD ddAddrHi)
{

}
#endif