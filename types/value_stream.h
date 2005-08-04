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
	V get_value(T p_pos);
	int get_index(T p_pos);
	
	void erase(T p_pos); 
	void erase(T p_from, T t_to); 
};


template<class T, class V>
int valuestream_binary_search(T p_pos,bool &p_exact) {

	
}

template<class T, class V>
int ValueStream<T,V>::find_pos(T p_pos,bool &p_exact) {

	/* The core of this class, the binary search */
	
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
	if (pos==-1)
		return; /* invalid pos? what? */
	if (!exact) { /* no exact position found, make room */
	
		stream.resize(stream.size()+1);
		memmove(&stream[pos+1],&stream[pos],sizeof(Value)*(stream.size()-pos));
	}

	stream[pos].value=p_value; /* Assign */

}

template<class T, class V>
V ValueStream<T,V>::get_value(T p_pos) {

	bool exact;
	int pos=find_pos(p_pos,exact);	
	if (pos==-1)
		return 0; /* invalid pos? what? */
	return stream[pos].val; /* no interpolation for now */
}

template<class T, class V>
int ValueStream<T,V>::get_index(T p_pos) {

	bool exact;
	return find_pos(p_pos,exact);	
}

template<class T, class V>
void ValueStream<T,V>::erase(T p_pos) {


}

template<class T, class V>
void ValueStream<T,V>::erase(T p_pos, T p_to_pos) {


}






#endif
	
	
