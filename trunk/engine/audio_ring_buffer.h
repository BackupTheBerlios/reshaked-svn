//
// C++ Interface: audio_ring_buffer
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDAUDIO_RING_BUFFER_H
#define RESHAKEDAUDIO_RING_BUFFER_H


#include "engine/audio_buffer.h"

namespace ReShaked {

/**
	@author Juan Linietsky <reshaked@gmail.com>
*/
class AudioRingBuffer{
	
	AudioBuffer buffer;
	
	int read_pos;
	int write_pos;
	
public:
	
	int size();
	int write_room();
	int read_pending();
	//read
	void copy_to(AudioBuffer *p_dst,int p_frames);
	void add_to(AudioBuffer *p_dst,int p_frames);
	//write
	void copy_from(AudioBuffer *p_src,int p_frames,bool p_advance=true);
	void add_from(AudioBuffer *p_src,int p_frames,bool p_advance=true);
	
	void advance_write_pos(int p_frames);
	
	void setup(int p_channels,int p_len);
	
	AudioRingBuffer();
	~AudioRingBuffer();

};

}

#endif
