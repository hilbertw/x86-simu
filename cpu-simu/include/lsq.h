#ifndef __LSQ_H__
#define __LSQ_H__


typedef struct lsq_entry {
	unsigned int virt_address;
	bit1_t op_type;   /* 0--read, 1--write */
	bit1_t lock;      /* 1--assert LOCK    */
	
	bit2_t wide;      /* 0/1/2/3: 8/16/32  */

    unsigned int data;  /* rob slot number if read  */
} LSQ_ENTRY;

#define LSQ_ENTRIES 48

extern LSQ_ENTRY lsq [LSQ_ENTRIES];

bit1_t lsq_busy ();

/* IN port interface */
void lsq_push (
	

#endif   /*    __LSQ_H__   */