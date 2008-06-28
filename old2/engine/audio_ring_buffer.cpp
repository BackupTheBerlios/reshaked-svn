//
// C++ Implementation: audio_ring_buffer
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "audio_ring_buffer.h"

namespace ReShaked {

	
int AudioRingBuffer::size() {
	
	return buffer.get_size();
}

int AudioRingBuffer::read_pending() {
	
	return size()-write_room();
}

int AudioRingBuffer::write_room() {
	
	if (write_pos==read_pos)
		return buffer.get_size();
	else if (write_pos<read_pos)
		return read_pos-write_pos;
	else
		return (buffer.get_size()+read_pos)-write_pos;
}

void AudioRingBuffer::copy_to(AudioBuffer *p_dst,int p_frames) {
	
	ERR_FAIL_COND(p_frames<0);
	ERR_FAIL_COND(p_dst->get_channels()!=buffer.get_channels());
	ERR_FAIL_COND(p_frames>p_dst->get_size());
	ERR_FAIL_COND(p_frames>read_pending());
	
	
	for (int i=0;i<buffer.get_channels();i++) {
		
		float *dst=p_dst->get_buffer(i);
		float *src=buffer.get_buffer(i);
		
		int current_read_pos=read_pos;
		int current_size=buffer.get_size();
		for (int j=0;j<p_frames;j++) {
			
			dst[j]=src[current_read_pos];
			src[current_read_pos]=0; //clear what we read so it can be written to
			if ((++current_read_pos)>=current_size)
				current_read_pos=0;
		}
		
	}	
	
	
	

	read_pos=(read_pos+p_frames)%buffer.get_size();
}


void AudioRingBuffer::add_to(AudioBuffer *p_dst,int p_frames) {
	
	ERR_FAIL_COND(p_frames<0);
	ERR_FAIL_COND(p_dst->get_channels()!=buffer.get_channels());
	ERR_FAIL_COND(p_frames>p_dst->get_size());
	ERR_FAIL_COND(p_frames>read_pending());
	
	
	for (int i=0;i<buffer.get_channels();i++) {
		
		float *dst=p_dst->get_buffer(i);
		float *src=buffer.get_buffer(i);
		
		int current_read_pos=read_pos;
		int current_size=buffer.get_size();
		for (int j=0;j<p_frames;j++) {
			
			dst[j]+=src[current_read_pos];
			src[current_read_pos]=0; //clear what we read so it can be written to
			if ((++current_read_pos)>=current_size)
				current_read_pos=0;
		}
		
	}	
	
	
	
	read_pos=(read_pos+p_frames)%buffer.get_size();
}



void AudioRingBuffer::copy_from(AudioBuffer *p_src,int p_frames,bool p_advance_write_pos) {
	
	ERR_FAIL_COND(p_frames<0);
	ERR_FAIL_COND(p_frames>write_room());
	ERR_FAIL_COND(p_src->get_channels()!=buffer.get_channels());
	ERR_FAIL_COND(p_frames>p_src->get_size());
	
	
	for (int i=0;i<buffer.get_channels();i++) {
		
		float *src=p_src->get_buffer(i);
		float *dst=buffer.get_buffer(i);
		
		int current_write_pos=write_pos;
		int current_size=buffer.get_size();
		for (int j=0;j<p_frames;j++) {
			
			dst[current_write_pos]=src[j];
			if ((++current_write_pos)>=current_size)
				current_write_pos=0;
		}
		
	}	
	
	if (p_advance_write_pos)
		advance_write_pos(p_frames);
	
}

void AudioRingBuffer::add_from(AudioBuffer *p_src,int p_frames,bool p_advance_write_pos) {
	
	ERR_FAIL_COND(p_frames<0);
	ERR_FAIL_COND(p_frames>write_room());
	ERR_FAIL_COND(p_src->get_channels()!=buffer.get_channels());
	ERR_FAIL_COND(p_frames>p_src->get_size());
	
	
	for (int i=0;i<buffer.get_channels();i++) {
		
		float *src=p_src->get_buffer(i);
		float *dst=buffer.get_buffer(i);
		
		int current_write_pos=write_pos;
		int current_size=buffer.get_size();
		for (int j=0;j<p_frames;j++) {
			
			dst[current_write_pos]+=src[j];
			if ((++current_write_pos)>=current_size)
				current_write_pos=0;
		}
		
	}
	
	if (p_advance_write_pos)
		advance_write_pos(p_frames);
}

void AudioRingBuffer::advance_write_pos(int p_frames) {
	
	ERR_FAIL_COND(p_frames<0);
	ERR_FAIL_COND(p_frames>write_room());
	
	write_pos=(write_pos+p_frames)%buffer.get_size();
	
}

void AudioRingBuffer::setup(int p_channels,int p_len) {
	
	buffer.setup( p_channels, p_len );
	read_pos=write_pos=0;
}

AudioRingBuffer::AudioRingBuffer() {
	
	
}


AudioRingBuffer::~AudioRingBuffer()
{
}


}
