#include <stdio.h>
#include "mch.h"
#include "apic.h"
#include "vga.h"
#include "log.h"
#include "cpu.h"
extern int cpu_no;
extern CCPU cpu[];
extern CIoAPIC io_apic;
extern bool step;
//bool CMCH::smram_decode(){return smram_open||(smram_en&&/*cpu[cpu_no].SMIACT()*/smiact);}
//void CMCH::assert_smiact(bool a) {smiact=a;}

char CMCH::mem_read_byte(unsigned int address)
{
	unsigned char dat;
	switch(mem_rd_addr_range_type(address))
	{
	case 1: // vram  graphics:
		dat=vram_read_byte1(address);
		break;
	case 2: // vram  text:
		dat=vram_read_byte(address-0xb8000);
		break;
	case 3:// vga bios
		dat=((char *)vgabios_map)[address-0xc0000];
		break;

	case 4:// c0000~f0000:fwd to hub?not shadowed
		dat=0xff;
		break;
 	case 5:// f0000~100000,  not shadowed
		dat=ich.mem_read_byte(address|0xfff00000);
		break;
	case 6: // ram
		dat=*(((char *)ram_map)+address);
		break;
	case 7: // apic: only accessed in dword
		dat=0;
		break;
	case 8: // ich
		ich.mem_read_string(address,(char *)&dat,1);
		break;
	case 9:
		//
		// smram
		//
		dat=*(((char *)smram_map+address-0xa0000));
		break;
	}
	
	
	return dat;
} 
short CMCH::mem_read_short(unsigned int address)
{
	unsigned short dat;
	switch(mem_rd_addr_range_type(address))
	{
	case 1: // vram  graphics:
		dat=vram_read_word1(address);
		break;
	case 2: // vram  text:
		dat=vram_read_word(address-0xb8000);
		break;
	case 3:// vga bios
		dat=*(unsigned short*)((char *)vgabios_map+(address-0xc0000));
		break;

	case 4:// c0000~f0000:fwd to hub?not shadowed,n/a
		dat=0xffff;
		break;
 	case 5:// f0000~100000,  not shadowed
		dat=ich.mem_read_short(address|0xfff00000);
		break;
	case 6: // ram
		dat=*(unsigned short*)((char *)ram_map+address);
		break;
	case 7: // apic: only accessed in dword
		dat=0;
		break;
	case 8: // ich
		ich.mem_read_string(address,(char *)&dat,2);
		break;
	case 9:
		//
		// smram
		//
		dat=*(unsigned short*)(((char *)smram_map+address-0xa0000));
		break;
	}
	
	
	return dat;
}
int CMCH::mem_read_int(unsigned int address)
{
	 int dat;
	switch(mem_rd_addr_range_type(address))
	{
	case 1: // vram  graphics:
		dat=vram_read_dword1(address);
		break;
	case 2: // vram  text:
		dat=vram_read_dword(address-0xb8000);
		break;
	case 3:// vga bios
		dat=*(unsigned long*)((char *)vgabios_map+(address-0xc0000));
		break;

	case 4:// c0000~f0000:fwd to hub?not shadowed
		dat=0xffffffff;
		break;
 	case 5:// f0000~100000,  not shadowed
		dat=ich.mem_read_int(address|0xfff00000);
		break;
	case 6: // ram
		dat=*(unsigned int*)((char *)ram_map+address);
		break;
	case 7: // apic: only accessed in dword
		dat=io_apic.read_reg(address-io_apic_base);
		break;
	case 8: // ich
		ich.mem_read_string(address,(char *)&dat,4);
		break;
	case 9:
		//
		// smram
		//
		dat=*(unsigned int*)(((char *)smram_map+address-0xa0000));
		break;
	}
	
	//printf("rd int:%x %x\n",address,dat);
	return dat;
}

void CMCH::mem_read_string(unsigned int address,char *buff,int count)
{
	//printf("rd int:%x %x\n",address,count);
	if(address>=0xa0000&&address<0xc0000 )
	{
		if(smram_decode())
			memcpy(buff,(char *)smram_map+address-0xa0000,count);
		else
		{
			if(count==4) *(unsigned long *)buff=vram_read_dword1(address);
			else if(count==2) *(unsigned short *)buff=vram_read_word1(address);
			else while(count) 
			{
				count--;
				buff[count]=vram_read_byte1(address+count);
			}

		}
		return;
	}
/*
	if(address>=0xa0000&&address<0xa0000+80*480) 
	{
		if(count==4) *(unsigned long *)buff=vram_read_dword1(address-0xa0000);
		else if(count==2) *(unsigned short *)buff=vram_read_word1(address-0xa0000);
		else while(count) 
		{
			count--;
			buff[count]=vram_read_byte1(address-0xa0000+count);
		}
		return;

	}
	else if(address>=0xb8000&&address<0xc0000) 
	{
		if(count==4) *(unsigned long *)buff=vram_read_dword(address-0xb8000);
		else if(count==2) *(unsigned short *)buff=vram_read_word(address-0xb8000);
		else  while(count) 
		{
			count--;
			buff[count]=vram_read_byte(address-0xb8000+count);
		}
		return;
	}*/
	else

	if(address>=0xc0000&&address <CFG_VGABIOS_SIZE+0xc0000) // c000:0-ffff->hub interface
	{
		unsigned short tmp=1<<((address-0xc0000)>>14);// 0x4000 per range
		if((tmp&pam_rd_enable)==0) // fwd to expansion slot through hub interface
		{
			memcpy(buff,(char *)vgabios_map+(unsigned short)(address-0xc0000),count);
			return;
		}
	}
	else

	if(address>=0xc0000&&address <0xf0000) // c000:0-ffff->hub interface
	{
		unsigned short tmp=1<<((address-0xc0000)>>14);// 0x4000 per range
		if((tmp&pam_rd_enable)==0)// fwd to hub interface. n/a
		{
			memset(buff,0xff,count);
			return;
		}
	}
	else
	if(address<0x100000
		&&address>=0xf0000)
	{
		if((0xf000&pam_rd_enable)==0) //fwd to hub interface
		{
			// fff00000~4G is  mapped  to the same space
			ich.mem_read_string(address|0xfff00000,buff,count);
			return;
		}
	}


	if(address <CFG_RAM_SIZE) memcpy(buff,(char *)ram_map+address,count);
	else if((address&0xfff00000)==io_apic_base) 
	{
		assert(count==4);
		*(int *)buff=io_apic.read_reg(address-io_apic_base); 
	}
	else
		ich.mem_read_string(address,buff,count);
}

void CMCH::mem_write_byte(unsigned int address,char dat)
{
	switch(mem_wr_addr_range_type(address))
	{
	case 1: // vram  graphics:
		vram_write_byte1(address,dat);
		break;
	//case 2: // vram  text:
	//	vram_write_byte(address-0xb8000,dat);

	//	//log("vram:%x,%x(%c)\n",address,dat,dat);
	//	break;
	case 3:// vga bios
		break;

	case 6: // ram
		*((char *)((char *)ram_map)+address)=dat;
		break;
	case 7: // apic: only accessed in dword

		break;
	case 8: // ich
		ich.mem_write_string(address,(char *)&dat,1);
		break;
	case 9: // smram
		*((char *)((char *)smram_map+address-0xa0000))=dat;


		break;
	}
}
void CMCH::mem_write_short(unsigned int address,short dat)
{
	switch(mem_wr_addr_range_type(address))
	{
	case 1: // vram  graphics:
		vram_write_word1(address,dat);
		break;
	//case 2: // vram  text:
	//	vram_write_word(address-0xb8000,dat);
	//	//log("vram:%x,%x(%c)\n",address,dat,(char)dat);
	//	break;
	case 3:// vga bios
		break;

	case 6: // ram
		*(short *)((char *)ram_map+address)=dat;
		break;
	case 7: // apic: only accessed in dword

		break;
	case 8: // ich
		ich.mem_write_string(address,(char *)&dat,2);
		break;
	case 9: // smram
		*((short *)((char *)smram_map+address-0xa0000))=dat;
		break;
	}
}
void CMCH::mem_write_int(unsigned int address,int dat)
{
	switch(mem_wr_addr_range_type(address))
	{
	case 1: // vram  graphics:
		vram_write_word1(address,dat);
		vram_write_word1(address+2,dat>>16);
		break;
	//case 2: // vram  text:
	//	vram_write_word(address-0xb8000,(unsigned short)dat);
	//	vram_write_word(address-0xb8000+2,(unsigned short)(dat>>16));
	//	//log("vram:%x,%x(%c%c)\n",address,dat,(char)dat,(unsigned char)(dat>>16));
	//	break;
	case 3:// vga bios
		break;

	case 6: // ram
		*(int *)((char *)ram_map+address)=dat;
		break;
	case 7: // apic: only accessed in dword
		io_apic.write_reg(address-io_apic_base,dat);
		break;
	case 8: // ich
		ich.mem_write_string(address,(char *)&dat,4);
		break;
	case 9: // smram
		*((int *)((char *)smram_map+address-0xa0000))=dat;
		break;
	}
	//if(dat==0x40000600)//c1b07c70
	//{
	//	log("wr int:%x %x\n",address,dat);
	//	step=true;
	//}
}


void CMCH::mem_write_string(unsigned int address,char *buff,int count)
{

	if(address>=0xa0000&&address<0xc0000 )
	{
		if(smram_decode())
			memcpy((char *)smram_map+address-0xa0000,buff,count);
		else
		{
		   while(count) 
			{
					count--;
					vram_write_byte1(address+count,buff[count]);
			}

		}

		return;	
	}

/*
	if(address>=0xa0000&&address<0xa0000+80*480) 
	{
		if(count==4) vram_write_dword1(address-0xa0000,*(unsigned long *)buff);
		else if(count==2) vram_write_word1(address-0xa0000,*(unsigned short *)buff);
		else vram_write_byte1(address-0xa0000,*(unsigned char *)buff);
		return;
	}
	else if(address>=0xb8000&&address<0xc0000) 
	{
		if(count==4) 
		{
			vram_write_word(address-0xb8000,*(unsigned short *)buff);
			vram_write_word(address+2-0xb8000,*(unsigned short *)(buff+2));
		}
		else if(count==2) vram_write_word(address-0xb8000,*(unsigned short *)buff);
		else vram_write_byte(address-0xb8000,*(unsigned char *)buff);
		return;
	}
*/
	else 

	if(address<0x100000
		&&address>=0xc0000)
	{

		unsigned short tmp=1<<((address-0xc0000)>>14);// 0x4000 per range
		if((tmp&pam_wr_enable)==0) // fwd to hub interface
			return;
		// fall throuh to make shadow
	}

	if(address <CFG_RAM_SIZE) memcpy((char *)ram_map+address,buff,count);
	else if ((address&0xfff00000)==io_apic_base) 
	{
		assert(count==4);
		io_apic.write_reg(address-io_apic_base,*(int *)buff); 
	}
	else
	ich.mem_write_string(address,buff,count);

}



int  CMCH::mem_rd_addr_range_type(unsigned int address)
{
	if(address>=0xa0000
		&&address<0xc0000)
	{
		if(smram_decode())
			return 9;
		else
		{
			return 1;
		}
	}
	/*
	if(address>=0xa0000&&address<0xa0000+80*480) 
	{
		return 1;

	}
	else if(address>=0xb8000&&address<0xc0000) 
	{
		return 2;
	}*/
	else

	if(address>=0xc0000&&address <CFG_VGABIOS_SIZE+0xc0000) // c000:0-ffff->hub interface
	{
		//if(address>=0xcdba2+0x800 && address<0xcdba2+0x1000) ::step=true;

		unsigned short tmp=1<<((address-0xc0000)>>14);// 0x4000 per range
		if((tmp&pam_rd_enable)==0) // fwd to expansion slot through hub interface
		{
			return 3;
		}
	}
	else

	if(address>=0xc0000&&address <0xf0000) // c000:0-ffff->hub interface
	{
		unsigned short tmp=1<<((address-0xc0000)>>14);// 0x4000 per range
		if((tmp&pam_rd_enable)==0)// fwd to hub interface. n/a
		{
			return 4;
		}
	}
	else
	if(address<0x100000
		&&address>=0xf0000)
	{
		if((0xf000&pam_rd_enable)==0) //fwd to hub interface
		{
			// fff00000~4G is  mapped  to the same space
			return 5;
		}
	}
	if(address <CFG_RAM_SIZE) return 6;
	else if((address&0xfff00000)==io_apic_base) return 7;
	else  
	return 8;
		
}

int  CMCH::mem_wr_addr_range_type(unsigned int address)
{
	if(address>=0xa0000
		&&address<0xc0000)
	{
		//log("smram open:%x,enabled:%x,smiact:%x\n",smram_open,smram_en,cpu[0].SMIACT());
		if(smram_decode())
			return 9;
		else
		{
			return 1;
		}
	}

	else 

	if(address<0x100000
		&&address>=0xc0000)
	{

		unsigned short tmp=1<<((address-0xc0000)>>14);// 0x4000 per range
		if((tmp&pam_wr_enable)==0) // fwd to hub interface
			return 3;
		// fall throuh to make shadow
	}


	if(address <CFG_RAM_SIZE) return 6;
	else if ((address&0xfff00000)==io_apic_base) return 7;
	else  
		return 8;

}