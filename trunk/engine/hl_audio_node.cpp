//
// C++ Implementation: hl_audio_node
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "hl_audio_node.h"

const AudioNodeInfo *HL_AudioNode::get_info() const {

	return _info;
}

int HL_AudioNode::get_instanced_channels() const {

	return _instanced_channels;
}

void HL_AudioNode::set_mix_rate(float p_hz) {

	_mix_rate=p_hz;
	mix_rate_changed();
	
}

int HL_AudioNode::get_port_count( PortType p_type, PortFlow p_flow ) const {

	switch(p_type) {
	
		case PORT_AUDIO: {
		
			return _audio_buffers[p_flow].size();
		};
		case PORT_EVENT: {
		
			return _event_buffers[p_flow].size();
		};
		case PORT_CONTROL: {
		
			return _event_buffers[p_flow].size();
		};
	}
	
	return -1;
}


int HL_AudioNode::get_audio_port_channel_count( PortFlow p_flow, int p_port) const {


	ERR_FAIL_INDEX_V( p_port, _audio_buffers[p_flow].size(), 0 );
	
	return _audio_buffers[p_flow][p_port].buffers.size();
}


void HL_AudioNode::connect_audio_buffer( PortFlow p_flow, int p_port, int p_channel, sample_t *p_buffer ) {

	ERR_FAIL_INDEX( p_port, _audio_buffers[p_flow].size() );
	ERR_FAIL_INDEX( p_channel, _audio_buffers[p_flow][p_port].buffers.size() );
	
	_audio_buffers[p_flow][p_port].buffers[p_channel]=p_buffer;

}

void HL_AudioNode::connect_event_buffer( PortFlow p_flow, int p_port, MusicEvent *p_buffer ) {

	ERR_FAIL_INDEX( p_port, _event_buffers[p_flow].size() );
	
	_event_buffers[p_flow][p_port]=p_buffer;

}


ControlPort* HL_AudioNode::get_control_port( PortFlow p_flow,int p_port ) {

	ERR_FAIL_INDEX_V( p_port, _control_ports[p_flow].size(), NULL );
	
	return _control_ports[p_flow][p_port];

}


void HL_AudioNode::add_audio_port( String p_name,PortFlow p_flow, int p_channels ) {
	
	if (p_channels<0)
		p_channels=_instanced_channels;
		
	ERR_FAIL_COND( p_channels<1 || p_channels>8 );
	
	_audio_buffers[p_flow].resize( _audio_buffers[p_flow].size() +1 );
	_AudioPort& ap=_audio_buffers[p_flow][ _audio_buffers[p_flow].size() -1 ];
	
	ap.buffers.resize( p_channels );
	
	for (int i=0;i<ap.buffers.size();i++)
		ap.buffers[i]=0;
		
	_port_names[PORT_AUDIO][p_flow].push_back(p_name);
	
}

void HL_AudioNode::add_event_port( String p_name, PortFlow p_flow ) {

	_event_buffers[p_flow].push_back(0);
	_port_names[PORT_EVENT][p_flow].push_back(p_name);
}

void HL_AudioNode::add_control_port( PortFlow p_flow, ControlPort* p_port ) {

	_control_ports[p_flow].push_back(p_port);
	_port_names[PORT_CONTROL][p_flow].push_back(p_port->get_name());

}


void HL_AudioNode::mix_rate_changed() {

	//?
}

HL_AudioNode::HL_AudioNode(int p_instanced_channels,const AudioNodeInfo *p_info) {

	_instanced_channels=p_instanced_channels;
	_info=p_info;
}


HL_AudioNode::~HL_AudioNode()
{
}


