#include "log.h"
#include "irq-wire.h"
#include "cpu.h"

extern CAPICBus apic_bus;
extern CIoAPIC io_apic;
extern CCPU cpu[CFG_N_CPUS];

//
//
// irq0,1,8-->i/o apic->bsp local apic                                  
//       |                 |
//       |                 V
//       +---> 8259a---> imcr--->cpu0  intr             pic  mode
//               |         
//               +-----> bsp  lint0              virtual wire mode via local apic
//               +----->io_apic    
//                      +----------> bsp lint0   virtual wire mode via io apic
//                      +----------> apic bus    virtual wire mode via symmetric io apic
//
//interrupt mode configuration register
// default: pic mode
unsigned char imcr;


void assert_irq(int irq_no)
{
	//
	// 
	//

	if(irq_no<0x10)
		_8259_assert_irq((irq_no>>3),(irq_no&7));

	if(irq_no==0)
	io_apic.assert_irq(2);
	else 
	io_apic.assert_irq(irq_no);


}
void deassert_irq(int irq_no)
{
	//
	// 
	//

	if(irq_no<0x10)
		_8259_deassert_irq((irq_no>>3),(irq_no&7));

	if(irq_no==0)
	io_apic.deassert_irq(2);
	else 
	io_apic.deassert_irq(irq_no);


}

//
// 8259 intr
//
void assert_intr()
{
	//log0("assert intr:imcr.en=%x.\n",imcr);
	//
	// if imcr.en=0,bypass local apic
	//
	if(imcr==0)
		cpu[0].assert_intr();
	else
	{
		cpu[0].lapic.assert_lint0();
		io_apic.assert_irq(0);   // ExtINT
	}
}



//
// 8259 intr
//
void deassert_intr()
{
	//
	// if imcr.en=0,bypass local apic
	//
	if(imcr==0)
		cpu[0].deassert_intr();
	else
	{
		cpu[0].lapic.deassert_lint0();
		io_apic.deassert_irq(0);   // ExtINT
	}
}



static unsigned char port_22h;

void io_imcr(
	  void *lpContext,
	  ADDR_RANGE_CMD op, 
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,                    // optional 
	  unsigned int nBytes )            // in or out buffer
 {
	 switch(op)
	 {
	 case ADDR_RANGE_READ:  
		 if(nBytes==1)
		 {
			 *pszBuff=(dwAddress==0x22)?port_22h:imcr; 
		 }
		 else
		 {
			 pszBuff[0]=port_22h;
			 pszBuff[1]=imcr;
		 }
		 break;
	 case ADDR_RANGE_WRITE: 
		 assert(nBytes==1);
		 		 
		 if(dwAddress==0x22)
		{
			port_22h=(unsigned char)*pszBuff;
			}
		else if(port_22h==0x70)
		{
			imcr=(unsigned char)*pszBuff;
#if LOG_APIC
			log("Select %cPIC mode.\n",imcr?'A':' ');
#endif
		}
		 break;
	 default:
		 return;
	 }

 }