//
// C++ Interface: pattern_track
//
// Description: 
//
//
// Author: Juan Linietsky <coding@reduz.com.ar>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//


#ifndef PATTERN_TRACK_H
#define PATTERN_TRACK_H

#include "engine/pattern.h"
#include "engine/track.h"

namespace ReShaked {


class PatternTrack : public Track {
	
	
    Pattern pattern;

        void process_track(AudioBuffer *p_in_buf,AudioBuffer *p_out_buf,int p_frames);
public:
	/* pone a brilar la nota cuando estas arriba */
	
	Pattern& get_pattern();
	PatternNote get_note(Tick p_tick,int p_column);
	void set_note(Tick p_tick,int p_column,const PatternNote &p_note);


        PatternTrack(int p_channels);
	
	
};


};


#endif
