#ifndef  __MSR_H__
#define  __MSR_H__

#define MSR_P5_MC_ADDR           0x00 //0         
#define MSR_P5_MC_TYPE			 0x01 //1   
#define MSR_TSC					 0x10 //16  
#define MSR_CESR				 0x11 //17  
#define MSR_CTR0				 0x12 //18  
#define MSR_CTR1				 0x13 //19  
#define MSR_APICBASE			 0x1B //27  
#define MSR_EBL_CR_POWERON		 0x2A //42  
#define MSR_TEST_CTL			 0x33 //51  
#define MSR_BIOS_UPDT_TRIG		 0x79 //121 
#define MSR_BBL_CR_D0			 0x88 //136 
#define MSR_BBL_CR_D1			 0x89 //137 
#define MSR_BBL_CR_D2			 0x8A //138 
#define MSR_BBL_CR_D3			 0x8B //139 
#define MSR_PERFCTR0			 0xC1 //193 
#define MSR_PERFCTR1			 0xC2 //194 
#define MSR_MTRRcap				 0xFE //254 
#define MSR_BBL_CR_ADDR			 0x116 //278 
#define MSR_BBL_CR_DECC			 0x118 //280 
#define MSR_BBL_CR_CTL			 0x119 //281 
#define MSR_BBL_CR_TRIG			 0x11A //282 
#define MSR_BBL_CR_BUSY			 0x11B //283 
#define MSR_BBL_CR_CTL3			 0x11E //286 
#define MSR_MCG_CAP              0x179 //377 
#define MSR_MCG_STATUS			 0x17A //378 
#define MSR_MCG_CTL				 0x17B //379 
#define MSR_EVNTSEL0			 0x186 //390 
#define MSR_DEBUGCTLMSR			 0x1D9 //473 
#define MSR_LASTBRANCHFROMIP	 0x1DB //475 
#define MSR_LASTBRANCHTOIP		 0x1DC //476 
#define MSR_LASTINTFROMIP		 0x1DD //477 
#define MSR_LASTINTTOIP			 0x1DE //478 
#define MSR_ROB_CR_BKUPTMPDR6	 0x1E0 //480 
#define MSR_MTRRphysBase0		 0x200 //512 
#define MSR_MTRRphysMask0		 0x201 //513 
#define MSR_MTRRphysBase1		 0x202 //514 
#define MSR_MTRRphysMask1		 0x203 //515 
#define MSR_MTRRphysBase2		 0x204 //516 
#define MSR_MTRRphysMask2		 0x205 //517 
#define MSR_MTRRphysBase3		 0x206 //518 
#define MSR_MTRRphysMask3		 0x207 //519 
#define MSR_MTRRphysBase4		 0x208 //520 
#define MSR_MTRRphysMask4		 0x209 //521 
#define MSR_MTRRphysBase5		 0x20A //522 
#define MSR_MTRRphysMask5		 0x20B //523 
#define MSR_MTRRphysBase6		 0x20C //524 
#define MSR_MTRRphysMask6		 0x20D //525 
#define MSR_MTRRphysBase7		 0x20E //526 
#define MSR_MTRRphysMask7		 0x20F //527 
#define MSR_MTRRfix64K_00000	 0x250 //592 
#define MSR_MTRRfix16K_80000	 0x258 //600 
#define MSR_MTRRfix16K_A0000	 0x259 //601 
#define MSR_MTRRfix4K_C0000		 0x268 //616 
#define MSR_MTRRfix4K_C8000		 0x269 //617 
#define MSR_MTRRfix4K_D0000		 0x26A //618 
#define MSR_MTRRfix4K_D8000		 0x26B //619 
#define MSR_MTRRfix4K_E0000		 0x26C //620 
#define MSR_MTRRfix4K_E8000		 0x26D //621 
#define MSR_MTRRfix4K_F0000		 0x26E //622 
#define MSR_MTRRfix4K_F8000		 0x26F //623 
#define MSR_MTRRdefType			 0x2FF //767 
#define MSR_MC0_CTL              0x400 //1024            
#define MSR_MC0_STATUS			 0x401 //1025 
#define MSR_MC0_ADDR			 0x402 //1026 
#define MSR_MC0_MISC			 0x403 //1027 
#define MSR_MC1_CTL				 0x404 //1028 
#define MSR_MC1_STATUS			 0x405 //1029 
#define MSR_MC1_ADDR			 0x406 //1030 
#define MSR_MC1_MISC			 0x407 //1031
#define MSR_MC2_CTL              0x408 //1032 
#define MSR_MC2_STATUS			 0x409 //1033 
#define MSR_MC2_ADDR			 0x40A //1034 
#define MSR_MC2_MISC			 0x40B //1035 
#define MSR_MC4_CTL				 0x40C //1036 
#define MSR_MC4_STATUS			 0x40D //1037 
#define MSR_MC4_ADDR			 0x40E //1038 
#define MSR_MC4_MISC			 0x40F //1039 
#define MSR_MC3_CTL				 0x410 //1040 
#define MSR_MC3_STATUS			 0x411 //1041 
#define MSR_MC3_ADDR			 0x412 //1042 
#define MSR_MC3_MISC			 0x413 //1043 
				   
#define MAX_MSR1 MSR_MC3_MISC+1		   
					   
#define MAX_MSR2 0x20			   
#define MAX_MSR3 0x20

#endif   /* __MSR_H__*/