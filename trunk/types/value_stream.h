//
// C++ Interface: value_stream
//
// Description:
//
//
// Author: Juan Linietsky <coding@reduz.com.ar>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef VALUE_STREAM_H
#define VALUE_STREAM_H

#include <vector>
#include "error_macros.h"
#define INVALID_STREAM_INDEX -50

extern void value_stream_global_lock();
extern void value_stream_global_unlock();

template<class T, class V>
class ValueStream {
private:
	struct Value {

		T pos;
		V val;
	};


	std::vector<Value> stream;

	int find_pos(T p_pos,bool &p_exact);
public:

	void insert(T p_pos,V p_value);
	int get_exact_index(T p_pos); /* return INVALID_STREAM_INDEX if pos is not exact */
	int get_prev_index(T p_pos); /* get index to pos previous or equal to value, if nothing less than it, return -1 */
	int get_next_index(T p_pos); /* get index to pos next or equal to value, if greater than last element, then stream_size+1 is returned */
	const V& get_index_value(int p_idx); /* return a const reference to     const V& get_index_value(int p_idx);  return a const reference to null if index is invalid! */
	V& get_index_value_w(int p_idx); /* return a reference to     const V& get_index_value(int p_idx); return a const reference to null if index is invalid! */
	const T& get_index_pos(int p_idx); /* return a const reference to     const T& get_index_pos(int p_idx);  return a const reference to null if index is invalid! */
	
	int get_stream_size();
	
	void erase_index(int p_index);
	void erase_index_range(int p_from_index, int p_to_index);
	
	bool find_values_in_range(T p_begin, T p_end, int *p_from_idx, int *p_to_idx); ///< return values inside a certain range
	void clear();

};


template<class T, class V>
int ValueStream<T,V>::find_pos(T p_pos,bool &p_exact) {

    /* The core of this class, the binary search */
    p_exact=false;

    if (stream.empty())
	return INVALID_STREAM_INDEX;


	int low = 0;
	int high = stream.size() -1;
	int middle;
	Value *a=&stream[0];

	while( low <= high )
	{
		middle = ( low  + high ) / 2;

		if( p_pos == a[  middle ].pos ) { //match
			p_exact=true;
			return middle;
		} else if( p_pos < a[ middle].pos )
			high = middle - 1; //search low end of array
		else
			low = middle + 1; //search high end of array
	}

	/* adapt so we are behind 2 */

	if (a[middle].pos<p_pos)
		middle++;
	return middle;
}


template<class T, class V>
void ValueStream<T,V>::insert(T p_pos,V p_value) {

	
	value_stream_global_lock();
	
	Value new_v;
	new_v.pos=p_pos;
	new_v.val=p_value;
	
	bool exact;
	int pos=find_pos(p_pos,exact);
	if (pos==INVALID_STREAM_INDEX) { //it's empty!
		stream.push_back(new_v);
	} else if (!exact) { /* no exact position found, make room */
		if (pos==stream.size())
			stream.push_back(new_v); //it's at the end, just pushit back
		else
			stream.insert(stream.begin()+pos,new_v);
	} else {
		
		stream[pos]=new_v; /* Overwrite, sine exact position */
	}

	value_stream_global_unlock();
	
}

template<class T, class V>
const V& ValueStream<T,V>::get_index_value(int p_index) {

    ERR_FAIL_INDEX_V(p_index,stream.size(), *((V*)(NULL)));
    return stream[p_index].val;
}

template<class T, class V>
V& ValueStream<T,V>::get_index_value_w(int p_index) {

	ERR_FAIL_INDEX_V(p_index,stream.size(), *((V*)(NULL)));
	return stream[p_index].val;
}

template<class T, class V>
const T& ValueStream<T,V>::get_index_pos(int p_index) {

	ERR_FAIL_INDEX_V(p_index,stream.size(), *((T*)(NULL)));
	return stream[p_index].pos;
}

template<class T, class V>
int ValueStream<T,V>::get_exact_index(T p_pos) {

	bool exact;
	int pos=find_pos(p_pos,exact);
	if (!exact)
	    return INVALID_STREAM_INDEX;
        return pos;
}

template<class T, class V>
int ValueStream<T,V>::get_prev_index(T p_pos) {

	if (stream.size()==0 || stream[0].pos>p_pos)
		return -1;
	bool exact;
	int pos=find_pos(p_pos,exact);
	if (pos==INVALID_STREAM_INDEX)
	    return INVALID_STREAM_INDEX;
	if (exact)
	    return pos;
	pos--;
	if (pos==-1)
	    return INVALID_STREAM_INDEX; /* smaller than first or empty stream */

	return pos;

}

template<class T, class V>
int ValueStream<T,V>::get_next_index(T p_pos) {

	if (stream.size()==0)
		return 0;
	
	bool exact;
	int pos=find_pos(p_pos,exact);

        return pos;

}

template<class T, class V>
int ValueStream<T,V>::get_stream_size() {

    return stream.size();
}


template<class T, class V>
void ValueStream<T,V>::erase_index(int p_index) {

    ERR_FAIL_INDEX(p_index,stream.size());
    value_stream_global_lock();
    stream.erase( stream.begin() + p_index );
    value_stream_global_unlock();
	
}

template<class T, class V>
void ValueStream<T,V>::erase_index_range(int p_from_idx,int p_to_idx) {


}

template<class T, class V>
bool ValueStream<T,V>::find_values_in_range(T p_begin, T p_end, int *p_from_idx, int *p_to_idx) {
	
	//initialzie to invalid
	*p_from_idx=-1;
	*p_to_idx=-1;
	
	int from_idx = get_next_index( p_begin );
	
	int first_fit=-1;
	int last_fit=-1;
	
	while (from_idx<stream.size()) {
		
		
		T pos=stream[from_idx].pos;
		bool fits=(pos>=p_begin) && (pos<=p_end);
		if (fits) {
			
			if (first_fit==-1)
				first_fit=from_idx;
			last_fit=from_idx;
		}
		
		if (pos>p_end)
			break;
		
		from_idx++;
	}
	
	if (first_fit==-1) //no fits
		return true;
	
	*p_from_idx=first_fit;
	*p_to_idx=last_fit;

	return false;

}

template<class T, class V>
void ValueStream<T,V>::clear() {
	
	stream.clear();	
}
#endif
