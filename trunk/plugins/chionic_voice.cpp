//
// C++ Implementation: chionic_voice
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "chionic_voice.h"
#include "dsp/formulas.h"
#include "dsp/resample.h"

#define C5_FREQ 261.6255653006

namespace ReShaked {


void ChionicVoice::event(Event p_event) {
	
	switch (p_event) {
		
		
		case NOTE_ON: {
			
			cycles=0;		
			done=false;
			first_mix=true;
			usecs=0;
			
			for (int i=0;i<ChionicParams::MAX_LAYERS;i++) {
				
				layer[i].env_vol.reset();
				layer[i].env_pan.reset();
				layer[i].env_depth.reset();
				layer[i].env_pitch.reset();
				layer[i].env_cutoff.reset();
				layer[i].env_reso.reset();
				layer[i].done=false;				
			}

						
		} break;
		case NOTE_OFF: {
			
			done=true;
			/* nothing */			
		} break;
		default: {}		
		
	}

}

inline bool ChionicVoice::process_layer_source(int p_layer,int p_source,float p_note,float p_freq_adjust,float p_gain_adjust) {
	
	ERR_FAIL_INDEX_V(p_source,params->global.sources.size(),true);
	ChionicParams::Source *source = &params->global.sources[p_source];
	
	float local_freq=note_to_freq(p_note);
	
	double local_cycles = cycles * p_freq_adjust;
	float freq=freq_for_mix*p_freq_adjust;
	
	

	
	double local_cycles_to_mix=cycles_to_mix*p_freq_adjust;
	
	if (source->sample) {
	
		Sample *sample=source->sample;
		double sample_cycle_size_in_frames=sample->get_base_freq() / C5_FREQ;
		
		double sample_pos=local_cycles*sample_cycle_size_in_frames;
		
		double sample_frames_to_mix=local_cycles_to_mix*sample_cycle_size_in_frames;
		
		/* SETTING UP MIX DATA */
		
		ResampleData rd;
		
		/* Fixed Point */
		
		rd.fp_increment=llrint((sample_frames_to_mix/(float)frames_to_mix)*(double)(1<<FRAC_BITS));
		
		switch(sample->get_loop_type()) {
			case Sample::LOOP_NONE: rd.loop_mode=RESAMPLE_LOOP_NONE; break;
			case Sample::LOOP_FORWARD: rd.loop_mode=RESAMPLE_LOOP_FORWARD; break;
			case Sample::LOOP_BIDI: rd.loop_mode=RESAMPLE_LOOP_BIDI; break;
		}
		
		rd.fp_loop_begin=(Sint64)sample->get_loop_begin()<<FRAC_BITS;
		rd.fp_loop_end=(Sint64)sample->get_loop_end()<<FRAC_BITS;
		rd.fp_size=(Sint64)sample->get_length()<<FRAC_BITS;
		
		// disable loop if begin > end 
		if (rd.fp_loop_begin>=rd.fp_loop_end)
			rd.loop_mode=RESAMPLE_LOOP_NONE;
		
		// Compute current pos, and fit inside loops
		// In this case, all this isnt very needed, as I could just keep
		// rd in all the next mixes. However, I need to learn to mix this
		// way to write choinic and other ideas later.
		
		rd.fp_pos=llrint(sample_pos*(double)(1<<FRAC_BITS));
		
		if (rd.loop_mode==RESAMPLE_LOOP_NONE) {
		
			if (rd.fp_pos >= rd.fp_size)
				return true; //past end of sample, dont bother and go back
			
		} if (rd.loop_mode==RESAMPLE_LOOP_FORWARD) {
			//wrap position to loop
			if (rd.fp_pos>=rd.fp_loop_begin) {
				rd.fp_pos-=rd.fp_loop_begin;
				rd.fp_pos%=rd.fp_loop_end-rd.fp_loop_begin;
				rd.fp_pos+=rd.fp_loop_begin;
			}
		} else if (rd.loop_mode==RESAMPLE_LOOP_BIDI) {
			//pingpong is a bit trickier
			if (rd.fp_pos>=rd.fp_loop_begin) {
				
				rd.fp_pos-=rd.fp_loop_begin;
				Sint64 loopsize=rd.fp_loop_end-rd.fp_loop_begin;
				rd.fp_pos%=loopsize*2;
				if (rd.fp_pos>loopsize) {
					rd.fp_pos=loopsize-(rd.fp_pos-loopsize);
					rd.fp_increment=-rd.fp_increment;
				}
				rd.fp_pos+=rd.fp_loop_begin;
			}
		} 
		
		/* Source Buffers */
		int src_chans=sample->get_channels();
		for (int i=0;i<src_chans;i++) {
				
			rd.src_buffers[i]=sample->get_buffer(i);
		}
		
		/* Dest Buffers */
		int dst_chans=get_buffer_count();
		
		for (int i=0;i<dst_chans;i++) {
			
			rd.dst_buffers[i]=get_buffer(i);
		}
		
		/* Volumes */
		
		for (int i=0;i<MAX_CHANS;i++) {
			
			rd.src_amps[i]=layer[p_layer].current_volumes[i]*p_gain_adjust;
			rd.incr_amps[i]=layer[p_layer].current_volumes_incr[i]*p_gain_adjust;
		}
		
		/* Loop */
		
		rd.write_pos=0;
		
		int written;
		if (src_chans==1 && dst_chans==1) { 
			// MONO -> MONO
			written=resample_with_loop<FRAC_BITS,1,1,INTERPOLATION_LINEAR>(rd,frames_to_mix);
		} else if (src_chans==1 && dst_chans==2) { 
			// MONO -> STEREO
			written=resample_with_loop<FRAC_BITS,1,2,INTERPOLATION_LINEAR>(rd,frames_to_mix);
		} else if (src_chans==2 && dst_chans==2) { 
			// STEREO -> STEREO
			written=resample_with_loop<FRAC_BITS,2,2,INTERPOLATION_LINEAR>(rd,frames_to_mix);
		} else if (src_chans==1 && dst_chans==4) { 
			// MONO -> QUAD
			written=resample_with_loop<FRAC_BITS,1,4,INTERPOLATION_LINEAR>(rd,frames_to_mix);
		} else if (src_chans==2 && dst_chans==4) { 
			// STEREO -> QUAD
			written=resample_with_loop<FRAC_BITS,2,4,INTERPOLATION_LINEAR>(rd,frames_to_mix);
		} else if (src_chans==4 && dst_chans==4) { 
			// QUAD -> QUAD
			written=resample_with_loop<FRAC_BITS,4,4,INTERPOLATION_LINEAR>(rd,frames_to_mix);
		} 
		
		return (written<frames_to_mix); //if if finished mixing in here, return true
	}	
	
	return true;
}

inline void ChionicVoice::set_volumes_from_pan(float p_pan,float p_depth,float *p_to) {
	
	int out_count=get_buffer_count();
	
	if (out_count==1)
		p_to[0]=1.0;
	
	else if (out_count==2) {
		
		float pan=p_pan;
		p_to[0]=(1.0-pan)*2.0;
		p_to[1]=pan*2.0;
	} else if (out_count==4) {
	
		float pan=p_pan;
		float depth=p_depth;
		
		float l=1.0-pan;
		float r=pan;
		
		p_to[0]=(1.0-depth)*l*4.0;
		p_to[1]=(1.0-depth)*r*4.0;
		
		p_to[2]=depth*l*4.0;
		p_to[3]=depth*r*4.0;
	} else {
		
		p_to[0]=1; // FRONT LEFT
		p_to[1]=1; // FRONT RIGHT
		p_to[2]=1; // REAR LEFT
		p_to[3]=1; //REAR RIGHT

	}
}

inline void ChionicVoice::process_layer(int p_layer) {
	
	
	ChionicParams::Layer &l=params->layer[p_layer];
	
	if (l.modulation_mode==ChionicParams::Layer::MODE_OFF) {
		layer[p_layer].done=true;
		return; //OFF
	}
	
	
	/* pre process ramped envelopes & LFOs, all but pitch */
	
	double env_process_time=(double)usecs_to_mix/1000.0; //envelopes are edited in msecs
	unsigned long lfo_time=(unsigned long)(usecs_to_mix/1000);
	
	layer[p_layer].env_vol.process(env_process_time);
	layer[p_layer].env_pan.process(env_process_time);
	layer[p_layer].env_depth.process(env_process_time);
	layer[p_layer].env_cutoff.process(env_process_time);
	layer[p_layer].env_reso.process(env_process_time);
	layer[p_layer].env_pitch.process(env_process_time);
	
	float note=get_current_note();
	
	
	/******** zNOTE & BLENDING PARAMETERS ********/
	
	float note_below=0;
	float note_above=0;
	float adjust_below=1;
	float adjust_above=1;
	int src_below=-1;
	int src_above=-1;
	float blend_coeff=0;
	bool done_below=false;
	bool done_above=false;

	
	{ /* determine src above and below */
	
		RegionMap &region_map=l.regions.map;
	
		int idx_total=region_map.get_stream_size();
		int idx_below=region_map.get_prev_index( lrintf(note) );
		int idx_above=(idx_below<(idx_total-1) && idx_below>=0)?(idx_below+1):-1;
		
		if (idx_below>=0) {//a note
			
			float idx_below_note=(float)region_map.get_index_pos(idx_below);
			
			note_below = (float)region_map.get_index_value(idx_below).base_note + ( note-idx_below_note );
	
			src_below=region_map.get_index_value(idx_below).source_idx;
			
			adjust_below= note_to_freq( region_map.get_index_value(idx_below).base_note ) / note_to_freq(idx_below_note);
			
			if (idx_above>=0 && l.regions.blending.enabled) { // a blended note
			
				float idx_above_note=(float)region_map.get_index_pos(idx_above);
				
				blend_coeff=(note-idx_below_note)/(idx_above_note-idx_below_note);
				blend_coeff=vel_sens_function(blend_coeff,l.regions.blending.curve.get());
				
				note_above = (float)region_map.get_index_value(idx_above).base_note + ( note-idx_above_note );
				
				src_above=region_map.get_index_value(idx_above).source_idx;
				
				adjust_above= note_to_freq( region_map.get_index_value(idx_above).base_note ) / note_to_freq(idx_above_note);
				
			} else {
				
				done_above=true;
			}
			
		} 
			//transform to the actual note, mapped in range
	}
		

		
	
	
	
	/* mix previous stuff if different */
	
	if (src_below<0)
		return; //can't do much
	

	float gain=params->global.volume.send.get();
	
	{ /* Compute Gain */
		gain*=l.params.volume.send.get();
		gain*=layer[p_layer].env_vol.get();
		gain*=powf( 2.0, l.params.volume.lfo.get_value( lfo_time )*4.0 );
	}
	
	
	float pan=params->global.pan.pos.get();
	float depth=params->global.pan.depth.get();

	{ /** PAN && DEPTH **/

		
		pan+=l.params.pan.pos.get();
		depth+=l.params.pan.depth.get();
		pan+=layer[p_layer].env_pan.get()/2.0;
		depth+=layer[p_layer].env_depth.get()/2.0;
		pan+=l.params.pan.lfo.get_value( lfo_time )/2.0;
		depth+=l.params.pan.lfo_depth.get_value( lfo_time )/2.0;
		
		if (pan<0)
			pan=0;
		if (pan>1)
			pan=1;
		if (depth<0)
			depth=0;
		if (depth>1)
			depth=1;
	}
	
	float new_volumes[4];
	set_volumes_from_pan( pan, depth, new_volumes );
	
	for (int i=0;i<MAX_CHANS;i++) {
		
		new_volumes[i]*=gain;
		if (first_mix) {
			
			layer[p_layer].current_volumes[i]=new_volumes[i];
			layer[p_layer].current_volumes_incr[i]=0;
		} else {
		
			layer[p_layer].current_volumes_incr[i]=(new_volumes[i]-layer[p_layer].current_volumes[i])/(float)frames_to_mix;
		}
	}
	
	
	if (process_layer_source( p_layer, src_below, note_below, adjust_below, 1.0 - blend_coeff ))
		done_below=true;
	
	if (src_above>=0 && process_layer_source( p_layer, src_above, note_above, adjust_above, blend_coeff ) )
		done_above=true;
	
	if (done_above && done_below)
		layer[p_layer].done=true;
	
	for (int i=0;i<MAX_CHANS;i++) {
		
		layer[p_layer].current_volumes[i]=new_volumes[i];
	}	
}

void ChionicVoice::process(int p_frames) {
	
	frames_to_mix=p_frames;
	freq_for_mix=440.0*powf(2.0,(get_current_note()-69.0)/12.0);	
	mix_rate=get_mix_rate();
	
	double time_to_mix=(float)p_frames/mix_rate;
	cycles_to_mix=time_to_mix*freq_for_mix;
	usecs_to_mix=lrint(time_to_mix*1000000.0);
	usecs+=usecs_to_mix;
	
	bool all_done=true;
	
	for (int i=0;i<ChionicParams::MAX_LAYERS;i++) {
		
		
		process_layer( i );
		if (!layer[i].done)
			all_done=false;
		
	}
	
	if (all_done)
		done=true;
	cycles+=cycles_to_mix;
	first_mix=false;	
	
}

float ChionicVoice::priority() {
	
	return done?0.0:1.0;	
}

ChionicVoice::ChionicVoice(ChionicParams *p_params,ChionicVoiceBuffers *p_buffers) {
	
	params=p_params;
	buffers=p_buffers;
	
	for (int i=0;i<ChionicParams::MAX_LAYERS;i++) {
		
		ChionicParams::Layer &l=params->layer[i];
		
		layer[i].env_vol.set_envelope( &l.params.volume.envelope );
		layer[i].env_pan.set_envelope( &l.params.pan.envelope );
		layer[i].env_depth.set_envelope( &l.params.pan.envelope_depth );
		layer[i].env_pitch.set_envelope( &l.params.pitch.envelope );
		layer[i].env_cutoff.set_envelope( &l.params.filter.envelope_cutoff );
		layer[i].env_reso.set_envelope( &l.params.filter.envelope_resonance );

		
	}
}

ChionicVoice::~ChionicVoice() {
	
	
}


}
