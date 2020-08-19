#include "log.h"
#include <conio.h>
#include "history.h"

void CHistory::dump()
{
		int i,j=0;

		i=n_his==0?CFG_MAX_HISTORY-1:n_his-1;
		while(i!=n_his)
		{
	
			log0("%08x:%04x\n",history[i].cs,history[i].off);
			if(i==0)i=CFG_MAX_HISTORY-1;else i--;
			j++;
			if(j==20) {if(_getch()=='q') break; j=0;}
		}
}

void CHistory::add(unsigned long cs,unsigned long eip)
{
		unsigned int i=0;
		i=n_his==0?CFG_MAX_HISTORY-1:n_his-1;
		
		while(i!=n_his)
		{
			if(history[i].off==eip&&history[i].cs==cs) break;

		if(i==0) i=CFG_MAX_HISTORY-1;else i--;
			
		}

		if(i==n_his)
		{
			history[n_his].cs=cs;
			history[n_his].off=eip;
			n_his++;
			if(n_his>=CFG_MAX_HISTORY)n_his=0;
		}
}
void CHistory::add_w_dup(unsigned long cs,unsigned long eip)
{
		if(n_his>=CFG_MAX_HISTORY)n_his=0;
		history[n_his].cs=cs;
		history[n_his].off=eip;
		n_his++;

		
}