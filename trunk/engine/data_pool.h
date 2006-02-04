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


#include <vector>

namespace ReShaked {

/**
	
*/

class SharedBlock;

class SharedData {
	
	
	std::vector<SharedBlock*> owners;
	
	
	int get_block_idx(SharedBlock *p_block);
friend class SharedBlock;	
	void reference(SharedBlock *p_block);
	void dereference(SharedBlock *p_block);
	
public:	
	
	
	void reset();
	int get_refcount();
	
	SharedData();
	virtual ~SharedData();
	
};

class SharedBlock {
	
	SharedData *_shared_data;
	bool active;
public:	
	
	
	bool is_shared();
	bool is_active_shared();
	
	bool set_active(bool p_active);
	
	bool shared_with(SharedBlock *p_block);
	SharedBlock(SharedData *p_data);
	virtual ~SharedBlock();
	
};

}

#endif
