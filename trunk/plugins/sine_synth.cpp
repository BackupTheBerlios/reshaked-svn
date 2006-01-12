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

#define DEFAULT_AMP 0.1

namespace ReShaked {


static SoundPlugin* create_sine_synth(const SoundPluginInfo *p_info,int p_channels) {
	
	return new SineSynth(p_info,p_channels);
}

const SoundPluginInfo *SineSynth::create_info() {
	
	static SoundPluginInfo info;
	
	info.caption="Sine Synth";
	info.description="Extremely Simple Sinewave Synthesizer";
	info.long_description="This plugin serves as an example on how the simplest synth plugin can be built ";
	info.unique_ID="INTERNAL: sine_synth"; 
	info.category_path="/basic"; 
	info.can_custom_channels=true;
	info.has_internal_UI=false; 
	info.is_synth=true;
	info.xpm_preview=NULL;
	info.creation_func=&create_sine_synth;
	
	return &info;
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
	
	return 1;
}
Property& SineSynth::get_port(int p_index) {
	
	return release;
	
}
SoundPlugin::PortType SineSynth::get_port_type(int p_index) {
	
	return TYPE_WRITE;
	
}

/* Setting up */
void SineSynth::set_mixing_rate(float p_mixing_rate) { //sort of useless
	
	mixing_rate=p_mixing_rate;		
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
					
					printf("NOTE ON %i,%i\n",em.data.note.note,em.data.note.velocity);
					ofs=0;
					freq=440.0*powf(2.0,((float)em.data.note.note-69.0)/12.0);
					amp=DEFAULT_AMP*(float)em.data.note.velocity/127.0;
					float fade=release.get();
					float frames_for_fade=(float)mixing_rate*fade;
					amp_decr=amp/frames_for_fade;
					sust=true;
									
						
				} break;
				case EventMidi::MIDI_NOTE_OFF: {
					printf("NOTE OFF %i\n",em.data.note.note);
					
					sust=false;
				} break;
				default: {} //dont annoy, gcc
			}
		}
	}
	
	
	/* use the first one to mix */	
	float *buff=output_plug->get_buffer()->get_buffer(0);
	
	for (int i=0;i<p_frames;i++) {
			
		if (amp<=0.0) {
			buff[i]=0.0;
			continue;
		}
		
		float val=sinf(((double)ofs*freq/mixing_rate)*M_PI*2.0);
		val*=amp;
		if (!sust) {
			amp-=amp_decr;
			if (amp<0)
				amp=0;
		}
				
		buff[i]=val;
		ofs++;
	}
	
	/* process the rest, if they exist */
	for (int i=1;i<output_plug->get_channels();i++) {
		
		float *dst=output_plug->get_buffer()->get_buffer(i);
		
		for (int j=0;j<p_frames;j++) {
			
			dst[j]=buff[j];
		}
	}

}


SineSynth::SineSynth(const SoundPluginInfo *p_info,int p_channels) : SoundPlugin(p_info,p_channels) {
	
	if (p_channels<1)
		p_channels=1;
	
	output_plug = new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	
	release.set_all( 0.5, 0.01, 2, 0.5, 0.01, Property::DISPLAY_SLIDER, "release","Release","s");

	freq=1;
	amp=0;
	ofs=0;
	sust=false;
	amp_decr=0;
}


SineSynth::~SineSynth(){ 
	
	delete output_plug;
}



}
