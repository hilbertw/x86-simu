#define FETCH_BUFF_SIZE 16

#ifndef __MMUFECTH_H__
#define __MMUFECTH_H__

#include "ifetcher.h"
#include "mmu.h"

class CIFetcherMMU: public CFetcherBase
{
private:
	 unsigned char lines[3][FETCH_BUFF_SIZE]; 
							// buffer[0] == buffer[2] so that a circular buffer is simulated without MOD
	unsigned int line_offset;      // line to read from mem
	int buffer_id;
	bool user;
	unsigned int base;
	unsigned int limit;
	RW_STATUS mmu_fault;
	unsigned long mmu_fault_addr;
public:
	CMMU *p_mmu;

public:
	CIFetcherMMU (CMMU *_mmu) : p_mmu(_mmu)	{} 
	CIFetcherMMU () {} //don't init others

	void set_fetch_address(bool mode,unsigned int base,unsigned int limit, unsigned int ip, bool user);
	int fetch(PINSN_BASIC_INFO p_insn);
	void fill_buff();
	RW_STATUS fault() { return mmu_fault&0x1f;}
	unsigned long fault_addr() {return mmu_fault_addr;};
};


#endif