//
// C++ Interface: lock_free_ring_buffer
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef LOCK_FREE_RING_BUFFER_H
#define LOCK_FREE_RING_BUFFER_H



namespace ReShaked {


template<class T,int SIZE>
class LockFreeRingBuffer  {
	
	T rb[SIZE];
		
	
	int read_ptr;
	int write_ptr;
public:
				
	inline bool pending() { return (read_ptr!=write_ptr); }
	
	inline bool push(const T &p_element) { 
		if ( ((write_ptr+1)%SIZE)==read_ptr ) 
			return true;  
		rb[write_ptr]=p_element; 
		write_ptr=(write_ptr+1)%SIZE; 
		return false;
	}
	
	inline bool pop(T*p_to_element) {
		
		if (write_ptr==read_ptr)
			return true;
		*p_to_element=rb[read_ptr];
		read_ptr=(read_ptr+1)%SIZE; 
		return false;
	}
	
	void reset() {
		
		read_ptr=write_ptr=0;
	}
	
	LockFreeRingBuffer() { reset(); };
};

};

#endif

