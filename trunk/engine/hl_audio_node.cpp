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
#include <cmath>

String HL_ControlPort::get_name() const {

	return name;
}

String HL_ControlPort::get_suffix() const {

	return suffix;
}

float HL_ControlPort::get_min() const {

	return min;
}
float HL_ControlPort::get_max() const {

	return max;
}
float HL_ControlPort::get_step() const {

	return step;
}
float HL_ControlPort::get_default() const {

	return def;
}
float HL_ControlPort::get() const {

	return val;
}

void HL_ControlPort::set(float p_val,bool p_make_default) {

	if (p_val<min)
		p_val=min;
	if (p_val>max)
		p_val=max;
	val=p_val;
	
	if (step!=0) {

		val=floor( val / step + 0.5 ) * step;
	}
	
	if (p_make_default)
		def=val;
	
}
String HL_ControlPort::get_value_as_text(float p_value) const {

	if (p_value==min && min_string!="")
		return min_string;
	if (p_value==max && max_string!="")
		return max_string;
		
	String str = String::num(p_value);
	if (suffix!="")
		str+=" "+suffix;
	
	return str;
}

ControlPort::Hint HL_ControlPort::get_hint() const {

	return hint;
}

void HL_ControlPort::set_all(float p_val, float p_min, float p_max, float p_default, float p_step, Hint p_hint, String p_name, String p_suffix,String p_min_str,String p_max_str )  {

	val=p_val;
	min=p_min;
	max=p_max;	
	def=p_default;
	step=p_step;
	hint=p_hint;
	name=p_name;
	suffix=p_suffix;
	min_string=p_min_str;
	max_string=p_max_str;		

}

		
HL_ControlPort::HL_ControlPort(){ 

	min=0;
	max=1;
	val=0;
	step=0.01;
	def=0;
	hint=HINT_RANGE;
	
}


HL_ControlPort::~HL_ControlPort()
{
}


/****************/

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
		
			return _control_ports[p_flow].size();
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


