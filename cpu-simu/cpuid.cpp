#include "cpu.h"
#include "log.h"
#pragma warning (disable:4018)

//#define AMD

#ifndef AMD


// processor brand index
//0- This processor does not support the brand identification feature
//1- Celeron(R) processor?
//2- Pentium(R) III processor?
//3- Intel(R) Pentium III Xeon(R) processor
//4 - 7 - Reserved for future processor
//8- Intel Pentium 4 processor
//9 - 255 - Reserved for future processor
#define CELERON 1
#define PIII 2
#define PIII_XEON 3
#define PIV 8

static char brand_string[]="  Intel(R) Pentium(R) 4 CPU 1500MHz";
void CCPU::cpuid_ () 
{
	//log("cpuid:%x \n",__EAX__);
	switch(__EAX__)
	{

	case 0://1.1 EAX=0: Get vendor ID
		   //a twelve character ASCII string stored in EBX, EDX, ECX - in that order
		   //GenuineIntel processor values returned:
		__EBX__=0x756e6547;
		__EDX__=0x49656e69;
		__ECX__=0x6c65746e;
		__EAX__=3;
		break;

	case 1://1.2 EAX=1: Processor Info and Feature Bits 
		   //This returns the CPU's stepping, model, and family information in EAX (also called the signature of a CPU), 
		   // feature flags in EDX and ECX, and additional feature info in EBX.
		//eax:000006EC ebx:01020800 ecx:0000C189 edx:BFE9FBFF
		// vresion information
		__EAX__=CPU_SIG;
	

		//ebx brand_index | clflush| num_processor|lapic_id
		__EBX__= PIV|     // p4
			(8<<8)|     // cache line size 32 bytes
			(1<<16)|    //number threads per  processor core.HT=0,
			(id<<24);   // local apic physical id, 
						//Some of the bits of LocalApicId represent the CPU core within a processor and 
						//other bits represent the processor ID.
		//edx:feature information
		__ECX__=0x0000C189;
		__EDX__=0xBFE9FBFF;
		break;

	case 2://1.3 EAX=2: Cache and TLB Descriptor Info  
		   //This returns a list of descriptors indicating cache and TLB capabilities in EAX, EBX, ECX and EDX registers.

		__EAX__=1;//The least-significant byte in register EAX (register AL) indicates the number of times the CPUID instruction
		          //must be executed with an input value of 2 to get a complete description of the processor's caches and TLBs. 
		          //The first member of the family of Pentium 4 processors will return a 1.

		//If a register contains valid information, the information is contained in 1 byte descriptors.The most significant bit (bit 31) 
		// of each register indicates whether the register contains valid information (set to 0) or is reserved (set to 1).

		__EBX__=
		__ECX__=0x00000022;
		__EDX__=0x80000000;

		break;
 
	case 3://1.4 EAX=3: Processor Serial Number 
		__EAX__=0x00000000;
	    __EBX__=0x00000000;
		__ECX__=0x00000000;
		__EDX__=0x00000000;
		break;

	case 0x80000000://1.5 EAX=80000000h: Get Highest Extended Function Supported  
		   // The highest calling parameter is returned in EAX
		__EAX__=0x80000008;
		break;

	case 0x80000001://1.6 EAX=80000001h: Extended Processor Info and Feature Bits 
		   // is returns extended feature flags in EDX and ECX.
		__EAX__=0x00000000;
	    __EBX__=0x00000000;
		__ECX__=0x00000000;
		__EDX__=0x00100000;
		break;

	case 0x80000002://1.7 EAX=80000002h,80000003h,80000004h: Processor Brand String 
		__EAX__=*(unsigned long *)brand_string;
		__EBX__=*(unsigned long *)(brand_string+4);
		__ECX__=*(unsigned long *)(brand_string+8);
		__EDX__=*(unsigned long *)(brand_string+12);
		break;
	case 0x80000003:// These return the processor brand string in EAX, EBX, ECX and EDX.
		__EAX__=*(unsigned long *)(brand_string+16);
		__EBX__=*(unsigned long *)(brand_string+20);
		__ECX__=*(unsigned long *)(brand_string+24);
		__EDX__=*(unsigned long *)(brand_string+28);
		break;
	case 0x80000004:// CPUID must be issued with each parameter in sequence to get the entire 48-byte null-terminated ASCII processor brand string.
  		__EAX__=*(unsigned long *)(brand_string+32);
		__EBX__=*(unsigned long *)(brand_string+36);
		__ECX__=*(unsigned long *)(brand_string+40);
		__EDX__=*(unsigned long *)(brand_string+44);		  
		break;

	case 0x80000006://1.9 EAX=80000006h: Extended L2 Cache Features  
		break;

	case 0x80000005://1.8 EAX=80000005h: Reserved 
	case 0x80000007://1.10 EAX=80000007h: Reserved 
		break;

	case 0x80000008://1.11 EAX=80000008h: Virtual and Physical address Sizes 
		   // Returns largest virtual and physical address sizes in EAX.
		__EAX__=0x7fffffff;
		break;
	}
}
//
// intel T2080
//eax:00000000 ebx:eax:0000000A ebx:756E6547 ecx:6C65746E edx:49656E69
//eax:00000001 ebx:eax:000006EC ebx:01020800 ecx:0000C189 edx:BFE9FBFF
//eax:00000002 ebx:eax:02B3B001 ebx:000000F0 ecx:00000000 edx:2C043078
//eax:00000003 ebx:eax:00000000 ebx:00000000 ecx:00000000 edx:00000000
//eax:00000004 ebx:eax:00000000 ebx:00000000 ecx:00000000 edx:00000000
//eax:00000005 ebx:eax:00000040 ebx:00000040 ecx:00000003 edx:00001110
//eax:00000006 ebx:eax:00000001 ebx:00000002 ecx:00000001 edx:00000000
//eax:80000001 ebx:eax:00000000 ebx:00000000 ecx:00000000 edx:00100000
//eax:80000002 ebx:eax:756E6547 ebx:20656E69 ecx:65746E49 edx:2952286C
//eax:80000003 ebx:eax:55504320 ebx:20202020 ecx:20202020 edx:54202020
//eax:80000004 ebx:eax:30383032 ebx:20402020 ecx:33372E31 edx:007A4847
//eax:80000005 ebx:eax:00000000 ebx:00000000 ecx:00000000 edx:00000000
//eax:80000006 ebx:eax:00000000 ebx:00000000 ecx:04004040 edx:00000000
//eax:80000007 ebx:eax:00000000 ebx:00000000 ecx:00000000 edx:00000000
//eax:80000008 ebx:eax:00002020 ebx:00000000 ecx:00000000 edx:00000000

#else
void CCPU::cpuid_ () 
{
	switch(__EAX__)
	{

	case 0://1.1 EAX=0: Get vendor ID
		   //a twelve character ASCII string stored in EBX, EDX, ECX - in that order
		__EAX__=0x00000001; 
		__EBX__=0x68747541; 
		__ECX__=0x444D4163; 
		__EDX__=0x69746E65;
		break;

	case 1://1.2 EAX=1: Processor Info and Feature Bits 
		   //This returns the CPU's stepping, model, and family information in EAX (also called the signature of a CPU), 
		   // feature flags in EDX and ECX, and additional feature info in EBX.
		__EAX__=0x00060FB2;// vresion information 
		__EBX__=0x00020800; //ebx brand_index | clflush| num_processor|lapic_id
		__ECX__=0x00002001; 
		__EDX__=0x178BFBFF;//edx:feature information
		break;

	case 2://1.3 EAX=2: Cache and TLB Descriptor Info  
		   //This returns a list of descriptors indicating cache and TLB capabilities in EAX, EBX, ECX and EDX registers.

		__EAX__=1;//The least-significant byte in register EAX (register AL) indicates the number of times the CPUID instruction
		          //must be executed with an input value of 2 to get a complete description of the processor's caches and TLBs. 
		          //The first member of the family of Pentium 4 processors will return a 1.

		//If a register contains valid information, the information is contained in 1 byte descriptors.The most significant bit (bit 31) 
		// of each register indicates whether the register contains valid information (set to 0) or is reserved (set to 1).

		__EBX__=
		__ECX__=0x00000000;
		__EDX__=0x80000000;

		break;
 
	case 3://1.4 EAX=3: Processor Serial Number 
		__EAX__=
		__EBX__=
		__ECX__=0x00000000;
		__EDX__=0x80000000;
		break;

	case 0x80000000://1.5 EAX=80000000h: Get Highest Extended Function Supported  
		   // The highest calling parameter is returned in EAX
		break;

	case 0x80000001://1.6 EAX=80000001h: Extended Processor Info and Feature Bits 
		   // is returns extended feature flags in EDX and ECX.
		__EAX__=0x00060FB2; 
		__EBX__=0x0000008D1; 
		__ECX__=0x0000011F; 
		__EDX__=0xEBD3FBFF;
		break;
	case 0x80000002://1.7 EAX=80000002h,80000003h,80000004h: Processor Brand String 
		__EAX__=0x20444D41; 
		__EBX__=0x6C687441; 
		__ECX__=0x74286E6F; 
		__EDX__=0x3620296D;
		break;
	case 0x80000003:// These return the processor brand string in EAX, EBX, ECX and EDX.
		__EAX__=0x32582034; 
		__EBX__=0x61754420; 
		__ECX__=0x6F43206C; 
		__EDX__=0x50206572;
		break;
	case 0x80000004:// CPUID must be issued with each parameter in sequence to get the entire 48-byte null-terminated ASCII processor brand string.
		__EAX__=0x65636F72; 
		__EBX__=0x726F7373; 
		__ECX__=0x30323420; 
		__EDX__=0x00002B30;
		break;
	case 0x80000005://1.8 EAX=80000005h: Reserved 
		__EAX__=0xFF08FF08; 
		__EBX__=0xFF20FF20; 
		__ECX__=0x40020140; 
		__EDX__=0x40020140;
		break;

	case 0x80000006://1.9 EAX=80000006h: Extended L2 Cache Features  
		__EBX__=0x42004200;
		__ECX__=0x02008140;
		break;


	case 0x80000007://1.10 EAX=80000007h: Reserved 
		__EDX__=0x0000007F;
		break;

	case 0x80000008://1.11 EAX=80000008h: Virtual and Physical address Sizes 
		   // Returns largest virtual and physical address sizes in EAX.
		__EAX__=0x00002828;
		__ECX__=0x1001;
		break;
	}
}




//AMD AM4200
//eax:00000000 ebx:eax:00000001 ebx:68747541 ecx:444D4163 edx:69746E65
//eax:00000001 ebx:eax:00060FB2 ebx:00020800 ecx:00002001 edx:178BFBFF
//eax:00000002 ebx:eax:00000000 ebx:00000000 ecx:00000000 edx:00000000
//eax:00000003 ebx:eax:00000000 ebx:00000000 ecx:00000000 edx:00000000
//eax:00000004 ebx:eax:00000000 ebx:00000000 ecx:00000000 edx:00000000
//eax:00000005 ebx:eax:00000000 ebx:00000000 ecx:00000000 edx:00000000
//eax:00000006 ebx:eax:00000000 ebx:00000000 ecx:00000000 edx:00000000
//eax:80000001 ebx:eax:00060FB2 ebx:000008D1 ecx:0000011F edx:EBD3FBFF
//eax:80000002 ebx:eax:20444D41 ebx:6C687441 ecx:74286E6F edx:3620296D
//eax:80000003 ebx:eax:32582034 ebx:61754420 ecx:6F43206C edx:50206572
//eax:80000004 ebx:eax:65636F72 ebx:726F7373 ecx:30323420 edx:00002B30
//eax:80000005 ebx:eax:FF08FF08 ebx:FF20FF20 ecx:40020140 edx:40020140
//eax:80000006 ebx:eax:00000000 ebx:42004200 ecx:02008140 edx:00000000
//eax:80000007 ebx:eax:00000000 ebx:00000000 ecx:00000000 edx:0000007F
//eax:80000008 ebx:eax:00003028 ebx:00000000 ecx:00000001 edx:00000000

#endif

