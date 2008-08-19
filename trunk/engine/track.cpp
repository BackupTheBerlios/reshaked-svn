//
// C++ Implementation: track
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "track.h"

bool Track::Block::is_repeat_enabled() const {

	return _repeat;
}
void Track::Block::set_repeat_enabled(bool p_enabled)  {

	_repeat=p_enabled;
}


void Track::Block::reference() {

	_refcount++;
}
void Track::Block::unreference() {

	_refcount--;
	ERR_FAIL_COND(_refcount<0);

}

int Track::Block::get_refcount() {

	return _refcount;
}
Track::Block::Block() {

	_refcount=0;
	_repeat=false;
}

Track::Block::~Block() {

	ERR_FAIL_COND( _refcount > 0 );
}


void Track::remove_block(int p_index) {

	ERR_FAIL_INDEX( p_index, blocks.size());
	
	Track::Block *b = blocks[p_index];
	ERR_FAIL_COND(!b);
	blocks.erase(p_index);
	
	b->unreference();	
}


int Track::insert_block(Block* p_block, Tick p_at_pos) {

	if (is_block_motion_snapped()) {
	
		p_at_pos-=p_at_pos%TICKS_PER_BEAT;
	}

	Block *pb = dynamic_cast<Block*>(p_block);
	
	ERR_FAIL_COND_V(pb==NULL,-1);
	ERR_FAIL_COND_V(!block_fits( p_at_pos, p_block->get_length()),-1);
			
	pb->reference();
	return blocks.insert(p_at_pos,pb);			
}


int Track::get_block_count() const {

	return blocks.size();
}


Track::Block* Track::get_block(int p_index) const {

	ERR_FAIL_INDEX_V( p_index, blocks.size(), NULL );
	
	return blocks[p_index];
}

Tick Track::get_block_pos(int p_index) const {

	ERR_FAIL_INDEX_V( p_index, blocks.size(), 0 );
	return blocks.get_pos(p_index);

}
int Track::find_block_index(Block* p_block) const {

	for (int i=0;i<blocks.size();i++) {
	
		if (blocks[i]==p_block)
			return i;
	}
	
	return -1;
}
int Track::find_block_at_pos( Tick p_pos ) const {

	return blocks.find(p_pos);
}

int Track::get_block_at_pos( Tick p_pos ) const {

	int block = blocks.find(p_pos);
	
	if (block<0)
		return -1;
		
	if ( blocks.get_pos(block) + blocks[block]->get_length() <= p_pos )
		return -1;
		
	return block;
}

bool Track::get_blocks_in_range(Tick p_from, Tick p_to,int *p_from_res, int *p_to_res) const {

	if( p_to<p_from )
		return false;
		

	int from = blocks.find(p_from);
	int to = blocks.find(p_to); // to is final
	
	if (from<0 || p_from >= (blocks.get_pos(from)+blocks[from]->get_length()))
		from++;
		
	*p_from_res=from;
	*p_to_res=to;
		
	return from <= to;

}

bool Track::block_fits(Tick p_at_pos, Tick p_len,const std::list<int>& p_ignore_blocks) const {

	if (is_block_motion_snapped()) {
	
		p_at_pos-=p_at_pos%TICKS_PER_BEAT;
		if (p_len%TICKS_PER_BEAT) {
		
			p_len-=p_len%TICKS_PER_BEAT;
			p_len+=TICKS_PER_BEAT;
		}
	}

	int block_from,block_to;
	
	if (!get_blocks_in_range(p_at_pos,p_at_pos+p_len-1,&block_from,&block_to))
		return true; // just fits
	
	if (block_from==-1)
		return true; // nothing inbetween
	else if (p_ignore_blocks.empty()) {
		return false; // something inbetween and nothing to ignore
	}
	
	for(int i=block_from;i<=block_to;i++) {
	
		std::list<int>::const_iterator I = p_ignore_blocks.begin();
		
		bool ignored=false;
		
		for(;I!=p_ignore_blocks.end();I++) {
		
			if (*I==i) {
			
				ignored=true;
				break;
			}
		}
		
		if (!ignored)
			return false; // this block can't be ignored, doesn't fit!
	}
	
	return true; // all block inbetween were ignored
	

}

void Track::clear() {

	for (int i=0;i<get_block_count();i++) {
	
		get_block(i)->unreference();
	}

	blocks.clear();
}

bool Track::is_collapsed() const {

	return collapsed;
}
void Track::set_collapsed(bool p_collapsed) {

	collapsed=p_collapsed;
}


Track::Track(int p_instanced_channels,const AudioNodeInfo *p_info) : HL_AudioNode(p_instanced_channels,p_info) {

	collapsed=false;
}


Track::~Track()
{
}


