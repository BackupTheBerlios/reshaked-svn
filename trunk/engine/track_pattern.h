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

namespace ReShaked {

/**
@author Juan Linietsky
*/

class PatternPool;

class Track_Pattern : public Track {

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


	struct Pattern : public DataPool::Data {

		int length; //length in beats

		ValueStream<Position,Note> data;

		int refcount;


		Pattern() { length=1 ; refcount = 0; }
	};

	class  PatternBlock : public Block {

		Pattern *pattern;

		public:

		Pattern *get_pattern();
		Tick get_length();
		void set_length(Tick p_length);
		bool is_shared();
		PatternBlock(Pattern*);
	};

	void process_track(AudioBuffer *p_in_buf,AudioBuffer *p_out_buf,int p_frames);

	void create_block(Tick p_pos,BlockCreationData *p_creation_data);
	Block *create_duplicate_block(Block *p_block);
	Block *create_link_block(Block *p_block);
	void erase_block(int p_which);
	BlockCreationBehavior get_block_creation_behavior();
	DataPool *pool;

	String get_type_name();
	bool can_resize_from_begining();
public:


	Track_Pattern(int p_channels,DataPool *p_pool);
	~Track_Pattern();

};

}

#endif
