#include "config.h"
#include "pci.h"
#include "ich.h"
#include "filemap.h"
#include "addressmap.cpp"
extern CPCIBus pci_bus;

void unhandled_io(
	  void *lpContext,
	  ADDR_RANGE_CMD op, 
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,                    // optional 
	  unsigned int nBytes )  ;          // in or out buffer
#define BASE_BIOS ((unsigned long)-CFG_BIOS_SIZE)
//
// io: pci / isa
// mem: if fff000~4G, go bios map
char CICH ::io_read_byte(unsigned short address) 
{
	char tmp;

	if(!IO.ReadByte(address,&tmp)
		&&! pci_bus.IoBusOp(true,(BYTE *)&tmp,address,0,1))
		unhandled_io(NULL,ADDR_RANGE_READ,address,(char *)&tmp,1);//tmp=-1;

	return  tmp;
}
short CICH ::io_read_short(unsigned short address)
{
	short tmp;
	if(!IO.ReadWord(address,&tmp)
		&&!pci_bus.IoBusOp(true,(BYTE *)&tmp,address,0,2))
		unhandled_io(NULL,ADDR_RANGE_READ,address,(char *)&tmp,2);//tmp=0xffff;

	return tmp;
}
int CICH ::io_read_int(unsigned short address)
{
	int tmp;

	if(!IO.ReadDWord(address,&tmp)
		&&!pci_bus.IoBusOp(true,(BYTE *)&tmp,address,0,4))
		unhandled_io(NULL,ADDR_RANGE_READ,address,(char *)&tmp,4);//tmp=0xffffffff;

	return tmp;

}

void CICH ::io_write_byte(unsigned short address,char dat) 
{
	if (!IO.WriteByte(address,dat)
	&&!pci_bus.IoBusOp(false,(BYTE *)&dat,address,0,1))
	unhandled_io(NULL,ADDR_RANGE_WRITE,address,(char *)&dat,1);
}
void CICH ::io_write_short(unsigned short address,short dat)
{
	if (!IO.WriteWord(address,dat)
	&&!pci_bus.IoBusOp(false,(BYTE *)&dat,address,0,2))
	unhandled_io(NULL,ADDR_RANGE_WRITE,address,(char *)&dat,2);
}
void CICH ::io_write_int(unsigned short address,int dat)
{
	if (!IO.WriteDWord(address,dat)
	&&!pci_bus.IoBusOp(false,(BYTE *)&dat,address,0,4))
	unhandled_io(NULL,ADDR_RANGE_WRITE,address,(char *)&dat,4);
}

char CICH ::mem_read_byte(unsigned int address)
{
	//if(address<TOM2)
	//{
	//	return *(BYTE *)((char *)nvs_map+(address-TOM1));
	//}
	//else
	if(address>=BASE_BIOS)
	{
		return *(BYTE *)((char *)bios_map+(address-BASE_BIOS));
	}else
		return -1;//mem.ReadByte(address);
}
short CICH ::mem_read_short(unsigned int address)
{
	//if(address<TOM2)
	//{
	//	return *(short *)((char *)nvs_map+(address-TOM1));
	//}
	//else
	if(address>=BASE_BIOS)
	{
		return *(short *)((char *)bios_map+(address-BASE_BIOS));
	}else
		return -1;//mem.ReadWord(address);
}
int CICH ::mem_read_int(unsigned int address)
{
	//if(address<TOM2)
	//{
	//	return *(int *)((char *)nvs_map+(address-TOM1));
	//}
	//else
	if(address>=BASE_BIOS)
	{
		return *(int *)((char *)bios_map+(address-BASE_BIOS));
	}else
		return -1;//mem.ReadDWord(address);
}
void CICH ::mem_read_string(unsigned int address,char *buff,DWORD count)
{
	unsigned long bytes_vailable,n;

l1:
	//if(address<TOM2)
	//{
	//	memcpy(buff,(char *)nvs_map+(address-TOM1),count);
	//}
	//else
	if(address>=BASE_BIOS)
	{
		memcpy(buff,(char *)bios_map+(address-BASE_BIOS),count);
	}
	else if((bytes_vailable=pci_bus.dev_sel(false,address,0)!=0))
	{
		n=(bytes_vailable<count)?bytes_vailable:count;

		count-=n;
		while(n>=4)
		{
			(*(DWORD *)buff)=pci_bus.read();
			buff+=4;
			n -=4;
		}
		pci_bus.dev_sel_end();

		if(count) goto l1;
	}
	else
		//mem.ReadString(address,buff,count);
		memset(buff,0xff,count);
}

void CICH ::mem_write_byte(unsigned int address,char dat)
{
		//mem.WriteByte(address,dat);
	//if(address<TOM2)
	//{
	//	*(char *)((char *)nvs_map+(address-TOM1))=dat;
	//}
}
void CICH ::mem_write_short(unsigned int address,short dat)
{
	//if(address==TOM2-1)
	//{
	//	*(char *)((char *)nvs_map+(address-TOM1))=dat;
	//}
	//else if(address<TOM2-1)
	//{
	//	*(short *)((char *)nvs_map+(address-TOM1))=dat;
	//}
}
void CICH ::mem_write_int(unsigned int address,int dat)
{
	//if(address<TOM2-3)
	//{
	//	*(int *)((char *)nvs_map+(address-TOM1))=dat;
	//}
	//else if(address<TOM2)
	//{
	//	memcpy((char *)nvs_map+(address-TOM1),(char *)&dat,TOM2-address);
	//}
}
void CICH ::mem_write_string(unsigned int address,char *buff,DWORD count)
{
	unsigned long bytes_vailable,n;

	//if(address<TOM2)
	//{
	//	n=TOM2-address;
	//	memcpy((char *)nvs_map+(address-TOM1),buff,count>n?n:count);
	//}
l1:
	if((bytes_vailable=pci_bus.dev_sel(false,address,0)!=0))
	{
		n=(bytes_vailable<count)?bytes_vailable:count;

		count-=n;
		while(n>=4)
		{
			pci_bus.write(*(DWORD *)buff);
			buff+=4;
			n -=4;
		}
		pci_bus.dev_sel_end();

		if(count) goto l1;
	}

}