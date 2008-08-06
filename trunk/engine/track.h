//
// C++ Interface: track
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TRACK_H
#define TRACK_H

#include "engine/hl_audio_node.h"
#include "value_stream.h"
#include <list>
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class Track : public HL_AudioNode {
public:
	
	class Block {
	
		bool _repeat;
		int _refcount;
	public:		
		bool is_repeat_enabled() const;
		void set_repeat_enabled(bool p_enabled);
				
		virtual Tick get_length() const=0;
		virtual void set_length(Tick p_length)=0;
		
		void reference();
		void unreference();
	
		Block();
		virtual ~Block();
	};
private:


	ValueStream<Tick,Block*> blocks;

public:

	enum BlockCreationBehavior {
          	BLOCK_CREATE_AND_MOVE,
		BLOCK_CREATE_AND_RESIZE
	};
	
	/* special track block handling functions */
	virtual bool accepts_block( Block *p_block ) const=0;
	
	struct BlockCreationData {
	
		virtual ~BlockCreationData() {} 
	};
	
	virtual Block * create_block(BlockCreationData *p_data=NULL)=0; ///< create block, can return NULL if creation data wasn't as expected
	
	virtual BlockCreationBehavior get_block_creation_behavior() const=0; ///< what to do when creating a block
	virtual bool is_block_motion_snapped() const=0; ///< block motion is snapped to beat
	virtual int get_visible_columns() const=0;

	/* common to all tracks block handling functions */

	int insert_block(Block* p_block, Tick p_at_pos);
	Block * take_block(int p_index); // remove block from track, but don't unref it (taker should)
	void remove_block(int p_index); // remove block from track, and unref it
	
	int get_block_count() const;
	Block* get_block(int p_index) const;
	int find_block_index(Block* p_block) const;
	int find_block_at_pos( Tick p_pos ) const;
	int get_block_at_pos( Tick p_pos ) const;
	void get_blocks_in_rage(Tick p_from, Tick p_to,int *p_from_res, int *p_to_res) const;
	bool block_fits(Tick p_at_pos, Tick p_len,const std::list<int>& p_ignore_blocks=std::list<int>()) const; // helper
	
	virtual void clear();
	
	Track(int p_instanced_channels,const AudioNodeInfo *p_info);	
	~Track();

};

#endif
