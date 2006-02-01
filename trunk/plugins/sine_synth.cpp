//
// C++ Implementation: sine_synth
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sine_synth.h"
#include "math.h"
#include "pixmaps/synth_sine.xpm"

#define DEFAULT_AMP 0.1

namespace ReShaked {


static SoundPlugin* create_sine_synth(const SoundPluginInfo *p_info,int p_channels) {
	
	return new SineSynth(p_info,p_channels);
}

const SoundPluginInfo *SineSynth::create_info() {
	
	static SoundPluginInfo info;
	
	info.caption="Sinth";
	info.description="Extremely Simple Sinewave Synthesizer";
	info.long_description="This plugin serves as an example on how the simplest synth plugin can be built ";
	info.unique_ID="INTERNAL_sine_synth"; 
	info.category_path="/basic"; 
	info.can_custom_channels=true;
	info.has_internal_UI=false; 
	info.is_synth=true;
	info.xpm_preview=(const char**)synth_sine_xpm;
	info.creation_func=&create_sine_synth;
	
	return &info;
}



/* MIDI SYNTH VOICE */



void SineSynth::Voice::event(Event p_event) {
	
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
	
	
void SineSynth::Voice::process(int p_frames) {
		
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
	
	
float SineSynth::Voice::priority() {
	
	return amp;
}


SineSynth::Voice::Voice(Property *p_release) {
	
	release=p_release;
}






int SineSynth::get_input_plug_count() {
	
	return 0;
}
int SineSynth::get_output_plug_count() {
	
	return 1;
	
}
AudioPlug *SineSynth::get_input_plug(int p_index) {
	
	return NULL;	
}

AudioPlug *SineSynth::get_output_plug(int p_index) {
	
	return output_plug;	
}

int SineSynth::get_port_count() {
	
	return midi_synth->get_input_property_count()+midi_synth->get_output_property_count();

}
Property& SineSynth::get_port(int p_index) {
	
	
	if (p_index<midi_synth->get_input_property_count())
		return *midi_synth->get_input_property(p_index);
	else 
		return *midi_synth->get_output_property(p_index-midi_synth->get_input_property_count());
	
}
SoundPlugin::PortType SineSynth::get_port_type(int p_index) {
	
	if (p_index<midi_synth->get_input_property_count())
		return TYPE_WRITE;
	else 
		return TYPE_READ; 
	
	
}

/* Setting up */
void SineSynth::set_mixing_rate(float p_mixing_rate) { //sort of useless
	
	midi_synth->set_mix_rate( p_mixing_rate );

}

/* Processing */
void SineSynth::process(int p_frames) {
	
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

void SineSynth::reset() {
	
	midi_synth->reset();	
}
SineSynth::SineSynth(const SoundPluginInfo *p_info,int p_channels) : SoundPlugin(p_info,p_channels) {
	
	if (p_channels<1)
		p_channels=1;
	
	output_plug = new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	
	release.set_all( 0.5, 0.01, 2, 0.5, 0.01, Property::DISPLAY_SLIDER, "release","Release","s");

	
	std::vector<MidiSynth::Voice*> voices;
	for (int i=0;i<32;i++) { //32 voices!
		
		voices.push_back( new Voice(&release) );
		
		
	}
	
	midi_synth = new MidiSynth(p_channels,voices);
	
	midi_synth->add_input_property(&release);
}


SineSynth::~SineSynth(){ 
	
	
	delete midi_synth;
	delete output_plug;
}



}
