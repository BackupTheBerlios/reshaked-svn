//
// C++ Interface: proxy_node
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDPROXY_NODE_H
#define RESHAKEDPROXY_NODE_H

#include "engine/audio_node.h"
#include <vector>

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/


class ProxyNodeBase {
public:	
	virtual ~ProxyNodeBase();
};

class ProxyNode : public AudioNode {
	
	
	std::vector<AudioPlug*> input_plugs;
	std::vector<AudioPlug*> output_plugs;
	
	ProxyNodeBase * process_instance;
	void (ProxyNodeBase::*process_method)(int);
	
	bool system;
	String caption;
	String get_caption();
	bool is_system();	

public:
	/* virtual */
	AudioPlug *get_input_plug(int p_index);
	AudioPlug *get_output_plug(int p_index);
	int get_input_plug_count();
	int get_output_plug_count();
	
	/* api */
	void add_input_plug(int p_channels);
	void add_output_plug(int p_channels);
	
	void set_process_method(ProxyNodeBase * p_instance, void (ProxyNodeBase::*p_process_method)(int) );
	
	void process(int p_frames);
	
	void set_system(bool p_system);
	void set_caption(String p_caption);
	
	
	ProxyNode();
	~ProxyNode();

};

}

#endif
