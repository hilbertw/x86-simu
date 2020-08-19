#ifndef __MCH_H__
#define __MCH_H__

#include "config.h"
#include "filemap.h"
#include "ich.h"

extern CICH ich;

class CMCH
{
public:
	unsigned long io_apic_base;//0xfee00000 at reset
	unsigned short pam_rd_enable;
	unsigned short pam_wr_enable;
	bool smram_open; // d_open & !d_lock&!d_cls
	bool smram_en;   
	bool smiact;// smiact#

	//
	// r/w ram file map
	//
	CFileMap smram_map,ram_map,vgabios_map;
	//
	// readonly bios rom file map
	//
public:
	CMCH():io_apic_base(CFG_IOAPIC_BASE){reset();}

	void open_smram(bool open){smram_open=open;}
	void en_smram(bool en){smram_en=en;}
	void assert_smiact(bool a){smiact=a;}

	//bool smram_decode();

	void reset() {pam_rd_enable=pam_wr_enable=0;smram_open=smram_en=smiact=false;}
	void set_pam_rd_en(unsigned short mask,unsigned short value)
	{
	pam_rd_enable &=~mask;
	pam_rd_enable |=(value &mask);
	}

	void set_pam_wr_en(unsigned short mask,unsigned short value)
	{
	pam_wr_enable &=~mask;
	pam_wr_enable |=(value &mask);
	}

	char io_read_byte(unsigned short address) {return ich.io_read_byte(address);}
	short io_read_short(unsigned short address){return ich.io_read_short(address);}
	int io_read_int(unsigned short address){return ich.io_read_int(address);}



	void io_write_byte(unsigned short address,char dat) {ich.io_write_byte(address,dat);}
	void io_write_short(unsigned short address,short dat){ich.io_write_short(address,dat);}
	void io_write_int(unsigned short address,int dat){ich.io_write_int(address,dat);}


	char mem_read_byte(unsigned int address);
	short mem_read_short(unsigned int address);
	int mem_read_int(unsigned int address);

	void mem_read_string(unsigned int address,char *buff,int count);

	void mem_write_byte(unsigned int address,char dat);
	void mem_write_short(unsigned int address,short dat);
	void mem_write_int(unsigned int address,int dat);

	void mem_write_string(unsigned int address,char *buff,int count);

	int mem_rd_addr_range_type(unsigned int address);
	int mem_wr_addr_range_type(unsigned int address);

	unsigned long dword(unsigned long offset) {return *(unsigned long *)((char *)ram_map+offset);}
	unsigned short word(unsigned long offset) {return *(unsigned short *)((char *)ram_map+offset);}
	unsigned char byte(unsigned long offset) {return *(unsigned char *)((char *)ram_map+offset);}

	bool smram_decode(){return smram_open||(smram_en&&/*cpu[cpu_no].SMIACT()*/smiact);}
};

#ifndef  OFFSET
#define OFFSET(t,m) ((unsigned long)&((t *)NULL)->m)
#endif

//
// each insn updates some registers.commit in wb  stage
//


#define MCH_STAE_SIZE  OFFSET(CMCH,smram_map)

#endif   /*    __MCH_H__   */
   