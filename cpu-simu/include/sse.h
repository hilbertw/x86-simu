#ifndef __fpu_h__
#define __fpu_h__

#include "common.h"
#include "mmu.h"

class CSSE
{
	CMMU * p_mmu; // load & store
	 
public:
	CSSE(CMMU * _p_mmu):p_mmu(_p_mmu) {}





	#include "sse-inc.h"
};


#endif