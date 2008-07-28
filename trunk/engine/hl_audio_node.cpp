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
float HL_ControlPort::get_initial() const {

	return initial;
}
float HL_ControlPort::get() const {

	return val;
}

void HL_ControlPort::set(float p_val,bool p_make_initial) {

	if (p_val<min)
		p_val=min;
	if (p_val>max)
		p_val=max;
	
	if (step!=0) {

		p_val=floor( p_val / step + 0.5 ) * step;
	}
	
	val=p_val;	
	if (p_make_initial)
		initial=p_val;
	
}
String HL_ControlPort::get_value_as_text(float p_value) const {

	if (p_value==min && min_string!="")
		return min_string;
	if (p_value==max && max_string!="")
		return max_string;
		
	int decimals=4;
	
	if (step!=0) {
		
		decimals=0;
		double s = step;
		s-=floor(s);
		while(s<0.9999 && decimals<4) {
			s*=10.0;
			s-=floor(s);
			decimals++;
		}		
	}
	
	String str = String::num(p_value,decimals);
	if (suffix!="")
		str+=" "+suffix;
	
	return str;
}

void HL_ControlPort::set_exp_range(bool p_enabled) {

	exp_range=p_enabled;
}

ControlPort::Hint HL_ControlPort::get_hint() const {

	return hint;
}

void HL_ControlPort::set_all(float p_val, float p_min, float p_max, float p_step, Hint p_hint, String p_name, String p_suffix,String p_min_str,String p_max_str )  {

	val=p_val;
	initial=p_val;
	min=p_min;
	max=p_max;	
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
	initial=0;
	hint=HINT_RANGE;
	exp_range=false;
	
}


HL_ControlPort::~HL_ControlPort()
{
}


/****************/

String HL_EnumControlPort::get_name() const {

	return name;
}
String HL_EnumControlPort::get_suffix() const {

	return "";
}

float HL_EnumControlPort::get_min() const {

	return 0;
}
float HL_EnumControlPort::get_max() const {

	return values.size()-1;
}
float HL_EnumControlPort::get_step() const {

	return 1;
}
float HL_EnumControlPort::get_initial() const {

	return initial;
}
float HL_EnumControlPort::get() const {

	return val;
}	

void HL_EnumControlPort::set_enum(String p_name,std::vector<String> p_enum) {

	name=p_name;
	values=p_enum;
}

void HL_EnumControlPort::set(float p_val,bool p_make_default) {

	if (p_val<0)
		p_val=0;
	else if (p_val>=values.size())
		p_val=values.size()-1;
		
	val=(int)p_val;
	if (p_make_default)
		initial=(int)p_val;
}

String HL_EnumControlPort::get_value_as_text(float p_value) const {

	int idx=(int)p_value;
	if (values.size()==0)
		return "<UNCONFIGURED>";
	if (idx<0)
		idx=0;
	if (idx>=values.size())
		idx=values.size()-1;

	return values[idx];
	
}
ControlPort::Hint HL_EnumControlPort::get_hint() const {

	return HINT_ENUM;
}

	
HL_EnumControlPort::HL_EnumControlPort() {

}
HL_EnumControlPort::~HL_EnumControlPort() {

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


Error HL_AudioNode::save( TreeSaver * p_tree ) {

	for (int i=0;i<get_port_count( PORT_CONTROL, PORT_IN );i++) {
	
		ControlPort *cp = get_control_port(  PORT_IN , i );
				
		p_tree->add_float( cp->get_name(), cp->get_initial() );
		
	}
}

Error HL_AudioNode::load( TreeLoader * p_tree )  {

	for (int i=0;i<p_tree->get_var_count();i++) {
	
		String varname = p_tree->get_var_name(i);
		printf("found var %s\n",varname.ascii().get_data());
		
		for (int j=0;j<get_port_count( PORT_CONTROL, PORT_IN );j++) {
		
			ControlPort *cp = get_control_port(  PORT_IN , j );
			
			if (cp->get_name()==varname) {
				float value = p_tree->get_float( varname );
				printf("found where to place it, value %f\n",value);
				cp->set( value, true );
			}
			
		}
		
	}

}


HL_AudioNode::HL_AudioNode(int p_instanced_channels,const AudioNodeInfo *p_info) {

	_instanced_channels=p_instanced_channels;
	_info=p_info;
}


HL_AudioNode::~HL_AudioNode()
{
}


