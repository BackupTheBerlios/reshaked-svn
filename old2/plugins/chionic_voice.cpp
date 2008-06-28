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
				layer[i].env_vol.set_sustain(true);
				layer[i].env_pan.set_sustain(true);
				layer[i].env_depth.set_sustain(true);
				layer[i].env_pitch.set_sustain(true);
				layer[i].env_cutoff.set_sustain(true);
				layer[i].env_reso.set_sustain(true);
				
				layer[i].done=false;		
						
				layer[i].filter_mode=params->layer[i].params.filter.type.get_current();
				
				for (int j=0;j<MAX_CHANS;j++) {
					
					layer[i].filter[j].ha1=0;
					layer[i].filter[j].ha2=0;
					layer[i].filter[j].hb1=0;
					layer[i].filter[j].hb2=0;
					layer[i].filter[j].old=Filter::Coeffs();
				}
			}

			
						
		} break;
		case NOTE_OFF: {
			
			for (int i=0;i<ChionicParams::MAX_LAYERS;i++) {
				
				layer[i].env_vol.set_sustain(false);
				layer[i].env_pan.set_sustain(false);
				layer[i].env_depth.set_sustain(false);
				layer[i].env_pitch.set_sustain(false);
				layer[i].env_cutoff.set_sustain(false);
				layer[i].env_reso.set_sustain(false);
			}

			
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
		AudioBuffer &ab=buffers->voice[p_layer].buffer;

		for (int i=0;i<dst_chans;i++) {
			
			rd.dst_buffers[i]=ab.get_buffer(i);
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
	
	layer[p_layer].mixed=false;
	
	ChionicParams::Layer &l=params->layer[p_layer];
	
	if (l.modulation_mode==ChionicParams::Layer::MODE_OFF) {
		layer[p_layer].done=true;
		
		return; //OFF
	}
	
	buffers->voice[p_layer].buffer.clear(frames_to_mix);
	
	/* pre process ramped envelopes & LFOs, all but pitch */
	
	double env_process_time=(double)usecs_to_mix/1000.0; //envelopes are edited in msecs
	unsigned long lfo_time=(unsigned long)(usecs/1000);
	
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
	

	float gain=get_total_amplitude();
	
	{ /** VOLUME & GAIN **/
		
		gain*=params->global.volume.send.get();
		gain*=l.params.volume.send.get();
		if (layer[p_layer].env_vol.is_active())
			gain*=layer[p_layer].env_vol.get();
		gain*=powf( 2.0, l.params.volume.lfo.get_value( lfo_time )*4.0 );
		gain*=get_volume_scale(note,l.params.volume.pitch_scale.get());
		
		/* Range */
		
		float velocity=(float)get_velocity()/127.0;
		float expression=get_expression();
		
		float range_src=(l.params.volume.velocity_range_blend_expr.get()>0.1)?(velocity*expression):velocity;
		
		float range_modifier=1.0;
		
		float range_beg=l.params.volume.velocity_range_begin.get();
		float range_end=l.params.volume.velocity_range_end.get();
		float range_blend=l.params.volume.velocity_range_blend.get();
		
		if (range_beg>range_end)
			SWAP(range_beg,range_end);
		
		if (range_src<range_beg) {
			
			float blend_from=range_beg-range_blend;
			
			if (range_src<=blend_from) {
				
				range_modifier=0;
			} else {
					
				range_modifier=(range_src-blend_from)/(range_beg-blend_from);
			}
			
		} else if (range_src>range_end) {
			
			
			float blend_to=range_end+range_blend;
			
			if (range_src>=blend_to) {
				
				range_modifier=0;
			} else {
					
				range_modifier=1.0-(range_src-range_end)/(blend_to-range_end);
			}
			
		}
		
		gain*=range_modifier;
		
		//printf("rmod: %f, scale %f, lfo %f, send %f, global send, %f, total amp %f\n",range_modifier,get_volume_scale(note,l.params.volume.pitch_scale.get()),powf( 2.0, l.params.volume.lfo.get_value( lfo_time )*4.0 ),l.params.volume.send.get(),params->global.volume.send.get(),get_total_amplitude());
	}
	
	
	float pan=params->global.pan.pos.get();
	float depth=params->global.pan.depth.get();

	{ /** PAN && DEPTH **/

		
		pan+=l.params.pan.pos.get()-0.5;
		depth+=l.params.pan.depth.get()-0.5;
		if (layer[p_layer].env_pan.is_active())
			pan+=layer[p_layer].env_pan.get()/2.0;
		if (layer[p_layer].env_depth.is_active())
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
	
	
	/* modulate from sources (ADD & RING)*/
	
	if (l.modulation_mode==ChionicParams::Layer::MODE_ON_ADD || l.modulation_mode==ChionicParams::Layer::MODE_RING) {
		
		const std::vector<int> &sources= params->global.modulation_source[p_layer].read_from;
		
		for (int i=0;i<sources.size();i++) {
			
			int src_idx=sources[i];
			ERR_CONTINUE(src_idx<0);
			
			AudioBuffer *src_ab=(src_idx>=ChionicParams::MAX_LAYERS)?buffers->input:&buffers->voice[src_idx].buffer;
			
			AudioBuffer *dst_ab=&buffers->voice[p_layer].buffer;
			
			ERR_CONTINUE(dst_ab==src_ab);
			ERR_CONTINUE(dst_ab->get_channels()!=src_ab->get_channels());
			
						
			if (l.modulation_mode==ChionicParams::Layer::MODE_ON_ADD) {
				
				dst_ab->add_from( src_ab, frames_to_mix ); 
			} else {
				
				dst_ab->mult_from( src_ab, frames_to_mix ); 
				
			}
		}
	}
	
	/* FILTER */	
	
	if (layer[p_layer].filter_mode>0) {
		
		/* SETUP CUTOFF */		
		float cutoff=l.params.filter.cutoff.get();
		
		
		float env_lfo_cutoff_semitones=l.params.filter.lfo_cutoff.get_value( lfo_time );
				
		if (layer[p_layer].env_cutoff.is_active())
			env_lfo_cutoff_semitones+=layer[p_layer].env_cutoff.get();
		
		//freq tracking from the middle C (60)
		env_lfo_cutoff_semitones+=(note-60)*l.params.filter.freq_tracking.get();
		//velocity sens (translate from octaves to semitones)
		env_lfo_cutoff_semitones+=12.0*l.params.filter.cutoff_velsens.get()*(get_velocity()/127.0);
		
		cutoff*=pow(2,env_lfo_cutoff_semitones/12.0);
		
		if (cutoff<l.params.filter.cutoff.get_min())
			cutoff=l.params.filter.cutoff.get_min();
		
		if (cutoff>l.params.filter.cutoff.get_max())
			cutoff=l.params.filter.cutoff.get_max();
		
		float resonance=l.params.filter.resonance.get();
		
		if (layer[p_layer].env_reso.is_active())
			resonance+=layer[p_layer].env_reso.get();
		
		resonance+=l.params.filter.lfo_resonance.get_value( lfo_time );
		resonance+=2.0*l.params.filter.resonance_velsens.get()*(get_velocity()/127.0);
		
		if (resonance<=0)
			resonance=0.0001;
		if (resonance>3)
			resonance=3;
		
		AudioBuffer *ab=&buffers->voice[p_layer].buffer;
		for (int i=0;i<ab->get_channels();i++) {
				
			Filter f;
			
			switch(layer[p_layer].filter_mode) {
				
				case 1: f.set_mode( Filter::LOWPASS ); break;
				case 2: f.set_mode( Filter::BANDPASS ); break;
				case 3: f.set_mode( Filter::HIGHPASS ); break;
				case 4: f.set_mode( Filter::NOTCH ); break;	
			}
			
			f.set_cutoff( cutoff );
			f.set_resonance( resonance );
			f.set_sampling_rate( mix_rate );
			
			Filter::Coeffs c,c_inc;
			
			f.prepare_coefficients( &c );
			
			Layer::FilterH &fh=layer[p_layer].filter[i];
			
			float *buff=ab->get_buffer( i );
			
			if (c==fh.old) { //no change, run normally
				
				for (int j=0;j<frames_to_mix;j++) {
				
					float pre=buff[j];
					buff[j] = (buff[j] * c.b0 + fh.hb1 * c.b1  + fh.hb2 * c.b2 + fh.ha1 * c.a1 + fh.ha2 * c.a2); 	
					fh.ha2=fh.ha1;                                                             		
					fh.hb2=fh.hb1;                                                             		
					fh.hb1=pre;
					fh.ha1=buff[j];
					
				}
			} else { //changed, interpolate filter to avoid clickery
				
				c_inc.a1=(c.a1-fh.old.a1)/(float)frames_to_mix;
				c_inc.a2=(c.a2-fh.old.a2)/(float)frames_to_mix;
				c_inc.b1=(c.b1-fh.old.b1)/(float)frames_to_mix;
				c_inc.b2=(c.b2-fh.old.b2)/(float)frames_to_mix;
				c_inc.b0=(c.b0-fh.old.b0)/(float)frames_to_mix;
				
				c=fh.old;				
				
				for (int j=0;j<frames_to_mix;j++) {
				
					float pre=buff[j];
					buff[j] = (buff[j] * c.b0 + fh.hb1 * c.b1  + fh.hb2 * c.b2 + fh.ha1 * c.a1 + fh.ha2 * c.a2); 	
					fh.ha2=fh.ha1;                                                             		
					fh.hb2=fh.hb1;                                                             		
					fh.hb1=pre;
					fh.ha1=buff[j];
					
					c.a1+=c_inc.a1;
					c.a2+=c_inc.a2;
					c.b0+=c_inc.b0;
					c.b1+=c_inc.b1;
					c.b2+=c_inc.b2;
					
					
				}
				
			}
			
			fh.old=c;
			
		}
		
	}
	
	
	if (done_above && done_below)
		layer[p_layer].done=true;
	
	//check note finished because of envelope finished
	if (layer[p_layer].env_vol.is_active() && layer[p_layer].env_vol.has_finished() && FLOATS_EQ(layer[p_layer].env_vol.get(),0))
		layer[p_layer].done=true;
		
	for (int i=0;i<MAX_CHANS;i++) {
		
		layer[p_layer].current_volumes[i]=new_volumes[i];
	}	
	
	layer[p_layer].mixed=true;
}

void ChionicVoice::process(int p_frames) {
	
	frames_to_mix=p_frames;
	freq_for_mix=440.0*powf(2.0,(get_current_note()-69.0)/12.0);	
	mix_rate=get_mix_rate();
	
	double time_to_mix=(float)p_frames/mix_rate;
	cycles_to_mix=time_to_mix*freq_for_mix;
	usecs_to_mix=lrint(time_to_mix*1000000.0);
	usecs+=usecs_to_mix;
	
	/* process layers */
	for (int i=0;i<ChionicParams::MAX_LAYERS;i++) {
		
		
		process_layer( i );
		
	}
	
	/* copy outputs to out */
	bool all_done=true;
	
	const std::vector<int> &sources= params->global.modulation_source[ChionicParams::MAX_LAYERS].read_from; //out
		
	for (int i=0;i<sources.size();i++) {
	
		int src_idx=sources[i];
		
		ERR_CONTINUE(src_idx<0 || src_idx>ChionicParams::MAX_LAYERS);
		
		if (src_idx==ChionicParams::MAX_LAYERS)
			continue; //pointless to do here
		
		if (!layer[src_idx].mixed)
			continue;
		
		AudioBuffer *src_ab=(src_idx>=ChionicParams::MAX_LAYERS)?buffers->input:&buffers->voice[src_idx].buffer;
		ERR_CONTINUE(get_buffer_count()!=src_ab->get_channels());
		printf("must mix from source %i\n",src_idx);
		for (int j=0;j<src_ab->get_channels();j++) {
			
			float *src=src_ab->get_buffer(j);
			float *dst=get_buffer(j);
			
			for (int k=0;k<p_frames;k++) {
				
				dst[k]+=src[k];
			}
			
		}
		
		if (!layer[src_idx].done)
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
