#include "82801-uhci.h"
#include "log.h"
#include "usb.h"
//
// config space r/w notify
//
void C82801UHCI::ConfigCallback(int cmd,unsigned int address)
 {

#if LOG_USB
	if((address&0xfc)==0x20)
	{
		DECL_CONFIG_SPACE_REG

		char dev_no=((ICH82801_UHCI*)pConfigSpaceHdr)->ctrl_no;

		log("USB %d %c Base:%x\n",dev_no,cmd<'w'?'r':'w',((DWORD*)pConfigSpaceRegs)[8]);
	}
#endif
 }
//
// mem/i/o bus operation on function
//
DWORD C82801UHCI::DoBusOp(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num)
{
	DECL_CONFIG_SPACE_REG

	char dev_no=((ICH82801_UHCI*)pConfigSpaceHdr)->ctrl_no;

	if(Op==IO_WR)
	{
		usb_write(ddAddrLo,pBuffer,dwBytes,dev_no);
	}
	else if(Op==IO_RD)
	{
		usb_read(ddAddrLo,pBuffer,dwBytes,dev_no);
	}
#if LOG_USB	
	log("UHCI %d %c: %x-",dev_no,(Op==IO_RD)?'r':'w',ddAddrLo);
  
	if(dwBytes==1)  log0("%02x\n",(unsigned char)pBuffer[0]);
	else if(dwBytes==2)  log0("%04x\n",*(unsigned short *)pBuffer);
	else if(dwBytes==4)  log0("%08x\n",*(unsigned int *)pBuffer);
	else
	{
		for(unsigned int i=0;i<dwBytes;i++)
		{
		log0("%02x ",(unsigned char)pBuffer[i]);
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
int C82801UHCI::DoDeviceSelect(bool io,DWORD ddAddrLo,DWORD ddAddrHi)
{

}
#endif