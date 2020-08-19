
#define  _CRT_SECURE_NO_WARNINGS 1


#include "util.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "assert.h"
SLIST  pool_list;
#define POOL_SIZE 0xffff

char * mem;// 64 KB
unsigned int mem_size;
unsigned int alloc_pos;

#define string(x)    (mem+x)
#define end_string() (mem+alloc_pos)
static void check_mem(unsigned int required);

void sys_error();

void check_mem(unsigned int required)
{

	SLIST *p;
	if(mem_size-alloc_pos>=required)
		return;


	assert(required<POOL_SIZE);

	p=add_node(&pool_list,POOL_SIZE); // 64k

	if(!p)
	{
		sys_error();
	}
	mem=(char *)(p+1);
	alloc_pos=0;
	mem_size=POOL_SIZE;
}
	
char * put_string(char *src, unsigned int bytes)
{
	unsigned int old_pos;

	check_mem(bytes+1);

	old_pos=alloc_pos;
	strncpy( end_string(),src,bytes);
	alloc_pos+=bytes;
	mem[alloc_pos++]=0;  // null
	//if(strcmp(mem+old_pos,"pHalExamineMBR")==0)
	printf("%s\n",mem+old_pos);
	return mem+old_pos;
}


static bool mp_check_mem(PMEM_POOL mem_pool,unsigned int required)
{

	SLIST *p;
	if(mem_pool->mem_size-mem_pool->alloc_pos>=required)
		return true;


	assert(required<POOL_SIZE);

	p=add_node(&pool_list,POOL_SIZE); // 64k

	if(!p)return false;
	mem_pool->mem=(char *)(p+1);
	mem_pool->alloc_pos=0;
	mem_pool->mem_size=POOL_SIZE;
	return true;
}
char * mp_alloc(PMEM_POOL mem_pool, unsigned int size)
{
	unsigned int old_pos;

	if(!mp_check_mem(mem_pool,size)) return NULL;

	old_pos=mem_pool->alloc_pos;
	mem_pool->alloc_pos+=size;

	return mem_pool->mem+old_pos;
}

char * mp_calloc(PMEM_POOL mem_pool, unsigned int size)
{
	char *p;
	if(p=mp_alloc(mem_pool,size))
	{
		memset(p,0,size);
	}
	return p;

}




SLIST * mp_add_node(SLIST*head,unsigned int extra_size,PMEM_POOL mem_pool)
{
SLIST *p= (SLIST *)mp_calloc(mem_pool,extra_size + sizeof (SLIST ));

if (p)
p->next=head->next;

head->next=p;

return p;
}



SLIST * mp_add_node_ex(SLIST_EX*list,unsigned int extra_size,PMEM_POOL mem_pool)
{
	SLIST *p= (SLIST *)mp_calloc(mem_pool,extra_size + sizeof (SLIST ));

	p->next=NULL;
	if(!list->head) 
	{
		list->head=p;
	}
	else
	{
	
		list->tail->next=p;
	}

	list->tail=p;

	return p;
}


//
// a double list is initialized with prev and next pointed to itself
//  head->prev and head->next are always uot empty
//
DLIST * mp_add_node_dl(DLIST*head,unsigned int extra_size,PMEM_POOL mem_pool)
{
DLIST *p= (DLIST *)mp_calloc(mem_pool,extra_size + sizeof (DLIST ));

if (p)
{
	head->prev->next=p;
	p->next=head;
	p->prev=head->prev;
	head->prev=p;
}

return p;
}