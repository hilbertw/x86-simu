#include "82801-lpc.h"
#include "acpi82801.h"
#include "log.h"
//
// config space r/w notify
//
 void C82801LPC::ConfigCallback(int dwCommand,unsigned int address)
 {
	 DECL_CONFIG_SPACE_REG

	// set acpi base register
	if((address&0xfc)==0x40)
	{
		PCI_HEADER()->ddBase[0]=((DWORD*)pConfigSpaceRegs)[0x10]&(-2);
		log("acpi base:%x %x\n",((DWORD*)pConfigSpaceRegs)[0x10],PCI_HEADER()->ddBase[0]);
	}
	// gpio base
	else if((address&0xfc)==0x58)
	{
		PCI_HEADER()->ddBase[1]=((DWORD*)pConfigSpaceRegs)[0x58/4]&(-2);
		log("gpio base:%x %x\n",((DWORD*)pConfigSpaceRegs)[0x58/4],PCI_HEADER()->ddBase[1]);
	}
	else if(address>=0xa0&&address<=0xa8)
	{

		//log4("General PM Configuration",cmd,address,*(long *)((char*)pConfigSpaceRegs+address));

		if(0xa1==address)
		((char*)pConfigSpaceRegs)[0xa1]=2;//PWRBTN_LVL ¡ª RO. This read-only bit indicates 
		                                            //the current state of the PWRBTN# signal.
													//0 = Low.
													//1 = High. 	
	}
 }

 char gpio_reg[0x40];
//
// mem/i/o bus operation on function
//
DWORD C82801LPC::DoBusOp(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num)
{
	//	assert((bar_num==0&&ddAddrLo<0x80&&ddAddrLo+dwBytes<0x80)
	//		||(bar_num==1&&ddAddrLo<0x40&&ddAddrLo+dwBytes<0x40));



	if(Op==IO_WR)
	{
		if(bar_num==0)
		{
			
			unsigned  int i;
			for(i=0;i<dwBytes;i++)
			{
				acpi82801.set_reg(ddAddrLo+i,pBuffer[i]);
			/*	if((acpi_reg_attr[i>>3]&(1<<(i&7)))==0)
					acpi_reg[ddAddrLo]= *pBuffer;// r/w
				else 
					acpi_reg[ddAddrLo]&= *pBuffer;// r/wc*/
			}
			
			
		}
		else if (ddAddrLo + dwBytes<sizeof(gpio_reg))
		{
		if(dwBytes==4) *(DWORD *)(gpio_reg+ddAddrLo)= *(DWORD *)pBuffer;
		else if(dwBytes==2) *(WORD *)(gpio_reg+ddAddrLo)= *(WORD *)pBuffer;
		if(dwBytes==1) *(gpio_reg+ddAddrLo)= *pBuffer;	
		}
	}
	else 	if(Op==IO_RD)
	{
		if(bar_num==0)
		{
			unsigned int i;
			for(i=0;i<dwBytes;i++)
				pBuffer[i]=acpi82801[ddAddrLo+i];
		}
		else
		{
			if(dwBytes==4)  *(DWORD *)pBuffer=*(DWORD *)(gpio_reg+ddAddrLo);
			else if(dwBytes==2)  *(WORD *)pBuffer=*(WORD *)(gpio_reg+ddAddrLo);
			if(dwBytes==1)  *pBuffer=*(gpio_reg+ddAddrLo);
		}

	}
#if LOG_GPIO
	if(bar_num!=0)

	log4("GPIO",(Op==IO_RD)?'r':'w',ddAddrLo,
	    dwBytes==1? (unsigned char)pBuffer[0]:
		dwBytes==2? *(unsigned short *)pBuffer:
		dwBytes==4? *(unsigned long *)pBuffer:0);
#endif	

#if LOG_ACPI
	if(bar_num==0)
		log4("ACPI",(Op==IO_RD)?'r':'w',ddAddrLo,
		dwBytes==1? (unsigned char)pBuffer[0]:
		dwBytes==2? *(unsigned short *)pBuffer:
		dwBytes==4? *(unsigned long *)pBuffer:0);
#endif	
	return dwBytes;
}
#if 0
//
// address select
//
int C82801LPC::DoDeviceSelect(bool io,DWORD ddAddrLo,DWORD ddAddrHi)
{

}
#endif