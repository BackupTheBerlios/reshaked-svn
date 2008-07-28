//
// C++ Interface: chorus_plugin
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CHORUS_NODE_H
#define CHORUS_NODE_H

#include "engine/hl_audio_node.h"

/**
	@author red <red@killy>
 */
 
 class ChorusNode : public HL_AudioNode {

	enum {
		
		MAX_DELAY_MS=50,
		MAX_DEPTH_MS=20,
		MAX_WIDTH_MS=50,
		MAX_VOICES=4,
		CYCLES_FRAC=16,
		CYCLES_MASK=(1<<CYCLES_FRAC)-1,
		MAX_CHANNELS=4
	};

	struct Voice {
	
		HL_ControlPort delay;
		HL_ControlPort rate;
		HL_ControlPort depth;
		HL_ControlPort level;
		HL_ControlPort width;
		HL_ControlPort cutoff;
		HL_ControlPort pan;
		HL_ControlPort pan_depth;
		
		unsigned long long cycles;
		std::vector<float> filter_h;
		
	} voice[MAX_VOICES];

	HL_ControlPort wet;
	HL_ControlPort dry;
	
	sample_t **ring_buffers;
	unsigned int buffer_pos;
	unsigned int buffer_mask;
		
	virtual void mix_rate_changed(); 
	virtual void process(const ProcessInfo& p_info);

	ChorusNode(int p_instanced_channels,const AudioNodeInfo *p_info);	
	
public:

	static AudioNode *creation_func(int p_channels,const AudioNodeInfo *p_info);
	static const AudioNodeInfo *get_creation_info();


	~ChorusNode();

};

 
#endif
