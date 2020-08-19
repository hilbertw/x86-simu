#include "mmu.h"
#include "log.h"
 CMMU::to_physical_address(unsigned int linear_address,bool code, 
							   unsigned int &physical_address,PTE       **pp_pte)
{
	if (_cr0_.pg==0)
	{
		physical_address = linear_address&a20m;

		*(int)&tmp_pte=3; // writeable, present,user
		tmp_pte.pcd=!_cr0_.cd;
		tmp_pte.pwt=!_cr0_.nw;

		*pp_pte=&tmp_pte;
		return true;
	}
    

	PTE * pte;
	bool page_fault = !get_tlb_entry (linear_address,code,user, false,pte);

	//
	// if page fault or not present, page fault
	//
	if( page_fault)
	{
		return false;
	}

	physical_address = PHYSICAL_ADDRESS(*pte, linear_address);
	physical_address&=a20m;

	*pp_pte=pte;
	return true;
}

bool CMMU::read(unsigned int linear_address,bool code, bool user, char *buffer, int count)
{
	bool pcd,pwt,page_size_4M;
	unsigned int physical_address;

	PTE *pte;


	//
	// check page boundary.
	//
	while (count)
	{
		//
		// address translate
		//
		if (!to_physical_address(linear_address, code,user, false,physical_address,pte))
		{
			// not present, read
			_cr2_ = linear_address;
			return (user?PAGE_FAULT_USER)|PAGE_FAULT_NOTPRESENT;
		}
		//
		// previlige check
		//
		if(!check_page_fault(pte,user,write)
		{
			// not present, read
			_cr2_ = linear_address;
			return (user?PAGE_FAULT_USER)|PAGE_FAULT_PROT;
		}

		 pcd = pte.pcd;
		 pwt = pte.pwt;
		 page_size_4M = pte.page_size;
		int page_bounary = page_size_4M ? 0x400000: 0x1000 ;
		int max_bytes = (linear_address | (page_bounary-1)) - linear_address + 1;

		if (max_bytes>count) max_bytes = count;

		// if cacheable, read from cache
		if( pcd)
		{
			// cache not implemented as yet
			p_memory->mem_read_string (physical_address,0,buffer,max_bytes);
		}
		else
		{
			p_memory->mem_read_string (physical_address,0,buffer,max_bytes);
		}
		count -= max_bytes;
		linear_address+=max_bytes;
		buffer+=max_bytes;

	}
	return PAGE_FAULT_NONE;
}

bool CMMU::write(unsigned int linear_address, bool user, char *buffer, int count)
{
	bool pcd,pwt,page_size_4M;
	unsigned int physical_address;
	//
	// check page boundary.
	//
	while (count)
	{
		// we will never write to a itlb
		if (!to_physical_address(linear_address, false,user, true,physical_address,page_size_4M,pcd,pwt))
		{
			return false;
		}
		int page_bounary = page_size_4M ? 0x1000 : 0x400000;
		int max_bytes = (linear_address | (page_bounary-1)) - linear_address + 1;

		if (max_bytes>count) max_bytes = count;

		// if cacheable, read from cache
		if( pcd)
		{
			p_memory->mem_write_string (physical_address,0,buffer,max_bytes);
		}
		else
		{
			p_memory->mem_write_string (physical_address,0,buffer,max_bytes);
		}
		count -= max_bytes;
		linear_address+=max_bytes;
		buffer+=max_bytes;
	}
	return true;
}
bool CMMU::fetch(unsigned int linear_address, bool user, char *buffer, int count)
{
	return read(linear_address,true,user,buffer,count);
}

bool CMMU::ld(unsigned int linear_address, bool user, char *buffer, int count)
{
	//printf("ld:%x %x\n",linear_address,count);
	return (count==1)?ldb(linear_address,user,(unsigned char *)buffer):
		(count==2)?ldw(linear_address,user,(unsigned short *)buffer):
		(count==4)?lddw(linear_address,user,(unsigned long *)buffer):
		/**/read(linear_address,false,user,buffer,count);
}

bool CMMU::st(unsigned int linear_address, bool user, char *buffer, int count)
{
	//if(linear_address>=0xb800&&linear_address<0xb820)
	//log4("st",'w',linear_address,*(unsigned long *)buffer);
	//if(linear_address>=0xa0000&&linear_address<0xa0020)
	//log4("st",'0'+count,linear_address,*(unsigned long *)buffer);
	return (count==1)?stb(linear_address,user,*(unsigned char *)buffer):
		(count==2)?stw(linear_address,user,*(unsigned short *)buffer):
		(count==4)?stdw(linear_address,user,*(unsigned long *)buffer):
		/**/write(linear_address,user,buffer,count);
}


bool CMMU::do_address_translation(unsigned int linear_address,bool user, bool write, PTE *out_pte)
{
	bool pwt,pcd;
	
	PTE  pte;
	PDE  pde;

	int  data;

	// load PDE
	pwt = _cr3_.pwt == 1;
	pcd = _cr3_.pcd == 1;

	int pde_address = (_cr3_.pdbr<<12) + (linear_address>>22) * 4;
	//
	// if pcd is true, check cache for
	//

	//
	//  pcd is true but cache miss, read from mem, and cache it
	//

	//
	// pcd is disabled, read from mem directly, and don't cache ir
	//
	data = p_memory->mem_read_int( pde_address);

	pde = *(PDE *) &data;

	//
	// if not present, page fault
	//
	if(!check_page_fault(pde,user,write))
	{
		return false;
	}
	// load PTE from mem ( (pde & 0xfffff000) + 4*((la >> 12) & 0x3ff));
	pwt = pde.pwt == 1;
	pcd = pde.pcd == 1;

	int pte_address = BASE_ADDRESS(pde) + 4 * ((linear_address >> 12) & 0x003ff) ;
	//
	// if pcd is true, check cache for
	//

	//
	//  pcd is true but cache miss, read from mem, and cache it
	//

	//
	// pcd is disabled, read from mem directly, and don't cache ir
	//	
	
	data = p_memory->mem_read_int( pte_address);
	pte = *(PTE *) &data;

	*out_pte = pte;
	return true;
}


bool CMMU::get_tlb_entry(unsigned int linear_address,bool code, bool user, bool write,PTE *out_pte)
{
	bool page_fault;

	PTE pte;
	TLB_HANDLE tlb_handle;
	CTLB & tlb = code ? itlb : dtlb;
	//
	// hits TLB?
	//
	if (tlb.hit(PAGE_NUMBER(linear_address), &tlb_handle))
	{
		pte = tlb.get_pte(tlb_handle);
		page_fault = !check_page_fault(pte,user,write);
	}
	else
	//
	// miss, do 
	//
	{
		page_fault = !do_address_translation(linear_address,user,write, &pte);
		if (!page_fault)
		{
			//
			// add to DTLB
			//
			tlb_handle = tlb.insert(PAGE_NUMBER(linear_address), &pte);
			page_fault = !check_page_fault(pte,user,write);
		}
	}

	//
	// page fault?
	//
	if (page_fault)
	{
		_cr2_ = linear_address;
		return false;   // page fault
	}

	//
	// tlb now has the mapping
	//
	*out_pte = tlb.get_pte(tlb_handle);
    tlb.set_accessed(tlb_handle);

	if (write) 
		tlb.set_modified(tlb_handle);

	return true;
}



   // check protection. pxe is a pte or pde.
   // user is true if CPL==3
bool CMMU::check_page_fault(PXE pxe, bool user, bool write)
{
     if (!PAGE_PRESENT(pxe))  
        return false; // page fault -- page not present

     if (!PAGE_USER(pxe) && user)
        return false; // page fault -- not access for user
   
     if (write && !PAGE_WRITEABLE(pxe))
	 {
       if (user)   
         return false; // page fault -- not writable

       else if (!(PAGE_USER(pxe)))

         return false; // page fault -- not writable

       else if (_cr0_.wp==1)      //The cacheable Write-Protected memory type.
         return false; // page fault -- not writable
	 }

	 return true;
}
   


bool CMMU::lddw(unsigned int linear_address, bool user,unsigned  long *pdw)
{
	bool pcd,pwt,page_size_4M;
	unsigned int physical_address;

	if (!to_physical_address(linear_address, false,user, false,physical_address,page_size_4M,pcd,pwt))
	{
			return false;
	}
	// if cacheable, read from cache
	if( pcd)
	{
		*pdw=p_memory->mem_read_int (physical_address);
	}
	else
	{
		*pdw=p_memory->mem_read_int (physical_address);
	}
	return true;
}
bool CMMU::ldw(unsigned int linear_address, bool user,unsigned  short *pw)
{
	bool pcd,pwt,page_size_4M;
	unsigned int physical_address;

	if (!to_physical_address(linear_address, false,user, false,physical_address,page_size_4M,pcd,pwt))
	{
			return false;
	}
	// if cacheable, read from cache
	if( pcd)
	{
		*pw=p_memory->mem_read_short (physical_address);
	}
	else
	{
		*pw=p_memory->mem_read_short (physical_address);
	}
	return true;
}
bool CMMU::ldb(unsigned int linear_address, bool user, unsigned char *pc)
{
	bool pcd,pwt,page_size_4M;
	unsigned int physical_address;

	if (!to_physical_address(linear_address, false,user, false,physical_address,page_size_4M,pcd,pwt))
	{
			return false;
	}
	// if cacheable, read from cache
	if( pcd)
	{
		*pc=p_memory->mem_read_byte (physical_address);
	}
	else
	{
		*pc=p_memory->mem_read_byte (physical_address);
	}
	return true;
}
bool CMMU::stdw(unsigned int linear_address, bool user,unsigned  long dw)
{
	bool pcd,pwt,page_size_4M;
	unsigned int physical_address;

	if (!to_physical_address(linear_address, false,user, false,physical_address,page_size_4M,pcd,pwt))
	{
			return false;
	}
	// if cacheable, read from cache
	if( pcd)
	{
		p_memory->mem_write_int (physical_address,dw);
	}
	else
	{
		p_memory->mem_write_int (physical_address,dw);
	}
	return true;
}
bool CMMU::stw(unsigned int linear_address, bool user,unsigned  short w)
{
	bool pcd,pwt,page_size_4M;
	unsigned int physical_address;

	if (!to_physical_address(linear_address, false,user, false,physical_address,page_size_4M,pcd,pwt))
	{
			return false;
	}
	// if cacheable, read from cache
	if( pcd)
	{
		p_memory->mem_write_short (physical_address,w);
	}
	else
	{
		p_memory->mem_write_short (physical_address,w);
	}
	return true;
}
bool CMMU::stb(unsigned int linear_address, bool user, unsigned char c)
{
	bool pcd,pwt,page_size_4M;
	unsigned int physical_address;

	if (!to_physical_address(linear_address, false,user, false,physical_address,page_size_4M,pcd,pwt))
	{
			return false;
	}
	// if cacheable, read from cache
	if( pcd)
	{
		p_memory->mem_write_byte (physical_address,c);
	}
	else
	{
		p_memory->mem_write_byte (physical_address,c);
	}
	return true;
}



   

