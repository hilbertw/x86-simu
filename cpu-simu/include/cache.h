/* class to simulate a TLB.
   Copyright (C) 2005 Huaibo Wang.
*/
#ifndef __CACHE_H__
#define __CACHE_H__

#ifndef DWORD
typedef unsigned long DWORD;
#endif 

class cache_base {
	int  total;
	int  miss;

public:
	cache_base () : total (0), miss (0)
	{
		clear_all ();
	}

	void stat (int i) 
	{
		total ++;
		if (i) miss ++;
	}

	void dump_stat ()
	{
	}

	virtual void clear_all () = 0;
};

/*
 * full associative
 * replacement policy is no longer the function of address
 * typically, the oldest cache line is evicted,i.e. LRU
 */
template <int N_ENTRIES, class TAG_TYPE, class DATA_TYPE>
class cache_full_associative : public cache_base {
    /* entry and age are separate in order to
	 * emulate the pipeline model
	 */
	struct entry_def {
			bool   valid;
			TAG_TYPE     tag;     
			DATA_TYPE    data;   
	} entry [N_ENTRIES];

	int  age [N_ENTRIES];

public:
	void invalid (TAG_TYPE tag, int page_off);

	bool seek (TAG_TYPE tag, DATA_TYPE *data, int page_off);
	// for speculative execution, don't stall	
	bool snoop (TAG_TYPE tag, int page_off);
	void insert (TAG_TYPE tag, DATA_TYPE data, int page_off);
	bool test (TAG_TYPE tag, DATA_TYPE *data, int page_off);

	void fill_line (TAG_TYPE tag, DATA_TYPE  data, int page_off);

	void clear_all ();
};

#define N_WAY 4
/*
 * N-WAY set-associative
 * replacement policy is no longer the function of address
 * typically, it is pseudo-LRU
 */
template <int N_ENTRIES, class TAG_TYPE, class DATA_TYPE,int N_WAYS>
class cache_n_way : public cache_base{

    /* entry and age are separate in order to
	 * emulate the pipeline model
	 */
	struct entry_def {
			DATA_TYPE    data; // be the firstso that pointer can be casted to rntry *

			TAG_TYPE     tag;  
			char   state;//'M','S','I','D'
			char   age;// pseufo age
	  
	} entry [N_ENTRIES] [N_WAY];


public:
	cache_n_way() {clear_all();}
	void set_state (DATA_TYPE *data,char state)
	{
		((struct entry_def *)data)->state=state;
	}
	char  get_state (DATA_TYPE *data)
	{
		return ((struct entry_def *)data)->state;
	}
	void invalid (TAG_TYPE tag, int page_off);

	bool hit (TAG_TYPE tag, DATA_TYPE **data, int page_off);
	void fill_line (TAG_TYPE tag, DATA_TYPE * data, int page_off);
	// return true if a  dirty lineis to be replaced
	bool get_old_line (DATA_TYPE **data, int page_off,
		DWORD *wr_buffer_address_lo,DWORD* wr_buffer_address_hi);
	void clear_all ();
};


#endif   /*  __CACHE_H__  */