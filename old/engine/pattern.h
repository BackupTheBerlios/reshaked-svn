//
// C++ Interface: pattern
//
// Description:
//
//
// Author: Juan Linietsky <coding@reduz.com.ar>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//


#ifndef PATTERN_H
#define PATTERN_H

#include "types/typedefs.h"
#include "types/value_stream.h"



namespace ReShaked {

struct PatternNote {

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

	PatternNote(unsigned char p_note=NO_NOTE,unsigned char p_volume=MAX_VOLUME, unsigned char p_portamento=NO_PORTAMENTO) {

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

struct PatternPosition {

	Tick tick;
	int column;

	inline bool operator==(const PatternPosition& eq) { return tick==eq.tick && column==eq.column; }

	inline bool operator<(const PatternPosition& eq) { if (tick>eq.tick) return false; else if (tick<eq.tick) return true; else return (column<eq.column); }

	inline bool operator>(const PatternPosition& eq) { if (tick<eq.tick) return false; else if (tick>eq.tick) return true; else return (column>eq.column); }

	inline bool operator<=(const PatternPosition& eq) {
		return (*this==eq || *this<eq);
	}
	inline bool operator>=(const PatternPosition& eq) {
		return (*this==eq || *this>eq);
	}

	PatternPosition(Tick p_tick=0,int p_column=0) { tick=p_tick; column=p_column; }

};

class Pattern : public ValueStream<PatternPosition,PatternNote> {


public:


};

};

#endif