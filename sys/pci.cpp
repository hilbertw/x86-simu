#include <assert.h>
#include "pci.h"
#include "log.h"
//
// ignore unaligned access. 
//
void CPCIBus::read_config_reg(DWORD reg,BYTE * pszBuffer, char bytes)
{
	int i;
	DWORD ddValue;


	for(i=0;i<n_functions;i++)
	{
		if(functions[i].Address.w==(WORD)(reg>>8)) break;
	}

	if(i==n_functions                                       // unregistered function
		||((PCONFIG_SPACE_HDR)(functions[i].lpPciFunction->pConfigSpaceHdr))->dwHeaderSize<(BYTE)reg) //  invalid register
	{
		if(bytes==1)
		pszBuffer[0]=0xff;
		else if(bytes==2)
		*(WORD*)pszBuffer=0xffff;
		else
		*(DWORD*)pszBuffer=0xffffffff;
	}
	else
	{
		CONFIG_SPACE_HDR* pConfigSpaceHdr=(CONFIG_SPACE_HDR*)pci_bus.functions[i].lpPciFunction->pConfigSpaceHdr;
		char *pConfigSpaceRegs=(char *)(pConfigSpaceHdr+1);
	
		//
		// if reading BaseAddress register,and it is written -1,return blocksize
		//
		if(reg>=0x10&& reg<0x28)
		{
			DWORD bar_num=((BYTE)reg>>2)-4;
				
			ddValue=PCI_HEADER()->ddBase[bar_num];

			if(INDICATOR(pConfigSpaceHdr->BlockSpec[bar_num])==0) // mem
			{
				ddValue|=0xf;
				//ddValue|= functions[i].lpPciFunction->BlockSpec[bar_num]&0xf;
			}
			else
			{
				ddValue|= 3;// reserved bit& indicator
			}
			//
			// if bar read only, return -1. 
			//
			if(*(short *)&pConfigSpaceHdr->aReadOnlyFields[0]&(0x10<<bar_num))
			{
				ddValue=0xffffffff;
			}
			else if(ddValue==0xffffffff)
			{
				ddValue=*(DWORD *)&pConfigSpaceHdr->BlockSpec[bar_num];
				if(ddValue==0) ddValue=0xffffffff;
			}

			//make life simple. bar is read in dword
			*(DWORD*)pszBuffer=ddValue;
		}
		else
		{
			if(bytes==1)
			pszBuffer[0]=pConfigSpaceRegs[(BYTE)reg];
			else if(bytes==2)
			*(WORD*)pszBuffer=*(WORD*)(pConfigSpaceRegs+(BYTE)reg);
			else
			*(DWORD*)pszBuffer=*(DWORD*)(pConfigSpaceRegs+(BYTE)reg);
		}
		//
		// do post-read status update
		//
		functions[i].lpPciFunction->ConfigCallback('r'+bytes-1,(unsigned char)reg);	
	}
	
}
void  CPCIBus::write_config_reg(DWORD reg,BYTE * pszBuffer,char bytes)
{
	int i;
	DWORD ddValue;

	for(i=0;i<n_functions;i++)
	{
		if(functions[i].Address.w==(WORD)(reg>>8)) break;
	}

	if(i<n_functions                                       // unregistered function
		&&((PCONFIG_SPACE_HDR)(functions[i].lpPciFunction->pConfigSpaceHdr))->dwHeaderSize>(BYTE)reg) //  invalid register
	{

		CONFIG_SPACE_HDR* pConfigSpaceHdr=(CONFIG_SPACE_HDR*)pci_bus.functions[i].lpPciFunction->pConfigSpaceHdr;
		char *pConfigSpaceRegs=(char *)(pConfigSpaceHdr+1);
		//
		// readonly register can not be written.
		// if written to base address register, mask off bits
		//
		if(!(pConfigSpaceHdr->aReadOnlyFields[((BYTE)reg)>>5]&(1<<(((BYTE)reg>>2)&7))))
		{

			if(bytes==1)
			pConfigSpaceRegs[(BYTE)reg]=pszBuffer[0];
			else if(bytes==2)
			*(WORD*)(pConfigSpaceRegs+(BYTE)reg)=*(WORD*)pszBuffer;
			else
			*(DWORD*)(pConfigSpaceRegs+(BYTE)reg)=*(DWORD*)pszBuffer;

		

			if((BYTE)reg>=0x10 &&(BYTE)reg<0x28)
			{
				DWORD bar_num=((BYTE)reg>>2)-4;

				if( bytes==4)
					ddValue=*(DWORD*)pszBuffer;
				else
					ddValue=PCI_HEADER()->ddBase[bar_num];

				if(INDICATOR(pConfigSpaceHdr->BlockSpec[bar_num])==0) // mem
				{
					ddValue|=0xf;
				}
				else
				{
					ddValue|=3; // mask off reserved bit and indicator
				}


				if(ddValue!=0xffffffff)
				{
					ddValue&=pConfigSpaceHdr->BlockSpec[bar_num];

					if(INDICATOR(pConfigSpaceHdr->BlockSpec[bar_num])==0) // mem
					{
						ddValue&= 0xfffffff0;// mask off 'indicator','type', 'prefetchable' bit
					}
					else
					{
						ddValue&= 0xfffffffc; // mask off reserved bit and indicator
					}

					PCI_HEADER()->ddBase[bar_num]=ddValue;
				}
			}
		}

		functions[i].lpPciFunction->ConfigCallback('w'+bytes-1,(unsigned char)reg);
	}
}

DWORD  CPCIBus::MemBusOp(bool read ,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes)
{
	log0("PCI MEM %c :%x  %x\n",read?'r':'w',ddAddrLo,
				read?dwBytes:
			 	dwBytes==1? (unsigned char)pBuffer[0]:
				dwBytes==2? *(unsigned short *)pBuffer:
				dwBytes==4? *(unsigned int *)pBuffer:0
				);
	return DoBusOp(false, read , pBuffer, ddAddrLo, ddAddrHi,dwBytes);
}

DWORD  CPCIBus::IoBusOp(bool read ,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes)
{
#if LOG_PCI
	if(ddAddrLo!=0x808)
			log0("PCI I/O %c :%x %x \n",read?'r':'w',ddAddrLo,
				read?dwBytes:
			 	dwBytes==1? (unsigned char)pBuffer[0]:
				dwBytes==2? *(unsigned short *)pBuffer:
				dwBytes==4? *(unsigned int *)pBuffer:0
				);
#endif
	return DoBusOp(true, read , pBuffer, ddAddrLo, ddAddrHi,dwBytes);
}



void  CPCIBus::RegisterFunction(WORD dwAddress,
		CPCIFunction *lpPciFunction)
{
	assert(n_functions<CFG_MAX_PCI_FUNC);

	functions[n_functions].Address.w=dwAddress;
	functions[n_functions].lpPciFunction=lpPciFunction;
	n_functions++;
}
//
// start burst mode bus cycle
// return bytes available for read/write, 0 if no device selected
//
DWORD CPCIBus::dev_sel(bool io,DWORD ddAddrLo,DWORD ddAddrHi)
{
	int i,j;
	for(i=0;i<n_functions;i++)
	{
		j=functions[i].lpPciFunction->DoDeviceSelect(io,ddAddrLo,ddAddrHi);

		// falls into a device
		if(j!=-1){
			DWORD ddBaseMask=io?~1:~0xf;
			CONFIG_SPACE_HDR *lpConfigSpace=((CONFIG_SPACE_HDR*)functions[i].lpPciFunction->pConfigSpaceHdr);

			dwSelectedFunction=i;

			dwOffset=ddAddrLo&~ddBaseMask;

			DWORD dwBytesAvailable=~(ddBaseMask&lpConfigSpace->BlockSpec[j])-dwOffset;

			dev_sel_for_io=io;
			bar_num=j;
			return dwBytesAvailable;
		}
	}

	return 0;

}

//
// 32 bit bus read/wrote
//
DWORD  CPCIBus::read()
{
	DWORD tmp=-1;

	if(-1!=dwSelectedFunction)
	{
		functions[dwSelectedFunction].lpPciFunction->DoBusOp(dev_sel_for_io?IO_RD:MEM_RD,
							(BYTE *)&tmp,
							dwOffset,
							0,
							4,
							bar_num);
	dwOffset+=4;
	}
	return tmp;
}
void CPCIBus::write(DWORD  data)
{

	if(-1!=(DWORD)dwSelectedFunction)
	{
		functions[dwSelectedFunction].lpPciFunction->DoBusOp(dev_sel_for_io?IO_WR:MEM_WR,
							(BYTE *)&data,
							dwOffset,
							0,
							4,
							bar_num);
		dwOffset+=4;
	}
}
//
// burst mode ends
//
void CPCIBus::dev_sel_end()
{
	dwSelectedFunction=-1;
}





DWORD CPCIBus::DoBusOp(bool io,bool read ,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes)
{
	int i,j;
	//if(ddAddrLo==0xc02)
	//{
	//	log("hit");
	//}
	for(i=0;i<n_functions;i++)
	{

		j=functions[i].lpPciFunction->DoDeviceSelect(io,ddAddrLo,ddAddrHi);

		// falls into a device
		if(j!=-1){
			DWORD ddBaseMask=io?~1:~0xf;
			CONFIG_SPACE_HDR *lpConfigSpace=((CONFIG_SPACE_HDR*)functions[i].lpPciFunction->pConfigSpaceHdr);
			DWORD dwOffset=ddAddrLo-((PCI_HEADER*)(lpConfigSpace+1))->ddBase[j];

			DWORD dwBytesAvailable=~(ddBaseMask&lpConfigSpace->BlockSpec[j])-dwOffset;

			if(dwBytes>dwBytesAvailable)dwBytes=dwBytesAvailable;
			functions[i].lpPciFunction->DoBusOp(
				io?(read?IO_RD:IO_WR):(read?MEM_RD:MEM_WR),
				pBuffer,
				dwOffset,
				0,
				dwBytes,
				j);

			return dwBytes;
		}
	}
	return 0;
}




void CPCIBus::assign_sbus_num(unsigned char sbus_number)// reassign secondary pci bus number
{
	int i;
	for(i=0;i<n_functions;i++)
	{
		if(functions[i].Address.s.uBus!=0)
			functions[i].Address.s.uBus=sbus_number;
	}
}