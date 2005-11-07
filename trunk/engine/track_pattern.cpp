//
// C++ Implementation: track_pattern
//
// Description:
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "track_pattern.h"

namespace ReShaked {



void Track_Pattern::create_block(Tick p_pos,BlockCreationData *p_creation_data) {

	if (!block_fits(p_pos,TICKS_PER_BEAT))
		return; //cant add thing

	Pattern *p = new Pattern;
	PatternBlock *pb = new PatternBlock(p);

	if (add_block(pb,p_pos)) { //no add? fuck must be a bug!?

		delete p;
		delete pb;
		ERR_PRINT("Failed adding pattern after previous check? WTF?");
		return;
	}


	pool->add_data(p);

}



BlockList::BlockCreationBehavior Track_Pattern::get_block_creation_behavior() {

	return BLOCK_CREATE_AND_RESIZE;
}

BlockList::Block *Track_Pattern::create_duplicate_block(Block *p_block) {

	PatternBlock *b = dynamic_cast<PatternBlock*>( p_block );
	ERR_FAIL_COND_V( b==NULL , NULL );

	//new pattern
	Pattern *np = new Pattern;
	*np=*b->get_pattern(); //copy
	np->refcount=0; //refcount to zero

	//new block
	pool->add_data(np);
	PatternBlock *nb = new PatternBlock(np);

	return nb;


}

BlockList::Block *Track_Pattern::create_link_block(Block *p_block) {

	PatternBlock *b = dynamic_cast<PatternBlock*>( p_block );
	ERR_FAIL_COND_V( b==NULL , NULL );

	//new block
	PatternBlock *nb = new PatternBlock(b->get_pattern());

	return nb;

}


void Track_Pattern::erase_block(int p_which) {



}

bool Track_Pattern::can_resize_from_begining() {

	return false;
}
void Track_Pattern::process_track(AudioBuffer *p_in_buf,AudioBuffer *p_out_buf,int p_frames) {



}

Track_Pattern::Pattern *Track_Pattern::PatternBlock::get_pattern() {

	return pattern;

}
Tick Track_Pattern::PatternBlock::get_length() {

	return pattern->length*TICKS_PER_BEAT;
}

void Track_Pattern::PatternBlock::set_length(Tick p_length) {

	if (is_shared())
		return;

	if (p_length<TICKS_PER_BEAT)
		return; //pattern cant be less than a beat;

	pattern->length=p_length/TICKS_PER_BEAT;
	//length = p_length;


}


bool Track_Pattern::PatternBlock::is_shared() {

	return pattern->refcount>1;

}

Track_Pattern::PatternBlock::PatternBlock(Pattern* p) {

	pattern = p;
	p->refcount++;

}

Track_Pattern::PatternBlock* Track_Pattern::get_block(int p_index) {

	PatternBlock *pb=dynamic_cast<PatternBlock*>( BlockList::get_block( p_index ) );
	return pb;
	
}
int Track_Pattern::get_visible_columns() {
	
	return data.visible_columns;	
}

String Track_Pattern::get_type_name() {

	return "pattern";
}
Track_Pattern::Track_Pattern(int p_channels,DataPool *p_pool) : Track(p_channels,BLOCK_TYPE_FIXED_TO_BEAT) {

	pool=p_pool;
	data.visible_columns=1;
}


Track_Pattern::~Track_Pattern()
{
}


}
