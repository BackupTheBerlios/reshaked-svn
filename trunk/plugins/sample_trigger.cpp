//
// C++ Implementation: sample_trigger
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sample_trigger.h"


#include <math.h>
#include "dsp/resample.h"
#include "dsp/formulas.h"
#include "dsp/tree_helpers_dsp.h"
#include "icon_trigger.xpm"



namespace ReShaked {


static SoundPlugin* create_sample_trigger(const SoundPluginInfo *p_info,int p_channels) {
	
	return new SampleTrigger(p_info,p_channels);
}

const SoundPluginInfo *SampleTrigger::create_info() {
	
	static SoundPluginInfo info;
	
	info.caption="Sample Trigger";
	info.description="Trigger Fx/Drums";
	info.long_description="Sample Trigger is configured to respond to a specific note. When this note is played, the sample loaded will start. This is mainly useful for drums or sound FX.";
	info.unique_ID="INTERNAL_sample_trigger"; 
	info.category="Synthesizers"; 
	info.can_custom_channels=true;
	info.custom_channels.push_back(1);
	info.custom_channels.push_back(2);
	info.custom_channels.push_back(4);
	info.has_internal_UI=false; 
	info.is_synth=true;
	info.xpm_preview=(const char**)icon_trigger_xpm;
	info.creation_func=&create_sample_trigger;
	info.version=1;
	
	return &info;
}




void SampleTrigger::set_volumes_from_pan(float *p_to) {
	
	int out_count=get_channels_created();
	
	if (out_count==1)
		p_to[0]=1.0;
	
	else if (out_count==2) {
		
		float pan=param.pan.get();
		p_to[0]=(1.0-pan)*2.0;
		p_to[1]=pan*2.0;
	} else if (out_count==4) {
	
		float pan=param.pan.get();
		float depth=param.depth.get();
		
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

/*	
	
void SampleTrigger::Voice::process(int p_frames) {
		
	if (done)
		return;

	if (adsr.is_finished()) {
		done=true;
		return;
	}
	// Compute cycles and all that stuff 
	
	float freq=440.0*powf(2.0,(get_current_note()-69.0)/12.0);	
	float mix_rate_cache=get_mix_rate();
	
	double time_to_mix=(float)p_frames/mix_rate_cache;
	double cycles_to_mix=time_to_mix*freq;
	
	double sample_cycle_size_in_frames=sample.get_base_freq() / C5_FREQ;
	
	double sample_pos=cycles*sample_cycle_size_in_frames;
	
	double sample_frames_to_mix=cycles_to_mix*sample_cycle_size_in_frames;
	
	adsr.process( time_to_mix );
	
	float current_volume=adsr.get_val()*get_total_amplitude();
	
	// SETTING UP MIX DATA 
	
	ResampleData rd;
	
	// Fixed Point 
	
	rd.fp_increment=llrint((sample_frames_to_mix/(float)p_frames)*(double)(1<<FRAC_BITS));
	
	switch(sample.get_loop_type()) {
		case Sample::LOOP_NONE: rd.loop_mode=RESAMPLE_LOOP_NONE; break;
		case Sample::LOOP_FORWARD: rd.loop_mode=RESAMPLE_LOOP_FORWARD; break;
		case Sample::LOOP_BIDI: rd.loop_mode=RESAMPLE_LOOP_BIDI; break;
	}
	
	rd.fp_loop_begin=(Sint64)sample.get_loop_begin()<<FRAC_BITS;
	rd.fp_loop_end=(Sint64)sample.get_loop_end()<<FRAC_BITS;
	rd.fp_size=(Sint64)sample.get_length()<<FRAC_BITS;
	
	// disable loop if begin > end 
	if (rd.fp_loop_begin>=rd.fp_loop_end)
		rd.loop_mode=RESAMPLE_LOOP_NONE;
	
	// Compute current pos, and fit inside loops
	// In this case, all this isnt very needed, as I could just keep
	// rd in all the next mixes. However, I need to learn to mix this
	// way to write choinic and other ideas later.
	
	rd.fp_pos=llrint(sample_pos*(double)(1<<FRAC_BITS));
	
	if (rd.loop_mode==RESAMPLE_LOOP_FORWARD) {
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
	
	// Source Buffers 
	int src_chans=sample.get_channels();
	for (int i=0;i<src_chans;i++) {
			
		rd.src_buffers[i]=sample.get_buffer(i);
	}
	
	// Dest Buffers 
	int dst_chans=get_buffer_count();
	
	for (int i=0;i<dst_chans;i++) {
		
		rd.dst_buffers[i]=get_buffer(i);
	}
	
	// Volumes 
	
	float current_vol[MAX_CHANS];
	
	set_volumes_from_pan( current_vol );
	
	for (int i=0;i<dst_chans;i++)
		current_vol[i]*=current_volume;
	
	if (first_mix) {
		
		for (int i=0;i<dst_chans;i++)
			prev_vol[i]=current_vol[i];
		
		first_mix=false;
	}
	
	for (int i=0;i<MAX_CHANS;i++) {
		
		rd.src_amps[i]=prev_vol[i];
		if (prev_vol[i]==current_vol[i])
			rd.incr_amps[i]=0;
		else
			rd.incr_amps[i]=(current_vol[i]-prev_vol[i])/(float)p_frames;
	}
	
	// Loop 
	
	rd.write_pos=0;
	
	int written;
	if (src_chans==1 && dst_chans==1) { 
		// MONO -> MONO
		written=resample_with_loop<FRAC_BITS,1,1,INTERPOLATION_LINEAR>(rd,p_frames);
	} else if (src_chans==1 && dst_chans==2) { 
		// MONO -> STEREO
		written=resample_with_loop<FRAC_BITS,1,2,INTERPOLATION_LINEAR>(rd,p_frames);
	} else if (src_chans==2 && dst_chans==2) { 
		// STEREO -> STEREO
		written=resample_with_loop<FRAC_BITS,2,2,INTERPOLATION_LINEAR>(rd,p_frames);
	} else if (src_chans==1 && dst_chans==4) { 
		// MONO -> QUAD
		written=resample_with_loop<FRAC_BITS,1,4,INTERPOLATION_LINEAR>(rd,p_frames);
	} else if (src_chans==2 && dst_chans==4) { 
		// STEREO -> QUAD
		written=resample_with_loop<FRAC_BITS,2,4,INTERPOLATION_LINEAR>(rd,p_frames);
	} else if (src_chans==4 && dst_chans==4) { 
		// QUAD -> QUAD
		written=resample_with_loop<FRAC_BITS,4,4,INTERPOLATION_LINEAR>(rd,p_frames);
	} 
	
	if (written<p_frames)
		done=true;
	cycles+=cycles_to_mix;
	
	// move previous vols to current! 
	for (int i=0;i<MAX_CHANS;i++)
		prev_vol[i]=current_vol[i];
}
	
*/


int SampleTrigger::get_input_plug_count() {
	
	return 0;
}
int SampleTrigger::get_output_plug_count() {
	
	return 1;
	
}
AudioPlug *SampleTrigger::get_input_plug(int p_index) {
	
	return NULL;	
}

AudioPlug *SampleTrigger::get_output_plug(int p_index) {
	
	return output_plug;	
}

int SampleTrigger::get_port_count() {
	
	return property_list.size();

}

Property& SampleTrigger::get_port(int p_index) {
	
	static LocalProperty error_prop;
	
	ERR_FAIL_INDEX_V(p_index,property_list.size(),error_prop);
	
	
	return *property_list[p_index];
	
}
SoundPlugin::PortType SampleTrigger::get_port_type(int p_index) {
	
	return TYPE_WRITE;
	
}

/* Setting up */
void SampleTrigger::set_mixing_rate(float p_mixing_rate) { //sort of useless
	
	mix_rate=p_mixing_rate;

}


void SampleTrigger::process_to_buffer(int p_frames,AudioBuffer *p_buffer) {
	
	/* Setup Buffers */
	
	int dst_chans=get_channels_created();
	int src_chans=sample.get_channels();
	
	for (int i=0;i<dst_chans;i++) {
		
		rd.dst_buffers[i]=output_plug->get_buffer()->get_buffer(i);
	}
	
	/* Setup Volumes/Envelope */
	
	float current_volume=db_to_energy(param.gain.get())*velocity;
	
	if (is_off) {
		
		if (release_env==0.0) {
			sample_active=false;
			return;
		}
		
		float release_time=pow(param.release.get()*2,3);
		float amount_to_release=(float)p_frames/((float)mix_rate*release_time);
		release_env-=amount_to_release;
		if (release_env<0.0)
			release_env=0.0;
		
		current_volume*=release_env;
		
	}
	
	float current_vol[MAX_CHANS];
	
	set_volumes_from_pan( current_vol );
	
	for (int i=0;i<dst_chans;i++)
		current_vol[i]*=current_volume;
	
	if (first_mix) {
		
		for (int i=0;i<dst_chans;i++)
			prev_vol[i]=current_vol[i];
		
		first_mix=false;
	} 
	
	for (int i=0;i<MAX_CHANS;i++) {
		
		rd.src_amps[i]=prev_vol[i];
		rd.incr_amps[i]=(current_vol[i]-prev_vol[i])/(float)p_frames;
	}
	
	/* Setup sample/write buffer parameters */
	
	rd.write_pos=0; //start to zero
	rd.fp_loop_begin=(Sint64)sample.get_loop_begin()<<FRAC_BITS;
	rd.fp_loop_end=(Sint64)sample.get_loop_end()<<FRAC_BITS;
	rd.fp_size=(Sint64)sample.get_length()<<FRAC_BITS;
	
	for (int sc=0;sc<sample.get_channels();sc++) {
			
		rd.src_buffers[sc]=sample.get_buffer(sc);
	}
	
	rd.fp_increment=llrint((double)(sample.get_base_freq()/mix_rate)*(double)(1<<FRAC_BITS));
	switch(sample.get_loop_type()) {
		case Sample::LOOP_NONE: rd.loop_mode=RESAMPLE_LOOP_NONE; break;
		case Sample::LOOP_FORWARD: rd.loop_mode=RESAMPLE_LOOP_FORWARD; break;
		case Sample::LOOP_BIDI: rd.loop_mode=RESAMPLE_LOOP_BIDI; break;
	}
	
					// disable loop if begin > end 
	if (rd.fp_loop_begin>=rd.fp_loop_end)
		rd.loop_mode=RESAMPLE_LOOP_NONE;
	
	
	rd.write_pos=0;
	
	int written;
	if (src_chans==1 && dst_chans==1) { 
		// MONO -> MONO
		written=resample_with_loop<FRAC_BITS,1,1,INTERPOLATION_LINEAR>(rd,p_frames);
	} else if (src_chans==1 && dst_chans==2) { 
		// MONO -> STEREO
		written=resample_with_loop<FRAC_BITS,1,2,INTERPOLATION_LINEAR>(rd,p_frames);
	} else if (src_chans==2 && dst_chans==2) { 
		// STEREO -> STEREO
		written=resample_with_loop<FRAC_BITS,2,2,INTERPOLATION_LINEAR>(rd,p_frames);
	} else if (src_chans==1 && dst_chans==4) { 
		// MONO -> QUAD
		written=resample_with_loop<FRAC_BITS,1,4,INTERPOLATION_LINEAR>(rd,p_frames);
	} else if (src_chans==2 && dst_chans==4) { 
		// STEREO -> QUAD
		written=resample_with_loop<FRAC_BITS,2,4,INTERPOLATION_LINEAR>(rd,p_frames);
	} else if (src_chans==4 && dst_chans==4) { 
		// QUAD -> QUAD
		written=resample_with_loop<FRAC_BITS,4,4,INTERPOLATION_LINEAR>(rd,p_frames);
	} 
	
	if (written<p_frames) //couldnt write enough, which means we're done
		sample_active=false;
	
	/* move previous vols to current! */
	for (int i=0;i<MAX_CHANS;i++)
		prev_vol[i]=current_vol[i];
	
	
		
}

/* Processing */
void SampleTrigger::process(int p_frames) {
	
	const EventBuffer *ebuff=get_event_buffer();
	
	for (int i=0;i<ebuff->get_event_count();i++) {
		
		const Event *e=ebuff->get_event(i);
				
		if (e->type==Event::TYPE_MIDI) {
			
			const EventMidi &em=e->param.midi;
			switch (em.midi_type) {
				
				case EventMidi::MIDI_NOTE_ON: {
					
					
					
					if (sample.get_length()==0)
						break;
					if (em.data.note.note!=(int)param.note.get())
						break; //cant be triggered by this
			
					if (sample_active) {
						//declick.. luckily this crap doesnt use much CPU
						//process_to_buffer(&fadeout_buffer,FADEOUT_BUFFER_LEN);
						
						//then add to the declick ring buffer, where we stack up declicked samples...				
						//declick_ring_buffer.copy_from(&fadeout_buffer,FADEOUT_BUFFER_LEN);
					}		
					
					
				
					rd.fp_pos=0;
					
				
					velocity=(float)em.data.note.velocity/127.0;
					velocity=powf(velocity,2.0); //is this ok?
					sample_active=true;
					release_env=1.0;
					
					is_off=false;
					first_mix=true;
					//em.data.note.note
					//em.data.note.velocity 
					
				} break;
				case EventMidi::MIDI_NOTE_OFF: {
					
					
					if (!sample_active)
						break;
					if (sample.get_length()==0)
						break; 
					if (em.data.note.note!=(int)param.note.get())
						break; //cant be triggered by this
					
					is_off=true;
				} break;
				default: {} //dont annoy, gcc
			}
		}
	}
	
	
	
	/* use the first one to mix */	
	
	output_plug->get_buffer()->clear( p_frames );
	
	if (!sample_active)
		return;
	
	process_to_buffer( p_frames, output_plug->get_buffer() );

}

Sample& SampleTrigger::get_sample() {
	
	return sample;	
}

void SampleTrigger::reset() {
	
	sample_active=false;
}

void SampleTrigger::save(TreeSaver *p_saver) {
	
	p_saver->enter("sample");
	save_sample_to_tree(p_saver,&sample);
	p_saver->exit();
	
	SoundPlugin::save(p_saver);
	
}
void SampleTrigger::load(TreeLoader *p_loader) {
	
	p_loader->enter("sample");
	load_sample_from_tree(p_loader,&sample);
	p_loader->exit();
	
	SoundPlugin::load(p_loader);
	
}

SampleTrigger::SampleTrigger(const SoundPluginInfo *p_info,int p_channels) : SoundPlugin(p_info,p_channels) {
	
	if (p_channels<1)
		p_channels=1;
	
	output_plug = new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	
	param.note.set(60);
	param.note.config("note","Note");

	param.pan.set_all( 0.5, 0, 1, 0.5, 0.01, Property::DISPLAY_SLIDER, "pan","Pan");
	param.depth.set_all( 0.5, 0, 1, 0.5, 0.01, Property::DISPLAY_SLIDER, "pan_depth","Pan Depth");
	param.release.set_all( 0.3, 0, 1, 0.3, 0.01, Property::DISPLAY_SLIDER, "release","Release");
	param.gain.set_all( -24, -60, 24, 0, 0.1, Property::DISPLAY_SLIDER, "global_gain","Gain","dB");	
	
	property_list.push_back( &param.note );		    
	property_list.push_back( &param.pan );		    
	property_list.push_back( &param.depth );		    
	property_list.push_back( &param.release );		    
	property_list.push_back( &param.gain );		    
	
		
	mix_rate=44100;
	
	fadeout_buffer.setup(p_channels,FADEOUT_BUFFER_LEN);
	declick_ring_buffer.setup(p_channels,DECLICK_RING_BUFFER_LEN);
	sample_active=false;
	
}


SampleTrigger::~SampleTrigger(){ 
	

	delete output_plug;
}

}
