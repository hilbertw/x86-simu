#include "string.h"
#include "pci.h"
#include "log.h"
//
// config space r/w notify
//
void CGenricPCIFunction::ConfigCallback(int dwCommand,unsigned int address)
{
}
//
// mem/i/o bus operation on function
//
DWORD CGenricPCIFunction::DoBusOp(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num)
{
	if(Op==MEM_RD||Op==IO_RD)
		memset(pBuffer,0xff,dwBytes);
	return dwBytes;
}
//
// address select. matching bar[0-5]
//
int CGenricPCIFunction::DoDeviceSelect(bool io,DWORD ddAddrLo,DWORD ddAddrHi)
{
	DECL_CONFIG_SPACE_REG

	int j;
	for( j=0;j<6;j++)
	{

		DWORD ddBaseMask=io?~1:~0xf;

		if( CONFIG_SPACE_HDR()->BlockSpec[j]!=0   //implemented?
			&&(INDICATOR(CONFIG_SPACE_HDR()->BlockSpec[j])==(io?1:0)) //memory block
			&&PCI_HEADER()->ddBase[j]==
			(ddBaseMask&CONFIG_SPACE_HDR()->BlockSpec[j]&ddAddrLo))

		break;
		
	}

	return j==6?-1:j;
}

