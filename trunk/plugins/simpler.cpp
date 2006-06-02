//
// C++ Implementation: simpler
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "simpler.h"
#include <math.h>
#include "dsp/resample.h"
#include "dsp/tree_helpers_dsp.h"
#include "pixmaps/icon_simpler.xpm"

#define DEFAULT_AMP 0.1
#define C5_FREQ 261.6255653006
namespace ReShaked {


static SoundPlugin* create_simpler(const SoundPluginInfo *p_info,int p_channels) {
	
	return new Simpler(p_info,p_channels);
}

const SoundPluginInfo *Simpler::create_info() {
	
	static SoundPluginInfo info;
	
	info.caption="Simpler";
	info.description="Simple Sampler";
	info.long_description="Sampler with just the basics. Load Mono/Stereo files and ADSR.";
	info.unique_ID="INTERNAL_simpler"; 
	info.category="Synthesizers"; 
	info.can_custom_channels=true;
	info.custom_channels.push_back(1);
	info.custom_channels.push_back(2);
	info.custom_channels.push_back(4);
	info.has_internal_UI=false; 
	info.is_synth=true;
	info.xpm_preview=(const char**)icon_simpler_xpm;
	info.creation_func=&create_simpler;
	info.version=1;
	
	return &info;
}



/* MIDI SYNTH VOICE */


void Simpler::Voice::set_volumes_from_pan(float *p_to) {
	
	int out_count=get_buffer_count();
	
	if (out_count==1)
		p_to[0]=1.0;
	
	else if (out_count==2) {
		
		float pan=data->pan.get();
		p_to[0]=1.0-pan;
		p_to[1]=pan;
	} else if (out_count==4) {
	
		float pan=data->pan.get();
		float depth=data->depth.get();
		
		float l=1.0-pan;
		float r=pan;
		
		p_to[0]=(1.0-depth)*l;
		p_to[1]=(1.0-depth)*r;
		
		p_to[2]=depth*l;
		p_to[3]=depth*r;
	} else {
		
		p_to[0]=0.5; // FRONT LEFT
		p_to[1]=0.5; // FRONT RIGHT
		p_to[2]=0.5; // REAR LEFT
		p_to[3]=0.5; //REAR RIGHT

	}
}

void Simpler::Voice::event(Event p_event) {
	
	switch (p_event) {
		
		
		case NOTE_ON: {
			
			if (data->sample.get_length()==0)
				break;
			cycles=0;		
			done=false;
			for (int i=0;i<MAX_CHANS;i++)
				prev_vol[i]=0;
			
			first_mix=true;
			/* make env times curved */
			adsr.set_attack( pow(data->attack.get()*2,3) );
			adsr.set_decay( pow(data->decay.get()*2,3) );
			adsr.set_sustain( data->sustain.get() );
			adsr.set_release( pow(data->release.get()*2,3) );
			adsr.start();
						
		} break;
		case NOTE_OFF: {
			
			adsr.release_begin();
			/* nothing */			
		} break;
		
		
	}

	
}
	
	
void Simpler::Voice::process(int p_frames) {
		
	if (done)
		return;

	if (adsr.is_finished()) {
		done=true;
		return;
	}
	/* Compute cycles and all that stuff */
	
	float freq=440.0*powf(2.0,(get_current_note()-69.0)/12.0);	
	float mix_rate_cache=get_mix_rate();
	
	double time_to_mix=(float)p_frames/mix_rate_cache;
	double cycles_to_mix=time_to_mix*freq;
	
	double sample_cycle_size_in_frames=data->sample.get_base_freq() / C5_FREQ;
	
	double sample_pos=cycles*sample_cycle_size_in_frames;
	
	double sample_frames_to_mix=cycles_to_mix*sample_cycle_size_in_frames;
	
	adsr.process( time_to_mix );
	
	float current_volume=adsr.get_val()*get_total_amplitude();
	
	/* SETTING UP MIX DATA */
	
	ResampleData rd;
	
	/* Fixed Point */
	
	rd.fp_increment=llrint((sample_frames_to_mix/(float)p_frames)*(double)(1<<FRAC_BITS));
	
	switch(data->sample.get_loop_type()) {
		case Sample::LOOP_NONE: rd.loop_mode=RESAMPLE_LOOP_NONE; break;
		case Sample::LOOP_FORWARD: rd.loop_mode=RESAMPLE_LOOP_FORWARD; break;
		case Sample::LOOP_BIDI: rd.loop_mode=RESAMPLE_LOOP_BIDI; break;
	}
	
	rd.fp_loop_begin=(Sint64)data->sample.get_loop_begin()<<FRAC_BITS;
	rd.fp_loop_end=(Sint64)data->sample.get_loop_end()<<FRAC_BITS;
	rd.fp_size=(Sint64)data->sample.get_length()<<FRAC_BITS;
	
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
	
	/* Source Buffers */
	int src_chans=data->sample.get_channels();
	for (int i=0;i<src_chans;i++) {
			
		rd.src_buffers[i]=data->sample.get_buffer(i);
	}
	
	/* Dest Buffers */
	int dst_chans=get_buffer_count();
	
	for (int i=0;i<dst_chans;i++) {
		
		rd.dst_buffers[i]=get_buffer(i);
	}
	
	/* Volumes */
	
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
	
	/* Loop */
	
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
	
	/* move previous vols to current! */
	for (int i=0;i<MAX_CHANS;i++)
		prev_vol[i]=current_vol[i];
}
	
	
float Simpler::Voice::priority() {
	
	return done?0:1;
}


Simpler::Voice::Voice(Data *p_data) {
	
	data=p_data;
}






int Simpler::get_input_plug_count() {
	
	return 0;
}
int Simpler::get_output_plug_count() {
	
	return 1;
	
}
AudioPlug *Simpler::get_input_plug(int p_index) {
	
	return NULL;	
}

AudioPlug *Simpler::get_output_plug(int p_index) {
	
	return output_plug;	
}

int Simpler::get_port_count() {
	
	return midi_synth->get_input_property_count()+midi_synth->get_output_property_count();

}
Property& Simpler::get_port(int p_index) {
	
	
	if (p_index<midi_synth->get_input_property_count())
		return *midi_synth->get_input_property(p_index);
	else 
		return *midi_synth->get_output_property(p_index-midi_synth->get_input_property_count());
	
}
SoundPlugin::PortType Simpler::get_port_type(int p_index) {
	
	if (p_index<midi_synth->get_input_property_count())
		return TYPE_WRITE;
	else 
		return TYPE_READ; 
	
	
}

/* Setting up */
void Simpler::set_mixing_rate(float p_mixing_rate) { //sort of useless
	
	midi_synth->set_mix_rate( p_mixing_rate );

}

/* Processing */
void Simpler::process(int p_frames) {
	
	const EventBuffer *ebuff=get_event_buffer();
	
	for (int i=0;i<ebuff->get_event_count();i++) {
		
		const Event *e=ebuff->get_event(i);
				
		if (e->type==Event::TYPE_MIDI) {
			
			const EventMidi &em=e->param.midi;
			switch (em.midi_type) {
				
				case EventMidi::MIDI_NOTE_ON: {
					
					
					midi_synth->control_note_on( em.data.note.note, em.data.note.velocity );
					/*
					printf("NOTE ON %i,%i\n",em.data.note.note,em.data.note.velocity);
					ofs=0;
					freq=440.0*powf(2.0,((float)em.data.note.note-69.0)/12.0);
					amp=DEFAULT_AMP*(float)em.data.note.velocity/127.0;
					float fade=release.get();
					float frames_for_fade=(float)mixing_rate*fade;
					amp_decr=amp/frames_for_fade;
					sust=true;*/
									
						
				} break;
				case EventMidi::MIDI_NOTE_OFF: {
					
					midi_synth->control_note_off( em.data.note.note );
					/*					
					printf("NOTE OFF %i\n",em.data.note.note);
					
					sust=false;
					
					*/
				} break;
				default: {} //dont annoy, gcc
			}
		}
	}
	
	
	/* use the first one to mix */	
	
	output_plug->get_buffer()->clear( p_frames );
	
	for (int i=0;i<output_plug->get_buffer()->get_channels();i++) {
		
		midi_synth->set_buffer( i, output_plug->get_buffer()->get_buffer( i ) );
	}
	
	midi_synth->process( p_frames);

}

Sample& Simpler::get_sample() {
	
	return data.sample;	
}

void Simpler::reset() {
	
	midi_synth->reset();	
}

void Simpler::save(TreeSaver *p_saver) {
	
	p_saver->enter("sample");
	save_sample_to_tree(p_saver,&data.sample);
	p_saver->exit();
	
	SoundPlugin::save(p_saver);
	
}
void Simpler::load(TreeLoader *p_loader) {
	
	p_loader->enter("sample");
	load_sample_from_tree(p_loader,&data.sample);
	p_loader->exit();
	
	SoundPlugin::load(p_loader);
	
}

Simpler::Simpler(const SoundPluginInfo *p_info,int p_channels) : SoundPlugin(p_info,p_channels) {
	
	if (p_channels<1)
		p_channels=1;
	
	output_plug = new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	
	data.attack.set_all( 0.01, 0, 1, 0.01, 0.01, Property::DISPLAY_SLIDER, "adsr_attack","Attack");
	data.decay.set_all( 0.1, 0, 1, 0.01, 0.01, Property::DISPLAY_SLIDER, "adsr_decay","Decay");
	data.sustain.set_all( 0.7, 0, 1, 0.7, 0.01, Property::DISPLAY_SLIDER, "adsr_sustain","Sustain");
	data.release.set_all( 0.3, 0, 1, 0.3, 0.01, Property::DISPLAY_SLIDER, "adsr_release","Release");

	data.pan.set_all( 0.5, 0, 1, 0.5, 0.01, Property::DISPLAY_SLIDER, "pan","Pan");
	data.depth.set_all( 0.5, 0, 1, 0.5, 0.01, Property::DISPLAY_SLIDER, "depth","Pan Depth");
	
	std::vector<MidiSynth::Voice*> voices;
	for (int i=0;i<32;i++) { //32 voices!
		
		voices.push_back( new Voice(&data) );
	}
	
	midi_synth = new MidiSynth(p_channels,voices);
	
	midi_synth->add_input_property(&data.attack);
	midi_synth->add_input_property(&data.decay);
	midi_synth->add_input_property(&data.sustain);
	midi_synth->add_input_property(&data.release);
	midi_synth->add_input_property(&data.pan);
	midi_synth->add_input_property(&data.depth);
	
}


Simpler::~Simpler(){ 
	
	
	delete midi_synth;
	delete output_plug;
}

}