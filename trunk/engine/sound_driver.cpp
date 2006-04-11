//
// C++ Implementation: sound_driver
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sound_driver.h"

namespace ReShaked {



const AudioPortLayout *SoundDriver::get_port_layout() {
	
	return port_layout;
}

int SoundDriver::InputNode::get_input_plug_count() {
	
	return 0;
		
}
int SoundDriver::InputNode::get_output_plug_count() {
	/* In goes to out, since we are "inside "the app */
	return input_plugs.size();
}
AudioPlug *SoundDriver::InputNode::get_input_plug(int p_index) {
	
	return NULL;
}
AudioPlug *SoundDriver::InputNode::get_output_plug(int p_index) {
	
	ERR_FAIL_INDEX_V(p_index,get_output_plug_count(),NULL);
	return input_plugs[p_index];
}
	
bool SoundDriver::InputNode::is_input_plug_enabled(int p_idx) {
	
	return false;
}
bool SoundDriver::InputNode::is_output_plug_enabled(int p_idx) {
	
	return driver->is_input_enabled(p_idx);	
}
String SoundDriver::InputNode::get_input_plug_caption(int p_which) {
	
	return "";
}
String SoundDriver::InputNode::get_output_plug_caption(int p_which) {
	
	if (p_which==0)
		return "Main";
	else {
		
		return "Aux "+String::num( p_which);
	}
}

String SoundDriver::InputNode::get_caption() {
	
	return "Rec";
}

void SoundDriver::InputNode::process(int p_frames) {
	
	driver->write_to_inputs(p_frames);
}
	
int SoundDriver::OutputNode::get_output_plug_count() {
	
	return 0;
	
}
int SoundDriver::OutputNode::get_input_plug_count() {
	/* Out goes to in, since we are "inside "the app */
	return output_plugs.size();
}
AudioPlug *SoundDriver::OutputNode::get_output_plug(int p_index) {
	
	return NULL;
}
AudioPlug *SoundDriver::OutputNode::get_input_plug(int p_index) {
	
	ERR_FAIL_INDEX_V(p_index,get_input_plug_count(),NULL);
	return output_plugs[p_index];
	
}

bool SoundDriver::OutputNode::is_output_plug_enabled(int p_idx) {
	
	return false;
}
bool SoundDriver::OutputNode::is_input_plug_enabled(int p_idx) {
	
	return driver->is_output_enabled(p_idx);	
}
String SoundDriver::OutputNode::get_output_plug_caption(int p_which) {
	
	return "";
}
String SoundDriver::OutputNode::get_input_plug_caption(int p_which) {
	
	if (p_which==0)
		return "Main";
	else {
		
		return "Aux "+String::num( p_which);
	}
}

String SoundDriver::OutputNode::get_caption() {
	
	return "Play";
}

void SoundDriver::OutputNode::process(int p_frames) {
	
	driver->read_from_outputs(p_frames);
}


/****************** INTERNAL *********************/

int SoundDriver::process_graph(int p_frames) {
	
	if (midi_input_handler)
		midi_input_handler->sound_thread_callback();
	return audio_process_base->process(p_frames);
}
int SoundDriver::get_input_count() {
	
	return input_node.input_plugs.size();
}
AudioPlug *SoundDriver::get_input_plug(int p_input) {
	
	return input_node.get_output_plug( p_input );
	
}
int SoundDriver::get_output_count() {
	
	return output_node.output_plugs.size();
	
}
AudioPlug *SoundDriver::get_output_plug(int p_output) {
	
	return output_node.get_input_plug( p_output );
}

void SoundDriver::set_audio_graph(AudioGraph *p_graph) {
	
	graph=p_graph;
}

AudioNode *SoundDriver::get_input_node() {
	
	return &input_node;
}
AudioNode *SoundDriver::get_output_node() {
	
	return &output_node;
}

void SoundDriver::set_audio_process_base(AudioProcessBase *p_base) {
	
	audio_process_base=p_base;
	
}


void SoundDriver::set_port_layout(const AudioPortLayout *p_layout) {
	
	ERR_FAIL_COND(get_status()!=STATUS_DISABLED);
	port_layout=p_layout;
	
	input_node.driver=this;
	
	
	for (int i=0;i<input_node.input_plugs.size();i++) {
		
		delete input_node.input_plugs[i];
	}
	input_node.input_plugs.clear();
	
	output_node.driver=this;
	
	for (int i=0;i<output_node.output_plugs.size();i++) {
		
		delete output_node.output_plugs[i];
	}
	output_node.output_plugs.clear();
	
	for (int i=0;i<p_layout->port_in_info.size();i++) {
		
		input_node.input_plugs.push_back( new AudioPlug( p_layout->port_in_info[i], AudioPlug::TYPE_OUTPUT, &input_node ) );
	}
	
	for (int i=0;i<p_layout->port_out_info.size();i++) {
		
		output_node.output_plugs.push_back( new AudioPlug( p_layout->port_out_info[i], AudioPlug::TYPE_INPUT, &output_node ) );
	}
	
	
	
}

void SoundDriver::set_midi_input_handler(MidiInputHandlerBase *p_handler) {
	
	midi_input_handler=p_handler;
}


}
