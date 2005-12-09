//
// C++ Implementation: data_pool
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "data_pool.h"
#include "error_macros.h"

namespace ReShaked {

void SharedData::reference() {
	
	refcount++;
}
void SharedData::dereference() {
	
	ERR_FAIL_COND(refcount<=0); //bug if it happens
		return;
		
	refcount--;
	
	if (refcount==0)
		delete this;
	
}

int SharedData::get_refcount() {
	
	return refcount;
}
void SharedData::reset_refcount() {
	
	refcount=0;
}
SharedData::SharedData() {
	
	reset_refcount();
	
}

SharedData::~SharedData() {
	
	ERR_FAIL_COND(refcount!=0); //just in case
	
}

}
