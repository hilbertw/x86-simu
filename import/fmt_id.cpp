#undef OPER_FMT_REC
#define OPER_FMT_REC(x) L#x,
//
// If new oper type is to be defined, go to oper_type.h
//
wchar_t * sz_fmt[]= {
#include "oper_fmt_def.h"
};
#include "windows.h"

#undef OPER_FMT_REC
#define OPER_FMT_REC(x) x,
//
// If new oper type is to be defined, go to oper_type.h
//
enum id_fmt {
	
#include "oper_fmt_def.h"
N_FMT
};

int wcs2id(LPCWSTR wcs_fmt)
{
	int i;
	for(i=0;i< N_FMT;i++)
	{
		if(wcscmp(wcs_fmt,sz_fmt[i])==0) break;
	}

	return i== N_FMT?-1:i;
}