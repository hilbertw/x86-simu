#include "82801-ide.h"
#include "log.h"

char * ide_desc[]={"pri address","pri contrl","sec address","sec contrl","bus master"};
//
// config space r/w notify
//
 void C82801IDE::ConfigCallback(int dwCommand,unsigned int address)
 {
	 DECL_CONFIG_SPACE_REG

	if(address>=0x10&&address<=0x13)
	log2("ide pri cmdbase",PCI_HEADER()->ddBase[0]);
    else if(address>=0x14&&address<=0x17)
	log2("ide pri ctrlbase",PCI_HEADER()->ddBase[1]);
    else if(address>=0x18&&address<=0x1b)
	log2("ide sec cmdbase",PCI_HEADER()->ddBase[2]);
    else if(address>=0x1c&&address<=0x1f)
	log2("ide sec ctrl base",PCI_HEADER()->ddBase[3]);
    else if(address>=0x20&&address<=0x23)
	log2("ide bus master base",PCI_HEADER()->ddBase[4]);
 }
//
// mem/i/o bus operation on function
//
DWORD C82801IDE::DoBusOp(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num)
{
	

	char *bus_master=((ICH82801_IDE*)pConfigSpaceHdr)->bus_master;

	DECL_CONFIG_SPACE_REG
	if(Op==IO_WR)
	{
		if(bar_num==4&&ddAddrLo<0x10) 
		{
			if(dwBytes==1)bus_master[ddAddrLo]=pBuffer[0];
			else if(dwBytes==2)*(short *)&bus_master[ddAddrLo]=*(short *)pBuffer;
			else if(dwBytes==4)*(int *)&bus_master[ddAddrLo]=*(int *)pBuffer;
		}
	}
	else if(Op==IO_RD)
	{
		if(bar_num==4) 
		{
			if(dwBytes==1)pBuffer[0]=bus_master[ddAddrLo];
			else if(dwBytes==2)*(short *)pBuffer=*(short *)&bus_master[ddAddrLo];
			else if(dwBytes==4)*(int *)pBuffer=*(int *)&bus_master[ddAddrLo];
		}
	}
	log("IDE %s : %c %x %x\n",ide_desc[bar_num],(Op==IO_RD)?'r':'w',ddAddrLo,
	 	dwBytes==1? (unsigned char)pBuffer[0]:
		dwBytes==2? *(unsigned short *)pBuffer:
		dwBytes==4? *(unsigned int *)pBuffer:0);
	return dwBytes;
}
#if 0
//
// address select
//
int C82801IDE::DoDeviceSelect(bool io,DWORD ddAddrLo,DWORD ddAddrHi)
{

}
#endif