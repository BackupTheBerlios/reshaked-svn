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
		

	pattern_array.push_back(p);	
	
}

void Track_Pattern::copy_block(int p_from_which,Tick p_to_where) {


	/* not ref yet! */	
	PatternBlock *b = dynamic_cast<PatternBlock*>( get_block(p_from_which) );
	
	ERR_FAIL_COND( b==NULL );
			
		
	if (!block_fits(p_to_where,b->get_length()))
		return; //cant add thing
	
	Pattern *p = b->get_pattern();
	PatternBlock *pb = new PatternBlock(b->get_pattern());
	
	if (add_block(pb,p_to_where)) { //no add? fuck must be a bug!?
		
		p->refcount--; //WICKED SHIT!!
		delete pb;
		ERR_PRINT("Failed adding pattern after previous check? WTF? BUG! BUG! BUG!!!!");
		return;
	}
}

void Track_Pattern::copy_block_ref(int p_from_which,Tick p_to_where) {

	PatternBlock *b = dynamic_cast<PatternBlock*>( get_block(p_from_which) );
	
	ERR_FAIL_COND( b==NULL );

		
	if (!block_fits(p_to_where,b->get_length()))
		return; //cant add thing
	
	Pattern *p = b->get_pattern();
	PatternBlock *pb = new PatternBlock(b->get_pattern());
	
	if (add_block(pb,p_to_where)) { //no add? fuck must be a bug!?
		
		p->refcount--; //WICKED SHIT!!
		delete pb;
		ERR_PRINT("Failed adding pattern after previous check? WTF? BUG! BUG! BUG!!!!");
		return;
	}
		
	
}
void Track_Pattern::erase_block(int p_which) {
	
	
	
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

bool Track_Pattern::PatternBlock::pos_snaps_to_beat() {
	
	return true;
}

bool Track_Pattern::PatternBlock::is_shared() {
	
	return pattern->refcount>1;
	
}

Track_Pattern::PatternBlock::PatternBlock(Pattern* p) {
	
	pattern = p;
	p->refcount++;
	
}


Track_Pattern::Track_Pattern(int p_channels) : Track(p_channels)
{
}


Track_Pattern::~Track_Pattern()
{
}


}
