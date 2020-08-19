#ifndef __acpi82801_h__
#define __acpi82801_h__
#include "acpi.h"
#include "smi82801.h"
#include "irq-wire.h"


typedef struct acpi_reg_82801
{
//00每01h PM1 Status PM1a_EVT_BLK 0000h R/W
//02每03h PM1 Enable PM1a_EVT_BLK+2 0000h R/W
	PM1_EVT_BLK PM1a_evt_blk;

	// 04每07h PM1 Control PM1a_CNT_BLK 00000000h R/W
	PM1_CNT_BLK PM1a_Cnt;

	//08每0Bh PM1 Timer PMTMR_BLK 00000000h RO
	PM_TMR_BLK ddPM1Timer;  

	//0Ch Reserved 〞 〞 〞
	unsigned long ddReserved;   


	//10h每13h Processor Control P_BLK 00000000h R/W
	//14h Level 2 P_BLK+4 00h RO
	//15h Level 3 P_BLK+5 0000hRO
	P_BLK P_blk;
          
	//16每1Fh Reserved 〞 〞 〞
	unsigned char aReserved0[10];

	// 20h PM2 Control PM2a_CNT_BLK 0000h R/W
	PM2_CNT_BLK PM2a_Cnt; 

	//21每27h Reserved 〞 〞 〞
	unsigned char aReserved1[7]; 
//  28每29h General Purpose Event 0 Status GPE0_BLK 0000h R/W
//  2A每2Bh General Purpose Event 0 Enables GPE0_BLK+2 0000h R/W

	GPE0_BLK GPE0_blk;
//  2C每2D General Purpose Event 1 Status GPE1_BLK 0000h R/W
//  2E每2F General Purpose Event 1 Enables GPE1_BLK+2 0000h R/W
	GPE1_BLK GPE1_blk;


unsigned short dwSMIControlEnable;//  30每31h SMI# Control and Enable 0000h R/W
unsigned short dwRserved2;      // 32h-33h
unsigned short dwSMIStatus;//  34每35h SMI Status Register  0000h R/W
unsigned short dwReserved3[5];  //36每3Fh Reserved  0000h RO
unsigned char MonitorSMIStatus; //  40h Monitor SMI Status  0000h R/W
unsigned char aReserved4[3];        //41h-43h Reserved   
unsigned long DeviceTrapStatus;//  44h Device Trap Status  0000h R/W
unsigned long DeviceTrapEnable;//  48h Trap Enable register  0000h R/W
unsigned short BusAddressTracker;//  4Ch每4Dh Bus Address Tracker Last Cycle RO
unsigned char BusCycleTracker;//  4Eh Bus Cycle Tracker Last Cycle RO
unsigned char SpeedStepControl;//  50h SpeedStep Control 0h WO
} ACPI_REG_82801;

class CACPI82801
{
	static unsigned char wc_mask[0x71];// write clear bytes
	static unsigned char ro_mask[0x71];// write clear bytes
public:
	union 
	{
		ACPI_REG_82801 reg;
		unsigned char r[0x71];
	};

	CACPI82801()
	{
	 r[1]=0x81;
	}

	//
	// don't check boundary
	//
	unsigned char operator[](int i){return r[i];}
	void set_reg(int i, unsigned char v);
};
//TCO SMI -TCO TIMEROUT No Yes none TIMEOUT
void do_tco();

//
//Periodic timer expires No Yes PERIODIC_EN=1 PERIODIC_STS
//see GEN_PMCON_1〞General PM Configuration 1 Register (PM〞D31:F0)
//
void do_acpi_periodic();
//
//ACPI Timer overflow (2.34 sec.) Yes Yes TMROF_EN=1 TMROF_STS
//
void do_acpi_timer();

extern CACPI82801  acpi82801;

//
// call sci or smi depending on reg 4h
//
//
//PM1_CNT〞Power Management 1 Control
// SCI Enable (SCI_EN) 〞 R/W. Selects the SCI interrupt or the SMI# interrupt for various events
//including the bits in the PM1_STS register (bit 10, 8, 0), and bits in GPE0_STS.
//0 = These events will generate an SMI#.

__inline void acpi_interrupt(enum smi_type smi_cause)
{
	unsigned char ich82801_get_acpi_cntl();
	if(acpi82801.reg.PM1a_Cnt.pm1_scontrol.SCI_EN==1)
	{
		// lpc interface reg 44h
	    assert_irq(ich82801_get_acpi_cntl()&7);
	}
	else
		invoke_smi(smi_cause);
}


void enable_acpi(bool enable);
#endif //__acpi82801_h__