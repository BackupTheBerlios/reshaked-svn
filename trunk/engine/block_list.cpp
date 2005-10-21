//
// File: /home/red/coding/svn/reshaked2/engine/block_list.h
// Created by: User <Email>
// Created on: Tue Oct 18 07:33:37 2005
//


#include "block_list.h"

namespace ReShaked {


Tick BlockList::get_next_empty_pos(Tick p_tick) {
	
	while (bl_private.block_list.get_exact_index( p_tick )!=INVALID_STREAM_INDEX)
		p_tick++; //offset a little bit so it can fit, small hack that works	
	
	return p_tick;
}


bool BlockList::add_block(Block *p_block,Tick p_pos) {

	if (block_type==BLOCK_TYPE_FREE_MOVING) {

		p_pos=get_next_empty_pos( p_pos );	
	} else {
		
		ERR_FAIL_COND_V( !block_fits(p_pos,p_block->get_length()), true );
	}
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
	
	
	prev_ok = (prev_idx==-1) || (p_current>=0 && prev_idx==p_current) || ((bl_private.block_list.get_index_pos(prev_idx)+bl_private.block_list.get_index_value_w(prev_idx)->get_length())<=p_pos);
	
	next_ok = (next_idx==get_block_count()) || (p_current>=0 && next_idx==p_current) || ((p_pos+p_size)<bl_private.block_list.get_index_pos(next_idx));
	
	return prev_ok && next_ok;
	
}

void BlockList::move_block(int p_which,Tick p_to_new_pos) {
	
	if (!is_move_block_allowed(p_which,p_to_new_pos))
		return; //not allowed
	
	if (p_to_new_pos==get_block_pos( p_to_new_pos ))
		return; //pointless to do anything
	
	Block *b = bl_private.block_list.get_index_value_w(p_which);
	bl_private.block_list.erase_index(p_which);
	//if blocks can overlap, there is a chance they may get the same pos, so give them the next contiguous pos, for non overlapping blocks or snapped to beat this wont happen
	p_to_new_pos=get_next_empty_pos( p_to_new_pos ); 
	bl_private.block_list.insert(p_to_new_pos,b);
	
}
bool BlockList::is_move_block_allowed(int p_which,Tick p_to_new_pos) {
	
	
	ERR_FAIL_INDEX_V(p_which,get_block_count(),false);

	if (block_type==BLOCK_TYPE_FREE_MOVING)
		return true;
		
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

int BlockList::get_block_idx_at_pos(Tick p_pos) { // NULL for no block


	if (p_pos<0)
		return -1;
	
	int prev_idx = bl_private.block_list.get_prev_index( p_pos );

	if (prev_idx==-1)
		return -1; //nuthin
		
	
	Block * b = get_block(prev_idx);
	
	if (( get_block_pos(prev_idx) + b->get_length() ) < p_pos )
		return -1;
	
	return prev_idx;
}

BlockList::BlockType BlockList::get_block_type() {
	
	return block_type;
}

BlockList::BlockList(BlockType p_type) {
	
	block_type=p_type;
}


BlockList::~BlockList() {
	
	
}

}
