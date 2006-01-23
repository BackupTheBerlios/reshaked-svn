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
	int claimcount;
	
public:	
	
	void reference();
	void dereference();
	
	void claim();
	void unclaim();
	
	void reset_refcount();
	int get_refcount();
	int get_claimcount();
	
	SharedData();
	virtual ~SharedData();
	
};



}

#endif
