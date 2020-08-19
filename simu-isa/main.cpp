#define _CRT_SECURE_NO_WARNINGS 1

#include "stdafx.h"
#include <stdio.h>
#include <string.h>

int __cdecl sys_init(int argc,char * argv[]);
int __cdecl sys_run(char * argv0);

int __cdecl main (int argc,char * argv[])
{
	if(sys_init(argc,argv)>=0)
	sys_run(argv[0]);
}
