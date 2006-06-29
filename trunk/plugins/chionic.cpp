//
// C++ Implementation: chionic
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "chionic.h"
#include "pixmaps/icon_chionic_2.xpm"
namespace ReShaked {


static SoundPlugin* create_chionic(const SoundPluginInfo *p_info,int p_channels) {
	
	/* Do not allow instancing at wrong channel numbers */
	if ( p_channels!=2 && p_channels !=4)
		return NULL;
	
	return new Chionic(p_info,p_channels);
}

const SoundPluginInfo *Chionic::create_info() {
	
	static SoundPluginInfo info;
	
	info.caption="Chionic 2";
	info.description="Advanced Sampler and Additive Synthesizer";
	info.long_description="Chionic 2 is an advanced sampler and additive synthesizer. It's capable of multi layered voices and many kind of modulations between them, as well as modulating an audio source. It also supports many features intended for producing extremely realistic instruments";
	info.unique_ID="INTERNAL_chionic"; 
	info.category="Synthesizers"; 
	info.can_custom_channels=true;
	info.custom_channels.push_back(2);
	info.custom_channels.push_back(4);
	info.has_internal_UI=false; 
	info.is_synth=true;
	info.xpm_preview=(const char**)icon_chionic_2_xpm;
	info.creation_func=&create_chionic;
	info.version=1;
	
	return &info;
}


/* Plugs */	

int Chionic::get_input_plug_count()  {
	
	return 1;	
	
}
int Chionic::get_output_plug_count()  {
	
	return 1;
	
}
AudioPlug *Chionic::get_input_plug(int p_index)  {
	
	return input_plug;
	
}
AudioPlug *Chionic::get_output_plug(int p_index)  {
	
	return output_plug;	
	
}


/* Port Info */
int Chionic::get_port_count()  {
	
	return midi_synth->get_input_property_count()+midi_synth->get_output_property_count();

	
}
Property& Chionic::get_port(int p_index)  {
	
	if (p_index<midi_synth->get_input_property_count())
		return *midi_synth->get_input_property(p_index);
	else 
		return *midi_synth->get_output_property(p_index-midi_synth->get_input_property_count());
	
}

SoundPlugin::PortType Chionic::get_port_type(int p_index)  {
	
	if (p_index<midi_synth->get_input_property_count())
		return TYPE_WRITE;
	else 
		return TYPE_READ; 
	
}


/* Setting up */
void Chionic::set_mixing_rate(float p_mixing_rate)  {
	
	midi_synth->set_mix_rate( p_mixing_rate );
	
}

void Chionic::reset() {
	
	midi_synth->reset();	
}

/* Processing */
void Chionic::process(int p_frames)  {
	
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
	
	
	
	output_plug->get_buffer()->clear( p_frames );
	
	for (int i=0;i<output_plug->get_buffer()->get_channels();i++) {
		
		midi_synth->set_buffer( i, output_plug->get_buffer()->get_buffer( i ) );
	}
	
	midi_synth->process( p_frames);
	
	
}

ChionicParams* Chionic::get_params() {
	
	return &params;
}
Chionic::Chionic(const SoundPluginInfo *p_info,int p_channels) : SoundPlugin(p_info,p_channels) {
	
	input_plug = new AudioPlug(p_channels,AudioPlug::TYPE_INPUT,this);
	output_plug = new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	
		
	std::vector<MidiSynth::Voice*> voices;
	for (int i=0;i<32;i++) { //32 voices!
		
		voices.push_back( new ChionicVoice(&params,&voice_buffers) );
		
	}
	
	midi_synth = new MidiSynth(p_channels,voices);

	
	
}
Chionic::~Chionic()  {
	
	
	
}


}
