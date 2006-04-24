//
// C++ Interface: ladspa_sound_plugin_source
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDLADSPA_SOUND_PLUGIN_SOURCE_H
#define RESHAKEDLADSPA_SOUND_PLUGIN_SOURCE_H

#include "engine/sound_plugin.h"
#include "version.h"
#include <string>
#ifdef LADSPA_ENABLED

namespace ReShaked {

/**
	@author red <red@killy>
*/
class LADSPA_SoundPluginSource{
	
	struct LADSPA_Struct {
		
		SoundPluginInfo plugin_info;
		String path;
		int index;
	};
	
	
	void scan_plugins(String p_dir);
	
	struct Sorter {

		bool operator()(const LADSPA_Struct*p_l, const LADSPA_Struct*p_r ) {

			std::string l_name=p_l->plugin_info.caption.ascii().get_data();
			std::string r_name=p_r->plugin_info.caption.ascii().get_data();
			return (r_name>l_name);
		}
	};

	
	std::vector<const LADSPA_Struct *> plugin_list;
	
	static LADSPA_SoundPluginSource *singleton;
	static SoundPlugin* create_ladspa_plugin(const SoundPluginInfo *p_info,int p_channels);	
	
public:
	
	
	LADSPA_SoundPluginSource();
	
	~LADSPA_SoundPluginSource();

};

}

#endif
#endif
