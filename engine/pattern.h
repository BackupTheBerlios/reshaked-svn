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



namespace ReShaked {

struct PatternNote {

	enum {

		NOTE_OFF=255,
		MAX_VOLUME=99,
		PREVIOUS_PORTAMENTO=0,
		NO_PORTAMENTO=100,
		NO_AUTO_OFF=0
	};

	unsigned char note;
	unsigned char volume;
	unsigned char portamento;
	Tick auto_off;

	PatternNote(unsigned char p_note,unsigned char p_volume=MAX_VOLUME, unsigned chr p_portamento=NO_PORTAMENTO) {

		note = p_note;
		volume = p_volume;
		portamento = p_portamento;
		auto_off=NO_AUTO_OFF;
	}

};

struct PatternPosition {

	Tick tick;
	int column;

	inline bool operator==(const PatternPosition& eq) { return tick==eq.tick && column==eq.column; }

	inline bool operator<(const PatternPosition& eq) { if (tick>eq.tick) return false; else if (tick<eq.tick) return true; else return (column<eq.column); }

	inline bool operator>(const PatternPosition& eq) { if (tick<eq.tick) return false; else if (tick>eq.tick) return true; else return (column>eq.column); }

};

class Pattern : public ValueStream<PatternPosition,Note> {


public:



};

};

#endif