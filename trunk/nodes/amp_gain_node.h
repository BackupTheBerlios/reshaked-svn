//
// C++ Interface: amp_gain_node
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef AMP_GAIN_NODE_H
#define AMP_GAIN_NODE_H

#include "engine/hl_audio_node.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class AmpGainNode : public HL_AudioNode {

	HL_ControlPort amp;

	virtual void process(const ProcessInfo& p_info);

	AmpGainNode(int p_instanced_channels,const AudioNodeInfo *p_info);	
	
public:

	static AudioNode *creation_func(int p_channels,const AudioNodeInfo *p_info);
	static const AudioNodeInfo *get_creation_info();


	~AmpGainNode();

};

#endif
