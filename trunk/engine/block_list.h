//
// File: /home/red/coding/svn/reshaked2/engine/block_list.h
// Created by: User <Email>
// Created on: Tue Oct 18 07:33:37 2005
//

#ifndef _BLOCKLIST_H_
#define _BLOCKLIST_H_

#include "typedefs.h"
#include "value_stream.h"

namespace ReShaked {

class BlockList {
public:
	
	enum BlockType {
		BLOCK_TYPE_FREE_MOVING, ///< Move anywhere, overlap possible
		BLOCK_TYPE_FIXED_TO_BEAT, ///< Move and size snap to beat, overlap not possible
	
	};
	
	class Block {

		public:

		virtual Tick get_length()=0;
		virtual void set_length(Tick p_length)=0;
		virtual bool is_shared()=0;
		virtual ~Block() {}
	};
	
	class BlockCreationData {
		
	public:		
	
		virtual ~BlockCreationData() {}
	};

	
private:	
	struct BlockListPrivate {
		
		ValueStream<Tick,Block*> block_list;
	} bl_private;
	
	
	BlockType block_type;
	Tick get_next_empty_pos(Tick p_tick);
	
	BlockList(); //cant be called
protected:
	
	bool add_block(Block *p_block,Tick p_pos);
	void remove_block(int p_index);
	
	
	
	
public:
	
	virtual void create_block(Tick p_pos,BlockCreationData *p_creation_data=NULL)=0;
	virtual void copy_block(int p_from_which,Tick p_to_where)=0;
	virtual void copy_block_ref(int p_from_which,Tick p_to_where)=0;
	virtual void move_block(int p_which,Tick p_to_new_pos);
	virtual bool is_move_block_allowed(int p_which,Tick p_to_new_pos);
	virtual void erase_block(int p_which)=0;

	bool block_fits(Tick p_pos,Tick p_size,int p_current=-1);
		
	int get_block_count();
	Block* get_block(int p_index);

	Tick get_block_pos(int p_index);
	int get_block_idx_at_pos(Tick p_pos); ///< val<0 for no block, return first find in overlap
	
	virtual String get_type_name()=0;
	BlockType get_block_type();		 
	/* properties */
	
	BlockList(BlockType p_type);
	virtual ~BlockList();
	

};


}

#endif	//_BLOCKLIST_H_
