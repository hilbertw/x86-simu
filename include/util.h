#ifndef __UTIL_H__
#define __UTIL_H__

#ifdef cplusplus
extern  "C"
{
#endif

//
// list utilities
//
typedef struct single_list
{
struct single_list *next;

}SLIST;

typedef struct list_with_tail
{
struct single_list *head;
struct single_list *tail;
}SLIST_EX;

SLIST * add_node_ex(SLIST_EX*list,int extra_size);
SLIST * add_node(SLIST**last,int extra_size);
void destroy_list(SLIST**head);

typedef struct double_list
{
struct double_list *next,*prev;

}DLIST;

DLIST * add_node_dl(DLIST*head,int extra_size);
void destroy_list_dl(DLIST*head);
#define DLIST_INIT(x) {&x,&x}

__inline void INIT_DLIST(DLIST *dl) {dl->next=dl->prev=dl;}

#define DLIST_EMPTY(x) (x.next==&x)
//
// memory pool
//
typedef struct _mem_pool
{
	SLIST * pool_list;
	//
	// cureent pool
	//
	char * mem;// 64 KB
	unsigned int mem_size;
	unsigned int alloc_pos;
} MEM_POOL,*PMEM_POOL;
char * mp_calloc(PMEM_POOL mem_pool, size_t size);
char * mp_alloc(PMEM_POOL mem_pool, size_t size);

__inline void mp_destroy( PMEM_POOL mem_pool)
{
	destroy_list(&mem_pool->pool_list);
}
_inline void  mp_init(PMEM_POOL mem_pool)
{
	mem_pool->pool_list=(SLIST *)0;
	mem_pool->mem_size=0;

}

//
// list in mem_pool
//
SLIST * mp_add_node_ex(SLIST_EX*list,int extra_size,PMEM_POOL mem_pool);
SLIST * mp_add_node(SLIST**last,int extra_size,PMEM_POOL mem_pool);
DLIST * mp_add_node_dl(DLIST*head,int extra_size,PMEM_POOL mem_pool);
//
// debug utilities
//
void sprint_mem32(char * buffer, unsigned long  va, char * _start,unsigned long  _size );
void print_mem32(char * _start,unsigned long  va,long _size );
void print_mem16(char * _start,unsigned short  seg,unsigned int  off,long _size );
int convert_hex(wchar_t  *s,wchar_t endchar, int max_length,unsigned int& val);
int htoi(char  *s,char endchar, int max_length,unsigned int& val);
#ifdef cplusplus
}
#endif
#endif  //__UTIL_H__