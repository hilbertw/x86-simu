#ifndef __Cache_if_H__
#define __Cache_if_H__
#include "biu.h"
#include "cache.h"
#include "memory-if.h"
#include "config.h"

typedef unsigned char CCacheLine [32];
typedef cache_n_way<CFG_L1_LINES,unsigned long,CCacheLine> L1Cache;
//
// L2 cache
//
class CCacheInterface
{
	CMemoryInterface *p_memory; // physical memory
	L1Cache L1Data,L1Code;
	//cache_n_way<CFG_L2_LINES,unsigned long,CCacheLine> L2;
	//CCacheLine write_buffer;
	//DWORD wr_buffer_address_lo;
	//DWORD wr_buffer_address_hi;
public:
	CCacheInterface(CMemoryInterface*p_memory_):p_memory(p_memory_) {}

	bool fetch(DWORD address_lo,DWORD address_hi, char *buffer);
	//char mem_read_byte(DWORD address);
	//short mem_read_short(DWORD address);
	int cache_read_dword(DWORD address_lo,DWORD address_hi);

	//void mem_write_byte(DWORD address,char c,);
	//void mem_write_short(DWORD address,short w);
	void cache_write_dword(DWORD address_lo,DWORD address_hi,DWORD dat,DWORD byte_enable);
	void invalid_all();
	void write_back();
private:
	CCacheLine* check_l1_line(L1Cache *l1,DWORD address_lo,DWORD address_hi);
};
#endif //__Cache_if_H__