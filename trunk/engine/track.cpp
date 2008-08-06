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
	if (_refcount==0)
		delete this;

}

Track::Block::Block() {

	_refcount=1;
	_repeat=false;
}

Track::Block::~Block() {


}


void Track::remove_block(int p_index) {

	Block *b = take_block(p_index);
	ERR_FAIL_COND(!b);
	
	b->unreference();	
}


int Track::insert_block(Block* p_block, Tick p_at_pos) {

	Block *pb = dynamic_cast<Block*>(p_block);
	
	ERR_FAIL_COND_V(pb==NULL,-1);
	ERR_FAIL_COND_V(!block_fits( p_at_pos, p_block->get_length()),-1);
			
	pb->reference();
	return blocks.insert(p_at_pos,pb);			
}

Track::Block * Track::take_block(int p_index) {

	ERR_FAIL_INDEX_V( p_index, blocks.size(), NULL );
	
	Track::Block *b = blocks[p_index];
	blocks.erase(p_index);
	
	
	return b;
}




int Track::get_block_count() const {

	return blocks.size();
}
Track::Block* Track::get_block(int p_index) const {

	ERR_FAIL_INDEX_V( p_index, blocks.size(), NULL );
	
	return blocks[p_index];
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

	return blocks.find_exact(p_pos);
}

void Track::get_blocks_in_rage(Tick p_from, Tick p_to,int *p_from_res, int *p_to_res) const {

	int idx = blocks.find(p_from);
	int c=blocks.size();
	
	// invalidate by default
	*p_from_res=-1;
	*p_to_res=-1;
	
	while(idx<c) {
	
		if (idx>=0) {
		
			if ( *p_from_res==-1 && (blocks.get_pos(idx) + blocks[idx]->get_length()) > p_from ) {
				*p_from_res=idx;
			}
			
			if (blocks.get_pos(idx) <=p_to )
				*p_to_res=idx;
			else
				break;
				
		}
		idx++;
	}
	
}

bool Track::block_fits(Tick p_at_pos, Tick p_len,const std::list<int>& p_ignore_blocks) const {


	int block_from,block_to;
	
	get_blocks_in_rage(p_at_pos,p_at_pos+p_len-1,&block_from,&block_to);
	
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


Track::Track(int p_instanced_channels,const AudioNodeInfo *p_info) : HL_AudioNode(p_instanced_channels,p_info)
{
}


Track::~Track()
{
}


