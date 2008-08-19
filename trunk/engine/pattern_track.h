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
	enum {
	
		MAX_VISIBLE_COLUMNS=16
	};

private:

	int visible_columns;
	bool mute;

	virtual void process(const ProcessInfo& p_info);

	PatternTrack(int p_instanced_channels,const AudioNodeInfo *p_info);	

public:

	
	struct Position {

		Tick tick;
		int column;

		inline bool operator==(const Position& eq) const { return tick==eq.tick && column==eq.column; }

		inline bool operator<(const Position& eq) const { if (tick>eq.tick) return false; else if (tick<eq.tick) return true; else return (column<eq.column); }

		inline bool operator>(const Position& eq) const { if (tick<eq.tick) return false; else if (tick>eq.tick) return true; else return (column>eq.column); }

		inline bool operator<=(const Position& eq) const {
			return (*this==eq || *this<eq);
		}
		inline bool operator>=(const Position& eq) const { 
			return (*this==eq || *this>eq);
		}

		Position(Tick p_tick=0,int p_column=0) { tick=p_tick; column=p_column; }

	};	
	
	struct Note {

		enum {

			MAX_NOTES=128,
			NOTE_OFF=254,
			MAX_VOLUME=99,
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
		
		bool operator==(const Note& p_note) const {
		
			return (p_note.volume == volume && p_note.note==note);
		}
	};
		
	class PatternBlock : public Block {
	
		ValueStream<Position,Note> notes;
		int beats_len;
	public:
		
		bool get_notes_in_local_range(Tick p_from, Tick p_to, int *p_note_from,int *p_note_to) const {
		
			int from = notes.find( p_from );
			int to = notes.find( p_to );
			
			if (to<0 || notes.get_pos(to).tick<p_from)
				return false; // out of range
			
			if (from<0 || notes.get_pos(from).tick<p_from)
				from++; // if from is less than first event, 
				
			
			*p_note_from=from;
			*p_note_to=to;

			return true;
		}
		
		inline int get_note_count() const { return notes.size(); }
		
		inline void set( const Position& p_pos, const Note& p_note) {
		
			if (p_note.is_empty()) {
			
				int exact_idx = notes.find_exact( p_pos );
				if (exact_idx >= 0)
					notes.erase( exact_idx );
			} else {
			
				notes.insert( p_pos, p_note );	
			}
		}
		inline const Note get( const Position& p_pos) {
		
			int exact_idx = notes.find_exact( p_pos );
			if (exact_idx >= 0)
				return notes[exact_idx];
			else
				return Note();
		}

		
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
			if (p_length<=0)
				return;
			beats_len=p_length;
		}
		
		PatternBlock() {
		
			beats_len=1;
		}
			
	};

	
	virtual bool accepts_block( Block *p_block ) const;
		
	virtual Block * create_block(BlockCreationData *p_data=NULL); 
	virtual BlockCreationBehavior get_block_creation_behavior() const; 
	virtual bool is_block_motion_snapped() const; 
	void set_visible_columns(int p_visible);
	virtual int get_visible_columns() const;
	
	void set_mute(bool p_mute);
	bool is_mute() const;
	

	static AudioNode *creation_func(int p_channels,const AudioNodeInfo *p_info);
	static const AudioNodeInfo *get_creation_info();


	~PatternTrack();

};

#endif
