#include <stdio.h>
#include "cpu.h"
#include "log.h"

#ifdef __GNUC__

#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))
#endif

//
// linear address= base(descriptor)+offset.
// limit check
//
// #GP(0) for an illegal memory operand effective address in the CS, DS, ES, FS, or GS segments; 
// #SS(0) for an illegal address in the SS segment;
// #PF(fault-code) for a page fault 
//
bool CCPU::read_mem(MEM_DESC &m_info,unsigned long & data, int data_size,int addr_size)
{
	unsigned long addr_hi,addr_lo;
	int data_bytes;

	assert(m_info.seg<MAX_SEG);
	gen_mem_addr(m_info,addr_size,addr_hi,addr_lo);

	assert((data_size&7)==0);
	data_bytes=data_size>>3;
	//
	// limit check
	//
	if(!check_limit(&descriptors[m_info.seg],addr_lo,addr_lo+data_bytes-1)
		||(__PE__==1&&__VM__==0 &&DPL(descriptors[m_info.seg])>max(__CPL__,RPL(selector(m_info.seg)))))
	{
		log("read addr=%x,limit=%x,cpl=%x,rpl=%x,dpl=%x\n",addr_lo,LIMIT(descriptors[m_info.seg]),__CPL__,RPL(selector(m_info.seg)),DPL(descriptors[m_info.seg]));
		if(m_info.seg==SS) SS_(0)
		else GP_(0);

		return false;
	}
	//
	// request mmu. raise #PF.
	//
	RW_STATUS status;
	if(status=mmu.ld(addr_lo+BASE(descriptors[m_info.seg]),__CPL__==3,(char *)&data,data_bytes))
	{
		PFT_(status&0x1f);
		return false;
	}

	return true;

}

// linear address= base(descriptor)+offset.
// limit check
//
// #GP(0) if the result is in a nonwritable segment; 
// #GP(0) for an illegal memory operand effective address in the CS, DS, ES, FS, or GS segments; 
// #SS(0) for an illegal address in the SS segment;
// #PF(fault-code) for a page fault 
//
bool CCPU::write_mem(MEM_DESC &m_info,unsigned long  data, int data_size,int addr_size)
{
	unsigned long addr_hi,addr_lo;
	int data_bytes;

	assert(m_info.seg<MAX_SEG);
	gen_mem_addr(m_info,addr_size,addr_hi,addr_lo);

	assert((data_size&7)==0);
	data_bytes=data_size>>3;
	//
	// limit check
	//
	if(!check_limit(&descriptors[m_info.seg],addr_lo,addr_lo+data_bytes-1)
		||(__PE__==1&&__VM__==0 &&DPL(descriptors[m_info.seg])>max(__CPL__,RPL(selector(m_info.seg)))))
	{
		log("write addr=%x,limit=%x,cpl=%x,rpl=%x,dpl=%x\n",addr_lo,LIMIT(descriptors[m_info.seg]),__CPL__,RPL(selector(m_info.seg)),DPL(descriptors[m_info.seg]));
		if(m_info.seg==SS) SS_(0)
		else GP_(0)

		return false;
	}
	//
	// request mmu. raise #PF.
	//
	RW_STATUS status;
	if(status=mmu.st(addr_lo+BASE(descriptors[m_info.seg]),__CPL__==3,(char *)&data,data_bytes))
	{
		PFT_(status&0x1f);
		return false;
	}

	return true;

}
bool CCPU::read_mem_raw(char seg,unsigned long addr_hi,unsigned long addr_lo,unsigned long & data, int data_size,int addr_size)
{
	int data_bytes;

	assert(seg<MAX_SEG);

	assert((data_size&7)==0);
	data_bytes=data_size>>3;
	//if(real_v86()){addr_hi=0;addr_lo&=0xffff;}
	//
	// limit check
	//
	if(!check_limit(&descriptors[seg],addr_lo,addr_lo+data_bytes-1)
		||(__PE__==1&&__VM__==0 &&DPL(descriptors[seg])>max(__CPL__,RPL(selector(seg)))))
	{
		log("read addr=%x,limit=%x,cpl=%x,rpl=%x,dpl=%x\n",addr_lo,LIMIT(descriptors[seg]),__CPL__,RPL(selector(seg)),DPL(descriptors[seg]));

		if(seg==SS) SS_(0)
		else GP_(0)

		return false;
	}
	//
	// request mmu. raise #PF.
	//
	RW_STATUS status;
	if(status=mmu.ld(addr_lo+BASE(descriptors[seg]),__CPL__==3,(char *)&data,data_bytes))
	{
		PFT_(status&0x1f);
		return false;
	}

	return true;

}

// linear address= base(descriptor)+offset.
// limit check
//
// #GP(0) if the result is in a nonwritable segment; 
// #GP(0) for an illegal memory operand effective address in the CS, DS, ES, FS, or GS segments; 
// #SS(0) for an illegal address in the SS segment;
// #PF(fault-code) for a page fault 
//
bool CCPU::write_mem_raw(char seg,unsigned long addr_hi,unsigned long addr_lo,unsigned long  data, int data_size,int addr_size)
{
	int data_bytes;

	assert(seg<MAX_SEG);

	assert(0==(data_size&7));
	data_bytes=data_size>>3;
	//if(real_v86()){addr_hi=0;addr_lo&=0xffff;}
	//
	// limit check
	//
	if(!check_limit(&descriptors[seg],addr_lo,addr_lo+data_bytes-1)
		||(__PE__==1&&__VM__==0 &&DPL(descriptors[seg])>max(__CPL__,RPL(selector(seg)))))
	{
		if(seg==SS) SS_(0)
		else GP_(0)

		return false;
	}
	//
	// request mmu. raise #PF.
	//
	RW_STATUS status;
	if(status=mmu.st(addr_lo+BASE(descriptors[seg]),__CPL__==3,(char *)&data,data_bytes))
	{
		PFT_(status&0x1f);
		return false;
	}

	return true;

}
void CCPU::gen_mem_addr(MEM_DESC &m_info,int addr_size,unsigned long & hi, unsigned long & lo)
{
	REG_CODE reg;
	unsigned long base,index, a,b;

	assert(addr_size==16||addr_size==32);
	reg.reg_cat=addr_size==0x20?RC_REGG32:RC_REGG16;

	if(m_info.fmt==AT_DIRECT)
	{
		lo=addr_size==16?(m_info.disp&0xffff):m_info.disp;
		hi=0;
	}
	else
	{
		if(m_info.fmt&AT_BASE)
		{
			reg.reg_no=m_info.base_reg_no;
			base=reg_file.read_reg(reg);
			a=base>>24;
			b=base&0xffffff;
		} else {a=b=0;}

		if(m_info.fmt&AT_INDEX)
		{
			reg.reg_no=m_info.index_reg_no;
			index=reg_file.read_reg(reg);
			a+=(index>>24)<<m_info.scale;
			b+=(index&0xffffff)<<m_info.scale;
		}


		a+=m_info.disp>>24;
		b+=m_info.disp&0xffffff;
		a+=(b>>24);
		b&=0xffffff;
		lo=b|((a&0xff)<<24);
		hi=a>>8;

		if( addr_size==16){hi=0;lo&=0xffff;}
	}
}

void CCPU::gen_mem_addr_raw(unsigned long base,unsigned long disp,int addr_size,unsigned long & hi, unsigned long & lo)
{
	unsigned long  a,b;

	assert(addr_size==16||addr_size==32);


	a=base>>24;
	b=base&0xffffff;


	a+=disp>>24;
	b+=disp&0xffffff;

	a+=(b>>24);
	b&=0xffffff;

	lo=b|((a&0xff)<<24);
	hi+=(a>>8);

	if( addr_size==16){hi=0;lo&=0xffff;}
}




bool   CCPU::read_string(MEM_DESC &m_info,char * data, int data_bytes,int addr_size)
{
	unsigned long addr_hi,addr_lo;

	assert(m_info.seg<MAX_SEG);
	gen_mem_addr(m_info,addr_size,addr_hi,addr_lo);

	//
	// limit check
	//
	if(!check_limit(&descriptors[m_info.seg],addr_lo,addr_lo+data_bytes-1)
		||(__PE__==1&&__VM__==0 &&DPL(descriptors[m_info.seg])>max(__CPL__,RPL(selector(m_info.seg)))))
	{
		if(m_info.seg==SS) SS_(0)
		else GP_(0)

		return false;
	}
	//
	// request mmu. raise #PF.
	//
	RW_STATUS status;
	if(status=mmu.read(addr_lo+BASE(descriptors[m_info.seg]),false,__CPL__==3,data,data_bytes))
	{
		PFT_(status&0x1f);
		return false;
	}

	return true;

}
bool   CCPU::write_string(MEM_DESC &m_info,char * data, int data_bytes,int addr_size)
{
	unsigned long addr_hi,addr_lo;

	assert(m_info.seg<MAX_SEG);
	gen_mem_addr(m_info,addr_size,addr_hi,addr_lo);

	//
	// limit check
	//
	if(!check_limit(&descriptors[m_info.seg],addr_lo,addr_lo+data_bytes-1)
		||(__PE__==1&&__VM__==0 &&DPL(descriptors[m_info.seg])>max(__CPL__,RPL(selector(m_info.seg)))))
	{
		if(m_info.seg==SS) SS_(0)
		else GP_(0)

		return false;
	}
	//
	// request mmu. raise #PF.
	//
	RW_STATUS status;
	if(status=mmu.write(addr_lo+BASE(descriptors[m_info.seg]),__CPL__==3,data,data_bytes))
	{
		PFT_(status&0x1f);
		return false;
	}

	return true;
}


//
// for debug use, no exception is reported.
//

bool CCPU::simulator_read_mem(MEM_DESC &m_info,unsigned long & data, int data_size,int addr_size)
{
	unsigned long addr_hi,addr_lo;
	int data_bytes;

	assert(m_info.seg<MAX_SEG);
	gen_mem_addr(m_info,addr_size,addr_hi,addr_lo);

	assert(data_size==16||data_size==32||data_size==8);
	data_bytes=data_size>>3;

	//
	// request mmu. raise #PF.
	//
	RW_STATUS status;
	if(status=mmu.ld(addr_lo+BASE(descriptors[m_info.seg]),0,(char *)&data,data_bytes))
	{
		return false;
	}

	return true;

}