#ifndef __sys_h__
#define __sys_h__
#include "addressmap.h"


	void io_8237(
		void *lpContext,
		ADDR_RANGE_CMD op,
		unsigned int dwAddress,           // start dwAddress on 32bit bus
		char *pszBuff,           // optional 
		unsigned int nBytes);            // in or out buffer
	void io_8042(
		void *lpContext,
		ADDR_RANGE_CMD op,
		unsigned int dwAddress,           // start dwAddress on 32bit bus
		char *pszBuff,           // optional 
		unsigned int nBytes);            // in or out buffer
	void io_8253(
		void *lpContext,
		ADDR_RANGE_CMD op,
		unsigned int dwAddress,           // start dwAddress on 32bit bus
		char *pszBuff,           // optional 
		unsigned int nBytes);            // in or out buffer
	void io_8259(
		void *lpContext,
		ADDR_RANGE_CMD op,
		unsigned int dwAddress,           // start dwAddress on 32bit bus
		char *pszBuff,           // optional 
		unsigned int nBytes);            // in or out buffer
	void ports_handler(
		void *lpContext,
		ADDR_RANGE_CMD op,
		unsigned int dwAddress,           // start dwAddress on 32bit bus
		char *pszBuff,           // optional 
		unsigned int nBytes);            // in or out buffer

	void pci_handler(
		void *lpContext,
		ADDR_RANGE_CMD op,
		unsigned int dwAddress,           // start dwAddress on 32bit bus
		char *pszBuff,                    // optional 
		unsigned int nBytes);            // in or out buffer

	bool register_index_data_ports(
		TAddressMap<unsigned short>  & io,
		LPCWSTR wcsTbl,
		short port);
	void apic_mem(
		void *lpContext,
		ADDR_RANGE_CMD op,
		unsigned int dwAddress,           // start dwAddress on 32bit bus
		char *pszBuff,           // optional 
		unsigned int nBytes);           // in or out buffer

	void floppy_handler(
		void *lpContext,
		ADDR_RANGE_CMD op,
		unsigned int dwAddress,           // start dwAddress on 32bit bus
		char *pszBuff,                    // optional 
		unsigned int nBytes);            // in or out buffer

	void cmos_handler(
		void *lpContext,
		ADDR_RANGE_CMD op,
		unsigned int dwAddress,           // start dwAddress on 32bit bus
		char *pszBuff,                    // optional 
		unsigned int nBytes);            // in or out buffer

	void io_vga(
		void *lpContext,
		ADDR_RANGE_CMD op,
		unsigned int dwAddress,           // start dwAddress on 32bit bus
		char *pszBuff,                    // optional 
		unsigned int nBytes);            // in or out buffer
	void io_vga_crtc(
		void *lpContext,
		ADDR_RANGE_CMD op,
		unsigned int dwAddress,           // start dwAddress on 32bit bus
		char *pszBuff,                    // optional 
		unsigned int nBytes);            // in or out buffer
	void io_cga(
		void *lpContext,
		ADDR_RANGE_CMD op,
		unsigned int dwAddress,           // start dwAddress on 32bit bus
		char *pszBuff,                    // optional 
		unsigned int nBytes);            // in or out buffer
	void io_mda(
		void *lpContext,
		ADDR_RANGE_CMD op,
		unsigned int dwAddress,           // start dwAddress on 32bit bus
		char *pszBuff,                    // optional 
		unsigned int nBytes);            // in or out buffer
	void io_ide1(
		void *lpContext,
		ADDR_RANGE_CMD op,
		unsigned int dwAddress,           // start dwAddress on 32bit bus
		char *pszBuff,                    // optional 
		unsigned int nBytes);            // in or out buffer

	void io_ide2(
		void *lpContext,
		ADDR_RANGE_CMD op,
		unsigned int dwAddress,           // start dwAddress on 32bit bus
		char *pszBuff,                    // optional 
		unsigned int nBytes);            // in or out buffer
	void lpc_handler(
		void *lpContext,
		ADDR_RANGE_CMD op,
		unsigned int dwAddress,           // start dwAddress on 32bit bus
		char *pszBuff,                    // optional 
		unsigned int nBytes);            // in or out buffer

	void apm_handler(
		void *lpContext,
		ADDR_RANGE_CMD op,
		unsigned int dwAddress,           // start dwAddress on 32bit bus
		char *pszBuff,                    // optional 
		unsigned int nBytes);            // in or out buffer
	void reset_handler(
		void *lpContext,
		ADDR_RANGE_CMD op,
		unsigned int dwAddress,           // start dwAddress on 32bit bus
		char *pszBuff,                    // optional 
		unsigned int nBytes);            // in or out buffer

	void port80_handler(
		void *lpContext,
		ADDR_RANGE_CMD op,
		unsigned int dwAddress,           // start dwAddress on 32bit bus
		char *pszBuff,                    // optional 
		unsigned int nBytes);            // in or out buffer

	void general_io(
		void *lpContext,
		ADDR_RANGE_CMD op,
		unsigned int dwAddress,           // start dwAddress on 32bit bus
		char *pszBuff,                    // optional 
		unsigned int nBytes);            // in or out buffer
	void cpu_if(
		void *lpContext,
		ADDR_RANGE_CMD op,
		unsigned int dwAddress,           // start dwAddress on 32bit bus
		char *pszBuff,                    // optional 
		unsigned int nBytes);            // in or out buffer

	void isa_pnp_wrp(
		void *lpContext,
		ADDR_RANGE_CMD op,
		unsigned int dwAddress,           // start dwAddress on 32bit bus
		char *pszBuff,                    // optional 
		unsigned int nBytes);            // in or out buffer

	void isa_pnp_pidxr(
		void *lpContext,
		ADDR_RANGE_CMD op,
		unsigned int dwAddress,           // start dwAddress on 32bit bus
		char *pszBuff,                    // optional 
		unsigned int nBytes);            // in or out buffer
	void isa_pnp_rdp(
		void *lpContext,
		ADDR_RANGE_CMD op,
		unsigned int dwAddress,           // start dwAddress on 32bit bus
		char *pszBuff,                    // optional 
		unsigned int nBytes);            // in or out buffer


	void io_imcr(
		void *lpContext,
		ADDR_RANGE_CMD op,
		unsigned int dwAddress,           // start dwAddress on 32bit bus
		char *pszBuff,                    // optional 
		unsigned int nBytes);            // in or out buffer

	bool clk_gen();
	bool ports_init();
	void LastErrorMsg();
	void mch845g_register();
	void ich82801_register();
	bool load_cmos();
	bool save_cmos();

	void smbus_register();
	bool vt_init(HINSTANCE hinst);
	bool floppy_mount(wchar_t * wcsImage, int drive_no, bool write_protected);
	bool ide_mount(wchar_t * wcsImage, int drive_no, unsigned int size, bool cdrom);
	void ide_unmount(int drive_no);
	void floppy_unmount(int drive_no);



#endif