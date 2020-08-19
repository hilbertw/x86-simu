#include <stdio.h>
#include <string.h>
#include "mmu-ifetcher.h"
#include "log.h"
void CIFetcherMMU::fill_buff()
{
	if(buffer_id==3)
	{
		memcpy(lines[0], lines[2],FETCH_BUFF_SIZE);
		buffer -=2*FETCH_BUFF_SIZE;
		buffer_id=1;
	}

	mmu_fault= p_mmu->fetch(base + line_offset,user, (char *)lines[buffer_id],FETCH_BUFF_SIZE);
	if(mmu_fault!=CMMU::PAGE_FAULT_NONE) 
	{	
		mmu_fault_addr=base + line_offset;
		goto ret;
	}
	bytes+=FETCH_BUFF_SIZE;
	line_offset += FETCH_BUFF_SIZE;


	buffer_id++;

		

ret: return;

}


void CIFetcherMMU::set_fetch_address(bool mode, unsigned int _base_,unsigned int _limit_,unsigned  int _ip_, bool _user_)
{
	address16_mode = mode;
	base = _base_;
	user = _user_;
	limit=_limit_;	 
	ip = _ip_;

	if (address16_mode)
	{
		ip = _ip_ & 0xffff;
	}
	
	line_offset = ip & ~(FETCH_BUFF_SIZE-1);
	buffer_id = 0;
	buffer=&lines[0][0];
	bytes=0;
	fill_buff();

	int start=ip & (FETCH_BUFF_SIZE-1);
	bytes-=start;
	buffer+=start;

}

int CIFetcherMMU::fetch(PINSN_BASIC_INFO p_insn)
{

	//
	//need fill the next buffer?
	//
	while( bytes <FETCH_BUFF_SIZE) 
	{
		fill_buff();
		if(mmu_fault!=CMMU::PAGE_FAULT_NONE) break;
	}
	//
	// fetch from buffer
	//
	return CFetcherBase::fetch( p_insn);
}
