#define  _CRT_SECURE_NO_DEPRECATE 1

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "util.h"
SLIST * add_node(SLIST*head,int extra_size)
{
SLIST *p= (SLIST *)calloc(extra_size + sizeof (SLIST ),1);

//memset((char *)p,0,extra_size + sizeof (SLIST ));

if (p)
p->next=head->next;

head->next=p;

return p;
}

void destroy_list(SLIST*head)
{

 SLIST*p,*q;
 p=head;
 while (p)
 {
	 q=p->next;
	 free(p);
	 p=q;
 }
 head->next=NULL;
}


void destroy_list_dl(DLIST*head)
{

 DLIST*p,*q;
 p=head->next;
 while (p!=head)
 {
	 q=p->next;
	 free(p);
	 p=q;
 }
 head->next=head->prev=head;
}


SLIST * add_node_ex(SLIST_EX*list,int extra_size)
{
	SLIST *p= (SLIST *)calloc(extra_size + sizeof (SLIST ),1);

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
DLIST * add_node_dl(DLIST*head,unsigned int extra_size)
{
DLIST *p= (DLIST *)calloc(extra_size + sizeof (DLIST ),1);

if (p)
{
	head->prev->next=p;
	p->next=head;
	p->prev=head->prev;
	head->prev=p;
}

return p;
}

void append_list_ex(SLIST_EX*dst_list,SLIST*list_to_add)
{
	// tail of list_to_add
	SLIST* p=list_to_add,*q=NULL;
	while(p) { q=p;p=p->next;}


	if(NULL!=dst_list->tail)
	dst_list->tail->next=list_to_add;
	else
		dst_list->head=list_to_add;

	dst_list->tail=p;
}

void list_foreach(SLIST*list,int (*p_do_func)(SLIST*list_node,void * p_para),void * p_para)
{
	SLIST* p=list;
	while(p) { if(p_do_func(p,p_para)) break;p=p->next;}
}

char convert_hexW(TCHAR h)
{
	char c;

	if(h>=L'0'&&h<=L'9') c=h-L'0';
	else if(h>=L'a'&&h<=L'f')c=h-L'a'+10;
	else if (h>=L'A'&&h<=L'F') c=h-L'A'+10;
	else c=-1;

	return c;
}
//
// return bytes including endchar
//
int convert_hexW(TCHAR  *s,TCHAR endchar, int max_length,unsigned int& val)
{
	int i=0;

	val=0;
	while (i<=max_length)
	{
		TCHAR c;

		if(s[i]==endchar) return i+1;

		c= convert_hexW(s[i]);
		if(c==-1) return 0;
		
		val <<=4;
		val |=(c&0xf);

		i++;
	}
	return 0;
}


int  htoi(TCHAR  *s,TCHAR endchar, int max_length,unsigned int& val)
{
	int i=0;

	val=0;
	while (i<=max_length)
	{
		TCHAR c;

		if(s[i]==endchar) return i+1;

		c= convert_hexW(s[i]);
		if(c==-1) return 0;
		
		val <<=4;
		val |=(c&0xf);

		i++;
	}
	return 0;
}

//
// return bytes including endchar
//
int convert_hexW(TCHAR  *s,int length,char * buffer)
{
	int i=0,j=0;

	char val1,val2;
	bool byte;

	if(length&1){
		byte=true;
		val1=0;
	}else {
		byte=false;
	}

	while (i<length)
	{
		TCHAR c;


		c= convert_hexW(s[i]);
		if(c==-1) return 0;
		
		if(byte){
			val2 =c;
			char val=(val1<<4)|val2;
			buffer[j++]=val;
		}
		else {
			val1=c;
		}

		byte=!byte;

		i++;
	}


	assert(!byte);

	return j;
}

char convert_hexA(char h)
{
	char c;

	if(h>=('0')&&h<=('9')) c=h-('0');
	else if(h>=('a')&&h<=('f'))c=h-('a')+10;
	else if (h>=('A')&&h<=('F')) c=h-('A')+10;
	else c=-1;

	return c;
}
//
// return bytes including endchar
//
int convert_hexA(char  *s,char endchar, int max_length,unsigned int& val)
{
	int i=0;

	val=0;
	while (i<=max_length)
	{
		char c;

		if(s[i]==endchar) return i+1;

		c= convert_hexA(s[i]);
		if(c==-1) return 0;
		
		val <<=4;
		val |=(c&0xf);

		i++;
	}
	return 0;
}


int  htoiA(char  *s,char endchar, int max_length,unsigned int& val)
{
	int i=0;

	val=0;
	while (i<=max_length)
	{
		char c;

		if(s[i]==endchar) return i+1;

		c= convert_hexA(s[i]);
		if(c==-1) return 0;
		
		val <<=4;
		val |=(c&0xf);

		i++;
	}
	return 0;
}

//
// return bytes including endchar
//
int convert_hexA(char  *s,int length,char * buffer)
{
	int i=0,j=0;

	char val1,val2;
	bool byte;

	if(length&1){
		byte=true;
		val1=0;
	}else {
		byte=false;
	}

	while (i<length)
	{
		char c;


		c= convert_hexA(s[i]);
		if(c==-1) return 0;
		
		if(byte){
			val2 =c;
			char val=(val1<<4)|val2;
			buffer[j++]=val;
		}
		else {
			val1=c;
		}

		byte=!byte;

		i++;
	}


	assert(!byte);

	return j;
}
void sprint_mem32(char * buffer, unsigned long  va, char * _start,unsigned long  _size )
{
	unsigned long  i,len=0, start_pos,bytes_in_line;

	start_pos=va&0xf;
	va&= 0xfffffff0;

	for( i=0;i<(unsigned long )_size;i+=16 ){
		unsigned long  j,k;
		unsigned char  c;

		//
		// address
		//
		len +=sprintf(buffer+len, "%08x ",va );

		//
		// 
		//
		for( j=0;j<start_pos;j++ ) len +=sprintf(buffer+len, "   ");
			

		bytes_in_line=16-start_pos;
		
		//
		// index of last byte in line
		//
		k=i+bytes_in_line;

		if( k>(unsigned long )_size ) k=(unsigned long )_size;
		for( j=i;j<k;j++) {
			len +=sprintf(buffer+len, "%02x%c",((unsigned char *)_start)[j] ,(j==i+7)?'-':' ');
		}
		
		for( ;j<(unsigned long )i+16 ;j++ ) len +=sprintf(buffer+len, "   ");

		for( j=0;j<start_pos;j++ ) buffer[len++]=' ';
		for( j=i;j<k;j++) 
			len +=sprintf(buffer+len, "%c",(c=((unsigned char *)_start)[j])<' '?'.':(c<0x80?c:'.') );
			
		len +=sprintf(buffer+len, "\xd\xa" );
		//
		// next line
		//
		va +=16;
		start_pos=0;
	}
}

void print_mem16(char * _start,unsigned short  seg,unsigned int  off,long _size )
{
	long i;
	char szBuffer[1024];

	for( i=0;i<_size;i+=16 )
	{
		long j,k;
		unsigned char c;
		char *s=szBuffer;
		k=i+16;

		if( k>_size ) k=_size;
		s +=sprintf(s, "%04x:%04x ",seg,off&0xffff );

		for( j=i;j<k;j++) {

			s+=sprintf(s,"%02x%c",((unsigned char *)_start)[j],(j==i+7)?'-':' ');
			
		}
		
		if( k<i+16 )
		for( ;j<i+16 ;j++ ) s+=sprintf(s,"   ");
		for( j=i;j<k;j++) 
		{
			c=((unsigned char *)_start)[j];
			s+=sprintf(s,"%c",c<' '?'.':(c<0x80?c:'.'));
		}
			
		s+=sprintf(s,"\n" );
		printf( "%s",szBuffer );
		off +=16;
		if(off>0xffff) {seg+=(off>>4);off&=0xff;}
	}
}


void print_mem32(char * _start,unsigned long  va,long _size )
{
	long i;
	char szBuffer[1024];

	for( i=0;i<_size;i+=16 )
	{
		long j,k;
		unsigned char c;
		char *s=szBuffer;
		k=i+16;

		if( k>_size ) k=_size;

		s +=sprintf(s, "%08x ",va );

		for( j=i;j<k;j++) {
			s+=sprintf(s,"%02x%c",((unsigned char *)_start)[j],(j==i+7)?'-':' ');
			
		}
		
		if( k<i+16 )
		for( ;j<i+16 ;j++ ) s+=sprintf(s,"   ");
		for( j=i;j<k;j++) 
		{
			c=((unsigned char *)_start)[j];
			s+=sprintf(s,"%c",c<' '?'.':(c<0x80?c:'.'));
		}
			
		s+=sprintf(s,"\n" );
		printf( "%s",szBuffer );
		va +=16;
	}
}

SLIST *  revert_list(SLIST* list)
{

	SLIST* p=NULL;
	SLIST* q;

	while(q=list)
	{
		list=list->next;

		q->next=p;
		p=q;
	}
	return p;
}


void merge_list(SLIST* dst,SLIST*list_to_add)
{
	SLIST * q=dst->next,*p=dst;
	while(q){
		p=q;
		q=q->next;
	}
	p->next=list_to_add->next;
}
void merge_list_dl(DLIST* dst,DLIST*list_to_add)
{
	DLIST * p=dst->prev;
	DLIST * q=list_to_add->prev;
	
	DLIST * head=list_to_add->next;
	p->next=head;	
	head->prev=p;

	q->next=dst;
	dst->prev=q;


}