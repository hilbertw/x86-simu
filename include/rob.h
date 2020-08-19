#ifndef __ROB_H__
#define __ROB_H__

typedef struct rob_entry {
	bit4_t delta_eip;
	bit8_t flags;
	bit8_t flag_mask;
	bit8_t reg_mask;   /* commited registers     */
	bit1_t valid;      /* result available       */
	bit1_t exception;  /* executed successfully? */
	unsigned int data;
} ROB_ENTRY;

#define ROB_ENTRIES 128

extern ROB_ENTRY robs [ROB_ENTRIES];

/* the allocation port interface */
bit7_t rob_port_alloc ( bit4_t delta_eip,
	bit8_t flag_mask,
	bit8_t reg_mask );

/* the update port interface */
void rob_port_write (
	bit3_t mask,      /* 1--flag, 2--data, 4-- exception */
	bit8_t flags,      
	unsigned int data );


#endif   /*    __ROB_H__   */