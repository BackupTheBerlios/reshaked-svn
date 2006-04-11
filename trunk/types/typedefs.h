
#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include "error_macros.h"
#include <iostream>
#include "rstring.h"
#include "version.h"

#define MAX_DIVISORS 12
static const char divisors[MAX_DIVISORS]={1,2,3,4,6,8,12,16,24,32,48,64};
//Minimum Common Denominator for the divisors
//This is done to avoid precision issues and simplify the editing code
#define TICKS_PER_BEAT 192 

#define SNAP_TICK_TO_BEAT(m_tick) ( (m_tick) - ((m_tick)%TICKS_PER_BEAT) )



#define DIGIT_1(m_val) ( (m_val)%10 )
#define DIGIT_2(m_val) ( (m_val/10)%10 )
#define DIGIT_3(m_val) ( (m_val/100)%10 )
#define DIGIT_4(m_val) ( (m_val/1000)%10 )

#define POW2( m_p ) ((m_p)*(m_p))

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

enum TrackType {
	TRACK_TYPE_PATTERN,
	TRACK_TYPE_AUDIO,
	TRACK_TYPE_BUS,
};

#define CMP2_MIN(m_s1,m_s2,m_d1,m_d2) ((m_s1>m_d1)?false:((m_s1<m_d1)?true:(m_s2<m_d2)))		
#ifdef foreach //fuck QT4
#undef foreach
#endif

#define foreach(m_itname,m_container) \
        for( typeof(m_container.begin()) m_itname=m_container.begin() ; \
        m_itname!=m_container.end() ; \
        m_itname++ )
        
        

template<class T>
inline void SWAP(T&a, T&b) {
	
	T c=b;
	b=a;
	a=c;
}

static inline int rand_from_seed(unsigned int *seed) {
	long k;
	long s = (long)(*seed);
	if (s == 0)
		s = 0x12345987;
	k = s / 127773;
	s = 16807 * (s - k * 127773) - 2836 * k;
	if (s < 0)
		s += 2147483647;
	(*seed) = (unsigned int)s;
	return (int)(s & RAND_MAX);
}

typedef long long Sint64;

#ifdef WIN32_ENABLED

#define rand_r rand_from_seed


#endif

#endif
