/* Functions to simulate a entry.
   Copyright (C) 2005 Huaibo Wang.
*/

#include "cache.h"

/*
 * cache_lru is a full-associative cache. replacement policy is LRU
 */

template <int N_ENTRIES,class TAG_TYPE, class DATA_TYPE>
void cache_full_associative<N_ENTRIES, TAG_TYPE, DATA_TYPE>::invalid (TAG_TYPE tag, int page_off)
{
	int i;

	for (i=0; i < N_ENTRIES; i++)
		if (entry [i].valid
			&& entry [i].tag == tag ) {

				entry [i].valid = false;

				break;
		}
}

template <int N_ENTRIES,class TAG_TYPE, class DATA_TYPE>
bool cache_full_associative<N_ENTRIES, TAG_TYPE, DATA_TYPE>::seek (TAG_TYPE tag, DATA_TYPE *data, int page_off)
{
	unsigned int i;

	for (i=0; i<N_ENTRIES; i++) {
		if (entry [i].valid
			&& entry [i].tag == tag) {

			int j;

			*data = entry[i].data;

			for (j=0; j<N_ENTRIES; j++) 
				if (entry [j].valid
					&& age [j] < age [i]) 
					age [j]++;

			age [i] = 0;

			return true;
		}
	}

	return false;
}

template <int N_ENTRIES,class TAG_TYPE, class DATA_TYPE>
bool cache_full_associative<N_ENTRIES, TAG_TYPE, DATA_TYPE>::test (TAG_TYPE tag, DATA_TYPE *data, int page_off)
{
	unsigned int i;

	for (i=0; i<N_ENTRIES; i++) {
		if (entry [i].valid
			&& entry [i].tag == tag) {

			*data = entry[i].data;

			return true;
		}
	}

	return false;
}

template <int N_ENTRIES,class TAG_TYPE, class DATA_TYPE>
void cache_full_associative<N_ENTRIES, TAG_TYPE, DATA_TYPE>::insert (TAG_TYPE tag, DATA_TYPE data, int page_off)
{
	unsigned int i;

    /* find free slot */
	for (i=0; i<N_ENTRIES; i++) {
		if (!entry [i].valid) {

			int j;

			entry [i].tag = tag;
			entry [i].data = data;
			
			for (j=0; j<N_ENTRIES; j++) 
				if (entry [j].valid
					&& j != i) 

					age [j]++;

			age [i] = 0;

			return;
		}
	}

	// replace the oldest entry, the oldest one must have an age of N_ENTRIES-1
	for (i=0; i<N_ENTRIES; i++) {
		if (age [i] < N_ENTRIES-1) age [i] ++;
		else {

			entry [i].tag = tag;
			entry [i].data = data;
		
			age [i] = 0;
		}
	}
	return;
}

template <int N_ENTRIES,class TAG_TYPE, class DATA_TYPE>
void cache_full_associative<N_ENTRIES, TAG_TYPE, DATA_TYPE>::clear_all ()
{
	unsigned int i;

	for (i=0; i<N_ENTRIES; i++) 
		entry [i].valid = 0;

}

/*
 * cache_n_way is a n-way associative cache.
 * replacement policy is pseudo-LRU
 *
 */
template < int N_ENTRIES,class TAG_TYPE, class DATA_TYPE,int N_WAYS>
void cache_n_way< N_ENTRIES, TAG_TYPE, DATA_TYPE,N_WAYS>::invalid (TAG_TYPE tag, int page_off)
{
	int i;
	int index=page_off;

	if (index < N_ENTRIES) 
	{
		for (i=0; i<N_WAYS; i++)
		if(entry [index][i].tag == tag)
		{
			entry [index] [i].state = 'I';
		}
		update_age(index,3^entry [index][i].age);
	}
	
}

template < int N_ENTRIES,class TAG_TYPE, class DATA_TYPE,int N_WAYS>
bool cache_n_way< N_ENTRIES, TAG_TYPE, DATA_TYPE,N_WAYS>::hit (TAG_TYPE tag, DATA_TYPE **data, int page_off)
{
	unsigned int i,j;
	/* map tag into a line */
	j = page_off;

	/* check which way is hit */
	for (i=0; i<N_WAYS; i++) {
		if (entry [j][i].state!='I'
			&& entry [j][i].tag == tag) {
			*data = &entry[j][i].data;

			update_age(j,entry [j][i].age);
			return 1;
		}
	}

	return 0;
}


template < int N_ENTRIES,class TAG_TYPE, class DATA_TYPE,int N_WAYS>
void cache_n_way< N_ENTRIES, TAG_TYPE, DATA_TYPE,N_WAYS>::fill_line (TAG_TYPE tag, DATA_TYPE *data, int page_off)
{
	unsigned int i,j;
	/* map tag into a line */
	j = page_off;

	/* check which way is hit */
	for (i=0; i<N_WAYS; i++) {
		if (entry [j][i].age == N_WAYS-1) {
			memcpy(&entry[j][i].data,data,sizeof DATA_TYPE);
			entry[j][i].tag=tag;
			entry [j][i].state!='S';
		}

		update_age(j,3);
	}	
}

template < int N_ENTRIES,class TAG_TYPE, class DATA_TYPE,int N_WAYS>
void cache_n_way< N_ENTRIES, TAG_TYPE, DATA_TYPE,N_WAYS>::clear_all ()
{
	unsigned int i,j;

	for (i=0; i<N_ENTRIES; i++) 
		for (j=0; j<N_WAYS; j++) {
			entry [i] [j].state = 'I';
			entry [i] [j].age=i;// open issue
		}
}



template < int N_ENTRIES,class TAG_TYPE, class DATA_TYPE,int N_WAYS>
bool cache_n_way< N_ENTRIES, TAG_TYPE, DATA_TYPE,N_WAYS>::get_old_line (DATA_TYPE **data, int page_off,
		DWORD *wr_buffer_address_lo,DWORD* wr_buffer_address_hi)
{
	unsigned int i,j;
	bool dirty=false;

	for (j=0; j<N_WAYS; j++) {
		if(entry [page_off] [j].state == 'I')
		{
			*data=NULL;
			break;
		}

		if(entry [page_off] [j].age == 3) 
		{
			dirty=true;
			entry [page_off] [j].state = 'I';
			*wr_buffer_address_lo=(entry[page_off][j].tag <<13) |page_off;
			*wr_buffer_address_hi=0;
			break;
		}
	}
	*data=&entry[page_off][j].data;
	//
	// if a line of age 3 was found, but  invalid line still exists, error
	//
	return dirty;

}