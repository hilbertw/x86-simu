#ifndef __82801_lpc_h__
#define __82801_lpc_h__

#include "pci.h"

extern CPCIBus pci_bus;

typedef struct isa_bridge_def
{
	CONFIG_SPACE_HDR common; // must be  the  first member
	PCI_HEADER pciHeader;
	union
	{
		int reg[(0xf4-0x40)>>2];
		struct  S
		{
			unsigned long pmbase; // offset 0x40;
			char acpi_cntl;
			char regs0[0x4e-0x45];
			short bios_cntl;
			char regs1[0x54-0x50];
			char tco_cntl;         //offset:0x54
			char regs2[0x58-0x55];
			unsigned long gpiobase; // offset 0x58;
			char gpio_cntl;         // offset:0x5c;
			char regs3[0x60-0x5d];
			char pirq_rout0[4];        // offset 0x60
			char sirq_cntl;          // offset:0x64
			char regs4[0x68-0x65];
			char pirq_rout1[4];        // offset 0x68
			char regs5[0x88-0x6c];
			char d31_err_cfg;         // offset:0x88
			char regs6[0x8a-0x89];     
			char d31_err_sts;         // offset:0x8a
			char regs7[0x90-0x8b];   
			short pci_dma_cfg;        //0x90
			char regs8[0xa0-0x92];  
			// power management
			short  gen_pmcon_1;     //offset:0xa0
			short  gen_pmcon_2;     //offset:0xa2
			char  gen_pmcon_3;     //offset:0xa4
			char  regs9[0xa8-0xa5];
			char  stpclk_del;     //offset:0xa8
			char  regs10[0xad-0xa9];
			char  usb_tdd;     //offset:0xad
			char  sata_rd_cfg;     //offset:0xae
			char  regs11[0xb8-0xaf];
			int   gpi_rout;         //offset:0xb8
			char  regs12[0xc0-0xbc];
			char trp_fwd_en;        //offset:0xc0
			char  regs15[0xc4-0xc1];
			short  mon_trp_rng[4];  //offset:0xc4
			char   mob_trp_mask;    //offse:0xcc
			//
			//
			char  regs16[0xd0-0xcd];

			unsigned long gen_cntl; // offset:0xd0
			char  gen_sta;          // offset:0xd4
			char  back_cntl;        //offser:0xd5
			char regs17[0xd8-0xd6];
			char rtc_conf;          //offset:0xd8
			char regs19[0xe0-0xd9];
			char com_dec;            //offset:0xe0
			char lpcfdd_dec;         //offset:0xe1
			char regs20[0xee-0xe2];
			char  pb_dec_en1;         //offset:0xe3
			short  gen1_dec;          // offset: 0xe4
			short  lpc_en;            // offset: 0xe6
			int    fb_sel1;           // offset: 0xe8
			short   gen2_dec;         // offset: 0xec
			short   fb_sel2;          // offset: 0xee
			char    fb_dec_en2;       // offset: 0xf0
			char regs21[0xf2-0xf1];
			char    func_dis;         // offset:0xf2
		}s;

	}u;

} ICH82801_LPC_IF;

class C82801LPC:public CGenricPCIFunction
{
public:
	C82801LPC(WORD dwAddress,void* pdata):
	CGenricPCIFunction (dwAddress,pdata)
	{}

	//
	// config space r/w notify
	//
	virtual  void ConfigCallback(int dwCommand,unsigned int address);
	//
	// mem/i/o bus operation on function
	//
	virtual DWORD DoBusOp(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num);
	//
	// address select
	//
	//virtual int DoDeviceSelect(bool io,DWORD ddAddrLo,DWORD ddAddrHi);

};
//example: 82845 chipset:
//C82801LPC(PCI_FUN_ADDR(0,0x1e,0),config_space_82801_lpc);
#endif