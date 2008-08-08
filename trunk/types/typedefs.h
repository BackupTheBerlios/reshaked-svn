
namespace ReShaked {
	
	
	
};
#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include "error_macros.h"
#include <iostream>
#include "rstring.h"


#define MAX_DIVISORS 12
static const char beat_divisors[MAX_DIVISORS]={1,2,3,4,6,8,12,16,24,32,48,64};
//Minimum Common Denominator for the divisors
//This is done to avoid precision issues and simplify the editing code
#define TICKS_PER_BEAT 192 

typedef float sample_t;

#define SNAP_TICK_TO_BEAT(m_tick) ( (m_tick) - ((m_tick)%TICKS_PER_BEAT) )



#define DIGIT_1(m_val) ( (m_val)%10 )
#define DIGIT_2(m_val) ( (m_val/10)%10 )
#define DIGIT_3(m_val) ( (m_val/100)%10 )
#define DIGIT_4(m_val) ( (m_val/1000)%10 )

#define POW2( m_p ) ((m_p)*(m_p))

//typedef signed char Note;

 enum NoteConstants {
	NOTE_EMPTY=-1,
	NOTE_OFF=-2,
	NOTE_MAX=128,
	NOTE_MIN=0,
};
//do, so break works
#define SWITCH(m_v) do { typeof(m_v) __m=m_v; if (0) {
#define CASE(m_v) } else if ((m_v)==__m) {
#define COND(m_c) } else if (m_c) {
#define DEFAULT	  } else {
#define END_SWITCH }} while(0);

enum TrackType {
	TRACK_TYPE_PATTERN,
	TRACK_TYPE_AUDIO,
	TRACK_TYPE_BUS,
};

#define CMP2_MIN(m_s1,m_s2,m_d1,m_d2) ((m_s1>m_d1)?false:((m_s1<m_d1)?true:(m_s2<m_d2)))		
        
        
#define EQ_THRESH 0.00001
#define FLOATS_EQ(m_f1,m_f2) ( fabsf((m_f1)-(m_f2))<EQ_THRESH )
#define FLOATS_NOT_EQ(m_f1,m_f2) ( fabsf((m_f1)-(m_f2))>=EQ_THRESH )

#if defined(__GNUC__) && (__GNUC__ >= 4 )
#    define _FORCE_INLINE_ __attribute__((always_inline)) inline
typedef	long long int64;
typedef unsigned long long uint64;
#elif defined(_MSVC)
#	define _FORCE_INLINE_ __forceinline
typedef __int64 int64 ;
typedef __uint64 uint64;
#else
#    define _FORCE_INLINE_ inline
typedef	long long int64;
typedef unsigned long long uint64;
#endif

typedef int64 Tick;


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
typedef unsigned long long Uint64;

/* ANSI compliant version of undenormalize */

typedef union {
	volatile float f;
	volatile unsigned int i;
} ls_pcast32;

static inline float undenormalise(volatile float f)
{
	volatile ls_pcast32 v;

	v.f = f;

        // original: return (v.i & 0x7f800000) == 0 ? 0.0f : f;
        // version from Tim Blechmann:
	return (v.i & 0x7f800000) < 0x08000000 ? 0.0f : f;
}

static inline const char * get_chan_name(int p_channels) {

 	static const char* chan_names[9]={"None","Mono","Stereo","3","Quad","5","6","7","8"};
 	if (p_channels<0)
 		p_channels=0;
 	if (p_channels>8)
 		p_channels=8;
 	return chan_names[p_channels];

}

#ifdef WIN32_ENABLED

#define rand_r rand_from_seed


#endif

#endif
