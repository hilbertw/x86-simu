#ifndef __pci_h__
#define  __pci_h__
#include "config.h"
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;

#pragma pack(push,1)
typedef struct pci_header_def
{
	WORD dwVendorId;
	WORD dwDeviceId;
	WORD dwCmd;
	WORD dwStatus;
	BYTE RevisionId;
	BYTE ClassCode[3];
	BYTE CacheLineSize;
	BYTE LatencyTimer;
	BYTE HeaderType;
	BYTE BIST;
	DWORD ddBase[6];
	DWORD ddCISPointer;
	WORD dwSubSystemVendorId;
	WORD dwSubSystemId;
	DWORD ddExpansionROMBase;
	BYTE CapabilityPointer;
	BYTE Reserved[3];
	DWORD ddReserved;
	BYTE InterruptLine;
	BYTE InterruptPin;
	BYTE MinGrant;
	BYTE MaxLatency;
}PCI_HEADER;

//
// header type 1
//
typedef struct pci_header1_def
{
	WORD dwVendorId;
	WORD dwDeviceId;
	WORD dwCmd;
	WORD dwStatus;
	BYTE RevisionId;
	BYTE ClassCode[3];
	BYTE CacheLineSize;
	BYTE LatencyTimer;
	BYTE HeaderType;
	BYTE padding[9];
	BYTE PrimaryBusNumber;
	BYTE SecondaryBusNumber;
	BYTE SubordinateBusNumber;
	BYTE SecondaryLatencyTimer;
	BYTE IOBase;
	BYTE IOLimit;
	WORD dwSecondaryStatus;
	WORD dwMemBase;
	WORD dwMemLimit;
	WORD dwPrefetchableMemBase;
	WORD dwPrefetchableMemLimit;
	DWORD ddPrefetchableMemBaseHi;
	DWORD ddPrefetchableMemLimitHi;
	WORD dwIOBaseHi;
	WORD dwIOLimitHi;

	BYTE CapabilityPointer;
	BYTE Reserved[3];
	DWORD ddExpansionROMBase;
	BYTE InterruptLine;
	BYTE InterruptPin;
	WORD dwBridgeControl;
}PCI_HEADER1;

#pragma pack(pop)



typedef struct pci_function_def
{
	WORD dwHeaderSize;	
	WORD Enabled;
	BYTE aReadOnlyFields[8];// 256 bytes in configyration space, 1 bit per dword
	DWORD BlockSpec[6];
}CONFIG_SPACE_HDR,*PCONFIG_SPACE_HDR;



#define INDICATOR(dd)(dd&1)
#define MEMORY_TYPE(dd)((dd>>1)&3)
#define MEMORY_PREFETCHABLE(dd)(dd&4)

#define MEMORY_SPEC(dd)(dd&0xf)
#define MDECODER_MASK(dd)(dd&0xfffffff0)
#define IODECODER_MASK(dd)(dd&0xfffffffc)

class CPCIFunction;

typedef struct pci_registry_def
{
	union func_address
	{
		struct {
			WORD uFunction:3;
			WORD uDev:5;
			WORD uBus:8;
		} s;
		WORD w;
	} Address;
	CPCIFunction *lpPciFunction;

}PCI_REGISTRY;



class CPCIBus
{
public:
	PCI_REGISTRY functions[CFG_MAX_PCI_FUNC];
	int n_functions;
	int total_irq[4];// number of funcs which assert inta,intb,...

	int * p_int_pin; // may be connected to an ioapic chip or an 8259a chip.

	//
	// emulating burst mode pci cycles
	//
	int dwSelectedFunction;
	DWORD dwOffset;
	bool dev_sel_for_io;
	int bar_num;
public:

	CPCIBus():dwSelectedFunction(-1){} //lpSelectedPciFunction((CONFIG_SPACE_HDR *)0) {}
	void read_config_reg(DWORD reg,BYTE * pBuffer, char bytes);
	void write_config_reg(DWORD reg,BYTE * pBuffer, char bytes);
    DWORD MemBusOp(bool read ,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes);
    DWORD IoBusOp(bool read ,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes);
    DWORD DoBusOp(bool io,bool read ,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes);

	void assign_sbus_num(unsigned char sbus_number);// reassign secondary pci bus number
	void RegisterFunction(WORD dwAddress,
		CPCIFunction *lpPciFunction);

	void FreshIntPin(); //  assert/deassert INTA#/INTB#/INTC#/INTD#
	//
	// start burst mode bus cycle
	// return bytes available for read/write, 0 if no device selected
	//
	DWORD dev_sel(bool io,DWORD ddAddrLo,DWORD ddAddrHi=0);

	//
	// 32 bit bus read/wrote
	//
	DWORD  read();
	void write(DWORD  data);
	//
	// burst mode ends
	//
	void dev_sel_end();
};

typedef enum bus_op {MEM_RD,MEM_WR,IO_WR,IO_RD}BUSOP;
extern CPCIBus pci_bus;
class CPCIFunction
{
public:
	CONFIG_SPACE_HDR * pConfigSpaceHdr;
	CPCIFunction()
	{}

	CPCIFunction(WORD dwAddress,void* pdata) 
	{
		pConfigSpaceHdr=(CONFIG_SPACE_HDR *)pdata;
		pci_bus.RegisterFunction(dwAddress,this);
	}

	//
	// config space r/w notify
	//
	virtual  void ConfigCallback(int dwCommand,unsigned int address)=0;
	//
	// mem/i/o bus operation on function
	//
	virtual DWORD DoBusOp(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num)=0;
	//
	// address select
	//
	virtual int DoDeviceSelect(bool io,DWORD ddAddrLo,DWORD ddAddrHi)=0;
};

class CGenricPCIFunction:public CPCIFunction
{
public:
	CGenricPCIFunction(WORD dwAddress,void* pdata):CPCIFunction(dwAddress,pdata) {}
	CGenricPCIFunction() {}

	//
	// config space r/w notify
	//
	virtual  void ConfigCallback(int dwCommand,unsigned int address);
	//
	// mem/i/o bus operation on function
	//
	virtual DWORD DoBusOp(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num);
	//
	// address select
	//
	virtual int DoDeviceSelect(bool io,DWORD ddAddrLo,DWORD ddAddrHi);
};

class CGenricPCIBridge:public CPCIFunction
{
public:
	CGenricPCIBridge(WORD dwAddress,void* pdata):CPCIFunction(dwAddress,pdata) {}
	CGenricPCIBridge() {}

	//
	// config space r/w notify
	//
	virtual  void ConfigCallback(int dwCommand,unsigned int address);
	//
	// mem/i/o bus operation on function
	//
	virtual DWORD DoBusOp(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num);
	//
	// address select
	//
	virtual int DoDeviceSelect(bool io,DWORD ddAddrLo,DWORD ddAddrHi);
};

#define PCI_FUN_ADDR(b,d,f) ((b<<8)|((d&0x1f)<<3)|(7&f))
#define DECL_PCI_CONTEXT(type,name) type * name =(type *)pContext;

#define DECL_CONFIG_SPACE_REG char *pConfigSpaceRegs=(char *)((CONFIG_SPACE_HDR *)pConfigSpaceHdr+1);
#define PCI_HEADER() ((PCI_HEADER *)pConfigSpaceRegs)
#define PCI_HEADER1() ((PCI_HEADER1 *)pConfigSpaceRegs)
#define CONFIG_SPACE_HDR() (((CONFIG_SPACE_HDR *)pConfigSpaceHdr))

void pci_raise_int(int no);

#endif // __pci_h__