//
// C++ Interface: hl_audio_node
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef HL_AUDIO_NODE_H
#define HL_AUDIO_NODE_H

#include <vector>

#include "engine/audio_node.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

class HL_ControlPort : public ControlPort {

	String max_string;
	String min_string;
	float min,max,step;	
	float val;
	float initial;
	
	bool exp_range;
	Hint hint;
	
	String name;
	String suffix;
public:


	virtual String get_name() const;
	virtual String get_suffix() const;

	virtual float get_min() const;
	virtual float get_max() const;
	virtual float get_step() const;
	virtual float get_initial() const;
	virtual float get() const;
	inline float operator()() const { return val; } // quick get
	
	virtual void set(float p_val,bool p_make_default=false); //set, optionally make the value the default too
	
	virtual String get_value_as_text(float p_value) const;
	virtual Hint get_hint() const;
	
	void set_exp_range(bool p_enabled);
	
	void set_all(float p_val, float p_min, float p_max, float p_step, Hint p_hint, String p_name, String p_suffix="",String p_min_str="",String p_max_str="");
	virtual void set_normalized(float p_val,bool p_make_initial=false); // set in range 0-1, internally converted to range
	virtual float get_normalized() const;
		
		
	HL_ControlPort();
	virtual ~HL_ControlPort();
};

class HL_EnumControlPort : public ControlPort {


	std::vector<String> values;
	int val;
	int initial;
	
	String name;
public:


	virtual String get_name() const;
	virtual String get_suffix() const;

	virtual float get_min() const;
	virtual float get_max() const;
	virtual float get_step() const;
	virtual float get_initial() const;
	virtual float get() const;
	
	
	virtual void set(float p_val,bool p_make_default=false); //set, optionally make the value the default too
	
	virtual String get_value_as_text(float p_value) const;
	virtual Hint get_hint() const;
	
	void set_enum(String p_name,std::vector<String> p_enum);
	inline int operator()() const { return val; }		;
	HL_EnumControlPort();
	virtual ~HL_EnumControlPort();
};

class HL_AudioNode : public AudioNode {

	struct _AudioPort {
	
		std::vector< sample_t * > buffers;
	};
	
	
	std::vector< _AudioPort > _audio_buffers[2];
	std::vector< MusicEvent* > _event_buffers[2];
	std::vector< ControlPort* > _control_ports[2];
	std::vector< String > _port_names[3][2];
	
	int _instanced_channels;
	const AudioNodeInfo *_info;
	float _mix_rate;
	
	/* Uneeded functions */
	
	virtual void set_mix_rate(float p_hz);	
	
	virtual const AudioNodeInfo *get_info() const;	
	virtual int get_instanced_channels() const;
	
	virtual void connect_audio_buffer( PortFlow p_flow, int p_port, int p_channel, sample_t *p_buffer );
	virtual void connect_event_buffer( PortFlow p_flow, int p_port, MusicEvent *p_buffer );

	/* Implemented by default, just saves and restores all the input control ports */
	virtual Error save( TreeSaver * p_tree );
	virtual Error load( TreeLoader * p_tree );
	
protected:
	
	/*  High Level Audio Node API */
	
	virtual int get_port_count( PortType p_type, PortFlow p_flow ) const;
	virtual int get_audio_port_channel_count( PortFlow p_flow, int p_port) const;
	
	virtual ControlPort* get_control_port( PortFlow p_flow,int p_port );
	
	inline sample_t * get_audio_buffer( PortFlow p_flow, int p_port, int p_channel);
	
	inline MusicEvent * get_event_buffer( PortFlow p_flow, int p_port, int p_channel);
	
	inline int get_channels() const { return _instanced_channels; } //inline version
	
	inline float get_mix_rate() const { return _mix_rate; }
	
	virtual void mix_rate_changed(); ///< Notifies when mix rate has changed, warrated to not happen at the same time as process()
	
	
	// setup functions, use in constructor only
	void add_audio_port( String p_name,PortFlow p_flow, int p_channels=-1 ); 
	void add_event_port( String p_name, PortFlow p_flow );
	void add_control_port( PortFlow p_flow, ControlPort* p_port );
	

	HL_AudioNode(int p_instanced_channels,const AudioNodeInfo *p_info);
	
public:
	
	/* Process */
		
	virtual void process(const ProcessInfo& p_info)=0;

 
	~HL_AudioNode();

};


/* Make these inline for speed */

sample_t * HL_AudioNode::get_audio_buffer( PortFlow p_flow, int p_port, int p_channel) {

	ERR_FAIL_INDEX_V( p_port, _audio_buffers[p_flow].size(), NULL );
	ERR_FAIL_INDEX_V( p_channel, _audio_buffers[p_flow][p_port].buffers.size(), NULL );
	
	return _audio_buffers[p_flow][p_port].buffers[p_channel];
}

MusicEvent * HL_AudioNode::get_event_buffer( PortFlow p_flow, int p_port, int p_channel) {

	ERR_FAIL_INDEX_V( p_port, _event_buffers[p_flow].size(), NULL );
	
	return _event_buffers[p_flow][p_port];
}

#endif
