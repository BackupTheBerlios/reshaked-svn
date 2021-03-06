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
#include "engine/audio_control.h"

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
			e.pos.tick+=get_block_pos( i );
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


Track_Pattern::PatternBlock* Track_Pattern::create_pattern_block() {
	
	Pattern *p = new Pattern;
	PatternBlock *pb = new PatternBlock(p);
	
	return pb;
	
}

BlockList::Block* Track_Pattern::create_block(BlockCreationData *p_creation_data) {


	
	return create_pattern_block();
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
	np->reset();

	//new block
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



bool Track_Pattern::can_resize_from_begining() {

	return false;
}

Track_Pattern::Pattern *Track_Pattern::PatternBlock::get_pattern() {

	return pattern;

}

Track_Pattern::Pattern::Pattern() {
	length=1;
	
}
Tick Track_Pattern::PatternBlock::get_length() {

	return pattern->length*TICKS_PER_BEAT;
}

void Track_Pattern::PatternBlock::set_length(Tick p_length) {

	if (is_active_shared())
		return;

	if (p_length<TICKS_PER_BEAT)
		return; //pattern cant be less than a beat;

	pattern->length=p_length/TICKS_PER_BEAT;
	//length = p_length;


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


Track_Pattern::PatternBlock::PatternBlock(Pattern* p) : Block(p) {

	pattern = p;

}

Track_Pattern::PatternBlock::~PatternBlock() {
	


	
}
Track_Pattern::PatternBlock* Track_Pattern::get_block(int p_index) {

	PatternBlock *pb=dynamic_cast<PatternBlock*>( BlockList::get_block( p_index ) );
	return pb;
	
}

void Track_Pattern::set_visible_columns(int p_columns) {
	
	if (p_columns<1)
		return;
	data.visible_columns=p_columns;	
	
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

bool Track_Pattern::shares_block_data(Block *p_block) {
	
	PatternBlock *b=dynamic_cast<PatternBlock*>(p_block);
	if (!b)
		return false; //not same type
	for (int i=0;i<get_block_count();i++)
		if (b->get_pattern()==get_block(i)->get_pattern())
			return true; //shared!
		
	return false;
	
}


bool Track_Pattern::accepts_block(Block *p_block) {
	
	return dynamic_cast<PatternBlock*>(p_block)!=NULL;	
}

void Track_Pattern::plugin_added_notify(SoundPlugin *p_plugin) {
	
}


void Track_Pattern::add_noteon_event_to_buffer(char p_note,char p_velocity,int p_column,EventBuffer &p_buffer,int p_frame_offset) {
	
	EventBuffer &event_buffer=get_event_buffer();
	
	//printf("note %i\n",p_note);
	
	if (data.last_note[p_column].is_note() && data.last_note[p_column].note==p_note) {
		/* IF the note is the same, we must mute it before */
		Event e;
		SET_EVENT_MIDI(e,EventMidi::MIDI_NOTE_OFF,0,data.last_note[p_column].note,0);
		e.frame_offset=p_frame_offset; //off te
		p_buffer.push_event(e);
		data.last_note[p_column]=Note(Note::NOTE_OFF); //avoid the next check
		
	}
	
	/* send new note */
	{
		int velocity=(int)p_velocity*127/99;
		Event e;
		SET_EVENT_MIDI(e,EventMidi::MIDI_NOTE_ON,0,p_note,velocity);
		e.frame_offset=p_frame_offset;
		p_buffer.push_event(e);

	}
	
	if (data.last_note[p_column].is_note() && data.last_note[p_column].note!=p_note) {
		/* If the note is different, mute it later */
		Event e;
		SET_EVENT_MIDI(e,EventMidi::MIDI_NOTE_OFF,0,data.last_note[p_column].note,0);
		e.frame_offset=p_frame_offset; //off te
		p_buffer.push_event(e);

	}
	
	
	data.last_note[p_column]=Note(p_note);	
	
}

void Track_Pattern::add_edit_event(const EventMidi &p_event_midi,int p_column) {
	
	if (p_column==-1) {
		
		AudioControl::mutex_lock();	
		Event e;
		e.type=Event::TYPE_MIDI;
		e.param.midi=p_event_midi;
		e.frame_offset=0;
		data.edit_event_buffer.push_event(e);
		AudioControl::mutex_unlock();	
		return;
	}
	
	ERR_FAIL_INDEX(p_column,MAX_COLUMNS);
	
	AudioControl::mutex_lock();	
	
	if (p_event_midi.midi_type==EventMidi::MIDI_NOTE_ON) {
		
		add_noteon_event_to_buffer(p_event_midi.data.note.note,p_event_midi.data.note.velocity,p_column,data.edit_event_buffer,0);
	} else {
		
		Event e;
		e.type=Event::TYPE_MIDI;
		e.param.midi=p_event_midi;
		e.frame_offset=0;
		data.edit_event_buffer.push_event(e);
		
		
	}
	AudioControl::mutex_unlock();	

}

void Track_Pattern::play_external_event(const EventMidi &p_event_midi) {
	
	Event m_e;
	m_e.type=Event::TYPE_MIDI;			       
	m_e.param.midi=p_event_midi;
	m_e.frame_offset=0;
	
	data.edit_event_buffer.push_event(m_e);
}



void Track_Pattern::track_block_pre_process(int p_frames,int p_block, Tick p_offset,Tick p_total, Tick p_from, Tick p_to) {

	EventBuffer &event_buffer=get_event_buffer();
	
	Tick block_pos=get_block_pos(p_block);
	Tick tick_from_local=p_from;
	Tick tick_to_local=p_to;
	if (tick_from_local<0)
		tick_from_local=0;
	if (tick_to_local<0)
		tick_to_local=0;
		
	Position pos_from_local(tick_from_local,0);
	Position pos_to_local(tick_to_local,MAX_COLUMNS);
		
	Pattern *pd=get_block(p_block)->get_pattern();
	int from_idx;
	int to_idx;
	if (!pd->data.find_values_in_range(pos_from_local,pos_to_local,&from_idx,&to_idx)) {
			//printf("ticks %i to %i have indexes %i to %i\n",(int)tick_from_local,(int)tick_to_local,from_idx,to_idx);
		
		for (int i=from_idx;i<=to_idx;i++) {
								
			Tick tick=pd->data.get_index_pos(i).tick;
			if (tick<tick_from_local)
				continue;
			if (tick>tick_to_local)
				break;
			int col=pd->data.get_index_pos(i).column;
			if (col<0 || col>=MAX_COLUMNS)
				continue; //wont play what is not visible
			int frame=(p_offset+tick)*(p_total)/(Tick)p_frames;
				
				//printf("something at %i, in index %i (%i to %i) out of %i\n",(int)tick,i,from_idx,to_idx,pd->data.get_stream_size());
			Note n = pd->data.get_index_value(i);
				
			if (n.is_note()) {
					
					
				add_noteon_event_to_buffer(n.note,n.volume,col,event_buffer,frame);
					
			} else if (n.is_note_off() && data.last_note[col].is_note()) {
					
					//printf("AT %i - note off\n",(int)tick_to);
					
				Event e;
				SET_EVENT_MIDI(e,EventMidi::MIDI_NOTE_OFF,0,data.last_note[col].note,0);
				e.frame_offset=frame; //off te
				event_buffer.push_event(e);
				data.last_note[col]=n;
			}
		}
	}
	
	
}

void Track_Pattern::track_pre_process(int p_frames) {
	/* I HATE THIS FUNCTION */
	
	EventBuffer &event_buffer=get_event_buffer();
	
	//add the edit events (if any) at offset 0
	for (int i=0;i<data.edit_event_buffer.get_event_count();i++) {
		
		Event e=*data.edit_event_buffer.get_event(i);
		e.frame_offset=0;
		event_buffer.push_event(e);
	}
	data.edit_event_buffer.clear();

	
	if (get_song_playback()->get_status()!=SongPlayback::STATUS_PLAY) {
		
		data.old_tick_to=-1;
		return; //nothing much to do
	}
	int block_count=get_block_count();
	if (block_count==0) {
		data.old_tick_to=-1;
		return; //nuthin' to do
	}
	
	/* Up to where we are going to process. */
	Tick tick_to=get_swinged_tick( get_song_playback()->get_current_tick_to() );
		
	Tick tick_from; //from where we process
	
	if (data.old_tick_to==-1 || tick_to<data.old_tick_to || abs(tick_to-data.old_tick_to)>TICKS_PER_BEAT)
		// if previous processing pos is invalid, or position changed
		tick_from=get_song_playback()->get_current_tick_from();
	else
		
		tick_from=data.old_tick_to+1;
	
	
	if (tick_from>tick_to) //we already processed up to this tick
		return; //nothing to do
	
	data.old_tick_to=tick_to;
	
	int block_idx=get_prev_block_from_idx(tick_from);
	//printf("PAT: from %i to %i\n",(int)tick_from,(int)tick_to);
	
	if (block_idx==-1) {
		
		block_idx=0;
	}
	
	while (block_idx<block_count && (get_block_pos(block_idx)<=tick_to) ) {
		
		/* process block */
		Tick block_pos=get_block_pos(block_idx);
		Tick block_len=get_block(block_idx)->get_length();
		Tick block_end=block_pos+block_len;
		
		/* If not inside the block, dont even bother */
		if (tick_from<block_end)
			track_block_pre_process( p_frames, block_idx, 0, tick_to-tick_from, tick_from-block_pos, tick_to-block_pos );
		
		/* Check if this has repeat, and has to be computed! */
		if (get_block(block_idx)->is_repeat_active() && tick_to>=block_end) {
			
			Tick rep_tick_from=tick_from;
			Tick rep_tick_to=tick_to;
			Tick rep_ofs=0;
			
			/* Wrap tick to repeat-able-space*/
			if (block_idx<(block_count-1) && tick_to>=get_block_pos(block_idx+1)) {
				
				rep_tick_to=get_block_pos(block_idx+1)-1;
			}
			
			if (tick_from<block_end) {
				rep_tick_from=block_end;
				rep_ofs=block_end-tick_from;
			}
			
			/* convert to local to block positions */
			
			rep_tick_from-=block_pos;
			rep_tick_to-=block_pos;
			
			if (rep_tick_from<=rep_tick_to) { //can do bussiness 
			
				int from_at_idx=rep_tick_from/block_len;
				int to_at_idx=rep_tick_to/block_len;
				
				/* convert to inside block positions  */
				
				rep_tick_from=rep_tick_from%block_len;
				rep_tick_to=rep_tick_to%block_len;
				
				/* process all blocks that need repeat! */
				for (int i=from_at_idx;i<=to_at_idx;i++) {
					
					Tick from_local=0;
					Tick to_local=block_len-1;
												
					// adjust depending on begin and end
					if (i==from_at_idx)
						from_local=rep_tick_from;
					
					if (i==to_at_idx)
						to_local=rep_tick_to;
					
//					printf("%i Repeat at %i, %i - inside %i\n",i-from_at_idx,(int)from_local,(int)to_local,(int)block_len);
					track_block_pre_process( p_frames, block_idx, rep_ofs, tick_to-tick_from, from_local, to_local );
					
					rep_ofs+=block_len-from_local;
				}
			}
		}
		
		/* 
		Tick tick_from_local=tick_from-block_pos;
		Tick tick_to_local=tick_to-block_pos;
		if (tick_from_local<0)
			tick_from_local=0;
		if (tick_to_local<0)
			tick_to_local=0;
		
		Position pos_from_local(tick_from_local,0);
		Position pos_to_local(tick_to_local,MAX_COLUMNS);
		
		Pattern *pd=get_block(block_idx)->get_pattern();
		int from_idx;
		int to_idx;
		if (!pd->data.find_values_in_range(pos_from_local,pos_to_local,&from_idx,&to_idx)) {
			//printf("ticks %i to %i have indexes %i to %i\n",(int)tick_from_local,(int)tick_to_local,from_idx,to_idx);
		
			for (int i=from_idx;i<=to_idx;i++) {
								
				Tick tick=pd->data.get_index_pos(i).tick;
				if (tick<tick_from_local)
					continue;
				if (tick>tick_to_local)
					break;
				int col=pd->data.get_index_pos(i).column;
				if (col<0 || col>=MAX_COLUMNS)
					continue; //wont play what is not visible
				int frame=tick*(tick_to-tick_from)/(Tick)p_frames;
				
				//printf("something at %i, in index %i (%i to %i) out of %i\n",(int)tick,i,from_idx,to_idx,pd->data.get_stream_size());
				Note n = pd->data.get_index_value(i);
				
				if (n.is_note()) {
					
					
					add_noteon_event_to_buffer(n.note,n.volume,col,event_buffer,frame);
					
				} else if (n.is_note_off() && data.last_note[col].is_note()) {
					
					//printf("AT %i - note off\n",(int)tick_to);
					
					Event e;
					SET_EVENT_MIDI(e,EventMidi::MIDI_NOTE_OFF,0,data.last_note[col].note,0);
					e.frame_offset=frame; //off te
					event_buffer.push_event(e);
					data.last_note[col]=n;
				}
			}
		} */

		
		
		/* Process note OFFs at end of block */
		
		/* Is there a reason for this?
		Tick block_len=get_block(block_idx)->get_length();
		if (tick_from_local<=block_len && tick_to_local>=block_len) {
			
			int frame=block_len*(tick_to_local-tick_from_local)/(Tick)p_frames;
			
			for (int i=0;i<MAX_COLUMNS;i++) {
				
				if (data.last_note[i].is_note()) {
					
					//printf("AT %i - note off\n",(int)tick_to);
					
					Event e;
					SET_EVENT_MIDI(e,EventMidi::MIDI_NOTE_OFF,0,data.last_note[i].note,0);
					e.frame_offset=frame; //off te
					event_buffer.push_event(e);
					data.last_note[i]=Note(Note::NOTE_OFF);
				}				
			}
		}
		
		*/
		block_idx++;
	}
	
	
	
}


void Track_Pattern::reset_last_notes() {
	
	for(int i=0;i<MAX_COLUMNS;i++) {
		
		data.last_note[i]=Note(Note::NOTE_OFF);
	}
}

bool Track_Pattern::can_use_synths() {
	
	
	return true;
}

Track_Pattern::Track_Pattern(int p_channels,GlobalProperties *p_global_props,SongPlayback *p_song_playback) : Track(p_channels,BLOCK_TYPE_FIXED_TO_BEAT,p_global_props,p_song_playback), swing_process(p_song_playback) {

	data.visible_columns=1;
	reset_last_notes();
	

	data.old_tick_to=-1;	
}


Track_Pattern::~Track_Pattern()
{
}


}
