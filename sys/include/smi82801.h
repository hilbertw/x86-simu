#ifndef __smi82801_h__
#define __smi82801_h__


void reset_smi();
//Starts Software SMI# Timer. When the SWSMI timer expires 
//(the timeout period depends upon the SWSMI_RATE_SEL bit setting), 
//SWSMI_TMR_STS is set and an SMI# is generated.



void do_smi_timer();
// check XX_EN and XX_STS to assert smi#
void do_smi();
enum smi_type
{
	SMI_APMC_WRITE,
	SMI_SMI_TIMER,
	SMI_TCO,
	SMI_ACPI_TIMER,
	SMI_ACPI_PER
};

typedef struct smi_source_desc
{
	unsigned char enable_reg;  // -1, no enabling condition required
	unsigned char enable_mask;
	unsigned char report_reg;  // -1 SMBus Host controller reports to SMBus host status reg.
	unsigned char report_mask;

}SMI_CAUSE_DESC;

void   invoke_smi(enum smi_type type );
#endif //__smi82801_h__