//
// C++ Implementation: sine_synth_node
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sine_synth_node.h"

#include "dsp/formulas.h"


void SineSynthNode::SineVoice::event(Event p_event) {

	if (p_event==NOTE_ON) {
		printf("I GOT NOTEONZOR\n");
		phase=0;
		release_env=1.0;
	}
}

void SineSynthNode::SineVoice::process(int p_frames,sample_t **p_out_buffers) {


	if (is_off()) {
	
		SineSynthNode * node = static_cast<SineSynthNode*>(get_synth());
		
		release_env-=(p_frames/get_mix_rate())/(node->release()/1000.0);		
		if (release_env<0)
			release_env=0;
	}
		
	float n = get_note();
	float inc = (note_to_freq( get_note() ) / get_mix_rate()) * ( 2.0 * M_PI);
	
	float amp = get_total_amplitude() * release_env;
	
	for( int i=0;i<get_channels();i++ ) {
	
		for (int j=0;j<p_frames;j++) {
		
			p_out_buffers[i][j] += sinf( phase + j * inc ) * amp;
		}
	}
	
	phase += p_frames * inc;
}

float SineSynthNode::SineVoice::get_priority() const {

	return release_env;
}

SineSynthNode::SineVoice::SineVoice() {

	phase=0;
	release_env=0;
	
}

AudioNode *SineSynthNode::creation_func(int p_channels,const AudioNodeInfo *p_info) {

	return new SineSynthNode(p_channels,p_info);
}


const AudioNodeInfo *SineSynthNode::get_creation_info() {

	static AudioNodeInfo _info;
	_info.caption="Sinewave Synthesizer";
	_info.short_caption="SineSynth";
	_info.description="Basic synth, usable as template.";
	_info.unique_ID="INTERNAL_SineSynth";
	_info.creation_func=&SineSynthNode::creation_func;
	_info.icon_string="node_sine_synth";
	_info.category="Synths";

	return &_info;
}


SineSynthNode::SineSynthNode(int p_instanced_channels,const AudioNodeInfo *p_info) : MIDI_SynthNode( p_instanced_channels, p_info) {

	release.set_all( 1000, 10, 4000, 1, ControlPort::HINT_RANGE, "Release","ms" );	
	
	add_control_port( PORT_IN, &release );
	
	std::vector<Voice*> v;
	v.resize(32);
	for (int i=0;i<v.size();i++) {
		v[i]=new SineVoice;
	}
	
	register_voices( v ); 
}


SineSynthNode::~SineSynthNode()
{
}


