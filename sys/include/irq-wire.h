#ifndef __irq_wire_h__
#define __irq_wire_h__

#include "8259.h"
#include "apic.h"


void assert_irq(int irq_no);
void assert_intr();
void deassert_irq(int irq_no);
void deassert_intr();

#endif // __irq_wire_h__