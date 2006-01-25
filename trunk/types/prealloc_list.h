//
// C++ Interface: prealloc_list
//
// Description: 
//
//
// Author: Juan Linietsky <coding@reduz.com.ar>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PREALLOC_LIST_H
#define PREALLOC_LIST_H


#include <vector>

/**
@author Juan Linietsky
*/
template<class T>
class PreallocList {

	struct Element {
	
		T data;
		bool used;
		Element *next,*prev;
		
		void reset() {
		
			used=false;
			next=prev=NULL;
	
		}
		
		Element() {
			reset();
		}
		
	};

	
	

	Element *head;
	Element tail;
	
	std::vector<Element> element_array ;
	
	inline Element* get_free_element();
		
public:

	class iterator {
	
	friend class PreallocList;
	
		Element *element_ptr;

	public:
		inline bool empty() { return (element_ptr==NULL); };
		inline iterator& operator++ () { element_ptr=element_ptr->next ? element_ptr->next : element_ptr; return *this; }
		inline iterator operator++ (int) { iterator old=*this; element_ptr=element_ptr->next ? element_ptr->next : element_ptr; return old; }
		inline iterator& operator-- () { element_ptr=element_ptr->prev ? element_ptr->prev : element_ptr; return *this; }

		inline T* operator->() { return &element_ptr->data; }
		inline T& operator*() { return element_ptr->data; }
		
		inline bool operator!=(const iterator& p_rval) const { return (element_ptr!=p_rval.element_ptr); }
		inline bool operator==(const iterator& p_rval) const { return (element_ptr==p_rval.element_ptr); }
	 	
		iterator() { element_ptr=NULL; }
		iterator(Element* p_element_ptr) { element_ptr=p_element_ptr; }
			
	};


	inline iterator begin();
	inline iterator end();
	
	inline iterator push_front(T& p_element);
	inline iterator push_back(T& p_element);
	
	inline bool pop_front();
	inline bool pop_back();
	
	inline bool empty();
	
	inline bool erase(iterator& p);
	
	inline int size();

	inline void clear();
	
	PreallocList(int p_prealloc_count=128);
	~PreallocList();
		
};



template<class T>
int PreallocList<T>::size() {

	int c=0;
	iterator I=begin(),E=end();
	
	while (I!=E) { c++; ++I; };
	
	return c;	
}


template<class T>
class PreallocList<T>::Element* PreallocList<T>::get_free_element() {

	for (int i=0;i<(int)element_array.size();i++) { //could be optimized.. wont bother to do it now
	
		if (!element_array[i].used) {
		
			element_array[i].used=true;
			return &element_array[i];
		}
	}

	return NULL; //no free cookies
}


template<class T>
class PreallocList<T>::iterator PreallocList<T>::begin() {

	return iterator(head);
}

template<class T>
class PreallocList<T>::iterator PreallocList<T>::end() {

	return iterator(&tail);
}


template<class T>
bool PreallocList<T>::empty() {

	return (head==&tail);
	
}

template<class T>
class PreallocList<T>::iterator PreallocList<T>::push_front(T& p_element) {

	Element * e = get_free_element();
	if (!e)
		return end();
		
	head->prev=e;
	e->next=head;
	head=e;
	e->data=p_element;
		
	return iterator(e);

}

template<class T>
class PreallocList<T>::iterator PreallocList<T>::push_back(T& p_element) {

	Element * e = get_free_element();
	if (!e)
		return end();
		
	e->next=&tail;
	e->prev=tail.prev;
	e->data=p_element;

	if (empty()) {
		head=e;
	} else {
		tail.prev->next=e;
	}
	
	tail.prev=e;
		
	return iterator(e);


}

template<class T>
void PreallocList<T>::clear() {
	
	while (!empty())
		pop_front();
	
}

template<class T>
bool PreallocList<T>::pop_front() {

	if (empty())
		return true;

	Element *e=head;
	head=e->next;
	head->prev=NULL;
	
	e->reset();
	
	
	return false;
}

template<class T>
bool PreallocList<T>::pop_back() {

	if (empty())
		return true;
	

	Element *e=tail.prev;
	
	if (e->prev) {
		e->prev->next=&tail;
		tail.prev=e->prev;
	} else {
		tail.prev=NULL;
		head=&tail;
	}
	
	e->reset();

	return false;
}


template<class T>
bool PreallocList<T>::erase(iterator& p) {

	if (empty())
		return false;
	Element *e=p.element_ptr;
	
	if (e==head) {
	
		pop_front();
		return true;
	}
	if (e==tail.prev) {
	
		pop_back();
		return true;
	}
	
	if (e==&tail)
		return false;

	e->prev->next=e->next;
	e->next->prev=e->prev;
	e->reset();
	
	return true;

}



template<class T>
PreallocList<T>::PreallocList(int p_prealloc_count) {

	element_array.resize(p_prealloc_count);
	head=&tail;
}

template<class T>
PreallocList<T>::~PreallocList() {


}

#endif
