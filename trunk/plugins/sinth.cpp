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


static SoundPlugin* create_sine_synth(const SoundPluginInfo *p_info,int p_channels) {
	
	return new Sinth(p_info,p_channels);
}

const SoundPluginInfo *Sinth::create_info() {
	
	static SoundPluginInfo info;
	
	info.caption="Sinth";
	info.description="2-Osc Simple Synthesis";
	info.long_description="Generic 2 Oscillators Additive Synthesizer. Meant as an easy way to recreate vintage sounds, as well as a starting point for creating own ones. This synthesizer produces mono sounds (no matter the amount of channels), so adding a Panner is recommended.";
	info.unique_ID="INTERNAL_sinth"; 
	info.category="Synthesizers"; 
	info.can_custom_channels=true;
	info.has_internal_UI=false; 
	info.is_synth=true;
	info.xpm_preview=(const char**)icon_sinth_xpm;
	info.creation_func=&create_sine_synth;
	info.version=1;
	
	return &info;
}



/* MIDI SYNTH VOICE */



void Sinth::Voice::event(Event p_event) {
	
	switch (p_event) {
		
		
		case NOTE_ON: {
			
			ofs=0;
		
			amp=(float)get_velocity()/127.0;
			float fade=release->get();
			float frames_for_fade=(float)get_mix_rate()*fade;
			amp_decr=amp/frames_for_fade;
					
		} break;
		case NOTE_OFF: {
			
			/* nothing */			
		} break;
		
		
	}

	
}
	
	
void Sinth::Voice::process(int p_frames) {
		
	float freq=440.0*powf(2.0,(get_current_note()-69.0)/12.0);	
	float mix_rate_cache=get_mix_rate();
	bool off=is_off();
	
	if (amp<=0.0) {
		return;
	}
		
	
	
	int buffers=get_buffer_count();
	for (int i=0;i<p_frames;i++) {
			
		
		float val=sinf(((double)ofs*freq/mix_rate_cache)*M_PI*2.0);
		val*=amp*DEFAULT_AMP;
		if (off) {
			amp-=amp_decr;
			if (amp<0)
				amp=0;
		}
				
		/* This is not meant to be done by a real plugin, after all
		this synth supports all channel configurations.. */
		
		for (int j=0;j<buffers;j++) {
			
			get_buffer(j)[i]+=val;			
			
		}
		ofs++;
	}
	
	
	//printf("process %i\n",p_frames);
}
	
	
float Sinth::Voice::priority() {
	
	return amp;
}


Sinth::Voice::Voice(Property *p_release) {
	
	release=p_release;
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
	
	
	for (int i=0;i<output_plug->get_buffer()->get_channels();i++) {
		
		output_plug->get_buffer()->clear( p_frames );
		midi_synth->set_buffer( i, output_plug->get_buffer()->get_buffer( i ) );
	}
	
	midi_synth->process( p_frames);

}

void Sinth::reset() {
	
	midi_synth->reset();	
}
Sinth::Sinth(const SoundPluginInfo *p_info,int p_channels) : SoundPlugin(p_info,p_channels) {
	
	if (p_channels<1)
		p_channels=1;
	
	output_plug = new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	
	std::vector<MidiSynth::Voice*> voices;
	for (int i=0;i<32;i++) { //32 voices!
		
		voices.push_back( new Voice(&release) );
		
		
	}
	
	midi_synth = new MidiSynth(p_channels,voices);
	
	for (int i=0;i<2;i++) {
		
		Data::OSC &o=data.osc[i];
		String pr="osc_"+String::num(i+1);
		//String prl="Osc "+String::num(i+1)+"/";
		String prl="";
		
		o.env_attack.set_all( 0.01, 0, 1.0, 0.00, 0.01, Property::DISPLAY_SLIDER, pr+"env_attack",prl+"Envelope Attack");
		o.env_decay.set_all( 0.01, 0, 1.0, 0.00, 0.01, Property::DISPLAY_SLIDER, pr+"env_decay",prl+"Envelope Decay");
		o.env_decay.set_all( 1.0, 0, 1.0, 0.00, 0.01, Property::DISPLAY_SLIDER, pr+"env_sustain",prl+"Envelope Sustain");
		o.env_sustain.set_all( 0.3, 0, 1.0, 0.00, 0.01, Property::DISPLAY_SLIDER, pr+"env_release",prl+"Envelope Release");
			
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
	f.cutoff.set_all( 8000, 20, 16000, 20, 1, Property::DISPLAY_SLIDER, "filter_cutoff","Filter Cutoff","hz");
	f.resonance.set_all( 1, 0, 4, 1, 0.01, Property::DISPLAY_SLIDER, "filter_resonance","Filter Resonance");
	f.env_attack_enabled.set_all( 0,0,1,0,1,Property::DISPLAY_SLIDER, "filter_attack_enabled","Filter Attack Enabled","","Off","On");
	f.env_attack_time.set_all( 0,0,1,0,0.01,Property::DISPLAY_SLIDER, "filter_attack_time","Filter Attack Time");
	f.env_attack_level.set_all( 0,0,1,0,0.01,Property::DISPLAY_SLIDER, "filter_attack_level","Filter Attack Level");
	f.env_release_enabled.set_all( 0,0,1,0,1,Property::DISPLAY_SLIDER, "filter_release_enabled","Filter Release Enabled","","Off","On");
	f.env_release_time.set_all( 0,0,1,0,0.01,Property::DISPLAY_SLIDER, "filter_release_time","Filter Release Time");
	f.env_release_level.set_all( 0,0,1,0,0.01,Property::DISPLAY_SLIDER, "filter_release_level","Filter Release Level");
	
	f.lfo_rate.set_all( 1,0.1,10,1,0.1,Property::DISPLAY_SLIDER, "filter_lfo_rate","Filter LFO Rate","hz");
	f.lfo_depth.set_all( 0,0,12,0,0.1,Property::DISPLAY_SLIDER, "filter_lfo_depth","Filter LFO Depth","st"); //semitones
	
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
	
	
	release.set_all( 0.5, 0.01, 2, 0.5, 0.01, Property::DISPLAY_SLIDER, "release","Release","s");

	
	midi_synth->add_input_property(&release);
}


Sinth::~Sinth(){ 
	
	
	delete midi_synth;
	delete output_plug;
}



}
