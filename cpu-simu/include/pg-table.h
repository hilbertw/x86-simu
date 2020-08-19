/* class to simulate a TLB.
   Copyright (C) 2005 Huaibo Wang.
*/
#ifndef __pg_table__
#define __pg_table__

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

typedef struct pte_4k_def
{
	unsigned int present : 1;
	unsigned int read_write : 1;
	unsigned int user_supervisor : 1;
	unsigned int pwt : 1;         // page write through
	unsigned int pcd : 1;	      // page cache disabled
	unsigned int accessed : 1;	
	unsigned int dirty : 1;	

	unsigned int pat : 1;	  // 4K or 4M page
	unsigned int G : 1;       //global

    unsigned int avail : 3;         //available to software
	unsigned int table_address_hi : 20;
} PTE,PTE4K;

typedef struct pde4k_def
{
	unsigned int present : 1;
	unsigned int read_write : 1;
	unsigned int user_supervisor : 1;
	unsigned int pwt : 1;         // page write through
	unsigned int pcd : 1;	      // page cache disabled
	unsigned int accessed : 1;	
	unsigned int dirty : 1;  // available

	unsigned int page_size : 1;	  // 4K or 4M page
	unsigned int G : 1;

    unsigned int availl : 3;         //available to software
	unsigned int table_address_hi : 20;
} PDE,PDE4K;

typedef struct pde2m_def
{
	unsigned int present : 1;
	unsigned int rsved : 2;
	unsigned int pwt : 1;         // page write through
	unsigned int pcd : 1;	      // page cache disabled
	unsigned int accessed : 1;	
	unsigned int dirty : 1;	

	unsigned int page_size : 1;	  // 4K or 4M page
	unsigned int G : 1;
    unsigned int avail : 3;         //available to software
	unsigned int pat : 1;
 
	unsigned int reserved : 8;
	unsigned int table_address_hi : 11;
} PDE2M;

typedef struct pde4m_def
{
	unsigned int present : 1;
	unsigned int read_write : 1;
	unsigned int user_supervisor : 1;
	unsigned int pwt : 1;         // page write through
	unsigned int pcd : 1;	      // page cache disabled
	unsigned int accessed : 1;	
	unsigned int dirty : 1;	

	unsigned int page_size : 1;	  // 4K or 4M page
	unsigned int G : 1;
    unsigned int avail : 3;         //available to software
	unsigned int pat : 1;
 
	unsigned int reserved : 9;
	unsigned int table_address_hi : 10;
} PDE4M;
//
// PAE for 32 bit physical address
//
typedef struct pae32_pde4k_def
{
	PDE4K old;
	unsigned int  base_hi;
}PAE32_PDE4K;

typedef struct pae32_pte4k_def
{
	PTE4K old;
	unsigned int  base_hi;
}PAE32_PTE4K;


typedef struct pae32_pde2m_def
{
	PDE2M old;
	unsigned int  base_hi;
}PAE32_PDE2M;

//typedef struct pae32_pte2m_def
//{
//	PTE2M old;
//	unsigned int  base_hi;
//}PAE32_PTE2M;

//
// PAE for 36 bit physical address
//

typedef struct pse36_pde4m_def
{
	unsigned int present : 1;
	unsigned int read_write : 1;
	unsigned int user_supervisor : 1;
	unsigned int pwt : 1;         // page write through
	unsigned int pcd : 1;	      // page cache disabled
	unsigned int accessed : 1;	
	unsigned int dirty : 1;	

	unsigned int page_size : 1;	  // 4K or 4M page
	unsigned int G : 1;
    unsigned int avail : 3;         //available to software
	unsigned int pat : 1;
 	unsigned int ba_hi : 4;
	unsigned int reserved : 5;

	unsigned int table_address_hi : 10;
}PSE36_PDE4M;
//
//IA-32e mode
//
typedef struct ia32e_pde4k_def
{
	PDE4K old;
	unsigned int  base_hi;
}IA32e_PDE4K;



typedef struct ia32e_pte4k_def
{
	PTE4K old;
	unsigned int  base_hi;
}IA32e_PTE4K,IA32e_PDPTE4K,IA32e_PM4L_4K;


typedef struct ia32e_pde2m_def
{
	PDE2M old;
	unsigned int  base_hi;
}IA32e_PDE2M;

//typedef struct ia32e_pte2m_def
//{
//	PTE2M old;
//	unsigned int  base_hi;
//}IA32e_PTE2M,IA32e_PDPTE2M,IA32e_PM4L_2M;

typedef struct _pdpte_def
{
	unsigned int present : 1;
	unsigned int reserved : 2;
	unsigned int pwt : 1;         // page write through
	unsigned int pcd : 1;	      // page cache disabled
	unsigned int accessed : 1;	
	unsigned int avl : 1;	

	unsigned int zero : 2;	  // 
    unsigned int avail : 3;         //available to software

	unsigned int pd_base_lo : 20;
	unsigned int pd_base_hi;
}PDPTE;

#define BASE_ADDRESS36(pde)      ((pde).table_address_hi<<22)
#define BASE_ADDRESS(pde)      ((pde).table_address_hi<<12)
#define PAGE_ADDRESS(pte)      ((pte).table_address_hi<<12)
#define PAGE_PRESENT(pte)      ((pte).present==1)
#define PAGE_USER(pxe)         ((pxe).user_supervisor==1)
#define PAGE_WRITEABLE(pxe)    ((pxe).read_write==1)
#define PAGE_NUMBER(addr)      ((addr)>>12)

#define PHYSICAL_ADDRESS(pte,address) (((pte).table_address_hi<<12) | ((address) & 0x00000fff))
#define PHYSICAL_ADDRESS_4M(pde,address) (((pde).table_address_hi<<22) | ((address) & 0x003fffff))
#define PHYSICAL_ADDRESS_2M(pde,address) (((pde).table_address_hi<<21) | ((address) & 0x001fffff))

typedef PTE4K PXE;
#endif   /*  __pg_table__  */