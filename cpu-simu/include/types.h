typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;

typedef unsigned char UCHAR;
typedef unsigned int  UINT;
typedef unsigned short USHORT;

#define MK_SHORT(a) (*(unsigned short *)&(a))
#define MK_LONG(a)  (*(unsigned long *)&(a))