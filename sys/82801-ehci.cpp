#include "82801-ehci.h"
#include "log.h"
#include "assert.h"
#include "usb.h"


// config space r/w notify
//
 void C82801EHCI::ConfigCallback(int dwCommand,unsigned int address)
 {
#if LOG_USB
	if((address&0xfc)==0x10)
	{
		DECL_CONFIG_SPACE_REG

		char dev_no=7;

		log("EHCI %d %c Mem Base:%x\n",dev_no,dwCommand<'w'?'r':'w',((DWORD*)pConfigSpaceRegs)[4]);
	}
#endif
 }
//
// mem/i/o bus operation on function
//
DWORD C82801EHCI::DoBusOp(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num)
{
	DECL_CONFIG_SPACE_REG

	char dev_no=7;

	if(Op==MEM_WR)
	{
		usb_write(ddAddrLo,pBuffer,dwBytes,dev_no);
	}
	else if(Op==MEM_RD)
	{
		usb_read(ddAddrLo,pBuffer,dwBytes,dev_no);
		if(dwBytes==1)* pBuffer=0;
		else if(dwBytes==2)* (WORD*)pBuffer=0;
		else * (DWORD*)pBuffer=0;


	}
#if LOG_USB	
	log("EHCI %d %c: %x-",dev_no,(Op==IO_RD)?'r':'w',ddAddrLo);
	if(dwBytes<=4)
    log0("%x\n",
	 	dwBytes==1? (unsigned char)pBuffer[0]:
		dwBytes==2? *(unsigned short *)pBuffer:
		dwBytes==4? *(unsigned int *)pBuffer:0);
	else
	{
		for(unsigned int i=0;i<dwBytes;i++)
		{
		log0("%x ",(unsigned char)pBuffer[i]);
		}
		log0("\n");
	}
#endif
	return dwBytes;
}
#if 0
//
// address select
//
int C82801EHCI::DoDeviceSelect(bool io,DWORD ddAddrLo,DWORD ddAddrHi)
{

}
#endif