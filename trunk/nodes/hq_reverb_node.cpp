//
// C++ Implementation: hq_reverb_node
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "hq_reverb_node.h"


void HQ_ReverbNode::mix_rate_changed() {

	float new_rate = get_mix_rate();
	
	for (int i=0;i<get_channels();i++) {
					
		reverb[i].set_mix_rate(new_rate);
	}
	
}

AudioNode *HQ_ReverbNode::creation_func(int p_channels,const AudioNodeInfo *p_info) {

	return new HQ_ReverbNode(p_channels,p_info);
}


const AudioNodeInfo *HQ_ReverbNode::get_creation_info() {

	static AudioNodeInfo _info;
	_info.caption="High Quality Reverb";
	_info.short_caption="HQ Reverb";
	_info.description="High Quality, customizable reverb.";
	_info.unique_ID="INTERNAL_HQ_ReverbNode";
	_info.creation_func=&HQ_ReverbNode::creation_func;
	_info.icon_string="node_hq_reverb";
	_info.category="Effects";

	return &_info;
}

void HQ_ReverbNode::process(const ProcessInfo& p_info) {

	
	for (int i=0;i<get_channels();i++) {
		
		Reverb &r=reverb[i];
		
		r.set_predelay( predelay.get() );
		r.set_predelay_feedback( predelay_fb.get() );
		r.set_highpass( hpf.get() );
		r.set_room_size( room_size.get() );
		r.set_damp( lpf.get() );
		r.set_extra_spread( spread.get() );
		r.set_wet( wet.get() );
		r.set_dry( dry.get() );
		
		const sample_t *src = get_audio_buffer( PORT_IN, 0, i );
		sample_t *dst = get_audio_buffer( PORT_OUT, 0, i );
		
		r.process( src , dst , p_info.audio_buffer_size );
		
	}
}

HQ_ReverbNode::HQ_ReverbNode(int p_instanced_channels,const AudioNodeInfo *p_info) : HL_AudioNode(p_instanced_channels,p_info) {

	if (p_instanced_channels<1)
		p_instanced_channels=1;
	
	add_audio_port("Input",PORT_IN);
	add_audio_port("Wet+Dry",PORT_OUT);
			
	reverb = new Reverb[p_instanced_channels];
	
	for (int i=0;i<get_channels();i++) {
		
		reverb[i].set_mix_rate( 44100.0 ); //some default
		reverb[i].set_extra_spread_base( (float)i*0.000521 ); //gives stereoysh effect

	}
	
	predelay.set_all( 100.0, 20.0, 500.0, 1.0, ControlPort::HINT_RANGE, "PreDelay","ms");
	predelay_fb.set_all( 0.0,0.0,1.0,0.01, ControlPort::HINT_RANGE, "Pre-Fbk");
	room_size.set_all( 0.7,0.0,1.0,0.01, ControlPort::HINT_RANGE, "RoomSize");
	hpf.set_all( 0.0,0.0,16384.0,1.0, ControlPort::HINT_RANGE, "Pre-HPF","hz","Off");
	lpf.set_all( 0.5,0.0,1.0,0.01, ControlPort::HINT_RANGE, "Damp","","None","Full");
	spread.set_all( 1.0,0.0,1.0,0.01, ControlPort::HINT_RANGE, "Width","","Mono");
	wet.set_all( 0.3,0.0,1.0,0.01, ControlPort::HINT_RANGE, "Wet","","Silence");
	dry.set_all( 0.8,0.0,1.0,0.01, ControlPort::HINT_RANGE, "Dry","","Silence");
	
	hpf.set_exp_range(true);
	room_size.set_visible(true);
	wet.set_visible(true);
	
	add_control_port( PORT_IN, &predelay);
	add_control_port( PORT_IN, &predelay_fb);
	add_control_port( PORT_IN, &room_size);
	add_control_port( PORT_IN, &hpf);
	add_control_port( PORT_IN, &lpf);
	add_control_port( PORT_IN, &spread);
	add_control_port( PORT_IN, &wet);
	add_control_port( PORT_IN, &dry);
}


HQ_ReverbNode::~HQ_ReverbNode() {
		
	delete[] reverb;
}

