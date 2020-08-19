#include "82845-host.h"
#include "mch.h"
#include "log.h"
void __cdecl vt_show_smram(bool opened, bool enabled);

extern CMCH mch;  // 845g will update pam




//
// config space r/w notify
//
 void C82845HOST::ConfigCallback(int cmd,unsigned int address)
 {

	//
	// read config reg, no op
	//
	if(cmd<'w') return;

	DECL_CONFIG_SPACE_REG

	if(address==0x90)
	{//PAM0[7:4] R R WE RE 0F0000h¨C0FFFFFh BIOS Area 90h
		mch.set_pam_rd_en(0xf000,(pConfigSpaceRegs[0x90]&0x10)?0xf000:0);
		mch.set_pam_wr_en(0xf000,(pConfigSpaceRegs[0x90]&0x20)?0xf000:0);
	}
	else if(address<=0x96 &&address>0x90)
	{
		char c=pConfigSpaceRegs[address];
		int pos=(address-0x91)<<1;

		int rd_enable=(c&1)|((c&0x10)>>3);

		int wr_enable=((c&2)|((c&0x20)>>3))>>1;

		mch.set_pam_rd_en(3<<pos,rd_enable<<pos);
		mch.set_pam_wr_en(3<<pos,wr_enable<<pos);
	}
	else if((address&0xfc)==0xc4)
	{
     //pConfigSpaceRegs[0xc4]=0x0;
     //pConfigSpaceRegs[0xc5]=0x2;
	}
	else if((address&0xfe)==0xc8)
	{

	
		//log("AGP ERROR:%x EAP:%x\n",
		//((unsigned short *)&CONFIG_SPACE_HDR()->pciHeader)[0xc8],
		//((unsigned long *)&CONFIG_SPACE_HDR()->pciHeader)[0x8c]
		//);                      

		pConfigSpaceRegs[0xc8]=0x0;
	}
	else if((address&0xfc)==0x7c)
	{
             
	//	pConfigSpaceRegs[0x7c]|=1;//ddr  sdram
	}

	else if((address)==0xb4)
	{
		//
		// set aperture base22:31
		//
		PCI_HEADER()->ddBase[0]=

			(1+~(pConfigSpaceRegs[0xb4]&0x1f))<<22;
	}
	else if((address&0xfc)==0x9c)
	{
		if(cmd>='w') 
		{
			bool opened,enabled;
			//
			// d_open=1, d_lock=0?_cls=1, d_lock=0?
			//
			opened=((pConfigSpaceRegs[0x9d]&0x70)==0x40);
			//
			// d_en
			//
			enabled=((pConfigSpaceRegs[0x9d]&0x8)==0x8);

			mch.open_smram(opened );
			mch.en_smram(enabled);

			log("SMRAM: %s,%s\n",opened?"Opened":"Closed",enabled?"Enabled":"Disabled");
			vt_show_smram(opened,enabled);

			if(0!=(pConfigSpaceRegs[0x9d]&0x10))
			{
				//d_lock, once set, can not reset again, make 0x9c~0x9f readonly 
				CONFIG_SPACE_HDR()->aReadOnlyFields[0x9c/32]|=((1<<((0x9c>>2)&0x7)));
			}
		}
	}
 }

#if 0
//
// mem/i/o bus operation on function
//
DWORD C82845HOST::DoBusOp(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num)
{

}
//
// address select
//
int C82845HOST::DoDeviceSelect(bool io,DWORD ddAddrLo,DWORD ddAddrHi)
{
}

#endif