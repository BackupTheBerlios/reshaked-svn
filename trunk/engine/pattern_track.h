//
// C++ Interface: pattern_track
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PATTERN_TRACK_H
#define PATTERN_TRACK_H

#include "engine/track.h"


/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class PatternTrack : public Track {
public:
	class PatternBlock;

private:

	int visible_columns;

	virtual void process(const ProcessInfo& p_info);

	PatternTrack(int p_instanced_channels,const AudioNodeInfo *p_info);	

public:

	
	struct Position {

		Tick tick;
		int column;

		inline bool operator==(const Position& eq) { return tick==eq.tick && column==eq.column; }

		inline bool operator<(const Position& eq) { if (tick>eq.tick) return false; else if (tick<eq.tick) return true; else return (column<eq.column); }

		inline bool operator>(const Position& eq) { if (tick<eq.tick) return false; else if (tick>eq.tick) return true; else return (column>eq.column); }

		inline bool operator<=(const Position& eq) {
			return (*this==eq || *this<eq);
		}
		inline bool operator>=(const Position& eq) {
			return (*this==eq || *this>eq);
		}

		Position(Tick p_tick=0,int p_column=0) { tick=p_tick; column=p_column; }

	};	
	
	struct Note {

		enum {

			MAX_NOTES=128,
			NOTE_OFF=254,
			MAX_VOLUME=99,
			PREVIOUS_PORTAMENTO=0,
			NO_PORTAMENTO=100,
			NO_AUTO_OFF=0,
			NO_VOLUME=100,
			NO_NOTE=255,
		};

		unsigned char note;
		unsigned char volume;

		Note(unsigned char p_note=NO_NOTE,unsigned char p_volume=MAX_VOLUME) {

			note = p_note;
			volume = p_volume;
		}

		bool is_note() { return note<MAX_NOTES; }
		bool is_note_off() { return note==NOTE_OFF; }
		bool is_empty() const { return (note==NO_NOTE) ; }
		void set_octave(unsigned char p_octave) { if (note<MAX_NOTES) note=(note%12)+p_octave*12;  }
	};
		
	class PatternBlock : public Block {
	
		ValueStream<Position,Note> notes;
		int beats_len;
	public:
		
		bool get_notes_in_local_range(Tick p_from, Tick p_to, int *p_note_from,int *p_note_to) {
		
			return false;
		}
		
		inline int get_note_count() const { return notes.size(); }
		inline Note get_note(int p_index) const { 
			ERR_FAIL_INDEX_V( p_index, get_note_count(), Note() ); 
			return notes[p_index]; 
			
		}
		
		inline Position get_note_pos(int p_index) const {
		
			ERR_FAIL_INDEX_V( p_index, get_note_count(), Position() ); 
			return notes.get_pos(p_index);
		}
		
		Tick get_length() const  { return beats_len*TICKS_PER_BEAT; }
		void set_length(Tick p_length) { 
		
			p_length/=TICKS_PER_BEAT;
			if (p_length==0)
				return;
			beats_len=p_length;
		}
			
	};

	
	virtual bool accepts_block( Block *p_block ) const;
		
	virtual Block * create_block(BlockCreationData *p_data=NULL); 
	virtual BlockCreationBehavior get_block_creation_behavior() const; 
	virtual bool is_block_motion_snapped() const; 
	virtual int get_visible_columns() const;

	static AudioNode *creation_func(int p_channels,const AudioNodeInfo *p_info);
	static const AudioNodeInfo *get_creation_info();


	~PatternTrack();

};

#endif
