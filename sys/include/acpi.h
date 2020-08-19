#ifndef __acpi_h__
#define __acpi_h__

typedef unsigned char  ACPI_BYTE;
typedef unsigned short ACPI_WORD;
typedef unsigned long  ACPI_DWORD;

typedef struct PM1_Status_Registers_Fixed_Hardware_Feature_Status_Bits
{
	ACPI_WORD TMR_STS:1;
	ACPI_WORD Reserved1:3;
	ACPI_WORD BM_STS:1;
	ACPI_WORD GBL_STS:1;
	ACPI_WORD Reserved2:2;
	ACPI_WORD PWRBTN_STS:1;
	ACPI_WORD SLPBTN_STS:1;
	ACPI_WORD RTC_STS:1;
	ACPI_WORD Ignore:1;// 11
	ACPI_WORD Reserved3:2;
	ACPI_WORD PCIEXP_WAKE_STS :1;
	ACPI_WORD WAK_STS:1;
}PM1_STS;

typedef struct PM1_Enable_Registers_Fixed_Hardware_Feature_Enable_Bits
{
	ACPI_WORD TMR_EN:1;// 0 TMR_EN This is the timer carry interrupt enable bit. When this bit is set 
	                    // then an SCI event is generated anytime the TMR_STS bit is set. When this bit 
	                    // is reset then no interrupt is generated when the TMR_STS bit is set.
	ACPI_WORD Reserved1:4;// 1-4 Reserved. These bits always return a value of zero.
	ACPI_WORD GBL_EN:1;  // 5 GBL_EN The global enable bit. When both the GBL_EN bit and the GBL_STS bit 
	                      // are set, an SCI is raised.
    ACPI_WORD Reserved2:2;// 6-7 Reserved
	ACPI_WORD PWRBTN_EN:1;// 8 PWRBTN_EN This optional bit is used to enable the setting of the 
	                       //  PWRBTN_STS bit to generate a power management event (SCI or wake).
	                       //  The PWRBTN_STS bit is set anytime the power button is asserted. 
	                       //  The enable bit does not have to be set to enable the setting of 
	                       //  the PWRBTN_STS bit by the assertion of the power button (see description of 
	                       //  the power button hardware).
                           //  Support for the power button is indicated by the PWR_BUTTON flag in 
	                       //  the FADT being reset (zero). If the PWR_BUTTON flag is set or a power button
	                       //   device object is present in the ACPI Namespace, then this bit field is ignored by OSPM.
ACPI_WORD SLPBTN_EN:1;//9 SLPBTN_EN This optional bit is used to enable the setting of the SLPBTN_STS bit to generate a power management event (SCI or wake). 
                       // The SLPBTN_STS bit is set anytime the sleep button is asserted. The enable bit does not have to be set to enable the setting of the 
                       // SLPBTN_STS bit by the active assertion of the sleep button (see description of the sleep button hardware). Support for the sleep button 
                       // is indicated by the SLP_BUTTON flag in the FADT being reset (zero). 
                       // If the SLP_BUTTON flag is set or a sleep button device object is present in the ACPI Namespace, then this bit field is ignored by OSPM.
ACPI_WORD RTC_EN:1;// 10 RTC_EN This optional bit is used to enable the setting of the RTC_STS bit to generate a wake event. 
                    // The RTC_STS bit is set any time the RTC generates an alarm.
ACPI_WORD Reserved3:3;//11-13 Reserved. These bits always return a value of zero.
ACPI_WORD PCIEXP_WAKE_DIS:1;// 14 PCIEXP_WAKE_DIS This bit is required for chipsets that implement PCI Express. 
                             // This bit disables the inputs to the PCIEXP_WAKE_STS bit in the PM1 Status register 
                             // from waking the system. Modification of this bit has no impact on the value of the 
                             // PCIEXP_WAKE_STS bit.
ACPI_WORD Reserved4:1;//15 Reserved. These bits always return a value of zero.
} PM1_EN;

typedef struct PM1_Control_Registers_Bits
{
ACPI_WORD SCI_EN:1; // 0 SCI_EN Selects the power management event to be either an SCI or 
                    //SMI interrupt for the following events. When this bit is set, 
                    //then power management events will generate an SCI interrupt. 
                    //When this bit is reset power management events will generate an SMI interrupt. 
                    //It is the responsibility of the hardware to set or reset this bit. 
                    //OSPM always preserves this bit position.
ACPI_WORD  BM_RLD:1;//1 BM_RLD When set, this bit allows the generation of a bus master request to 
                    //cause any processor in the C3 state to transition to the C0 state. 
                    //When this bit is reset, the generation of a bus master request does not affect 
                    //any processor in the C3 state.
ACPI_WORD GBL_RLS:1;//2 GBL_RLS This write-only bit is used by the ACPI software to raise an event to 
                    //the BIOS software, that is, generates an SMI to pass execution control to 
                    //the BIOS for IA-PC platforms. BIOS software has a corresponding enable and status bit
                    //to control its ability to receive ACPI events (for example, BIOS_EN and BIOS_STS). 
                    //The GBL_RLS bit is set by OSPM to indicate a release of the Global Lock and 
                    //the setting of the pending bit in the FACS memory structure.
ACPI_WORD Reserved1:6;// 3-8 Reserved. These bits are reserved by OSPM.
ACPI_WORD Ignore:1;  //9 IgnoreSoftware ignores this bit field.
ACPI_WORD SLP_TYPx:3;//10-12 SLP_TYPx Defines the type of sleeping state the system enters 
                     //when the SLP_EN bit is set to one. This 3-bit field defines the type of hardware 
                     //sleep state the system enters when the SLP_EN bit is set. The \_Sx object 
                     //contains 3-bit binary values associated with the respective sleeping state 
                     //(as described by the object). OSPM takes the two values from the \_Sx object 
                     //and programs each value into the respective SLP_TYPx field.
ACPI_WORD SLP_EN:1;//13  SLP_EN This is a write-only bit and reads to it always return a zero. 
                   //Setting this bit causes the system to sequence into the sleeping state associated with
                   //the SLP_TYPx fields programmed with the values from the \_Sx object.
ACPI_WORD Reserved2:2;//14-15 Reserved. This field always returns zero.
}  PM1_CNT;


typedef struct PM_Timer_Bits
{
	ACPI_DWORD  TMR_VAL:24;//0-23 TMR_VAL This read-only field returns the running count of 
	                       //the power management timer. This is a 24-bit counter 
	                       //that runs off a 3.579545-MHz clock and counts while in the S0 working 
	                       //system state. The starting value of the timer is undefined, thus 
	                       //allowing the timer to be reset (or not) by any transition to the 
	                       //S0 state from any other state. The timer is reset (to any initial value), 
	                       //and then continues counting until the system＊s 14.31818 MHz clock is 
	                       //stopped upon entering its Sx state. If the clock is restarted without a reset,
	                       //then the counter will continue counting from where it stopped.
	ACPI_DWORD   E_TMR_VAL:8;// 24-31 E_TMR_VAL This read-only field returns the upper eight bits of a 
	                         // 32-bit power management timer. If the hardware supports a 32-bit timer, 
	                         //then this field will return the upper eight bits;
} PM_TMR;

//size PM2_CNT_LEN

typedef struct PM2_Control_Register_Bits
{
	ACPI_DWORD ARB_DIS:1;//  0 ARB_DIS This bit is used to enable and disable the system arbiter. 
	                     // When this bit is CLEAR the system arbiter is enabled and the arbiter 
	                     // can grant the bus to other bus masters. When this bit is SET the system 
	                     //arbiter is disabled and the default CPU has ownership of the system.
                         //OSPM clears this bit when using the C0, C1 and C2 power states.
	ACPI_DWORD  Reserved:31;//Reserved >0 Reserved
}	PM2_CNT;


typedef struct Processor_Control_Register_Bits
{
	ACPI_DWORD  CLK_VAL:4; // 0-3 CLK_VAL Possible locations for the clock throttling value.
	ACPI_DWORD  THT_EN:1;  //4 THT_EN This bit enables clock throttling of the clock as set in the CLK_VAL field. 
	                       //   THT_EN bit must be reset LOW when changing the CLK_VAL field (changing the duty setting).

	ACPI_DWORD   CLK_THROTTLING_VAL:27;//5-31 CLK_VAL Possible locations for the clock throttling value.
} P_CNT;

typedef ACPI_BYTE P_LVL2;
typedef ACPI_BYTE P_LVL3;

//each register block contains its own length variable in the FADT
#define PM1_EVT_LEN  4  // PM1_EVT_LEN field in the FADT
#define GPE0_LEN     4
#define GPE1_LEN     4

typedef ACPI_BYTE GPE0_STS[GPE0_LEN/2];
typedef ACPI_BYTE GPE0_EN[GPE0_LEN/2];
typedef ACPI_BYTE GPE1_STS[GPE1_LEN/2];
typedef ACPI_BYTE GPE1_EN[GPE1_LEN/2];

//typedef ACPI_BYTE PM1_STS[PM1_EVT_LEN/2];
//typedef ACPI_BYTE PM1_EN[PM1_EVT_LEN/2];
//
// register blocks
//

typedef struct _PM1_EVT_BLK
{
PM1_STS pm1_states;
PM1_EN pm1_enable;
}PM1_EVT_BLK;

typedef struct _PM1_CNT_BLK
{
PM1_CNT pm1_scontrol;
}PM1_CNT_BLK;

typedef struct _PM2_CNT_BLK
{
PM2_CNT pm2_scontrol;
}PM2_CNT_BLK;


typedef struct _PM_TMR_BLK
{
PM_TMR pm_timer;
}PM_TMR_BLK;

typedef struct _P_BLK
{
P_CNT peocessor_control;
P_LVL2 processor_level2;
P_LVL3 processor_level3;
}P_BLK;


typedef struct _GPE1_BLK
{
GPE1_STS gp_event1_states;
GPE1_EN gp_event1_enble;
}GPE1_BLK;



typedef struct _GPE0_BLK
{
GPE0_STS gp_event0_states;
GPE0_EN gp_event0_enble;
}GPE0_BLK;
#if 0
typedef struct acpi_reg_845g
{
//00每01h PM1 Status PM1a_EVT_BLK 0000h R/W
//02每03h PM1 Enable PM1a_EVT_BLK+2 0000h R/W
	PM1_EVT_BLK PM1a_evt_blk;

	// 04每07h PM1 Control PM1a_CNT_BLK 00000000h R/W
	PM1a_CNT_BLK PM1_Cnt;

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
} ACPI_REG;
#endif

#endif // __acpi_h__