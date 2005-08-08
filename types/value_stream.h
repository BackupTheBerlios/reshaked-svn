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

#define INVALID_STREAM_INDEX -1

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
    int get_prev_index(T p_pos); /* get index to pos previous or equal to value */
    int get_next_index(T p_pos); /* get index to pos next or equal to value, if greater than last element, then stream_size+1 is returned */
    const V& get_index_value(int p_idx); /* return a const reference to null if index is invalid! */

    int get_stream_size();

    void erase_index(int p_index);
    void erase_index_range(int p_from_index, int p_to_index);
};


template<class T, class V>
int ValueStream<T,V>::find_pos(T p_pos,bool &p_exact) {

    /* The core of this class, the binary search */

    if (stream.empty())
	return INVALID_STREAM_INDEX;

	
	int low = 0;
	int high = stream.size() -1;
	int middle;
	Value *a=&stream[0];
	p_exact=false;

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

	bool exact;
	int pos=find_pos(p_pos,exact);	
	if (pos==INVALID_STREAM_INDEX) { //it's empty!
	    stream.resize(1);
            pos=0;
	} else if (!exact) { /* no exact position found, make room */
	
	    stream.resize(stream.size()+1);
	    if (pos<stream.size()) //if it's not adding at the last element
		memmove(&stream[pos+1],&stream[pos],sizeof(Value)*(stream.size()-pos));
	}

	stream[pos].val=p_value; /* Assign */

}

template<class T, class V>
const V& ValueStream<T,V>::get_index_value(int p_index) {

    ERR_FAIL_INDEX_V(p_index,stream.size(), *((V*)(NULL)));
    return stream[p_index].val;
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
    stream.erase( stream.begin() + p_index );

}

template<class T, class V>
void ValueStream<T,V>::erase_index_range(int p_from_idx,int p_to_idx) {


}






#endif
	
	
