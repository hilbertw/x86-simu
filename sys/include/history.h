#ifndef __history_h__
#define __history_h__
#include "config.h"

class CHistory
{
	struct {
		unsigned long  cs;
		unsigned long off;
	} history[CFG_MAX_HISTORY];
	 int n_his;
public:
	CHistory():n_his(0)
	{}
	void dump();
	void add(unsigned long cs,unsigned long eip);
	void add_w_dup(unsigned long cs,unsigned long eip);
};

#endif __history_h__