#include <fstream>
#include "config.h"
#include "json\json.h"

using namespace Json;

json_config::json_config()
{
	step = 0;
	n_cpu = CFG_N_CPUS;
	path_log = CFG_LOG_FPATH;
	path_save = CFG_SYSSTATUS_FPATH;
	path_cd_image = CFG_CDROM_IMAGE;
	path_hdd_image = CFG_IDE_IMAGE;
	path_floppy_image = CFG_FLOPPY_IMAGE;
	path_vga_bios_image = CFG_VGABIOS_IMAGE;
	path_bios_image = CFG_BIOS_IMAGE;
	path_ram_image = CFG_RAM_IMAGE;
	path_smram_image = CFG_SMRAM_IMAGE;
	path_nvs_image = CFG_NVS_IMAGE;

	hdd_size = CFG_IDE_SIZE_G;
	cd_size = CFG_CDROM_IMAGE_SIZE;
	vga_bios_size = CFG_VGABIOS_SIZE;
	bios_size = CFG_BIOS_SIZE;
	ram_size = CFG_RAM_SIZE;
	smram_size = CFG_SMRAM_SIZE;
	nvs_size = CFG_NVS_SIZE;
}
int load_json(JSON_CONFIG & cfg)
{
	std::string JsonString;

	std::ifstream in("config.json");
	if (in.good())
	{
		std::stringstream strStream;
		strStream << in.rdbuf(); //read the file
		std::string str = strStream.str(); //str holds the content of the file

		
		
		Json::Value root;
		Json::Reader reader;
		bool parsingSuccessful = reader.parse(str.c_str(), root);
		if (parsingSuccessful)
		{


			return 1;
		}					
	}

	return 0;
}