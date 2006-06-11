//
// C++ Implementation: sinth
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sinth.h"

#include "math.h"
#include "pixmaps/icon_sinth.xpm"

#define DEFAULT_AMP 0.1

namespace ReShaked {


static SoundPlugin* create_sinth(const SoundPluginInfo *p_info,int p_channels) {
	
	return new Sinth(p_info,p_channels);
}

const SoundPluginInfo *Sinth::create_info() {
	
	static SoundPluginInfo info;
	
	info.caption="Sinth";
	info.description="Simple 2-Osc Additive Synthesizer";
	info.long_description="Generic 2 Oscillators Additive Synthesizer. Meant as an easy way to recreate vintage sounds, as well as a starting point for creating own ones. This synthesizer produces mono sounds (no matter the amount of channels), so adding a Panner is recommended.";
	info.unique_ID="INTERNAL_sinth"; 
	info.category="Synthesizers"; 
	info.can_custom_channels=true;
	info.has_internal_UI=false; 
	info.is_synth=true;
	info.xpm_preview=(const char**)icon_sinth_xpm;
	info.creation_func=&create_sinth;
	info.version=1;
	
	return &info;
}



/* MIDI SYNTH VOICE */



void Sinth::Voice::event(Event p_event) {
	
	switch (p_event) {
		
		
		case NOTE_ON: {
			
			done=false;
			cycles_osc_1=0;
			cycles_osc_2=0;
			
			prev_gain_osc_1=0;
			prev_gain_osc_2=0;
			prev_gain_global=0;
	

			
			first_mix=true;
			/* make env times curved */
			adsr_osc1.set_attack( pow(data->osc[0].env_attack.get()*2,3) );
			adsr_osc1.set_decay( pow(data->osc[0].env_decay.get()*2,3) );
			adsr_osc1.set_sustain( data->osc[0].env_sustain.get() );
			adsr_osc1.set_release( pow(data->osc[0].env_release.get()*2,3) );
			adsr_osc1.start();
			
			adsr_osc2.set_attack( pow(data->osc[1].env_attack.get()*2,3) );
			adsr_osc2.set_decay( pow(data->osc[1].env_decay.get()*2,3) );
			adsr_osc2.set_sustain( data->osc[1].env_sustain.get() );
			adsr_osc2.set_release( pow(data->osc[1].env_release.get()*2,3) );
			adsr_osc2.start();
			
					
		} break;
		case NOTE_OFF: {
			
			adsr_osc1.release_begin();
			adsr_osc2.release_begin();

		} break;
		
		
	}

	
}
	
template<Sinth::ModulationMode t_mode>
void Sinth::Voice::process_template(int p_frames) {

	if (done)
		return;

	switch (t_mode) { /* Envelope Finalization Condition depends on each mode! */
		
		case MOD_ADD: {
			
			if (adsr_osc1.is_finished() && adsr_osc2.is_finished()) {
				done=true;
				return;
			}
		} break;
		case MOD_RING: {
			
			if (adsr_osc1.is_finished() || adsr_osc2.is_finished()) {
				done=true;
				return;
			}
		} break;
		case MOD_FM: {
			
			if (adsr_osc1.is_finished()) {
				done=true;
				return;
			}
		} break;
	}
	
	/* Compute cycles and all that stuff */
	
	float osc_1_note=get_current_note()+data->osc[0].tune_coarse.get()+data->osc[0].tune_fine.get()/100.0;
	float osc_2_note=get_current_note()+data->osc[1].tune_coarse.get()+data->osc[1].tune_fine.get()/100.0;
	
	float osc_1_freq=440.0*powf(2.0,(osc_1_note-69.0)/12.0);	
	float osc_2_freq=440.0*powf(2.0,(osc_2_note-69.0)/12.0);	
	
	float mix_rate_cache=get_mix_rate();
	
	double time_to_mix=(float)p_frames/mix_rate_cache;
	double osc_1_increment=(time_to_mix*osc_1_freq)/p_frames;
	double osc_2_increment=(time_to_mix*osc_2_freq)/p_frames;
	
	Uint64 osc_1_incr_fp=llrint(osc_1_increment*(double)FRAC_LEN);
	Uint64 osc_2_incr_fp=llrint(osc_2_increment*(double)FRAC_LEN);
	
	adsr_osc1.process( time_to_mix );
	adsr_osc2.process( time_to_mix );
	
	float osc_1_gain=adsr_osc1.get_val();
	float osc_2_gain=adsr_osc2.get_val();
	
	float mod_amount=data->modulation.amount.get()/100.0;
	
	float global_gain=get_total_amplitude();
	
	
	/* Osc 1 buffer, mask and shift */

	int submap_osc_1=(int)(log(osc_1_incr_fp>>(FRAC_SIZE-Oscillator::BASE_SIZE_BITS))/log(2.0));
	if (submap_osc_1<0)
		submap_osc_1=0;
	
	const float *osc_1_buffer=data->osc[0].osc.get_osc(submap_osc_1);
	int osc_1_shift=FRAC_SIZE-data->osc[0].osc.get_osc_bits(submap_osc_1);
	int osc_1_mask=(1<<data->osc[0].osc.get_osc_bits(submap_osc_1))-1;
	
	float osc_1_shift_to_frac=1.0/(float)(1<<osc_1_shift);
	unsigned int osc_1_shift_mask=(1<<osc_1_shift)-1;
	
	/* Osc 1 buffer, mask and shift */

	int submap_osc_2=(int)(log(osc_2_incr_fp>>(FRAC_SIZE-Oscillator::BASE_SIZE_BITS))/log(2.0));
	if (submap_osc_2<0)
		submap_osc_2=0;
	
	const float *osc_2_buffer=data->osc[1].osc.get_osc(submap_osc_2);
	int osc_2_shift=FRAC_SIZE-data->osc[1].osc.get_osc_bits(submap_osc_2);
	int osc_2_mask=(1<<data->osc[1].osc.get_osc_bits(submap_osc_2))-1;
	
	float osc_2_shift_to_frac=1.0/(float)(1<<osc_2_shift);
	unsigned int osc_2_shift_mask=(1<<osc_2_shift)-1;
	
	/* Buffer where to Mix */
	float *dst_buff=get_buffer( 0 );
	
	/* Gain Ramping to avoid Clicking */
	
	float osc_1_gain_ramp=prev_gain_osc_1;
	float osc_1_gain_ramp_inc=(osc_1_gain-prev_gain_osc_1)/(float)p_frames;
	float osc_2_gain_ramp=prev_gain_osc_2;
	float osc_2_gain_ramp_inc=(osc_2_gain-prev_gain_osc_2)/(float)p_frames;
	float global_gain_ramp=prev_gain_global;
	float global_gain_ramp_inc=(global_gain-prev_gain_global)/(float)p_frames;

	prev_gain_global=global_gain;
	prev_gain_osc_1=osc_1_gain;
	prev_gain_osc_2=osc_2_gain;
	
	/* Filter */
	
	
	if (first_mix) {
		
		filter.enabled=true;
		
		switch (data->filter.type.get_current()) {
		
			case 1: filter.mode=Filter::LOWPASS; break;
			case 2: filter.mode=Filter::BANDPASS; break;
			case 3: filter.mode=Filter::HIGHPASS; break;
			default: filter.enabled=false; break;
		
		}
		filter.ha1=0;
		filter.ha2=0;
		filter.hb1=0;
		filter.hb2=0;
		filter.lfo_count=0;
	}
	
	
	Filter::Coeffs filter_c;
	Filter::Coeffs filter_c_inc;
	
	if (filter.enabled) {
		
		Filter f;
		Filter::Coeffs fc_new;
		
		f.set_mode(filter.mode);
		
		
		float cutoff=data->filter.cutoff.get();
		
		if (first_mix) {
			
			filter.env_pos=cutoff*powf( 2.0 , (1.0-data->filter.env_attack_level.get())*8.0-4.0 );
			double time_to_reach_attack=pow((1.0-data->filter.env_attack_time.get())*2,3);
			double frames_to_reach_attack=mix_rate_cache*time_to_reach_attack;
			
			filter.attack_env_mult=pow(cutoff/filter.env_pos,1.0/frames_to_reach_attack);
			
			
			if (data->filter.env_attack_enabled.get()>0.0)
				cutoff=filter.env_pos;
			else
				filter.env_pos=cutoff; //so release can use it
			
			filter.release_end_pos=cutoff*powf( 2.0 , (1.0-data->filter.env_release_level.get())*8.0-4.0 );
			
			double time_to_reach_release=pow(data->filter.env_release_time.get()*2,3);
			double frames_to_reach_release=mix_rate_cache*time_to_reach_release;
			
			filter.release_env_mult=pow(filter.release_end_pos/cutoff,1.0/frames_to_reach_release);
			
			//printf("frames to reach %i, multiplier: %g\n",(int)frames_to_reach,filter.attack_env_mult);
			//	printf("source cutoff %g, target cutoff %g\n",filter.env_pos,cutoff);
		} else {
			
			
			
			if (!is_off()) {
				
				if (filter.env_pos<cutoff && filter.attack_env_mult>1.0) {
					
					filter.env_pos*=pow(filter.attack_env_mult,(float)p_frames);
					
					if (filter.env_pos>cutoff)
						filter.env_pos=cutoff;
					
				} else if (filter.env_pos>cutoff && filter.attack_env_mult<1.0) {
					
					filter.env_pos*=pow(filter.attack_env_mult,(float)p_frames);
					
					if (filter.env_pos<cutoff)
						filter.env_pos=cutoff;
					
				}
				
				
				if (data->filter.env_attack_enabled.get()==0.0)
					filter.env_pos=cutoff; //so release can use it
			} else if (data->filter.env_release_enabled.get()>0.0) {
				
				if (filter.env_pos<filter.release_end_pos && filter.release_env_mult>1.0) {
					
					filter.env_pos*=pow(filter.release_env_mult,(float)p_frames);
					
					if (filter.env_pos>filter.release_end_pos)
						filter.env_pos=filter.release_end_pos;
					
				} else if (filter.env_pos>filter.release_end_pos && filter.release_env_mult<1.0) {
					
					filter.env_pos*=pow(filter.release_env_mult,(float)p_frames);
					
					if (filter.env_pos<filter.release_end_pos)
						filter.env_pos=filter.release_end_pos;
					
				}
				
				//printf("release at %g, trying to reach %g, multiplier %g\n",filter.env_pos,filter.release_end_pos,filter.release_env_mult);
				
			
			}
			
		}
		
		if (data->filter.env_release_enabled.get()>0.0 || data->filter.env_attack_enabled.get())
			cutoff=filter.env_pos;
		
		/* lfo */
		
		int lfo_cycle_size=lrint(mix_rate_cache/data->filter.lfo_rate.get());
		float lfo_phase=(float)(filter.lfo_count%lfo_cycle_size)/(float)lfo_cycle_size;
		float lfo_val=sin(lfo_phase*2*M_PI)*data->filter.lfo_depth.get();
		cutoff*=pow(2,lfo_val/12.0);
		
		/* adjust cutoff */
		if (cutoff<data->filter.cutoff.get_min())
			cutoff=data->filter.cutoff.get_min();
		
		if (cutoff>data->filter.cutoff.get_max())
			cutoff=data->filter.cutoff.get_max();
		
		
		
		filter.lfo_count+=p_frames;
		f.set_cutoff( cutoff );
		f.set_resonance( data->filter.resonance.get() );
		f.set_sampling_rate(mix_rate_cache);
		f.prepare_coefficients(&fc_new);
		
		if (first_mix) {
			
			filter_c=fc_new;
		} else {
			
			filter_c_inc.b0=(fc_new.b0-filter.last_coeffs.b0)/(float)p_frames;
			filter_c_inc.b1=(fc_new.b1-filter.last_coeffs.b1)/(float)p_frames;
			filter_c_inc.b2=(fc_new.b2-filter.last_coeffs.b2)/(float)p_frames;
			filter_c_inc.a1=(fc_new.a1-filter.last_coeffs.a1)/(float)p_frames;
			filter_c_inc.a2=(fc_new.a2-filter.last_coeffs.a2)/(float)p_frames;
			filter_c=filter.last_coeffs;
		}
		
	} else {
		
		filter_c.b0=1.0;
		filter_c.b1=0;		
		filter_c.b2=0;		
		filter_c.a1=0;		
		filter_c.a2=0;		
	}
	for (int i=0;i<p_frames;i++) {
	
		
		/* Osc 2 Base, osc2 is computed normally always*/
		float val_osc_2=osc_2_buffer[(cycles_osc_2>>osc_2_shift)&osc_2_mask];
		
		/* Osc 2 Interpolate */
		float val_osc_2_n=osc_2_buffer[((cycles_osc_2>>osc_2_shift)+1)&osc_2_mask];

		float val_osc_2_frac=(float)(cycles_osc_2&osc_2_shift_mask)*osc_2_shift_to_frac;
		
		val_osc_2=val_osc_2+(val_osc_2_n-val_osc_2)*val_osc_2_frac;
		val_osc_2*=osc_2_gain_ramp;
		
		float out; //output will go here
		
		if (t_mode==MOD_FM) { // templatized condition removed at compile time for performance
			
			int phase_offset=lrintf(osc_1_mask*val_osc_2*mod_amount*3.0); //osc2 is used as offset
			//3.0 is a constant for intensifying the moudulation amount a bit more
			
			/* Osc 1 Base, osc1 is computed normally always*/
			float val_osc_1=osc_1_buffer[((cycles_osc_1>>osc_1_shift)+phase_offset)&osc_1_mask];
		
			/* Osc 1 Interpolate */
			float val_osc_1_n=osc_1_buffer[((cycles_osc_1>>osc_1_shift)+phase_offset+1)&osc_1_mask];

			float val_osc_1_frac=(float)((cycles_osc_1+(Uint64)phase_offset)&osc_1_shift_mask)*osc_1_shift_to_frac;
		
			out = val_osc_1+(val_osc_1_n-val_osc_1)*val_osc_1_frac;
			out*=osc_1_gain_ramp;
			
			
			
		} else {
			
			/* Osc 1 Base, osc1 is computed normally always*/
			float val_osc_1=osc_1_buffer[(cycles_osc_1>>osc_1_shift)&osc_1_mask];
		
			/* Osc 1 Interpolate */
			float val_osc_1_n=osc_1_buffer[((cycles_osc_1>>osc_1_shift)+1)&osc_1_mask];

			float val_osc_1_frac=(float)(cycles_osc_1&osc_1_shift_mask)*osc_1_shift_to_frac;
		
			val_osc_1=val_osc_1+(val_osc_1_n-val_osc_1)*val_osc_1_frac;
			val_osc_1*=osc_1_gain_ramp;
			
			if (t_mode==MOD_RING)
				out=val_osc_1*val_osc_2*mod_amount+val_osc_1*(1.0-mod_amount); //ringmod
			else
				out=val_osc_1+val_osc_2*mod_amount; //osc2

			
		}
		
		/* Global amp */
		out=out*global_gain_ramp;
		
		/* Filter! */
		float pre=out;
		out = (out * filter_c.b0 + filter.hb1 * filter_c.b1  + filter.hb2 * filter_c.b2 + filter.ha1 * filter_c.a1 + filter.ha2 * filter_c.a2); 	
		filter.ha2=filter.ha1;                                                             		
		filter.hb2=filter.hb1;                                                             		
		filter.hb1=pre;
		filter.ha1=out;
		
		dst_buff[i]+=out;
		
		/* increment index */
		cycles_osc_1+=osc_1_incr_fp;
		cycles_osc_2+=osc_2_incr_fp;
		
		/* increment ramp */
		osc_1_gain_ramp+=osc_1_gain_ramp_inc;
		osc_2_gain_ramp+=osc_2_gain_ramp_inc;
		global_gain_ramp+=global_gain_ramp_inc;
		
		/* increment filter ramp */
		
		filter_c.b0+=filter_c_inc.b0;
		filter_c.b1+=filter_c_inc.b1;
		filter_c.b2+=filter_c_inc.b2;
		filter_c.a1+=filter_c_inc.a1;
		filter_c.a2+=filter_c_inc.a2;
	}
	
	filter.last_coeffs=filter_c;
	
	first_mix=false;

}
	
	
void Sinth::Voice::process(int p_frames) {
		
	
	switch ( data->modulation.mode.get_current() ) {
		
		case MOD_ADD: process_template<MOD_ADD>(p_frames); break;
		case MOD_RING: process_template<MOD_RING>(p_frames); break;
		case MOD_FM: process_template<MOD_FM>(p_frames); break;

	}
	
}
	
	
float Sinth::Voice::priority() {
	
	return done?0:1;
}


Sinth::Voice::Voice(Data *p_data) {
	
	data=p_data;
}






int Sinth::get_input_plug_count() {
	
	return 0;
}
int Sinth::get_output_plug_count() {
	
	return 1;
	
}
AudioPlug *Sinth::get_input_plug(int p_index) {
	
	return NULL;	
}

AudioPlug *Sinth::get_output_plug(int p_index) {
	
	return output_plug;	
}

int Sinth::get_port_count() {
	
	return midi_synth->get_input_property_count()+midi_synth->get_output_property_count();

}
Property& Sinth::get_port(int p_index) {
	
	if (p_index<midi_synth->get_input_property_count())
		return *midi_synth->get_input_property(p_index);
	else 
		return *midi_synth->get_output_property(p_index-midi_synth->get_input_property_count());
	
}
SoundPlugin::PortType Sinth::get_port_type(int p_index) {
	
	if (p_index<midi_synth->get_input_property_count())
		return TYPE_WRITE;
	else 
		return TYPE_READ; 
	
	
}

/* Setting up */
void Sinth::set_mixing_rate(float p_mixing_rate) { //sort of useless
	
	midi_synth->set_mix_rate( p_mixing_rate );

}

/* Processing */
void Sinth::process(int p_frames) {
	
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
	
	midi_synth->process( p_frames); //process will only fill the first buffer, since it's a mono synth
	
	for (int i=1;i<output_plug->get_buffer()->get_channels();i++) {
	
		memcpy(output_plug->get_buffer()->get_buffer( i ),output_plug->get_buffer()->get_buffer( 0 ), p_frames*sizeof(float));
	}

}

void Sinth::reset() {
	
	midi_synth->reset();	
}

Oscillator* Sinth::get_osc(int p_osc) {
	
	ERR_FAIL_INDEX_V(p_osc,2,NULL);
	
	return &data.osc[p_osc].osc;
}

void Sinth::save(TreeSaver *p_saver) {
	
	p_saver->add_float_array( "osc_1_data", data.osc[0].osc.get_osc(), Oscillator::BASE_SIZE );
	p_saver->add_float_array( "osc_2_data", data.osc[1].osc.get_osc(), Oscillator::BASE_SIZE );
	SoundPlugin::save( p_saver ); //save properties normally
}
void Sinth::load(TreeLoader *p_loader) {
	
	if ( p_loader->get_float_array_len( "osc_1_data" )==Oscillator::BASE_SIZE ) {
		
		p_loader->get_float_array( "osc_1_data" , data.osc[0].osc.get_osc_w() );
		data.osc[0].osc.update_submaps();
		
	}
	
	if ( p_loader->get_float_array_len( "osc_2_data" )==Oscillator::BASE_SIZE ) {
		
		p_loader->get_float_array( "osc_2_data" , data.osc[1].osc.get_osc_w() );
		data.osc[1].osc.update_submaps();
	}
	
	SoundPlugin::load( p_loader ); //load properties normally
	
}


Sinth::Sinth(const SoundPluginInfo *p_info,int p_channels) : SoundPlugin(p_info,p_channels) {
	
	if (p_channels<1)
		p_channels=1;
	
	output_plug = new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	
	std::vector<MidiSynth::Voice*> voices;
	for (int i=0;i<32;i++) { //32 voices!
		
		voices.push_back( new Voice(&data) );
		
		
	}
	
	midi_synth = new MidiSynth(p_channels,voices);
	
	for (int i=0;i<2;i++) {
		
		Data::OSC &o=data.osc[i];
		String pr=String("osc_")+String::num(i+1)+"_";
		//String prl="Osc "+String::num(i+1)+"/";
		String prl="";
		
		o.env_attack.set_all( 0.01, 0, 1.0, 0.00, 0.01, Property::DISPLAY_SLIDER, pr+"env_attack",prl+"Envelope Attack");
		o.env_decay.set_all( 0.01, 0, 1.0, 0.00, 0.01, Property::DISPLAY_SLIDER, pr+"env_decay",prl+"Envelope Decay");
		o.env_sustain.set_all( 1.0, 0, 1.0, 0.00, 0.01, Property::DISPLAY_SLIDER, pr+"env_sustain",prl+"Envelope Sustain");
		o.env_release.set_all( 0.3, 0, 1.0, 0.00, 0.01, Property::DISPLAY_SLIDER, pr+"env_release",prl+"Envelope Release");
			
		o.tune_coarse.set_all( 0, -24, +24, 0, 1, Property::DISPLAY_SLIDER, pr+"tune_coarse",prl+"Coarse Tune","t");
		o.tune_fine.set_all( 0, -99, +99, 0, 1, Property::DISPLAY_SLIDER, pr+"tune_fine",prl+"Fine Tune","%");
		
		
		midi_synth->add_input_property(&o.env_attack);
		midi_synth->add_input_property(&o.env_decay);
		midi_synth->add_input_property(&o.env_sustain);
		midi_synth->add_input_property(&o.env_release);
			
		midi_synth->add_input_property(&o.tune_coarse);
		midi_synth->add_input_property(&o.tune_fine);
				
		
	}
	
	std::vector<String> mod_mode;
	mod_mode.push_back("add");
	mod_mode.push_back("ring");
	mod_mode.push_back("fm");
	data.modulation.mode.set_all("modulation_mode","Modulation Mode",mod_mode);
	data.modulation.amount.set_all(0, 0, 100, 0, 1, Property::DISPLAY_SLIDER, "modulation_amount","Modulation Amount","%");
	
	midi_synth->add_input_property(&data.modulation.mode);
	midi_synth->add_input_property(&data.modulation.amount);
	
	Data::Filt &f=data.filter;
	std::vector<String> filt_mode;
	filt_mode.push_back("Off");
	filt_mode.push_back("LPF");
	filt_mode.push_back("BPF");
	filt_mode.push_back("HPF");
	
	f.type.set_all("filter_type","Filter Type",filt_mode);
	f.type.set_write_only(true);
	f.cutoff.set_all( 8000, 20, 16000, 20, 1, Property::DISPLAY_SLIDER, "filter_cutoff","Filter Cutoff","hz");
	f.cutoff.set_quad_coeff( true );
	
	f.resonance.set_all( 1, 0, 3, 1, 0.01, Property::DISPLAY_SLIDER, "filter_resonance","Filter Resonance");
	f.env_attack_enabled.set_all( 0,0,1,0,1,Property::DISPLAY_SLIDER, "filter_attack_enabled","Filter Attack Enabled","","Off","On");
	f.env_attack_time.set_all( 0.5,0,1,0,0.01,Property::DISPLAY_SLIDER, "filter_attack_time","Filter Attack Time");
	f.env_attack_level.set_all( 0.5,0,1,0,0.01,Property::DISPLAY_SLIDER, "filter_attack_level","Filter Attack Level");
	f.env_release_enabled.set_all( 0,0,1,0,1,Property::DISPLAY_SLIDER, "filter_release_enabled","Filter Release Enabled","","Off","On");
	f.env_release_time.set_all( 0.5,0,1,0,0.01,Property::DISPLAY_SLIDER, "filter_release_time","Filter Release Time");
	f.env_release_level.set_all( 0.5,0,1,0,0.01,Property::DISPLAY_SLIDER, "filter_release_level","Filter Release Level");
	
	f.lfo_rate.set_all( 1,0.1,10,1,0.1,Property::DISPLAY_SLIDER, "filter_lfo_rate","Filter LFO Rate","hz");
	f.lfo_depth.set_all( 0,0,24,0,0.1,Property::DISPLAY_SLIDER, "filter_lfo_depth","Filter LFO Depth","st"); //semitones
	
	midi_synth->add_input_property(&f.type);
	midi_synth->add_input_property(&f.cutoff);
	midi_synth->add_input_property(&f.resonance);
			
	midi_synth->add_input_property(&f.env_attack_enabled);
	midi_synth->add_input_property(&f.env_attack_time);
	midi_synth->add_input_property(&f.env_attack_level);
			
	midi_synth->add_input_property(&f.env_release_enabled);
	midi_synth->add_input_property(&f.env_release_time);
	midi_synth->add_input_property(&f.env_release_level);
			
	midi_synth->add_input_property(&f.lfo_rate);
	midi_synth->add_input_property(&f.lfo_depth);
	
	

}


Sinth::~Sinth(){ 
	
	
	delete midi_synth;
	delete output_plug;
}



}
