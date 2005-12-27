//
// C++ Interface: sound_plugin
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSOUND_PLUGIN_H
#define RESHAKEDSOUND_PLUGIN_H

#include "engine/audio_node.h"
#include "engine/event_buffer.h"
#include "property.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/

class SoundPlugin; /* for creation func */

struct SoundPluginInfo {

	String caption; ///< Caption of the Plugin
	String description; ///< Short description of the plugin
	String long_description; ///< Long description of the plugin
	String unique_ID; ///< Unique String ID of plugin
	String category_path; ///< path to categorize this plugin
	bool can_custom_channels; ///< can create with custom channels (as mono/stereo/etc)
	bool has_internal_UI; ///< Plugin uses internal UI, for external plugins (VST/DSSI/ETC)
	bool is_synth; ///< Receives MIDI events (synth)
	const char ** xpm_preview; ///< There is a xpm preview image of it

	SoundPlugin* (*creation_func)(const SoundPluginInfo *,int);

};


class SoundPlugin : public AudioNode {
	
	int _channels_created;
	const SoundPluginInfo *_plugin_info;
	
	String get_caption();
	
public:
	enum PortType {
		TYPE_READ,
		TYPE_WRITE
	};
	
	/* Port Info */
	virtual int get_port_count()=0;
	virtual Property& get_port(int p_index)=0;
	virtual PortType get_port_type(int p_index)=0;
		
	
	/* Methods in case this is a synth */
	virtual void set_event_buffer(const EventBuffer *p_buffer);
	
	/* Setting up */
	virtual void set_mixing_rate(float p_mixing_rate)=0;
	
	/* Internal UI Methods, only for plugins with internal UI */
	virtual void show_internal_UI();
	virtual void hide_internal_UI();
	
	/* Golobal Info */	
	const SoundPluginInfo* get_info();
	int get_channels_created();
	
	/**
	 * Sound Plugin Constructor
	 * @param p_info info used to create this plugin
	 * @param p_channels channels used, if created with custom channels, -1 for plugins that dont use that
	 */
	SoundPlugin(const SoundPluginInfo *p_info,int p_channels=-1);
	~SoundPlugin();

};

}

#endif
