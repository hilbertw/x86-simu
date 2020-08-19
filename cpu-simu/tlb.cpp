/* Functions to simulate a TLB.
   Copyright (C) 2005 Huaibo Wang.
*/
#include "log.h"
#include "tlb.h"

#undef LOG_TLB
#define LOG_TLB 1

#define DB_ADDR 0x40000600

#ifdef __GNUC__

unsigned long long   sum_tlb_hit;
unsigned  long long  sum_tlb_miss;

#else

unsigned __int64  sum_tlb_hit;
unsigned  __int64  sum_tlb_miss;

#endif



CTLB::CTLB()
{
	invalid_all (false);
}
unsigned long vpn_mask[]={0,~((1<<12)-1),~((1<<22)-1),
~((1<<12)-1),~((1<<21)-1),~((1<<12)-1),~((1<<22)-1)};


bool CTLB::hit (unsigned int vaddr, PTE *pte,TLB_ENTRY_TYPE * p_type)
{
	unsigned int i;

	for (i=0; i<N_ENTRIES; i++) {
		
		unsigned long vpn=vaddr&vpn_mask[tlb[i].type];

		if (VALID(tlb [i]) && tlb [i].vpn == vpn) {
			switch(tlb [i].type)
			{
			case TLB_ENTRY_PTE4K:
				*(PTE4K*)pte=tlb [i].pte4k;
				break;
			case TLB_ENTRY_PDE4M:
				*(PDE4M*)pte=tlb [i].pde4m;
				break;
			case TLB_ENTRY_PAE32_PTE4K:
				*(PAE32_PTE4K*)pte=tlb [i].pae32_pte4k;
				break;
			case TLB_ENTRY_PAE32_PDE2M:
				*(PAE32_PDE2M*)pte=tlb [i].pae32_pde2m;
				break;
			case TLB_ENTRY_PSE36_PTE4K:
				*(PTE4K*)pte=tlb [i].pte4k;
				break;
			case TLB_ENTRY_PSE36_PDE4M:
				*(PSE36_PDE4M*)pte=tlb [i].pse36_pde4m;
				break;
			default:
				log0("bad tlb entry type:%d %d %x,\n",i,tlb [i].type,tlb [i].vpn);
				break;

			}
			*p_type=(TLB_ENTRY_TYPE)tlb [i].type;
#if LOG_TLB
			if(vaddr==DB_ADDR)
			log0("%x TLB hit.%d,%d\n",vaddr,i,tlb [i].type);
#endif
			//
			// increase the counter of other entries
			//
			if(age[i]!=0) 
			{
				increase_age();
				age[i]=0;
			}
			sum_tlb_hit++;
			return true;
		}
	}
#if LOG_TLB
	if(vaddr==DB_ADDR)
	log0("%x TLB miss\n",vaddr);
#endif

	sum_tlb_miss++;
	return false;
}


void CTLB::insert (unsigned int vaddr, PTE *pte,TLB_ENTRY_TYPE type)
{
	//
	// find a invalid entry
	//
	unsigned int i;

	for (i=0; i<N_ENTRIES; i++) {
		if(!VALID(tlb [i]))
		{
			break;
		}
		if(age[i]>=N_ENTRIES)
		{
			break;
		}
	}
#if 0
	//
	// if have to replace an entry, find the oldest one
	//
	if (i==N_ENTRIES)
	{
		int max_age_index=0;
		for (i=1; i<N_ENTRIES; i++) {
			if(age [i]>age[max_age_index])
			{
				max_age_index=i;
				break;
			}
		}
		// entry i is to be replaced.
		i=max_age_index;
	}
#else
	if (i==N_ENTRIES)
	{
		log0("lru failed.\n");
		i=0;   // for experiment, always make entry 0 the victim.
	}
#endif
	// update the entry
	//
	age [i] = 0;
	tlb [i].vpn = vaddr&vpn_mask[type];
	tlb [i].type=type;

	MARK_VALID(tlb [i]); 

	switch(type)
	{

	case TLB_ENTRY_PTE4K:
		tlb [i].pte4k=  *(PTE4K*)pte;
		break;
	case TLB_ENTRY_PDE4M:
		tlb [i].pde4m= *(PDE4M*)pte;
		break;
	case TLB_ENTRY_PAE32_PTE4K:
		tlb [i].pae32_pte4k= *(PAE32_PTE4K*)pte;
		break;
	case TLB_ENTRY_PAE32_PDE2M:
		tlb [i].pae32_pde2m= *(PAE32_PDE2M*)pte;
		break;
	case TLB_ENTRY_PSE36_PTE4K:
		tlb [i].pte4k=  *(PTE4K*)pte;
		break;
	case TLB_ENTRY_PSE36_PDE4M:
		tlb [i].pse36_pde4m= *(PSE36_PDE4M*)pte;
		break;
	default:
		MARK_INVALID(tlb [i]); 
		log0("bad tlb entry type to insert:%d\n",type);
	}

	//
	// increase age
	//
	increase_age();
#if LOG_TLB
	if(vaddr==DB_ADDR)
	log0("%x TLB insert to %d,%d\n",vaddr,i,type);
#endif

}
//
// writint to cr3,  non global tlb entries are flushed.
// writing to cr0.pg,cr4.ode,pae,pse, cause all tlb entries be flushed
//

void CTLB::invalid_all ( bool global)
{
	unsigned int i;

	for (i=0; i<N_ENTRIES; i++) {
		MARK_INVALID(tlb [i]); 
	}
}


void CTLB::increase_age ()
{
	unsigned int i;

	for (i=0; i<N_ENTRIES; i++) {
		if(VALID(tlb [i])&& age[i]<N_ENTRIES) age[i]++;
	}
}





void CTLB::invalid (unsigned int vaddr)
{
	unsigned int i;

	for (i=0; i<N_ENTRIES; i++) {
		
		unsigned long vpn=vaddr&vpn_mask[tlb[i].type];

		if (VALID(tlb [i]) && tlb [i].vpn == vpn) {


			MARK_INVALID(tlb [i]);
			break;
		}
	}
#if LOG_TLB
	log0("invalidate tlb :%x\n",vaddr);
#endif
}