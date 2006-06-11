//
// C++ Interface: track_pattern
//
// Description:
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDTRACK_PATTERN_H
#define RESHAKEDTRACK_PATTERN_H


#include "engine/track.h"
#include "engine/data_pool.h"
#include "engine/swing_process.h"
#include <list>

namespace ReShaked {

/**
@author Juan Linietsky
*/

class PatternPool;

class Track_Pattern : public Track {
	
 	class Pattern;
	
public:
	
	enum {
		MAX_COLUMNS=16
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
		unsigned char portamento;
		Tick auto_off;

		Note(unsigned char p_note=NO_NOTE,unsigned char p_volume=MAX_VOLUME, unsigned char p_portamento=NO_PORTAMENTO) {

			note = p_note;
			volume = p_volume;
			portamento = p_portamento;
			auto_off=NO_AUTO_OFF;
		}

		bool is_note() { return note<MAX_NOTES; }
		bool is_note_off() { return note==NOTE_OFF; }
		bool is_empty() const { return (note==NO_NOTE) ; }
		void set_octave(unsigned char p_octave) { if (note<MAX_NOTES) note=(note%12)+p_octave*12; }
	};

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

	struct NoteListElement {
		Position pos;
		Note note;
	};
	
	
	typedef std::list<NoteListElement> NoteList;
	
	
	class  PatternBlock : public Block {
		
		Pattern *pattern;
		
	public:
		

		bool copy_shared();
		
		bool get_notes_in_local_range(Tick p_from, Tick p_to, int *p_note_from,int *p_note_to);
		
		int get_note_count();
		Note get_note(int p_index);
		
		Position get_note_pos(int p_index);
		
		Pattern *get_pattern();
		Tick get_length();
		void set_length(Tick p_length);

		PatternBlock(Pattern*);
		~PatternBlock();
	};
	
private:
friend class Saver; //saver needs access to Pattern structure for comparing it
	struct Pattern : public SharedData {

		int length; //length in beats

		ValueStream<Position,Note> data;

		Pattern();
	};



	

	BlockCreationBehavior get_block_creation_behavior();


	String get_type_name();
	bool can_resize_from_begining();
	
	struct Data {
		
		int visible_columns;	
		EventBuffer event_buffer;
		EventBuffer edit_event_buffer;
		
		Note last_note[MAX_COLUMNS];
		Tick old_tick_to;;
	} data;
	
	SwingProcess swing_process;
	
	void plugin_added_notify(SoundPlugin *p_plugin);	
	void track_block_pre_process(int p_frames,int p_block, Tick p_offset, Tick p_total,Tick p_from, Tick p_to);
	void track_pre_process(int p_frames);
	
	void reset_last_notes();
	bool can_use_synths();
	void add_noteon_event_to_buffer(char p_note,char p_velocity,int p_column,EventBuffer &p_buffer,int p_frame_offset);
	
	
public:

	Block *create_duplicate_block(Block *p_block);
	Block *create_link_block(Block *p_block);
	
	//called from non-player_thread
	void add_edit_event(const EventMidi &p_event_midi,int p_column);
	void play_external_event(const EventMidi &p_event_midi);
	
	
	static PatternBlock* create_pattern_block();

	Block* create_block(BlockCreationData *p_creation_data);
	
	bool shares_block_data(Block *p_block);
	bool accepts_block(Block *p_block);
	
 	
	void set_note(const Position& p_pos,const Note& p_note);
	Note get_note(const Position& p_pos);
	
	bool is_pos_editable(Tick p_pos);
	
	void set_visible_columns(int p_columns);
	int get_visible_columns();
	
	PatternBlock* get_block(int p_index);
	NoteList get_notes_in_range(Tick p_from, Tick p_to);
	
	
	Track_Pattern(int p_channels,GlobalProperties *p_global_props,SongPlayback *p_song_playback);
	~Track_Pattern();

};
 
}

#endif
