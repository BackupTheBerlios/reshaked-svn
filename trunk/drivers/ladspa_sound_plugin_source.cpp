//
// C++ Implementation: ladspa_sound_plugin_source
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ladspa_sound_plugin_source.h"

#include "version.h"
#ifdef LADSPA_ENABLED

#include "drivers/ladspa.h"
#include <sys/types.h>
#include <dirent.h>
#include <dlfcn.h>

#include<algorithm>

#include "engine/sound_plugin_list.h"
#include "ladspa.xpm"
#include "drivers/sound_plugin_ladspa.h"
namespace ReShaked {

LADSPA_SoundPluginSource *LADSPA_SoundPluginSource::singleton=NULL;

SoundPlugin* LADSPA_SoundPluginSource::create_ladspa_plugin(const SoundPluginInfo *p_info,int p_channels) {
	
	for (int i=0;i<singleton->plugin_list.size();i++) {
		
		if (&singleton->plugin_list[i]->plugin_info!=p_info) 
			continue;
		
		return new SoundPlugin_LADSPA(	p_info, singleton->plugin_list[i]->path, singleton->plugin_list[i]->index, p_channels);
			
		
	}
	return NULL;
	
	//return new Simpler(p_info,p_channels);
}



void LADSPA_SoundPluginSource::scan_plugins(String p_dir) {

	DIR *dir;
	struct dirent *dirent;

	dir= opendir(p_dir.utf8().get_data());
	if (dir == NULL) return;

	while ((dirent= readdir(dir))) {

		void *library = NULL;
		char lib_name[PATH_MAX];
		snprintf(lib_name, PATH_MAX, "%s/%s", p_dir.utf8().get_data(), dirent->d_name);
		
		//check that we have this, since the user may have added a patht 
		bool already_have_it=false;
		
		for (int i=0;i<plugin_list.size();i++) {
				   
			if (plugin_list[i]->path==String(lib_name)) {
				already_have_it=true;
				break;
			}	   
		}
		
		if (already_have_it)
			continue;
		
		library = dlopen(lib_name, RTLD_LAZY);
		if (library == NULL)
			continue;

		LADSPA_Descriptor_Function descriptor_fn;
		descriptor_fn = (LADSPA_Descriptor_Function)dlsym(library, "ladspa_descriptor");
		if (descriptor_fn == NULL)
			descriptor_fn = (LADSPA_Descriptor_Function)dlsym(library, "_ladspa_descriptor");

		if (descriptor_fn == NULL) {
			dlclose(library);
			continue;
		}

		for (int k= 0;; ++k) {
			const LADSPA_Descriptor *descriptor;
			descriptor= descriptor_fn(k);
			if (descriptor == NULL) {
				break;
			}


			int out_count=0,in_count=0;
			for (int i=0; i < (int)descriptor->PortCount; i++) {

				if (LADSPA_IS_PORT_AUDIO(descriptor->PortDescriptors[i])) {

					if (LADSPA_IS_PORT_INPUT(descriptor->PortDescriptors[i]))
						in_count++;
					if (LADSPA_IS_PORT_OUTPUT(descriptor->PortDescriptors[i]))
						out_count++;
				} else if (LADSPA_IS_PORT_CONTROL(descriptor->PortDescriptors[i])) {
				}
			}

			bool can_stereo=(in_count == 2 && out_count == 2);

			LADSPA_Struct *plugin_data= new LADSPA_Struct;
			plugin_data->path=lib_name;
			plugin_data->index=k;
			SoundPluginInfo &info=plugin_data->plugin_info;

			info.caption=descriptor->Name;
			info.description="LADSPA Plugin";
			info.long_description="LADSPA Plugin Info:\n Name: "+String(descriptor->Name) +"\n ID: "+ String::num(descriptor->UniqueID) + "\n Author: " + String(descriptor->Maker) + "\n Copyright: " + String(descriptor->Copyright);
			info.unique_ID="LADSPA_"+String::num(descriptor->UniqueID);
			info.category="LADSPA"; 
		
			info.can_custom_channels=(in_count<=2 && out_count<=2);
			if (can_stereo) {
				
				info.custom_channels.push_back(2); //only this
			} 
			
			info.has_internal_UI=false; 
			info.is_synth=false;
			info.xpm_preview=(const char**)ladspa_xpm;
			info.creation_func=&LADSPA_SoundPluginSource::create_ladspa_plugin;
			info.version=1;
			
			
			plugin_list.push_back(plugin_data);
		}
		dlclose(library);
	}

	closedir(dir);
}

LADSPA_SoundPluginSource::LADSPA_SoundPluginSource() {
	ERR_FAIL_COND(singleton!=NULL);
	singleton=this;
	char *ladspa_path;

	ladspa_path= getenv("LADSPA_PATH");

	if (!ladspa_path) {
		scan_plugins("/usr/lib/ladspa");
		scan_plugins("/usr/local/lib/ladspa");
		scan_plugins("/opt/ladspa/lib");
	} else {
		scan_plugins(ladspa_path);
	}

#ifdef MACOSX_ENABLED
	scan_plugins("../Resources/ladspa");
	scan_plugins("~/Library/Application Support/ladspa");
	scan_plugins("/Library/Application Support/ladspa");
	scan_plugins("/System/Library/Application Support/ladspa");
#endif

	Sorter sorter;
	std::sort(plugin_list.begin(),plugin_list.end(),sorter);	
	
	for (int i=0;i<plugin_list.size();i++) {
		
		SoundPluginList::get_singleton()->add_info(&plugin_list[i]->plugin_info);		
	}
}


LADSPA_SoundPluginSource::~LADSPA_SoundPluginSource()
{
	
	for (int i=0;i<plugin_list.size();i++)
		delete plugin_list[i];
}


}

#endif
