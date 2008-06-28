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



int SharedData::get_block_idx(SharedBlock *p_block) {
	
	for (int i=0;i<owners.size();i++)
		if (owners[i]==p_block)
			return i;
	
	return -1;
	
}
void SharedData::reference(SharedBlock *p_block) {
	
	ERR_FAIL_COND(get_block_idx( p_block )>=0);

	owners.push_back(p_block);
}
void SharedData::dereference(SharedBlock *p_block) {
	
	int block_idx=get_block_idx( p_block );
	
	ERR_FAIL_COND(block_idx==-1);
	owners.erase( owners.begin()+block_idx );
	
	if (owners.empty())
		delete this;
	
}

int SharedData::get_refcount() {
	
	return owners.size();
}
void SharedData::reset() {
	
	owners.clear();
}

SharedData::SharedData() {
	
	
}

SharedData::~SharedData() {
	
	ERR_FAIL_COND(!owners.empty()); //just in case
	
}





/*************** SharedBlock ********/


bool SharedBlock::is_active_shared() {
	
	int active_count=0;
	for (int i=0;i<_shared_data->owners.size();i++)
		if (_shared_data->owners[i]->active)
			active_count++;
	
	return (active_count>1);
}  
bool SharedBlock::is_shared() {
	
	return (_shared_data->get_refcount()>1);
	
}

bool SharedBlock::set_active(bool p_active) {
	
	active=p_active;
}

bool SharedBlock::shared_with(SharedBlock *p_block) {
	
	return (p_block->_shared_data==_shared_data);
}
SharedBlock::SharedBlock(SharedData *p_data) {
	
	_shared_data=p_data;
	
	_shared_data->reference( this );
	active=false;
	
}

SharedBlock::~SharedBlock() {
	
	_shared_data->dereference( this );
}






}
