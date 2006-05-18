//
// C++ Interface: tool_plugins
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDTOOL_PLUGINS_H
#define RESHAKEDTOOL_PLUGINS_H

#include "engine/sound_plugin.h"


namespace ReShaked {

/**
	@author red <red@killy>
 */
class SplitterPlugin : public SoundPlugin {
			    
	AudioPlug *input_plug;
	std::vector<AudioPlug*> output_plugs;
	
public:


	static const SoundPluginInfo *create_info();

	/* Plugs */	

	int get_input_plug_count();
	int get_output_plug_count();
	AudioPlug *get_input_plug(int p_index);
	AudioPlug *get_output_plug(int p_index);


	/* Port Info */
	int get_port_count();
	Property& get_port(int p_index);
	PortType get_port_type(int p_index);
	

	/* Setting up */
	void set_mixing_rate(float p_mixing_rate);

	/* Processing */
	void process(int p_frames);

	SplitterPlugin(const SoundPluginInfo *p_info,int p_channels);
	~SplitterPlugin();

};



class MergerPlugin : public SoundPlugin {
			    
	AudioPlug *output_plug;
	std::vector<AudioPlug*> input_plugs;
	
public:


	static const SoundPluginInfo *create_info();

	/* Plugs */	

	int get_input_plug_count();
	int get_output_plug_count();
	AudioPlug *get_input_plug(int p_index);
	AudioPlug *get_output_plug(int p_index);


	/* Port Info */
	int get_port_count();
	Property& get_port(int p_index);
	PortType get_port_type(int p_index);


	/* Setting up */
	void set_mixing_rate(float p_mixing_rate);

	/* Processing */
	void process(int p_frames);

	MergerPlugin(const SoundPluginInfo *p_info,int p_channels);
	~MergerPlugin();

};



}

#endif
