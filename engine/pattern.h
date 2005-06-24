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
		
		NOTE_OFF=255
	};
			
	unsigned char note;
	unsigned char volume;

};
	
struct PatternPosition {
	
	Tick tick;
	int column;
	
	inline bool operator==(const PatternPosition& eq) { return tick==eq.tick && column==eq.column; }
	
	inline bool operator<(const PatternPosition& eq) { if (tick>eq.tick) return false; else if (tick<eq.tick) return true; else return (column<eq.column); }
	
	inline bool operator>(const PatternPosition& eq) { if (tick<eq.tick) return false; else if (tick>eq.tick) return true; else return (column>eq.column); }
	
};
	
class Pattern : public ValueStream<PatternPosition,Note> {
	
	
		
};

};

#endif