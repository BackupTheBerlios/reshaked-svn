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


void BlockList::copy_block(Block *p_from,Tick p_to_where,int p_existing) {

	ERR_FAIL_COND( p_from==NULL );

	if (block_type!=BLOCK_TYPE_FREE_MOVING)
		ERR_FAIL_COND( !block_fits(p_to_where,p_from->get_length(),p_existing) );


	Block *b = create_duplicate_block(p_from);

	add_block( b, p_to_where );

}
void BlockList::copy_block_link(Block *p_from,Tick p_to_where,int p_existing) {


	ERR_FAIL_COND( p_from==NULL );

	if (block_type!=BLOCK_TYPE_FREE_MOVING)
		ERR_FAIL_COND( !block_fits(p_to_where,p_from->get_length(),p_existing) );


	Block *b = create_link_block(p_from);

	add_block( b, p_to_where );
}

void BlockList::move_block(BlockList *p_from_track,int p_which,Tick p_to_pos) {

	Block * block = p_from_track->get_block( p_which );
	int current=(p_from_track==this)?p_which:-1;
	if (block_type!=BLOCK_TYPE_FREE_MOVING)
		ERR_FAIL_COND( !block_fits(p_to_pos, block->get_length(), current) );

	p_from_track->remove_block(p_which); //remove first so index doenst change
	add_block( block, p_to_pos);
	//move it
}

void BlockList::remove_block(int p_index) {

	ERR_FAIL_INDEX( p_index , get_block_count() );

	bl_private.block_list.erase_index( p_index );

}
bool BlockList::block_fits(Tick p_pos,Tick p_size,int p_current) {


	if (p_pos<0)
		return false;

	int next_idx = bl_private.block_list.get_next_index( p_pos );
	int prev_idx = bl_private.block_list.get_prev_index( p_pos );

	if (p_current!=-1 && next_idx==p_current)
		next_idx++;
	if (p_current!=-1 && prev_idx==p_current)
		prev_idx--;

	bool prev_ok=false;
	bool next_ok=false;


	prev_ok = (prev_idx==-1) /*|| (p_current>=0 && prev_idx==p_current) */ || ((bl_private.block_list.get_index_pos(prev_idx)+bl_private.block_list.get_index_value_w(prev_idx)->get_length())<=p_pos);

	next_ok = (next_idx==get_block_count()) || /* (p_current>=0 && next_idx==p_current) || */ ((p_pos+p_size)<=bl_private.block_list.get_index_pos(next_idx));

	return prev_ok && next_ok;

}

bool BlockList::is_move_block_allowed(int p_which,Tick p_to_new_pos) {


	ERR_FAIL_INDEX_V(p_which,get_block_count(),false);

	if (block_type==BLOCK_TYPE_FREE_MOVING)
		return true;

	Tick current_size=bl_private.block_list.get_index_value_w(p_which)->get_length();

	return block_fits(p_to_new_pos,current_size,p_which);
}

int BlockList::get_block_index(Block* p_block) {

	int bcount=get_block_count();
	for (int i=0;i<bcount;i++)
		if (get_block( i ) == p_block )
			return i;


	return -1;

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

	if (( get_block_pos(prev_idx) + b->get_length() ) <= p_pos )
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
