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

Track_Pattern::NoteList Track_Pattern::get_notes_in_range(Tick p_from, Tick p_to) {
	
	NoteList res;
	int block_from;
	int block_to;
//	printf("get blocks in range %f-%f:",(float)p_from/TICKS_PER_BEAT,(float)p_to/TICKS_PER_BEAT);
	if (get_blocks_in_rage( p_from, p_to, &block_from, &block_to )) {
//		printf(" none!\n");
		return res; //emty list, nothing fits!
	}
	//printf(" %i\n",(block_to-block_from)+1);
	
	for (int i=block_from;i<=block_to;i++) {
		
		PatternBlock *b = get_block(i);
		Tick tick_begin=get_block_pos(i);
		Tick tick_end=tick_begin+b->get_length()-1;
		int note_from;
		int note_to;
		Tick tick_from=(tick_begin<p_from)?p_from:tick_begin;
		Tick tick_to=(p_to>tick_end)?tick_end:p_to;
				
//		printf("scanning block %i, ticks %f-%f\n",i,(float)tick_from/TICKS_PER_BEAT,(float)tick_to/TICKS_PER_BEAT);
		
		if (b->get_notes_in_local_range( tick_from-tick_begin, tick_to-tick_begin, &note_from, &note_to) )
			continue; //couldnt find any note
		//printf("found %i notes\n",note_to-note_from);
		
		for (int j=note_from;j<=note_to;j++) {
			
			NoteListElement e;
			e.note=b->get_note( j );
			e.pos=b->get_note_pos( j );
			res.push_back(e);
		}
	}
	
	
	return res;
}




/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

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

Track_Pattern::Pattern::Pattern() {
	length=1;
	refcount=0;
	data.insert( 0, Note(36,22) );
	data.insert( TICKS_PER_BEAT, Note(22,58) );
	data.insert( TICKS_PER_BEAT*3, Note(77,98) );
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

bool Track_Pattern::PatternBlock::get_notes_in_local_range(Tick p_from, Tick p_to, int *p_note_from,int *p_note_to) {
	
	return pattern->data.find_values_in_range(p_from,p_to,p_note_from,p_note_to);
}

int Track_Pattern::PatternBlock::get_note_count() {
	
	return pattern->data.get_stream_size();
}
Track_Pattern::Note Track_Pattern::PatternBlock::get_note(int p_index) {
	
	ERR_FAIL_INDEX_V(p_index,pattern->data.get_stream_size(),Note());
	return pattern->data.get_index_value( p_index );
	
}
Track_Pattern::Position Track_Pattern::PatternBlock::get_note_pos(int p_index) {
	
	ERR_FAIL_INDEX_V(p_index,pattern->data.get_stream_size(),Position());
	return pattern->data.get_index_pos( p_index );
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

	
void Track_Pattern::set_note(const Position& p_pos,const Note& p_note) {
	
	int pb_idx = get_block_idx_at_pos( p_pos.tick );
	ERR_FAIL_COND(pb_idx==-1);
	PatternBlock *bp=get_block(pb_idx);
	ERR_FAIL_COND(bp==NULL);
	
	Position rel_pos( p_pos.tick-get_block_pos(pb_idx) , p_pos.column );

	if (p_note.is_empty()) { //sets an empty note, delete it */
		int tick_idx = bp->get_pattern()->data.get_exact_index( rel_pos );
		if (tick_idx==INVALID_STREAM_INDEX) {
			return; /* nothing to erase */
		}   

		bp->get_pattern()->data.erase_index(tick_idx);

	} else { /* just insert-overwrite wathever you can find */

		bp->get_pattern()->data.insert( rel_pos, p_note);
	
	}

}
	
Track_Pattern::Note Track_Pattern::get_note(const Position& p_pos) {
		
	int pb_idx = get_block_idx_at_pos( p_pos.tick );
	ERR_FAIL_COND_V(pb_idx==-1,Note());
	PatternBlock *bp=get_block(pb_idx);
	ERR_FAIL_COND_V(bp==NULL,Note());
	
	Position rel_pos( p_pos.tick-get_block_pos(pb_idx) , p_pos.column );
	
	int tick_idx = bp->get_pattern()->data.get_exact_index(rel_pos);
	if (tick_idx==INVALID_STREAM_INDEX)
		return Note(); //return empty note, as pos in tick does not exist */
	
	return bp->get_pattern()->data.get_index_value(tick_idx);

}

bool Track_Pattern::is_pos_editable(Tick p_pos) {
	
	return (get_block_idx_at_pos( p_pos )!=-1);
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
