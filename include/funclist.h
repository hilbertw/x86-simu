#ifndef __FUNCLIST_H__
#define __FUNCLIST_H__

#define MakePtr(a,b,c) ((a)(((LPSTR)b)+c))

typedef struct _EXPFUNC {
	char   *pOrdinalName;

	unsigned short  Ordinal;
	unsigned long   VAddress;  // 

	struct _EXPFUNC *pNext;
} EXPFUNC, *PEXPFUNC;

struct _DLLNAME;

typedef struct _IMPFUNC {
	char   *pOrdinalName;

	unsigned short  Ordinal;
	unsigned long   VAddress;  // IAT item

	struct _IMPFUNC *pNext;
} IMPFUNC, *PIMPFUNC;

typedef struct _DLLNAME {
	char    *lpName;
	PIMPFUNC lpFuncs;

	struct _DLLNAME *pNext;

} DLLNAME,*PDLLNAME;


PEXPFUNC hookedExpFunc( unsigned long virtualAddress );
PIMPFUNC hookedImpFunc( unsigned long virtualAddress );
#endif   /*    __FUNCLIST_H__   */