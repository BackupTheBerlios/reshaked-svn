//
// C++ Interface: vst_sound_plugin_source
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDVST_SOUND_PLUGIN_SOURCE_H
#define RESHAKEDVST_SOUND_PLUGIN_SOURCE_H

#include "version.h"

#ifdef VST_ENABLED

#include <vector>
#include "typedefs.h"
#include "engine/sound_plugin.h"
#include <string>
#include "aeffectx.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class VST_SoundPluginSource{

	struct VST_Struct {
		
		SoundPluginInfo plugin_info;
		String path;
		String dir;
		bool write_only;

	};
	
	std::vector<const VST_Struct *> plugin_list;
	void scan_path(String p_dir);
	
	struct Sorter {

		bool operator()(const VST_Struct*p_l, const VST_Struct*p_r ) {

			std::string l_name=p_l->plugin_info.caption.ascii().get_data();
			std::string r_name=p_r->plugin_info.caption.ascii().get_data();
			return (r_name>l_name);
		}
	};
	std::vector<String> path_list;
	
	static VstIntPtr VSTCALLBACK host(AEffect *effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void *ptr, float opt);	
	static SoundPlugin* create_vst_plugin(const SoundPluginInfo *p_info,int p_channels);	

	static VST_SoundPluginSource *singleton;
public:

	/* Path to VSTs */
	static void add_path(String p_path);

	void scan_plugins();
	
	VST_SoundPluginSource();
	~VST_SoundPluginSource();

};

}

#endif
#endif
