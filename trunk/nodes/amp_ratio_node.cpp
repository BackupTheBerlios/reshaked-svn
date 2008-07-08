//
// C++ Implementation: amp_ratio_node
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "amp_ratio_node.h"


AudioNode *AmpRatioNode::creation_func(int p_channels,const AudioNodeInfo *p_info) {

	return new AmpRatioNode(p_channels,p_info);
}


const AudioNodeInfo *AmpRatioNode::get_creation_info() {

	static AudioNodeInfo _info;
	_info.caption="Amplification (ratio)";
	_info.short_caption="Amp";
	_info.description="Ratio-Based Amplification";
	_info.unique_ID="INTERNAL_AmpRatioNode";
	_info.creation_func=&AmpRatioNode::creation_func;
	_info.category="Effects";

	return &_info;
}

void AmpRatioNode::process(const ProcessInfo& p_info) {

	for(int i=0;i<get_channels();i++) {
	
		const sample_t *src = get_audio_buffer( PORT_IN, 0, i );
		sample_t *dst = get_audio_buffer( PORT_OUT, 0, i );
		
		for (int j=0;j<p_info.audio_buffer_size;j++) {
		
			dst[j]=src[j]*amp();
		}
	}
}

AmpRatioNode::AmpRatioNode(int p_instanced_channels,const AudioNodeInfo *p_info) : HL_AudioNode(p_instanced_channels,p_info) {

	amp.set_all( 1.0, 0.0, 8.0, 1.0, 0.01, ControlPort::HINT_RANGE, "ratio","","Mute");

	add_audio_port("In",PORT_IN);
	add_audio_port("AmpOut",PORT_OUT);
	
	add_control_port( PORT_IN, &amp );
	
}

AmpRatioNode::~AmpRatioNode()
{
}


