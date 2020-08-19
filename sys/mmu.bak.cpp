#include "mmu.h"

#include "addressmap.cpp"

bool CMMU::to_physical_address(int linear_address,bool code, bool user, bool write, 
							   int &physical_address,bool &page_size_4M, bool &pcd, bool &pwt)
{
	if (_cr0_.pg==0)
	{
		physical_address = linear_address;
		pcd = pwt = true;
		page_size_4M = true;

		return true;
	}

    PTE        pte;

	bool page_fault = !get_tlb_entry (linear_address,code,user, false,&pte);

	//
	// if page fault or not present, page fault
	//
	if( page_fault)
	{
		return false;
	}

	physical_address = PHYSICAL_ADDRESS(pte, linear_address);

	 pcd = pte.pcd;
	 pwt = pte.pwt;
	 page_size_4M = pte.page_size;

	return true;
}

bool CMMU::read(int linear_address,bool code, bool user, char *buffer, int count)
{
	bool pcd,pwt,page_size_4M;
	int physical_address;
	//
	// check page boundary.
	//
	while (count)
	{
		if (!to_physical_address(linear_address, code,user, false,physical_address,page_size_4M,pcd,pwt))
		{
			return false;
		}
		int page_bounary = page_size_4M ? 0x1000 : 0x400000;
		int max_bytes = (linear_address | (page_bounary-1)) - linear_address + 1;

		if (max_bytes>count) max_bytes = count;

		// if cacheable, read from cache
		if( pcd)
		{
			// cache not implemented as yet
			biu.mem_read_string (physical_address,buffer,max_bytes);
		}
		else
		{
			biu.mem_read_string (physical_address,buffer,max_bytes);
		}
		count -= max_bytes;
	}
	return true;
}

bool CMMU::write(int linear_address, bool user, char *buffer, int count)
{
	bool pcd,pwt,page_size_4M;
	int physical_address;
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
		}
		else
		{
			biu.mem_write_string (physical_address,buffer,max_bytes);
		}
		count -= max_bytes;
	}
	return true;
}
bool CMMU::fetch(int linear_address, bool user, char *buffer, int count)
{
	return read(linear_address,true,user,buffer,count);
}

bool CMMU::ld(int linear_address, bool user, char *buffer, int count)
{
	return read(linear_address,false,user,buffer,count);
}

bool CMMU::st(int linear_address, bool user, char *buffer, int count)
{
	return write(linear_address,user,buffer,count);
}


bool CMMU::do_address_translation(int linear_address,bool user, bool write, PTE *out_pte)
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
	data = biu.mem_read_int( pde_address);

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
	
	data = biu.mem_read_int( pte_address);
	pte = *(PTE *) &data;

	*out_pte = pte;
	return true;
}


bool CMMU::get_tlb_entry(int linear_address,bool code, bool user, bool write,PTE *out_pte)
{
	bool page_fault;

	PTE pte;
	TLB_HANDLE tlb_handle;
	CTLB tlb = code ? itlb : dtlb;
	//
	// hits TLB?
	//
	if (tlb.hit(linear_address, &tlb_handle))
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
   