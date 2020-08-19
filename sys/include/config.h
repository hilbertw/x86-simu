#ifndef __config_h__
#define __config_h__
#include <string>

#define  CFG_CORE_COUNT_BITS 1
#define  CFG_CORE_COUNT (1<<CFG_CORE_COUNT_BITS)

#define CFG_L1_LINES 2048
#define CFG_L2_LINES 2048
#define  CFG_MAX_PCI_FUNC    16
#define  CFG_CACHE_LINE_SIZE 32

#define  CFG_MAX_SMBUS_NODES 10
#define  CFG_MAX_LAPIC_NODES  2
#define  CFG_IOAPIC_ID       CFG_MAX_LAPIC_NODES

#define  CFG_LAPIC_BASE  0xfee00000
#define  CFG_IOAPIC_BASE 0xfec00000

#define  CFG_RAM_IMAGE   L"ram0.bin"
#define  CFG_RAM_SIZE    0x10000000

#define  CFG_SMRAM_IMAGE   L"smram.bin"
#define  CFG_SMRAM_SIZE    0x20000

#define  CFG_NVS_IMAGE   L"acpi.nvs"
#define  CFG_NVS_SIZE    0x20000
#define MSI

#ifdef GIGABYTE
#define  CFG_BIOS_IMAGE   L"d:\\jj.rom"
#define  CFG_BIOS_SIZE   0x100000

#elif defined( JY)

#define  CFG_BIOS_IMAGE   L"d:\\GF7100-M3_080707.ROM"
#define  CFG_BIOS_SIZE   0x100000

#elif defined( MSI)

#define  CFG_BIOS_IMAGE   L"A6566IMS.610"
#define  CFG_BIOS_SIZE   0x40000

#else

#define  CFG_BIOS_IMAGE   L"d:\\bios2008.ROM"
#define  CFG_BIOS_SIZE   0x100000

#endif

//#define  CFG_VGABIOS_IMAGE   L"rage_iic.bin"
//#define  CFG_VGABIOS_SIZE   0x10000
#define  CFG_VGABIOS_IMAGE   L"tvga9kc.bin"
#define  CFG_VGABIOS_SIZE   0x8000
//#define  CFG_VGABIOS_IMAGE   L"vgabios.ROM"
//#define  CFG_VGABIOS_SIZE   0x10000
#define  CFG_FLOPPY_IMAGE   L"dos622.img"
//#define  CFG_FLOPPY_IMAGE   L"MSDOS71B.IMG"
#define  CFG_IDE_IMAGE      L"hdd.img"
#define  CFG_IDE_SIZE_G     2 
#define  CFG_CDROM_IMAGE      L"cd-dvd.iso"
#define  CFG_CDROM_IMAGE_SIZE 0

#define CFG_CMOS_DAT "cmos.dat"

#define CFG_LOG_FPATH "log.txt"
#define CFG_SYSSTATUS_FPATH L"state.sav"

#define CFG_MAX_HISTORY 1000

#define CFG_N_CPUS 1

#define TOM1 CFG_RAM_SIZE
#define TOM2 (TOM1+CFG_NVS_SIZE)


typedef struct  json_config{
	int step;
	unsigned int n_cpu;
	std::string  path_log;
	std::wstring  path_save;
	std::wstring  path_cd_image;
	std::wstring  path_hdd_image;
	std::wstring  path_floppy_image;
	std::wstring  path_vga_bios_image;
	std::wstring  path_bios_image;
	std::wstring  path_ram_image;
	std::wstring  path_smram_image;
	std::wstring  path_nvs_image;
	unsigned int cd_size;
	unsigned int hdd_size;
	unsigned int vga_bios_size;
	unsigned int bios_size;
	unsigned int ram_size;
	unsigned int smram_size;
	unsigned int nvs_size;

	json_config();
} JSON_CONFIG;

int load_json(JSON_CONFIG & cfg);

#endif  // __config_h__