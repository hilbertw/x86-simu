#ifndef __UTIL_H__
#define __UTIL_H__

#include "tchar.h"
#ifdef __cplusplus
extern "C" {

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
SLIST * add_node(SLIST* head,int extra_size);
SLIST *  revert_list(SLIST* list);
void destroy_list(SLIST*head);
void merge_list(SLIST* dst,SLIST*list_to_add);
void append_list_ex(SLIST_EX*dst_list,SLIST*list_to_add);
void list_foreach(SLIST*list,int (*p_do_func)(SLIST*list_node,void * p_para),void * p_para);

typedef struct double_list
{
struct double_list *next,*prev;

}DLIST;

DLIST * add_node_dl(DLIST*head,unsigned int extra_size);
void destroy_list_dl(DLIST*head);
void merge_list_dl(DLIST* dst,DLIST*list_to_add);

#define LIST_DATA(t,p) (*(t*)(p+1))
#define LIST_HEAD(x) (x).next
#define INIT_LIST(x) {(x).next=NULL;}
#define DLIST_INIT(x) {&(x),&(x)}

__inline void INIT_DLIST(DLIST *dl) {dl->next=dl->prev=dl;}

#define DLIST_EMPTY(x) ((x).next==&(x))

#define FOREACH_BEGIN( p,type,data) while(p){type data; data=(type)(p+1);
#define FOREACH_END( p) p=p->next;}

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
char * mp_calloc(PMEM_POOL mem_pool, unsigned int size);
char * mp_alloc(PMEM_POOL mem_pool, unsigned int size);

__inline void mp_destroy( PMEM_POOL mem_pool)
{
	destroy_list(mem_pool->pool_list);
	mem_pool->mem_size=0;

}
__inline void  mp_init(PMEM_POOL mem_pool)
{
	mem_pool->pool_list=(SLIST *)0;
	mem_pool->mem_size=0;

}

//
// list in mem_pool
//
SLIST * mp_add_node_ex(SLIST_EX*list,unsigned int extra_size,PMEM_POOL mem_pool);
SLIST * mp_add_node(SLIST*last,unsigned int extra_size,PMEM_POOL mem_pool);
DLIST * mp_add_node_dl(DLIST*head,unsigned int extra_size,PMEM_POOL mem_pool);
//
// debug utilities
//
void sprint_mem32(char * buffer, unsigned long  va, char * _start,unsigned long  _size );
void print_mem32(char * _start,unsigned long  va,long _size );
void print_mem16(char * _start,unsigned short  seg,unsigned int  off,long _size );

//
// unamed mem pool utility
//
char * put_string(char *src, unsigned int bytes);




#ifdef __cplusplus
}
#endif 

#ifdef UNICODE

#define htoi htoiW
#define convert_hex convert_hexW


#else
#define htoi htoiA
#define convert_hex convert_hexA
#endif
int convert_hexW(wchar_t  *s,wchar_t endchar, int max_length,unsigned int& val);
int convert_hexA(char  *s,char endchar, int max_length,unsigned int& val);
int convert_hexW(wchar_t  *s,int length,char* buffer);
int convert_hexA(char  *s,int length,char* buffer);

char convert_hexW(TCHAR h);
char convert_hexA(char h);

int htoiW(wchar_t  *s,wchar_t endchar, int max_length,unsigned int& val);
int htoiA(char  *s,char endchar, int max_length,unsigned int& val);

#endif  //__UTIL_H__