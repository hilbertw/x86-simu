#include "smbus.h"


unsigned char CSMBUS::buffer[SMBUS_BUFFSIZE];
SMBUS_NODE CSMBUS::nodes[CFG_MAX_SMBUS_NODES];

char *CSMBUS::cmd_type[8]={
	"quick",        //000
	"byte",         //001
	"byte data",    //010
	"word data",    //011
	"process call", //100
	"block",        //101
	"i2c read",     //110
	"bad"           //111
};
void CSMBUS::do_command()
{
}