//
// C++ Implementation: proxy_node
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "proxy_node.h"

namespace ReShaked {


ProxyNodeBase::~ProxyNodeBase() {
	
	
}

bool ProxyNode::is_system() {
	
	return system;
}
void ProxyNode::set_system(bool p_system) {
	
	system=p_system;
}
void ProxyNode::set_caption(String p_caption) {
	
	caption=p_caption;
}

AudioPlug *ProxyNode::get_input_plug(int p_index) {
	
	ERR_FAIL_INDEX_V(p_index,input_plugs.size(),NULL);	
	return input_plugs[p_index];
}
AudioPlug *ProxyNode::get_output_plug(int p_index) {
	
	ERR_FAIL_INDEX_V(p_index,output_plugs.size(),NULL);	
	return output_plugs[p_index];
	
}
int ProxyNode::get_input_plug_count() {

	return input_plugs.size();
}
int ProxyNode::get_output_plug_count() {

	return output_plugs.size();
}

void ProxyNode::add_input_plug(int p_channels) {
	
	input_plugs.push_back(new AudioPlug(p_channels,AudioPlug::TYPE_INPUT,this));
}
void ProxyNode::add_output_plug(int p_channels) {
	
	output_plugs.push_back(new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this));
}


void ProxyNode::set_process_method(ProxyNodeBase * p_instance, void (ProxyNodeBase::*p_process_method)(int) ) {
	
	process_instance=p_instance;
	process_method=p_process_method;
	
}

void ProxyNode::process(int p_frames) {
	
	ERR_FAIL_COND( process_instance==NULL );
	(process_instance->*process_method)(p_frames);
}
String ProxyNode::get_caption() {
	
	return caption;
}

ProxyNode::ProxyNode() {
	
	process_instance=NULL;
	caption="Proxy";
	system=false;
}


ProxyNode::~ProxyNode()
{
}



}
