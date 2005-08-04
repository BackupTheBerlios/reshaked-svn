
#include "error_macros.h"
#include <iostream>
#include <string>
#ifndef TYPEDEFS_H
#define TYPEDEFS_H


typedef std::string String;

#define TICKS_PER_BEAT 128
typedef long long Tick;

typedef signed char Note;

enum NoteConstants {
	NOTE_EMPTY=-1,
	NOTE_OFF=-2,
	NOTE_MAX=128,
	NOTE_MIN=0,
};




#endif
