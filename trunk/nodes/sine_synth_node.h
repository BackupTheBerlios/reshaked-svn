//
// C++ Interface: sine_synth_node
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SINE_SYNTH_NODE_H
#define SINE_SYNTH_NODE_H

#include "engine/midi_synth_node.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class SineSynthNode : public MIDI_SynthNode {

	class SineVoice : public Voice {
	
		double phase;
		float release_env;	
	public:
	
		virtual void event(Event p_event);	
		virtual void process(int p_frames,sample_t **p_out_buffers);	
		
		virtual float get_priority() const;
		
		SineVoice();
	};

friend class SineVoice;


	HL_ControlPort release;

	SineSynthNode(int p_instanced_channels,const AudioNodeInfo *p_info);	
public:

	static AudioNode *creation_func(int p_channels,const AudioNodeInfo *p_info);
	static const AudioNodeInfo *get_creation_info();


	~SineSynthNode();
};

#endif
