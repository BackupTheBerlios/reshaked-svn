//
// File: /home/red/coding/svn/reshaked2/engine/block_list.h
// Created by: User <Email>
// Created on: Tue Oct 18 07:33:37 2005
//


#include "block_list.h"

namespace ReShaked {

bool BlockList::add_block(Block *p_block,Tick p_pos) {

	ERR_FAIL_COND_V( !block_fits(p_pos,p_block->get_length()), true );
	bl_private.block_list.insert(p_pos,p_block);
	
	return false;
}
void BlockList::remove_block(int p_index) {
	
	
}
bool BlockList::block_fits(Tick p_pos,Tick p_size,int p_current) {
	

	if (p_pos<0)
		return false;
	
	int next_idx = bl_private.block_list.get_next_index( p_pos );
	int prev_idx = bl_private.block_list.get_prev_index( p_pos );
	
	bool prev_ok=false;
	bool next_ok=false;
	
	
	prev_ok = (prev_idx==-1) || (p_current>=0 && prev_idx==p_current) || ((bl_private.block_list.get_index_pos(prev_idx)+bl_private.block_list.get_index_value_w(prev_idx)->get_length())<p_pos);
	
	next_ok = (next_idx==get_block_count()) || (p_current>=0 && next_idx==p_current) || ((p_pos+p_size)<bl_private.block_list.get_index_pos(next_idx));
	
	return prev_ok && next_ok;
	
}

void BlockList::move_block(int p_which,Tick p_to_new_pos) {
	
	if (!is_move_block_allowed(p_which,p_to_new_pos))
		return; //not allowed
	
	Block *b = bl_private.block_list.get_index_value_w(p_which);
	bl_private.block_list.erase_index(p_which);
	bl_private.block_list.insert(p_to_new_pos,b);
	
}
bool BlockList::is_move_block_allowed(int p_which,Tick p_to_new_pos) {
	
	ERR_FAIL_INDEX_V(p_which,get_block_count(),false);
	
	Tick current_size=bl_private.block_list.get_index_value_w(p_which)->get_length();
	
	return block_fits(p_to_new_pos,current_size,p_which);
}

int BlockList::get_block_count() {
	
	return bl_private.block_list.get_stream_size();	
}

BlockList::Block* BlockList::get_block(int p_index) {
	
	ERR_FAIL_INDEX_V(p_index,get_block_count(),NULL);
	return 	bl_private.block_list.get_index_value_w(p_index);
	
	
}

Tick BlockList::get_block_pos(int p_index) {
	
	ERR_FAIL_INDEX_V(p_index,get_block_count(),0);
	return bl_private.block_list.get_index_pos(p_index);
	
}


BlockList::BlockList() {
	
	
}


BlockList::~BlockList() {
	
	
}

}
