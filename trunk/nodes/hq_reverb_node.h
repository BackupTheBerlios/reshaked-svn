//
// C++ Interface: hq_reverb_node
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef HQ_REVERB_NODE_H
#define HQ_REVERB_NODE_H

#include "engine/hl_audio_node.h"
#include "dsp/reverb.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class HQ_ReverbNode : public HL_AudioNode {

	Reverb *reverb;

	HL_ControlPort predelay;   
	HL_ControlPort predelay_fb;   
	HL_ControlPort hpf;
	HL_ControlPort room_size;   
	HL_ControlPort lpf;
	HL_ControlPort spread;
	HL_ControlPort dry;   
	HL_ControlPort wet;
	
	virtual void mix_rate_changed(); 
	virtual void process(const ProcessInfo& p_info);

	HQ_ReverbNode(int p_instanced_channels,const AudioNodeInfo *p_info);	
	
public:

	static AudioNode *creation_func(int p_channels,const AudioNodeInfo *p_info);
	static const AudioNodeInfo *get_creation_info();


	~HQ_ReverbNode();

};

#endif
