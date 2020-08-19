#include <stdio.h>
#include "cpu.h"
//All flags except VM and RF 
// #GP(0) fault if IOPL is less than 3, to permit emulation
// The I/O privilege level is altered only when executing at privilege level 0. 
//The interrupt flag is altered only when executing at a level at least as 
//privileged as the I/O privilege level. (Real-address mode is equivalent to 
//privilege level 0.) If a POPF instruction is executed with insufficient privilege, 
//an exception does not occur, but the privileged bits do not change.
void CCPU::Popfd()
{
	bool  save_vm;
	bool save_rf;

	bool save_if;
	unsigned char save_iopl;
	save_vm=__VM__;
	save_rf=__RF__;
	save_if=__IF__;
	save_iopl=__IOPL__;

	unsigned long data;
	if(!Pop(data,32)) return;
	(*(UEFLAGS*)&data).s.VM=save_vm;
	(*(UEFLAGS*)&data).s.RF=save_rf;

	if(real_v86() ||__CPL__==0);

	else
	{
	(*(UEFLAGS*)&data).s.IOPL=save_iopl;
	(*(UEFLAGS*)&data).s.IF=save_if;
	}
	__EFLAGS__=data;
}