


#ifndef AUDIO_NODE_H
#define AUDIO_NODE_H

#include "typedefs.h"
#include "rstring.h"
#include "engine/music_event.h"
#include "error_list.h"
#include <vector>

#include "tree_loader.h"
#include "tree_saver.h"

class AudioNode;

struct AudioNodeInfo {
	
	
	String caption; ///< Caption of the Node
	String short_caption; ///< Caption of the Node
	String description; ///< Short description of the node
	String long_description; ///< Long description of the node
	String unique_ID; ///< Unique String ID of node
	String category; ///< string to categorize this node
	std::vector<int> custom_channels; ///< Limit the can_custom_channesls value to the given ones, if not empty. if 0 is present, then node doesn't offer custom channels.
	String icon_string;
	int version; ///node_version, use any scheme you feel like inside
	
	AudioNode* (*creation_func)(int,const AudioNodeInfo *);  ///< creation func (desired channels, info)
	
};

struct AudioNodeConfig {

	int audio_buffer_size;
	int event_buffer_size;	
};

class ControlPort {

	bool visible;
public:

	enum Hint {
		
		HINT_RANGE, ///< just a range (Default)
		HINT_TOGGLE, ///< valid values 0.0f , 1.0f
		HINT_ENUM, ///< asking integer values to get_Value_as_text will return them
	};	

	virtual String get_name() const =0;

	virtual float get_min() const=0;
	virtual float get_max() const=0;
	virtual float get_step() const=0;
	virtual float get_initial() const=0;
	virtual float get() const=0;
	
	virtual void set(float p_val,bool p_make_initial=false)=0; //set, optionally make the value the default too
	virtual void set_normalized(float p_val,bool p_make_initial=false); // set in range 0-1, internally converted to range
	virtual float get_normalized() const;
	
	virtual String get_value_as_text(float p_value) const;
	virtual Hint get_hint() const;
	
	virtual void set_visible(bool p_visible);
	virtual bool is_visible() const;
		
	ControlPort();
	virtual ~ControlPort();
};



class AudioNode {

	int _x,_y; // coordinates in graph
	int _layer; // layer in graph, <0 means all layers
	String _name;
protected:
friend class AudioGraph;	
	
	virtual void graph_enter();
	virtual void graph_exit();	
public:
	
	/* Audio Port API */
	
		
	enum PortType {
		
		PORT_AUDIO,
		PORT_EVENT,
		PORT_CONTROL
	};
			
	enum PortFlow {
		
		PORT_IN,
		PORT_OUT,
	};
	
	virtual int get_port_count( PortType p_type, PortFlow p_flow ) const=0;
	virtual int get_audio_port_channel_count( PortFlow p_flow, int p_port) const=0;
	
	virtual String get_port_name( PortType p_type, PortFlow p_flow,int p_port ) const;
	
	virtual void connect_audio_buffer( PortFlow p_flow, int p_port, int p_channel, sample_t *p_buffer )=0;
	virtual void connect_event_buffer( PortFlow p_flow, int p_port, MusicEvent *p_buffer )=0;

	virtual ControlPort* get_control_port( PortFlow p_flow,int p_port )=0;
	
	/* AudioNode Info */
	
	virtual const AudioNodeInfo *get_info() const=0;	
	virtual int get_instanced_channels() const=0;
	
	virtual String get_name() const;
	void set_name(String p_name);
	
	/* Process */
	
	struct ProcessInfo {
		
		int audio_buffer_size;
		int event_buffer_size;
	};
	
	virtual void process(const ProcessInfo& p_info)=0;
	virtual void set_mix_rate(float p_hz)=0;	
	
	/* Load/Save */
	
	virtual Error save( TreeSaver * p_tree )=0;
	virtual Error load( TreeLoader * p_tree )=0;
	
	/* Graph Positioning and Layering */
	
	void set_x(int p_x);
	int get_x() const;
	void set_y(int p_y);
	int get_y() const;
	
	enum {
	
		LAYER_ALWAYS_VISIBLE=-1
	};
	
	void set_layer(int p_layer);
	virtual int get_layer() const;
			
	Error save_file( String p_filename );
	Error load_file( String p_filename );
		
	AudioNode();
	virtual ~AudioNode();
	
};


#endif
