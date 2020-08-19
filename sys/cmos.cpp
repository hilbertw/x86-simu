#define _CRT_SECURE_NO_WARNINGS 1

#include <assert.h>
#include <stdio.h>
#include "config.h"
#include "util.h"
#include "addressmap.h"
#include "sys.h"
#include "log.h"
#include "irq-wire.h"
#define ASSERT_IRQ8() assert_irq(8)
#define DEASSERT_IRQ8() 

unsigned char modified[0x100];
unsigned char cmos[0x100]=
{
 0x25,//	0000
 0x30,//	0001
 0x55,//	0002
 0x30,//	0003
 0x22,//	0004
 0x12,//	0005
 0x07,//	0006
 0x25,//	0007
 0x01,//	0008
 0x09,//	0009
 0x00,//	000A
 0x02,//	000B
 0x50,//	000C
 0x95,//	000D
 0x00,//	000E
 0x00,//	000F
 0x44,//	0010
 0x30,//	0011
 0x0F,//	0012
 0x30,//	0013
 0x0F,//	0014
 0x80,//	0015
 0x02,//	0016
 0xFF,//	0017
 0xFF,//	0018
 0x31,//	0019
 0x2F,//	001A
 0xFF,//	001B
 0x3F,//	001C
 0x10,//	001D
 0x00,//	001E
 0x00,//	001F
 0x3F,//	0020
 0xFF,//	0021
 0x3F,//	0022
 0x10,//	0023
 0x00,//	0024
 0x00,//	0025
 0x3F,//	0026
 0x00,//	0027
 0x00,//	0028
 0x30,//	0029
 0x87,//	002A
 0x47,//	002B
 0x43,//	002C
 0x43,//	002D
 0x08,//	002E
 0x3C,//	002F
 0xFF,//	0030
 0xFF,//	0031
 0x20,//	0032
 0xBF,//	0033
 0xBF,//	0034
 0x7F,//	0035
 0x07,//	0036
 0x00,//	0037
 0x00,//	0038
 0x00,//	0039
 0x00,//	003A
 0x00,//	003B
 0x00,//	003C
 0x00,//	003D
 0x21,//	003E
 0xD7,//	003F
 0x00,//	0040
 0x00,//	0041
 0x00,//	0042
 0x00,//	0043
 0x00,//	0044
 0x00,//	0045
 0x00,//	0046
 0x00,//	0047
 0x00,//	0048
 0x00,//	0049
 0x00,//	004A
 0x00,//	004B
 0x30,//	004C
 0x0F,//	004D
 0x00,//	004E
 0x00,//	004F
 0x00,//	0050
 0x00,//	0051
 0x00,//	0052
 0x00,//	0053
 0x00,//	0054
 0x00,//	0055
 0x00,//	0056
 0x00,//	0057
 0x00,//	0058
 0x00,//	0059
 0x00,//	005A
 0x00,//	005B
 0x00,//	005C
 0x00,//	005D
 0x00,//	005E
 0x01,//	005F
 0x96,//	0060
 0x00,//	0061
 0x00,//	0062
 0x00,//	0063
 0x00,//	0064
 0x00,//	0065
 0x7C,//	0066
 0x64,//	0067
 0x50,//	0068
 0xA5,//	0069
 0x4B,//	006A
 0x4B,//	006B
 0x19,//	006C
 0x04,//	006D
 0x04,//	006E
 0x00,//	006F
 0xF7,//	0070
 0xE0,//	0071
 0xF0,//	0072
 0x05,//	0073
 0x0A,//	0074
 0x00,//	0075
 0x0F,//	0076
 0x00,//	0077
 0x3C,//	0078
 0x5A,//	0079
 0x78,//	007A
 0x88,//	007B
 0xE0,//	007C
 0x08,//	007D
 0x01,//	007E
 0x00,//	007F
 0x5F,//	0080
 0x54,//	0081
 0x4A,//	0082
 0x45,//	0083
 0x59,//	0084
 0x14,//	0085
 0x4A,//	0086
 0x00,//	0087
 0x79,//	0088
 0x1E,//	0089
 0x1E,//	008A
 0x20,//	008B
 0x20,//	008C
 0x83,//	008D
 0x00,//	008E
 0x00,//	008F
 0x0F,//	0090
 0x0C,//	0091
 0x00,//	0092
 0x00,//	0093
 0x00,//	0094
 0x00,//	0095
 0x20,//	0096
 0x33,//	0097
 0x54,//	0098
 0x10,//	0099
 0x76,//	009A
 0x98,//	009B
 0xBA,//	009C
 0x00,//	009D
 0x32,//	009E
 0x54,//	009F
 0x76,//	00A0
 0x98,//	00A1
 0xBA,//	00A2
 0x00,//	00A3
 0x40,//	00A4
 0x7D,//	00A5
 0xD4,//	00A6
 0x47,//	00A7
 0x00,//	00A8
 0x00,//	00A9
 0x00,//	00AA
 0x0F,//	00AB
 0x00,//	00AC
 0x10,//	00AD
 0x11,//	00AE
 0x1A,//	00AF
 0x08,//	00B0
 0x1A,//	00B1
 0x09,//	00B2
 0x40,//	00B3
 0x00,//	00B4
 0x00,//	00B5
 0xC0,//	00B6
 0x80,//	00B7
 0x01,//	00B8
 0x00,//	00B9
 0xC0,//	00BA
 0x00,//	00BB
 0x00,//	00BC
 0x00,//	00BD
 0x00,//	00BE
 0x00,//	00BF
 0x00,//	00C0
 0x00,//	00C1
 0x00,//	00C2
 0x00,//	00C3
 0x80,//	00C4
 0x02,//	00C5
 0x60,//	00C6
 0x90,//	00C7
 0x8C,//	00C8
 0x00,//	00C9
 0x00,//	00CA
 0x00,//	00CB
 0x57,//	00CC
 0x18,//	00CD
 0x80,//	00CE
 0xA0,//	00CF
 0xD4,//	00D0
 0x42,//	00D1
 0x89,//	00D2
 0x28,//	00D3
 0x8D,//	00D4
 0xF8,//	00D5
 0x3F,//	00D6
 0x09,//	00D7
 0x00,//	00D8
 0x48,//	00D9
 0xA0,//	00DA
 0x3C,//	00DB
 0x0B,//	00DC
 0xB1,//	00DD
 0x06,//	00DE
 0x03,//	00DF
 0x3F,//	00E0
 0xBD,//	00E1
 0xF5,//	00E2
 0xF9,//	00E3
 0xF7,//	00E4
 0x79,//	00E5
 0xA4,//	00E6
 0x04,//	00E7
 0xC8,//	00E8
 0xC8,//	00E9
 0x00,//	00EA
 0x00,//	00EB
 0x00,//	00EC
 0x00,//	00ED
 0x00,//	00EE
 0x00,//	00EF
 0x0C,//	00F0
 0x00,//	00F1
 0x00,//	00F2
 0x00,//	00F3
 0xFF,//	00F4
 0xFF,//	00F5
 0xFF,//	00F6
 0xFF,//	00F7
 0xFF,//	00F8
 0xFF,//	00F9
 0xFF,//	00FA
 0xFF,//	00FB
 0xFF,//	00FC
 0x7F,//	00FD
 0xFF,//	00FE
 0xFF//	00FF
};
//
// unit 1us.
//
unsigned long pi_rate[]=
{
0,//0000 = Interrupt never toggles
3905,//0001 = 3.90625 ms
7812,//0010 = 7.8125 ms
122,//0011 = 122.070 ¦Ìs
244,//0100 = 244.141 ¦Ìs
488,//0101 = 488.281 ¦Ìs
976,//0110 = 976.5625 ¦Ìs
2000,//0111 = 1.953125 ms
4000,//1000 = 3.90625 ms
8000,//1001 = 7.8125 ms
15625,//1010 = 15.625 ms
31250,//1011 = 31.25 ms
62500,//1100 = 62.5 ms
125000,//1101 = 125 ms
250000//1110 = 250 ms
};

int s_bcd0,s_bcd1,
m_bcd0,m_bcd1,
h_bcd0,h_bcd1,
dom_bcd0,dom_bcd1,//day of month
dow_bcd0,dow_bcd1, // day of week
mon_bcd0,mon_bcd1,
y_bcd0,y_bcd1;

unsigned long count;// for divider chain.
unsigned long one_second;
//
// be called every 1us
//

void do_cmos_rtc()
{ 
	void do_acpi_periodic();// be lazy, get a 1Hz clock here

	one_second++;
	if(one_second>=100000      // suppose clok.255HMz, get 0.025s to make software faster
		&& (cmos[0xb]&0x80)==0)// update cycle occurs normally. once a second
	{
		one_second=0;

		do_acpi_periodic();

		if(s_bcd0==9){s_bcd1+=0x10;s_bcd0=0;}
		else s_bcd0++;

		cmos[0]=s_bcd0|s_bcd1;

		if(cmos[0]==0x59)
		{
			s_bcd1=s_bcd0=0;

			if(m_bcd0==9){m_bcd1+=0x10;m_bcd0=0;}
			else m_bcd0++;

			cmos[2]=m_bcd0|m_bcd1;	
		}

		if(cmos[2]==0x59)
		{
			m_bcd1=m_bcd0=0;

			if(h_bcd0==9){h_bcd1+=0x10;h_bcd0=0;}
			else h_bcd0++;
			cmos[4]=h_bcd0|h_bcd1;
		}

		if(cmos[4]==0x23)
		{
			h_bcd1=h_bcd0=0;

			if(dow_bcd0==7){dow_bcd0=0;}
			else dow_bcd0++;

			if(dom_bcd0==9){dom_bcd1+=0x10;dom_bcd0=0;}
			else dom_bcd0++;

			cmos[6]=dow_bcd0|dow_bcd1;
			cmos[7]=dom_bcd0|dom_bcd1;

		}


		if(cmos[7]==0x30)// 30 days now.
		{
			dom_bcd1=dom_bcd0=0;

			if(mon_bcd0==9){mon_bcd1+=0x10;mon_bcd0=0;}
			else mon_bcd0++;
			cmos[8]=mon_bcd0|mon_bcd1;
		}

		if(cmos[8]==0x12)// 12 months
		{
			mon_bcd1=mon_bcd0=0;

			if(y_bcd0==9){y_bcd1+=0x10;y_bcd0=0;}
			else y_bcd0++;
			cmos[9]=y_bcd0|y_bcd1;
		}
 
		//
		// alarm?
		//
		if( cmos[2]==cmos[3]
		&& cmos[4]==cmos[5]
		&& cmos[0]==cmos[1])
		{
			cmos[0xc]|=0xa0;
			
			if((cmos[0xb]&0x20)==0x20)//irq8 alarm
			ASSERT_IRQ8(); 
			//pci irqb#
		}
		 cmos[0xc]|=0x10; // update end
		 if(cmos[0xb]&0x10)
			ASSERT_IRQ8(); //irq8,update end
	}
	//if regA.rs!=0
	// if periodcal interrupt enabled 
	//
	if((cmos[0xa]&0xf)!=0)
	{
		count++;
		//
		// gen int
		//
		if(count>=pi_rate[(cmos[0xa]&0xf)-1])	
		{
			count=0;
			cmos[0xc]|=0xc0;		
			if((cmos[0xb]&0x40)==0x40)//irq8
				ASSERT_IRQ8(); 
		}


	}

	return;
}



unsigned char port_70h;
unsigned char port_72h;

 void cmos_handler(
	  void *lpContext,
	  ADDR_RANGE_CMD op, 
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,                    // optional 
	  unsigned int nBytes )            // in or out buffer
 {
	 switch(op)
	 {
	 case ADDR_RANGE_READ:  
		 *pszBuff=(dwAddress==0x71)?(char)cmos[port_70h&0x7f]:
			 (dwAddress==0x70)?(unsigned char)port_70h:
			 (dwAddress==0x73)?(char)cmos[(unsigned char)port_72h]:
			 (dwAddress==0x72)?(unsigned char)port_72h:0; 

		 if(dwAddress==0x71)
		 {
			unsigned int reg_no=port_70h&0x7f;

			 if(reg_no==0xc)
			 {
				 cmos[0xc]=0;
				 DEASSERT_IRQ8() 
			 }
			 else	if(reg_no==0xa)
				 cmos[0xa]^=0x80;// make uip edges


		 }

		 break;
	 case ADDR_RANGE_WRITE: 
		 if(dwAddress==0x70)port_70h=(unsigned char)*pszBuff;
		else if (dwAddress==0x71)
		{
			unsigned int reg_no=port_70h&0x7f;

			//if(reg_no==0x2e
			//	||reg_no==0x2f
			//	||reg_no==0x3e
			//	||reg_no==0x3f)
			//{
			//	if(cmos[reg_no]!=(unsigned char)*pszBuff)
			//	{
			//		log("cmos checksum error:%x %x %x\n",reg_no,(unsigned char)*pszBuff,cmos[reg_no]);
			//	}
			//	//break;
			//}

			cmos[reg_no]=(unsigned char)*pszBuff;
			modified[reg_no>>3]|=1<<(reg_no%7);

			 //
			 // Status Register D
			 //Bit 7 - Valid RAM - 1 indicates batery power good, 0 if dead or disconnected.
			 //
			 if(reg_no==0xd)
			 {
				 cmos[0xd]|=0x80;
			 }

			//if(reg_no<=0x77&&reg_no>=0x10) update_chksum();
		}
		else if(dwAddress==0x72)port_72h=(unsigned char)*pszBuff;
		else if (dwAddress==0x73)
		{
			unsigned int reg_no=port_72h;//|0x80;
			cmos[reg_no]=(unsigned char)*pszBuff;
			modified[reg_no>>3]|=1<<(reg_no%7);
		}
		 break;
	 default:
		 return;
	 }
#if  LOG_CMOS
	 if(dwAddress==0x71&&(port_70h&0x7f)==0x12&&op!=ADDR_RANGE_READ)
	 log4(
		 "CMOS",
		 op==ADDR_RANGE_READ?'r':'w',
		 port_70h&0x7f,	
 		nBytes==1? (unsigned char)pszBuff[0]:
		nBytes==2? *(unsigned short *)pszBuff:
		nBytes==4? *(unsigned long *)pszBuff:0);

	 else
	 if(dwAddress==0x73)
	 log4(
		 "CMOS 1",
		 op==ADDR_RANGE_READ?'r':'w',
		 port_72h,	
 		nBytes==1? (unsigned char)pszBuff[0]:
		nBytes==2? *(unsigned short *)pszBuff:
		nBytes==4? *(unsigned long *)pszBuff:0);


#endif

 }
bool load_cmos()
{
	char buffer[100];
	FILE * fp=fopen(CFG_CMOS_DAT,"rt");
	if(!fp) return true;

	buffer[0]=0;
	while(1)
	{
		unsigned int reg_no,value;
		int i;
		fgets(buffer,sizeof(buffer),fp);
		if(feof(fp)) break;
		if(buffer[0]=='\n')
			continue;
		
		i=htoiA(buffer,':',2,reg_no);
		if(i==0) continue;// skip lines with syntax error
		i=htoiA(buffer+i,';',2,value);

		cmos[reg_no]=value;
	}

	cmos[0xd]=0x80;
	fclose(fp);
	return true;
}

bool save_cmos()
{
	unsigned int i;
	FILE * fp=fopen(CFG_CMOS_DAT,"wt");
	if(!fp) return false;

	for(i=0;i<0x100;i++)
	{
		fprintf(fp,"%02x:%02x;//%c\n",
			i,
			cmos[i],
			(modified[i>>3]&(1<<(i&7)))?'*':' ');
	}

	fclose(fp);
	return true;
}

//
// test code
//
//
//sum1:10~2D
//

//                xor     bx, bx
//                mov     ah, 0
//                mov     cl, 90h ; '?

//loc_F007:                               ; CODE XREF: seg000:F01Aj
//                mov     al, cl
//                out     70h, al         ; CMOS Memory:
//                                        ;
//                jcxz    short $+2
//                jcxz    short $+2
//                in      al, 71h         ; CMOS Memory
//                jcxz    short $+2
//                add     bx, ax
//                inc     cl
//                cmp     cl, 0ADh ; '?
//                jbe     short loc_F007

//                mov     al, 0AEh ; '?
//                mov     ah, bh
//                call    wr_cmos
//                mov     al, 0AFh ; '?

//loc_F02C:                               ; CODE XREF: seg000:F03Fj
//                mov     ah, bl
//                jmp     wr_cmos

//
// sum2:37~77
//
//                xor     bx, bx
//                mov     ah, 0
//                mov     cx, 0B7h ; 

//loc_EFDC:                               ; CODE XREF: sub_F031-39j
//                mov     al, cl
//                out     70h, al         ; CMOS Memory:
//                                        ;
//                jcxz    short $+2
//                jcxz    short $+2
//                in      al, 71h         ; CMOS Memory
//                jcxz    short $+2
//                add     bx, ax
//                inc     cx
//                cmp     cx, 0BEh ; 
//                jnz     short loc_EFF3
//                inc     cx
//                inc     cx

//loc_EFF3:                               ; CODE XREF: sub_F031-42j
//                cmp     cx, 0F7h
//                jbe     short loc_EFDC
//                add     bx, 8018h

#if 0
void calc_chksum()
{
	unsigned short i,s=0;

	for(i=0x10;i<0x2e;i++) 
	{
		s+= (unsigned short)cmos[i];
	}

	cmos[0x2f]=(unsigned char)s;
	cmos[0x2e]=(unsigned char)(s>>8);

	s=0x8018;
	for(i=0x37;i<=0x77;i++)
	{
		if(i==0x3e) i++;
		else s+= (unsigned short)cmos[i];
	}

	//assert(s!=0x8018);

	cmos[0x3f]=(unsigned char)s;
	cmos[0x3e]=(unsigned char)(s>>8);
}
#endif