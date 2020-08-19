#include "mmu.h"
#include "log.h"

extern bool step;
#pragma  warning(disable: 4700 4101)
RW_STATUS CMMU::to_physical_address(unsigned int linear_address,bool code, bool user, bool write, 
							   unsigned int &physical_address,unsigned long &page_size, bool &pcd, bool &pwt)
{
	RW_STATUS status;
	/*
	if (linear_address >= 0x12240 && linear_address < 0x12244)
	{
		printf("");
	}
	*/
	if ((cr[0]&0x80000001)!=0x80000001)//_cr0_.pg==0||_cr0_.pe==0
	{
		physical_address = linear_address;
		pcd = pwt = true;
		page_size = 0x1000;

		return PAGE_FAULT_NONE;
	}
	
	union
	{
    PTE        pte;
	PDE4M      pde4m;
	PDE2M      pde2m;
	PDE4K      pde4k;
	PTE4K      pte4k;
	PAE32_PTE4K pae_pte;
	PAE32_PDE2M pae_pde;
	PSE36_PDE4M pse36_pde;
    };

	TLB_ENTRY_TYPE type;

	int tlb_no=code?0:1;
	bool tlb_hit;

	tlb_hit=tlb[tlb_no].hit(linear_address,&pte,&type);

	if(tlb_hit)
	{
		status=check_page_fault(pte,user,write);
		if(status!=PAGE_FAULT_NONE)
		tlb[tlb_no].invalid(linear_address);

		if(_cr4_.pae==1)//pae=1,pse=X
		{
			if(type!=TLB_ENTRY_PAE32_PTE4K
				&&type!=TLB_ENTRY_PAE32_PDE2M)
			{
				log0("bad tlb entry type for PAE:%d\n",type);
			}
		}
		else if (_cr4_.pse==1)  // pae=0,pae=1
		{
			if(type!=TLB_ENTRY_PSE36_PTE4K
				&&type!=TLB_ENTRY_PSE36_PDE4M)
			{
				log0("bad tlb entry type for PSE:%d\n",type);
			}
		}
		else 
		{
			if(type!=TLB_ENTRY_PTE4K
				&&type!=TLB_ENTRY_PDE4M)
			{
				log0("bad tlb entry type for PTE:%d\n",type);
			}
		}
	}
	else
	{

		if(_cr4_.pae==1)//pae=1,pse=X
		{
			status=do_address_translation_pae(linear_address,user,write, &pae_pte,&type);	
		}
		else if (_cr4_.pse==1)  // pae=0,pae=1
		{
			status=do_address_translation_pse(linear_address,user,write, &pte,&type);
		}
		else {
			status=do_address_translation(linear_address,user,write, &pte,&type);
		}
	}

	if( status!=PAGE_FAULT_NONE)
	{
		if(user) status |=PAGE_FAULT_USER;
		if(write) status |=PAGE_FAULT_WRITE;
		_cr2_=linear_address;
		return status;
	}
	if(!tlb_hit)
		tlb[tlb_no].insert(linear_address,&pte,type);


	if(type==TLB_ENTRY_PTE4K)
	{
		page_size=(4<<10);
		physical_address = PHYSICAL_ADDRESS(pte4k, linear_address);
		//log("4K page:%x,phy:%x,pte:%x\n",linear_address,physical_address,*(unsigned long *)&pte);
		// address_hi not implemented
	}
	else if(type==TLB_ENTRY_PDE4M)
	{
		page_size=(4<<20);
		physical_address = PHYSICAL_ADDRESS_4M(pde4m, linear_address);
		//log("4K page:%x,phy:%x,pte:%x\n",linear_address,physical_address,*(unsigned long *)&pte);
		// address_hi not implemented
	}
	else if(type==TLB_ENTRY_PAE32_PTE4K)
	{
		page_size=(4<<10);
		physical_address = PHYSICAL_ADDRESS(pae_pte.old, linear_address);
		//log("4K page:%x,phy:%x,pte:%x\n",linear_address,physical_address,*(unsigned long *)&pte);
		// address_hi not implemented
	}
	else if(type==TLB_ENTRY_PAE32_PDE2M)
	{
		page_size=(2<<20);
		physical_address = PHYSICAL_ADDRESS_2M( pae_pde.old, linear_address);
		//log("2M page:%x,phy:%x,pde:%x\n",linear_address,physical_address,*(unsigned long *)&pde2m);
		// address_hi not implemented
	}
	else if(type==TLB_ENTRY_PSE36_PTE4K)
	{
		page_size=(4<<10);
		physical_address = PHYSICAL_ADDRESS(pte4k, linear_address);
		//log("4K page:%x,phy:%x,pte:%x\n",linear_address,physical_address,*(unsigned long *)&pte);
		// address_hi not implemented
	}
	else if(type==TLB_ENTRY_PSE36_PDE4M)
	{
		page_size=(4<<20);
		physical_address = PHYSICAL_ADDRESS_4M( pse36_pde, linear_address);
		//log("4M page:%x,phy:%x,pte:%x\n",linear_address,physical_address,*(unsigned long *)&pde4m);
		// address_hi not implemented
		// address_hi=pse36_pde.ba_hi;
	}
	else
	{
		log0("bad pte type:%d\n",type);
		
	}


	 pcd = pte.pcd;
	 pwt = pte.pwt;


	physical_address&=a20m;

	return PAGE_FAULT_NONE;
}

RW_STATUS CMMU::read(unsigned int linear_address,bool code, bool user, char *buffer, int count)
{
	RW_STATUS status;

	bool pcd,pwt;
	unsigned long page_size;
	unsigned int physical_address;
	//
	// check page boundary.
	//
	while (count)
	{
		if (status=to_physical_address(linear_address, code,user, false,physical_address,page_size,pcd,pwt))
		{
			return status;
		}
		int max_bytes = (linear_address | (page_size-1)) - linear_address + 1;

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

RW_STATUS CMMU::write(unsigned int linear_address, bool user, char *buffer, int count)
{
	RW_STATUS status;
	bool pcd,pwt;
	unsigned long page_size;
	unsigned int physical_address;
	//
	// check page boundary.
	//
	while (count)
	{
		// we will never write to a itlb
		if (status=to_physical_address(linear_address, false,user, true,physical_address,page_size,pcd,pwt))
		{
			return status;
		}
		int max_bytes = (linear_address | (page_size-1)) - linear_address + 1;

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
	return PAGE_FAULT_NONE;
}
RW_STATUS CMMU::fetch(unsigned int linear_address, bool user, char *buffer, int count)
{
   RW_STATUS status;

   //printf("%x,%x\n",linear_address,count);
	return read(linear_address,true,user,buffer,count);
}

RW_STATUS CMMU::ld(unsigned int linear_address, bool user, char *buffer, int count)
{
   RW_STATUS status;
	//printf("ld:%x %x\n",linear_address,count);
	status= (count==1)?ldb(linear_address,user,(unsigned char *)buffer):
		(count==2&&(linear_address&1)==0)?ldw(linear_address,user,(unsigned short *)buffer):
		(count==4&&(linear_address&3)==0)?lddw(linear_address,user,(unsigned long *)buffer):
		read(linear_address,false,user,buffer,count);

 //  if(linear_address>=0xfee00000&&linear_address<0xfef00000)
 //  {
	//	log("ld:%d,%x,%x\n",count,linear_address,*(unsigned long *)buffer);
	//   //::step=true;
	//}
	return status;
}

RW_STATUS CMMU::st(unsigned int linear_address, bool user, char *buffer, int count)
{
   RW_STATUS status;

//   if(linear_address==0x80eb008)
//   if (linear_address == 0x12240)
 //  {
//		log("st:%d,%x,%x\n",count,linear_address,*(unsigned long *)buffer);
//	   ::step=true;
//	}
	status= (count==1)?stb(linear_address,user,*(unsigned char *)buffer):
		(count==2&&(linear_address&1)==0)?stw(linear_address,user,*(unsigned short *)buffer):
		(count==4&&(linear_address&3)==0)?stdw(linear_address,user,*(unsigned long *)buffer):
		write(linear_address,user,buffer,count);

	return status;

}


RW_STATUS CMMU::do_address_translation(unsigned int linear_address,bool user, bool write, PTE *out_pte,TLB_ENTRY_TYPE * p_type)
{
	RW_STATUS status;
	bool pwt,pcd;
	
	PTE  pte;
	PDE4K  pde;

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

	pde = *(PDE4K *) &data;

	//
	// if not present, page fault
	//
	status=check_page_fault(*(PXE*)&pde,user,write);
	if(PAGE_FAULT_NONE!=status)
	{
		return status;
	}
	p_memory->mem_write_int( pde_address,data|(write?0x60:0x20));

	*p_type=pde.page_size?TLB_ENTRY_PDE4M:TLB_ENTRY_PTE4K;

	if(pde.page_size==0)
	{
		// load PTE from mem ( (pde & 0xfffff000) + 4*((la >> 12) & 0x3ff));
		pwt = pde.pwt == 1;
		pcd = pde.pcd == 1;

		int pte_address = BASE_ADDRESS(pde) + 4 * ((linear_address >> 12) & 0x003ffU) ;
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

		//log("4K page:%x,pde:%x,pte:%x\n",linear_address,*(unsigned long *)&pde,*(unsigned long *)&pte);
		*out_pte = pte;
		status=check_page_fault(*(PXE*)&pte,user,write);

		if(status==PAGE_FAULT_NONE)
		p_memory->mem_write_int( pte_address,data|(write?0x60:0x20));
	}
	else
	{

		*out_pte = *(PTE *) &pde;
		status=PAGE_FAULT_NONE;
	}
	return status;
}


   // check protection. pxe is a pte or pde.
   // user is true if CPL==3
RW_STATUS CMMU::check_page_fault(PXE pxe, bool user, bool write)
{
     if (!PAGE_PRESENT(pxe))  
        return PAGE_FAULT_NP; // page fault -- page not present

    //   if (write&& _cr0_.wp==1)      //The cacheable Write-Protected memory type.
    //     return PAGE_FAULT_PROT; // page fault -- not writable

     if (user)
	 {
		 if (!PAGE_USER(pxe))
			return PAGE_FAULT_PROT; // page fault -- not access for user
   
	 // user page || in supervisor mode
		 if (write &&!PAGE_WRITEABLE(pxe))
			 return PAGE_FAULT_PROT; // page fault -- not writable



	 }
   //
   // kernel can write user pages
   //
	 return PAGE_FAULT_NONE;
}
   


RW_STATUS CMMU::lddw(unsigned int linear_address, bool user,unsigned  long *pdw)
{
   RW_STATUS status;
	bool pcd,pwt;
	unsigned long page_size;
	unsigned int physical_address;

	if (status=to_physical_address(linear_address, false,user, false,physical_address,page_size,pcd,pwt))
	{
			return status;
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
	return PAGE_FAULT_NONE;
}
RW_STATUS CMMU::ldw(unsigned int linear_address, bool user,unsigned  short *pw)
{
   RW_STATUS status;
	bool pcd,pwt;
	unsigned long page_size;
	unsigned int physical_address;

	if (status=to_physical_address(linear_address, false,user, false,physical_address,page_size,pcd,pwt))
	{
			return status;
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
	return PAGE_FAULT_NONE;
}
RW_STATUS CMMU::ldb(unsigned int linear_address, bool user, unsigned char *pc)
{
   RW_STATUS status;
	bool pcd,pwt;
	unsigned long page_size;
	unsigned int physical_address;

	if (status=to_physical_address(linear_address, false,user, false,physical_address,page_size,pcd,pwt))
	{
			return status;
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
	return PAGE_FAULT_NONE;
}
RW_STATUS CMMU::stdw(unsigned int linear_address, bool user,unsigned  long dw)
{
   RW_STATUS status;
	bool pcd,pwt;
	unsigned long page_size;
	unsigned int physical_address;

	if (status=to_physical_address(linear_address, false,user, true,physical_address,page_size,pcd,pwt))
	{
			return status;
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
	return PAGE_FAULT_NONE;
}
RW_STATUS CMMU::stw(unsigned int linear_address, bool user,unsigned  short w)
{
   RW_STATUS status;
	bool pcd,pwt;
	unsigned long page_size;
	unsigned int physical_address;

	if (status=to_physical_address(linear_address, false,user, true,physical_address,page_size,pcd,pwt))
	{
			return status;
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
	return PAGE_FAULT_NONE;
}
RW_STATUS CMMU::stb(unsigned int linear_address, bool user, unsigned char c)
{
   RW_STATUS status;
	bool pcd,pwt;
	unsigned long page_size;
	unsigned int physical_address;

	if (status=to_physical_address(linear_address, false,user, true,physical_address,page_size,pcd,pwt))
	{
			return status;
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
	return PAGE_FAULT_NONE;
}


RW_STATUS CMMU::do_address_translation_pae(unsigned int linear_address,bool user, bool write, PAE32_PTE4K *out_pte,TLB_ENTRY_TYPE * p_type)
{
	RW_STATUS status;
	bool pwt,pcd;

	// load PDE
	pwt = _cr3_pae_.pwt == 1;
	pcd = _cr3_pae_.pcd == 1;


    PDPTE pdpte;

	//force pdpte table located on a 32-byte boundary
	int pdpte_address = (_cr3_pae_.pdptbr<<5) + (linear_address>>30) * sizeof(PDPTE);
	//log0("cr3:%x, %x, %x\n",cr[3],_cr3_pae_.pdptbr,pdpte_address);
	p_memory->mem_read_string( pdpte_address,0,(char *)&pdpte,sizeof(PDPTE));

	//log0("PDPTE:%x, %x\n",pdpte.pd_base_lo,pdpte.pd_base_hi);

	union
	{
	PAE32_PDE2M pae_pde2m;
	PAE32_PDE4K pae_pde4k;
	};
	int pde_address = (pdpte.pd_base_lo<<12) + ((linear_address>>21)&0x1ff) * sizeof(pae_pde2m);
	//
	// if pcd is true, check cache for
	//

	//
	//  pcd is true but cache miss, read from mem, and cache it
	//

	//
	// pcd is disabled, read from mem directly, and don't cache ir
	//
	p_memory->mem_read_string( pde_address,0,(char *)&pae_pde2m,sizeof(pae_pde2m));
	//log0("PAE PDE:%x, %x\n",pde_address,pae_pde2m.old.table_address_hi);
	//
	// if not present, page fault
	//
	status=check_page_fault(*(PXE*)&pae_pde2m.old,user,write);
	if(PAGE_FAULT_NONE!=status)
	{
		return status;
	}
	*p_type=pae_pde2m.old.page_size?TLB_ENTRY_PAE32_PDE2M:TLB_ENTRY_PAE32_PTE4K;
	if(pae_pde2m.old.page_size==0)// 4K page
	{
		// load PTE from mem ( (pde & 0xfffff000) + 4*((la >> 12) & 0x3ff));
		pwt = pae_pde2m.old.pwt == 1;
		pcd = pae_pde2m.old.pcd == 1;

		PAE32_PTE4K pae_pte;
		int pte_address = (pae_pde4k.old.table_address_hi<<12) + sizeof(pae_pte) * ((linear_address >> 12) & 0x001ffU) ;
		//
		// if pcd is true, check cache for
		//

		//
		//  pcd is true but cache miss, read from mem, and cache it
		//

		//
		// pcd is disabled, read from mem directly, and don't cache ir
		//	
		
		p_memory->mem_read_string( pte_address,0,(char *)&pae_pte,sizeof(pae_pte));

		//log0("PAE PTE:%x, %x\n",pte_address,pae_pte.old.table_address_hi);

		//log("4K page:%x,pde:%x,pte:%x\n",linear_address,*(unsigned long *)&pde,*(unsigned long *)&pte);
		*out_pte = pae_pte;
		status=check_page_fault(*(PXE*)&pae_pte.old,user,write);
	}
	else// 2M page
	{

		*out_pte = *(PAE32_PTE4K *) &pae_pde2m;
		status=PAGE_FAULT_NONE;
	}
	return status;
}


RW_STATUS CMMU::do_address_translation_pse(unsigned int linear_address,bool user, bool write, PTE *out_pte,TLB_ENTRY_TYPE * p_type)
{
	RW_STATUS status;
	bool pwt,pcd;
	
	// load PDE
	pwt = _cr3_pae_.pwt == 1;
	pcd = _cr3_pae_.pcd == 1;

	union
	{
	unsigned long  data;
	PSE36_PDE4M pse36_pde;
	PDE4K pde;
	PTE pte;
	PXE pxe;
	};

	int pde_address = (_cr3_.pdbr<<12) +  ((linear_address>>22)&0x3ff) * sizeof(pse36_pde);

	//
	// if pcd is true, check cache for
	//

	//
	//  pcd is true but cache miss, read from mem, and cache it
	//

	//
	// pcd is disabled, read from mem directly, and don't cache ir
	//
	data=p_memory->mem_read_int( pde_address);


	//
	// if not present, page fault
	//
	status=check_page_fault(pxe,user,write);
	if(PAGE_FAULT_NONE!=status)
	{
		return status;
	}
	p_memory->mem_write_int( pde_address,data|(write?0x60:0x20));

	*p_type=pse36_pde.page_size?TLB_ENTRY_PSE36_PDE4M:TLB_ENTRY_PSE36_PTE4K;

	if(pse36_pde.page_size==0)// 4K page
	{
		// load PTE from mem ( (pde & 0xfffff000) + 4*((la >> 12) & 0x3ff));
		pwt = pse36_pde.pwt == 1;
		pcd = pse36_pde.pcd == 1;

		int pte_address = BASE_ADDRESS(pde) + 4 * ((linear_address >> 12) & 0x003ffU) ;
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



		//log("4K page:%x,pde:%x,pte:%x\n",linear_address,*(unsigned long *)&pde,*(unsigned long *)&pte);
		*out_pte = pte;
		status=check_page_fault(pxe,user,write);

		if(status==PAGE_FAULT_NONE)
		p_memory->mem_write_int( pte_address,data|(write?0x60:0x20));
	}
	else
	{
	*out_pte = pte;
	status=PAGE_FAULT_NONE;
	}
	
	return status;
}
