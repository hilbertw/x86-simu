/* class to simulate a TLB.
   Copyright (C) 2005 Huaibo Wang.
*/
#ifndef __TLB_H__
#define __TLB_H__
#include "pg-table.h"
#define N_ENTRIES 64
//
//TLB entry
//    int     vpn;        /* virtual page number */
//    int     frame;      /* ppn */
//    uchar   page_attr;  /* protection or Dirty/Ref/read/write/asid */
//

typedef struct tlb_entry_def {
	union {
    PTE        pte;
	PDE4M      pde4m;
	PDE2M      pde2m;
	PDE4K      pde4k;
	PTE4K      pte4k;
	PAE32_PTE4K pae32_pte4k;
	PAE32_PDE2M pae32_pde2m;
	PSE36_PDE4M pse36_pde4m;
	};
	unsigned long  type:3;
	unsigned long  flag:2;          /*global? valid? */
	unsigned long  vpn;
	//unsigned long  maddr;           // for experiments. where to read

} TLB_ENTRY;

typedef TLB_ENTRY *TLB_HANDLE;	

#define TLB_ENTRY_VALID       1
#define TLB_ENTRY_GLOBAL      2
typedef enum 
{
 TLB_ENTRY_PTE4K     = 1,
 TLB_ENTRY_PDE4M     = 2,
 TLB_ENTRY_PAE32_PTE4K      =3,
 TLB_ENTRY_PAE32_PDE2M      =4,
 TLB_ENTRY_PSE36_PTE4K      =5,
 TLB_ENTRY_PSE36_PDE4M      =6,
} TLB_ENTRY_TYPE;


#define VALID(x)         ((x).flag & TLB_ENTRY_VALID)
#define MODIFIED(x)      ((x).flag & TLB_ENTRY_MODIFIED)

#define MARK_VALID(x)    ((x).flag |= TLB_ENTRY_VALID)
#define MARK_MODIFIED(x) ((x).flag |= TLB_ENTRY_MODIFIED)

#define MARK_INVALID(x)  ((x).flag &= ~TLB_ENTRY_VALID)

class CTLB {
	TLB_ENTRY tlb [N_ENTRIES];
	int       age [N_ENTRIES];

public:
	CTLB(); 

	void invalid (unsigned int vaddr);

	bool hit (unsigned int vaddr, PTE *pte,TLB_ENTRY_TYPE *p_type);

	void insert (unsigned int vaddr, PTE *pte,TLB_ENTRY_TYPE type);

	void invalid_all (bool global);



private:
	void increase_age();
};




#endif   /*  __TLB_H__  */