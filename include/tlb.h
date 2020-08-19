/* class to simulate a TLB.
   Copyright (C) 2005 Huaibo Wang.
*/
#ifndef __TLB_H__
#define __TLB_H__

//
// PDE
//
//  31-12: Page table address 31..12
//  11-9 : Reserved
// 8  0
// 7  0 
// 6  D
// 5  A
// 4  0
// 3  0
// 2  U/S
// 1  R/W
// 0   P
//
//
typedef struct pte_pde_def
{
	unsigned int present : 1;
	unsigned int read_write : 1;
	unsigned int user_supervisor : 1;
	unsigned int pwt : 1;         // page write through
	unsigned int pcd : 1;	      // page cache disabled
	unsigned int accessed : 1;	
	unsigned int dirty : 1;	

	unsigned int zero1 : 1;
	unsigned int page_size : 1;	  // 4K or 4M page
	unsigned int zero0 : 1;

    unsigned int avl : 2;         //available to software
	unsigned int table_address_hi : 20;
} PDE,PTE,PXE;

#define BASE_ADDRESS(pde)      ((pde).table_address_hi<<12)
#define PAGE_ADDRESS(pte)      ((pte).table_address_hi<<12)
#define PAGE_PRESENT(pte)      ((pte).present==1)
#define PAGE_USER(pxe)         ((pxe).user_supervisor==0)
#define PAGE_WRITEABLE(pxe)    ((pxe).read_write==1)
#define PAGE_NUMBER(addr)      ((addr)>>12)

#define PHYSICAL_ADDRESS(pte,address) (((pte).table_address_hi<<12) | ((address) & 0x00000fff))


//
//TLB entry
//    int     vpn;        /* virtual page number */
//    int     frame;      /* ppn */
//    uchar   page_attr;  /* protection or Dirty/Ref/read/write/asid */
//

typedef struct tlb_entry_def {
	PTE          pte;           /* ppn */
	unsigned int tag:20;        /* vpn */
	char         flag;          /* significant change? valid? */

} TLB_ENTRY;

typedef TLB_ENTRY *TLB_HANDLE;	

#define TLB_ENTRY_VALID    1
#define TLB_ENTRY_MODIFIED 2

#define VALID(x)         ((x).flag & TLB_ENTRY_VALID)
#define MODIFIED(x)      ((x).flag & TLB_ENTRY_MODIFIED)

#define MARK_VALID(x)    ((x).flag |= TLB_ENTRY_VALID)
#define MARK_MODIFIED(x) ((x).flag |= TLB_ENTRY_MODIFIED)

#define MARK_INVALID(x)  ((x).flag &= ~TLB_ENTRY_VALID)

template <int N_ENTRIES>
class TTLB {
	TLB_ENTRY tlb [N_ENTRIES];
	int       age [N_ENTRIES];

public:
	TTLB(); 

	void invalid (unsigned int num);

	bool hit (unsigned int vpn, TLB_HANDLE *tlb_entry);

	TLB_HANDLE insert (unsigned int vpn, PTE *pte);

	void clear_all ();

	void set_accessed(TLB_HANDLE tlb_entry)
	{
		
		if(!tlb_entry->pte.accessed)
		{
			tlb_entry->pte.accessed = 1;

			MARK_MODIFIED(*tlb_entry);
		}
	}
	void set_modified(TLB_HANDLE tlb_entry)
	{
		if(!tlb_entry->pte.dirty)
		{
			tlb_entry->pte.dirty = 1;

			MARK_MODIFIED(*tlb_entry);
		}
	}

	PTE get_pte(TLB_HANDLE tlb_entry) 
	{
		return tlb_entry->pte;
	}

private:
	void increase_age();
};



typedef TTLB<64> CTLB;

#endif   /*  __TLB_H__  */