//
// C++ Interface: pattern_pool
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDDATA_POOL_H
#define RESHAKEDDATA_POOL_H


namespace ReShaked {

/**
	
*/


class SharedData {
	
	int refcount;
	
public:	
	
	void reference();
	void dereference();
	
	void reset_refcount();
	int get_refcount();
	
	SharedData();
	virtual ~SharedData();
	
};



}

#endif
