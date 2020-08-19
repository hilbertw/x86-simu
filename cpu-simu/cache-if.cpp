#include "cache-if.h"

#define CACHE_LINE_TAG(a,b)  (a>>13)
#define CACHE_LINE_INDEX(a,b) (a&0x1fff)

bool CCacheInterface::fetch(DWORD address_lo,DWORD address_hi, char *buffer)
{
	//
	// hit?
	//
}
int CCacheInterface::cache_read_dword(DWORD address_lo,DWORD address_hi)
{
	//
	// hit?
	//
}
void CCacheInterface::cache_write_dword(DWORD address_lo,DWORD address_hi,DWORD dat,DWORD byte_enable)
{
	//
	// hit?
	//

	//
	// l1 icache sync
	//
}

CCacheLine* CCacheInterface::check_l1_line(L1Cache *l1,DWORD address_lo,DWORD address_hi)
{
	CCacheLine cache_line,*p_cache_line;

	//
	// load from L2 if miss in l1
	//
	if(!l1->hit(CACHE_LINE_TAG( address_lo, address_hi),&p_cache_line,CACHE_LINE_INDEX( address_lo, address_hi)))
	{
		//
		// if miss in L2, 
		//
		if(!L2.hit(CACHE_LINE_TAG( address_lo, address_hi),&p_cache_line,CACHE_LINE_INDEX( address_lo, address_hi)))
		{
			DWORD wr_buffer_address_lo;
			DWORD wr_buffer_address_hi;
			//
			// get/alloc L2 line to fill
			//
			if(L2.get_old_line(CACHE_LINE_TAG( address_lo, address_hi),&cache_line,CACHE_LINE_INDEX( address_lo, address_hi),wr_buffer_address_lo,wr_buffer_address_hi))
			{
				//
				// swap out L2 line if necessary
				//
				p_memory->mem_write_string(wr_buffer_address_lo,cache_line,sizeof cache_line);
			}

			//
			// read memory to cache_line
			//
			p_memory->mem_read_string(address_lo,(char *)cache_line,sizeof cache_line);
			//
			//update cache line ages
			//

		}

		//
		// get/alloc L2 line to fill
		//
		if(L1.get_old_line(CACHE_LINE_TAG( address_lo, address_hi),&cache_line,CACHE_LINE_INDEX( address_lo, address_hi),wr_buffer_address_lo,wr_buffer_address_hi))
		{
			//
			// swap out L1 line to L2if necessary
			//
			p_memory->mem_write_string(wr_buffer_address_lo,cache_line,sizeof cache_line);
		}
		//
		// fill line of L1
		//
		p_cache_line=l1->fill_line(CACHE_LINE_TAG( address_lo, address_hi),p_cache_line,CACHE_LINE_INDEX( address_lo, address_hi));
	}

	return p_cache_line;
}

void CCacheInterface::invalid_all()
{
}
void CCacheInterface::write_back()
{
}

