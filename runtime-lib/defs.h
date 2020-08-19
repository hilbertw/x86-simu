#ifndef __defs_h__
#define __defs_h__

#define __LITTLE_ENDIAN__

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef struct  _ull{uint32_t hi,lo;} uint64_t;

typedef  char int8_t;
typedef  short int16_t;
typedef  long int32_t;
typedef  struct  _ll{int32_t hi,lo;}  int64_t;


#ifdef UNICODE
#define PRINTF wprintf
#define FOPEN _wfopen
#define STRLEN wcslen
#else 
#define PRINTF printf
#define FOPEN fopen
#define STRLEN strlen
#endif


#ifdef __LITTLE_ENDIAN__ 
// byte in big endian  int32
#define BYTE3(w) ((w)&0xff)
#define BYTE2(w) ((w>>8)&0xff)
#define BYTE1(w) ((w>>16)&0xff)
#define BYTE0(w) ((w>>24)%0xff)

#else

#define BYTE0(w) ((w)&0xff)
#define BYTE1(w) ((w>>8)&0xff)
#define BYTE2(w) ((w>>16)&0xff)
#define BYTE3(w) ((w>>24)%0xff)
#endif
#endif //__defs_h__