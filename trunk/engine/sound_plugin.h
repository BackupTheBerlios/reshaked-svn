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
#include "tree_saver.h"
#include "tree_loader.h"


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
	String category; ///< string to categorize this plugin
	bool can_custom_channels; ///< If true, then it can instance in ANY amount of channels, limited only by custom_channels
	std::vector<int> custom_channels; ///< Limit the can_custom_channesls value to the given ones.
	bool has_internal_UI; ///< Plugin uses internal UI, for external plugins (VST/DSSI/ETC)
	bool is_synth; ///< Receives MIDI events (synth)
	const char ** xpm_preview; ///< There is a xpm preview image of it, preferably 48x48

	int version; ///plugin_version, use any scheme you feel like inside
	
	SoundPlugin* (*creation_func)(const SoundPluginInfo *,int);

};

class SoundPlugin_MetaData {
	
public:	
	
	virtual ~SoundPlugin_MetaData() {}
};

class SoundPlugin : public AudioNode {
	
	int _channels_created;
	const SoundPluginInfo *_plugin_info;
	bool _skip_processing;
	int _duplicate;
	String _current_file;
	String _current_preset_name;
	SoundPlugin_MetaData *_metadata;
	
	const EventBuffer *_event_buffer;

protected:	
	
	const EventBuffer *get_event_buffer();
public:
	enum PortType {
		TYPE_READ,
		TYPE_WRITE
	};
	
	/* Port Info */
	virtual int get_port_count()=0;
	virtual Property& get_port(int p_index)=0;
	virtual Property& get_port_by_name(String p_name);
	virtual PortType get_port_type(int p_index)=0;
		
	
	/* Methods in case this is a synth */
	void set_event_buffer(const EventBuffer *p_buffer);
	
	/* Setting up */
	virtual void set_mixing_rate(float p_mixing_rate)=0;
	virtual void reset();	
	/* tell it to skip processing */
	void set_skip_processing(bool p_skip);
	bool skips_processing();
	
	
	/* Internal UI Methods, only for plugins with internal UI */
	virtual void show_internal_UI();
	virtual void hide_internal_UI();
	
	/* Golobal Info */	
	const SoundPluginInfo* get_info();
	String get_caption();	
	int get_duplicate();
	void set_duplicate(int p_duplicate);
	int get_channels_created();
	
	
	String get_current_file(); //current file the plugin last saved to
	void set_current_file(String p_current_file);
	
	String get_current_preset_name(); //current preset_name of the plugin
	void set_current_preset_name(String p_current_preset_name);
	
	virtual void save(TreeSaver *p_saver);	
	virtual void load(TreeLoader *p_saver);	
	
	/* Used to store some custom data from whoever is controlling it, most like some UI Setting */
	void set_metadata(SoundPlugin_MetaData *p_metadata);
	SoundPlugin_MetaData *get_metadata();
	
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
