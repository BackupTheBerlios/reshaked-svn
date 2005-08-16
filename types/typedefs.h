
#include "error_macros.h"
#include <iostream>
#include <string>
#ifndef TYPEDEFS_H
#define TYPEDEFS_H


typedef std::string String;

//this number helps for high accuracy on beat divisors
#define TICKS_PER_BEAT 3072

typedef long long Tick;

typedef signed char Note;

enum NoteConstants {
	NOTE_EMPTY=-1,
	NOTE_OFF=-2,
	NOTE_MAX=128,
	NOTE_MIN=0,
};

#define SWITCH(m_v) { typeof(m_v) __m=m_v; if (0) {
#define CASE(m_v) } else if ((m_v)==__m) {
#define COND(m_c) } else if (m_c) {
#define DEFAULT	  } else {		
#define END_SWITCH }}



#endif
