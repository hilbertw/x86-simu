#include "cpu.h"

#pragma warning (disable:4018)
//
// descriptor cached in tmp_desc3
//
void CCPU::switch_task(SELECTOR  tss_selector,int desc_cache_line_no,bool nesting)
{
	unsigned short oldtss;

	// save old TSS
	//__TSS__->eax <- __EAX__;
	//__TSS__->ebx <- __EBX__;		
	//__TSS__->ecx <- __ECX__;
	//__TSS__->edx <- __EDX__;
	//__TSS__->esi <- __ESI__;
	//__TSS__->edi <- __EDI__;
	//__TSS__->esp <- __ESP__;
	//__TSS__->ebp <- __EBP__;
	//__TSS__->eflags<-__EFLAGS__;
	//__TSS__->eip <- __EIP__;
	MEM_WRITEL(BASE(tr_desc)+OFFSET(TSS,eax),__EAX__);
	MEM_WRITEL(BASE(tr_desc)+OFFSET(TSS,ebx),__EBX__);
	MEM_WRITEL(BASE(tr_desc)+OFFSET(TSS,ecx),__ECX__);
	MEM_WRITEL(BASE(tr_desc)+OFFSET(TSS,edx),__EDX__);
	MEM_WRITEL(BASE(tr_desc)+OFFSET(TSS,esi),__ESI__);
	MEM_WRITEL(BASE(tr_desc)+OFFSET(TSS,edi),__EDI__);
	MEM_WRITEL(BASE(tr_desc)+OFFSET(TSS,ebp),__EBP__);
	MEM_WRITEL(BASE(tr_desc)+OFFSET(TSS,esp),__ESP__);
	MEM_WRITEL(BASE(tr_desc)+OFFSET(TSS,eflags),__EFLAGS__);
	MEM_WRITEL(BASE(tr_desc)+OFFSET(TSS,eip),__EIP__);

	//__TSS__->cs <- __CS__;
	//__TSS__->ds <- __DS__;
	//__TSS__->es <- __ES__;
	//__TSS__->ss <- __SS__;
	//__TSS__->fs <- __FS__;
	//__TSS__->gs <- __GS__;
	MEM_WRITEW(BASE(tr_desc)+OFFSET(TSS,cs),___CS___);
	MEM_WRITEW(BASE(tr_desc)+OFFSET(TSS,ds),___DS___);
	MEM_WRITEW(BASE(tr_desc)+OFFSET(TSS,es),___ES___);
	MEM_WRITEW(BASE(tr_desc)+OFFSET(TSS,ss),___SS___);
	MEM_WRITEW(BASE(tr_desc)+OFFSET(TSS,fs),___FS___);
	MEM_WRITEW(BASE(tr_desc)+OFFSET(TSS,gs),___GS___);

	//__TSS__->ldt<- ldt_seg;
	MEM_WRITEW(BASE(tr_desc)+OFFSET(TSS,ldt),___LDT___);
	//__TSS__->cr3<- ___CR3___;
	MEM_WRITEL(BASE(tr_desc)+OFFSET(TSS,cr3),___CR3___);


	//__TSS__->link<- oldtss;
	oldtss = ___TR___;
	MEM_WRITEW(BASE(tr_desc)+OFFSET(TSS,link),oldtss);

	// load new TSS
	tr_desc      = descriptors[desc_cache_line_no];
	__TR__       = tss_selector;

	//__EAX__ <- __TSS__->eax;
	//__EBX__ <- __TSS__->ebx;
	//__ECX__ <- __TSS__->ecx;
	//__EDX__ <- __TSS__->edx;
	//__ESI__ <- __TSS__->esi;
	//__EDI__ <- __TSS__->edi;
	//__ESP__ <- __TSS__->esp;
	//__EBP__ <- __TSS__->ebp;
	//__EIP__ <- __TSS__->eip;

	__EAX__=MEM_READL(BASE(tr_desc)+OFFSET(TSS,eax));
	__EBX__=MEM_READL(BASE(tr_desc)+OFFSET(TSS,ebx));
	__ECX__=MEM_READL(BASE(tr_desc)+OFFSET(TSS,ecx));
	__EDX__=MEM_READL(BASE(tr_desc)+OFFSET(TSS,edx));
	__ESI__=MEM_READL(BASE(tr_desc)+OFFSET(TSS,esi));
	__EDI__=MEM_READL(BASE(tr_desc)+OFFSET(TSS,edi));
	__EBP__=MEM_READL(BASE(tr_desc)+OFFSET(TSS,ebp));
	__ESP__=MEM_READL(BASE(tr_desc)+OFFSET(TSS,esp));
	__EFLAGS__ =MEM_READL(BASE(tr_desc)+OFFSET(TSS,eflags));
	__NEXT_EIP__ =MEM_READL(BASE(tr_desc)+OFFSET(TSS,eip));

	//__CS__ <- __TSS__->cs;
	//__DS__ <- __TSS__->ds;
	//__ES__ <- __TSS__->es;
	//__SS__ <- __TSS__->ss;
	//__FS__ <- __TSS__->fs;
	//__GS__ <- __TSS__->gs;

	___CS___=MEM_READW(BASE(tr_desc)+OFFSET(TSS,cs));
	___DS___=MEM_READW(BASE(tr_desc)+OFFSET(TSS,ds));
	___ES___=MEM_READW(BASE(tr_desc)+OFFSET(TSS,es));
	___SS___=MEM_READW(BASE(tr_desc)+OFFSET(TSS,ss));
	___FS___=MEM_READW(BASE(tr_desc)+OFFSET(TSS,fs));
	___GS___=MEM_READW(BASE(tr_desc)+OFFSET(TSS,gs));

	load_descriptor(gdt_base,&cs_desc,__CS__.index);
	load_descriptor(gdt_base,&es_desc,__ES__.index);
	load_descriptor(gdt_base,&ds_desc,__DS__.index);
	load_descriptor(gdt_base,&ss_desc,__SS__.index);
	load_descriptor(gdt_base,&fs_desc,__FS__.index);
	load_descriptor(gdt_base,&gs_desc,__GS__.index);

	//ldt_seg <-__TSS__->ldt
	___LDT___ =MEM_READW(BASE(tr_desc)+OFFSET(TSS,ldt));

	load_descriptor(gdt_base,&tmp_desc2,ldt_seg.index);

	ldt_base   = BASE (tmp_desc2);
	ldt_limit  = LIMIT(tmp_desc2);

	//___CR3___ <-__TSS__->cr3
	___CR3___ =MEM_READL(BASE(tr_desc)+OFFSET(TSS,cr3));

	//
	// task switch to vm86.
	//
	if(__VM__==1) cpl=3;

	//if (the instruction pointer is not within code segment limit) #GP(0); 
	if (__NEXT_EIP__ > LIMIT(cs_desc) ) GP(0);
}