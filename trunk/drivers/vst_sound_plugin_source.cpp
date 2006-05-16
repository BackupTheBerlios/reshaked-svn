//
// C++ Implementation: vst_sound_plugin_source
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "vst_sound_plugin_source.h"

#include "version.h"

#ifdef VST_ENABLED

#include <dirent.h>
#include<algorithm>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream.h>
#include <fstream.h>
#include <string.h>

#include "engine/sound_plugin_list.h"
#include "vst_plugin.h"

#include "vst.xpm"
namespace ReShaked {

VST_SoundPluginSource * VST_SoundPluginSource::singleton=NULL;
/* Path to VSTs */
SoundPlugin* VST_SoundPluginSource::create_vst_plugin(const SoundPluginInfo *p_info,int p_channels) {
	
	printf("Attempt to instance\n");
	for (int i=0;i<singleton->plugin_list.size();i++) {
		
		if (&singleton->plugin_list[i]->plugin_info!=p_info) {
			printf("skipped on %s\n",singleton->plugin_list[i]->plugin_info.caption.ascii().get_data());
			continue;
		}
		printf("instancing plugin.. info: %p , path %s, chans %i\n",p_info,singleton->plugin_list[i]->path.ascii().get_data(),p_channels);
		return new VST_Plugin(	p_info, singleton->plugin_list[i]->path, singleton->plugin_list[i]->dir, singleton->plugin_list[i]->write_only,p_channels);

		
	}
	return NULL;
	
	
	//return new Simpler(p_info,p_channels);
}

void VST_SoundPluginSource::scan_path(String p_path) {

	DIR *dir;
	struct dirent *dirent;

	dir= opendir(p_path.utf8().get_data());
	if (dir == NULL) return;

	while ((dirent= readdir(dir))) {


		char lib_name[PATH_MAX];
		snprintf(lib_name, PATH_MAX, "%s\\%s", p_path.utf8().get_data(), dirent->d_name);
		
		
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

		
		printf("Scanning %s\n",lib_name);
		
		
		
		
		HINSTANCE libhandle=LoadLibrary(lib_name);

		if (libhandle==NULL) {
			printf("invalid file: %s\n",lib_name);
			continue;
		}
		
		AEffect* (__cdecl* getNewPlugInstance)(audioMasterCallback);
		getNewPlugInstance=(AEffect*(__cdecl*)(audioMasterCallback))GetProcAddress(libhandle, "main");

		if (getNewPlugInstance==NULL) {
			FreeLibrary(libhandle);
			WARN_PRINT("Can't find symbol 'main'");
			continue;
		}
		AEffect* ptrPlug=getNewPlugInstance(&VST_SoundPluginSource::host);

		if (ptrPlug==NULL) {
			WARN_PRINT("Can't instance plugin.");
			
			FreeLibrary(libhandle);
			continue;
		}
		
		if (ptrPlug->magic!=kEffectMagic) {
			WARN_PRINT("Can't instance plugin, corrupted");
			FreeLibrary(libhandle);
			continue;
			
		}
		ptrPlug->dispatcher(ptrPlug,effOpen,0,0,NULL,0.0f);
		
		VST_Struct *plugin_data= new VST_Struct;
		plugin_data->path=lib_name;
		plugin_data->dir=p_path;
		SoundPluginInfo &info=plugin_data->plugin_info;

		String name=dirent->d_name;
		name=name.substr(0,name.find("."));
		info.caption=name;
		info.description="VST Plugin";
		info.long_description="VST Info:\n Name: "+info.caption +"\n ID: "+ String::num(ptrPlug->uniqueID) + "\n Version: " + String(ptrPlug->version);
		info.unique_ID="VST_"+String::num(ptrPlug->uniqueID);
		info.is_synth=/*(ptrPlug->dispatcher(ptrPlug,effGetVstVersion,0,0,NULL,0.0f)==2  */ptrPlug->flags & effFlagsIsSynth;		
		info.category=info.is_synth?"VST Instruments":"VST Effects"; 
		info.can_custom_channels=true;
		info.custom_channels.push_back(1);			
		info.custom_channels.push_back(2);			
		info.custom_channels.push_back(4);			
		info.has_internal_UI=(ptrPlug->flags & effFlagsHasEditor);
		
		info.xpm_preview=(const char**)vst_xpm;
		info.creation_func=&VST_SoundPluginSource::create_vst_plugin;
		info.version=ptrPlug->version;		

		if (ptrPlug->flags & effFlagsProgramChunks) {
			
			info.description+=" (CS)";
		}
		
		/* Perform the "write only" test */
	
		plugin_data->write_only=true;	//i cant really be certain of anything with VST plugins, so this is always true
		/*
		if (ptrPlug->numParams) {
			
			ptrPlug->setParameter(ptrPlug,0,1.0); //set 1.0
			float res=ptrPlug->getParameter(ptrPlug,0);
			if (res<0.8) { //try if it's not near 1.0, with some threshold, then no reading (far most of the ones that dont support this will just return 0)
				

			}
	} */
		
		plugin_list.push_back(plugin_data);
		
		ptrPlug->dispatcher(ptrPlug,effClose,0,0,NULL,0.0f);
		FreeLibrary(libhandle);

	}
}


VstIntPtr VSTCALLBACK VST_SoundPluginSource::host(AEffect *effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void *ptr, float opt)
{	
	long retval=0;

	switch (opcode)
	{
		//VST 1.0 opcodes
		case audioMasterVersion:
			//Input values:
			//none

			//Return Value:
			//0 or 1 for old version
			//2 or higher for VST2.0 host?
			retval=2;
			break;
		case audioMasterGetSampleRate:			
			
			effect->dispatcher(effect,effSetSampleRate,0,0,NULL,44100); //just crap
			break;			
		case audioMasterGetBlockSize:
			//Input Values:
			//None

			//Return Value:
			//not tested, always return 0

			//NB - Host must despatch effSetBlockSize to the plug in response
			//to this call
			//Check despatcher notes for any return codes from effSetBlockSize
			effect->dispatcher(effect,effSetBlockSize,0,1024,NULL,0.0f);

			break;			
		case audioMasterCanDo:
			//Input Values:
			//<ptr> predefined "canDo" string

			//Return Value:
			//0 = Not Supported
			//non-zero value if host supports that feature

			//NB - Possible Can Do strings are:
			//"sendVstEvents",
			//"sendVstMidiEvent",
			//"sendVstTimeInfo",
			//"receiveVstEvents",
			//"receiveVstMidiEvent",
			//"receiveVstTimeInfo",
			//"reportConnectionChanges",
			//"acceptIOChanges",
			//"sizeWindow",
			//"asyncProcessing",
			//"offline",
			//"supplyIdle",
			//"supportShell"

			if (strcmp((char*)ptr,"sendVstEvents")==0 ||
						 strcmp((char*)ptr,"sendVstMidiEvent")==0 ||
						 strcmp((char*)ptr,"supplyIdle")==0)
			{
				retval=1;
			}
			else
			{
				retval=0;
			}

			break;			
		case audioMasterGetLanguage:
			//Input Values:
			//None

			//Return Value:
			//kVstLangEnglish
			//kVstLangGerman
			//kVstLangFrench
			//kVstLangItalian
			//kVstLangSpanish
			//kVstLangJapanese
			
			retval=kVstLangEnglish;
			break;			
			
			
	}
	return retval; //stupid plugin, i'm just reading stuff, dont annoy me with questions!
};


void VST_SoundPluginSource::add_path(String p_path) {
	ERR_FAIL_COND(singleton==NULL);
	singleton->path_list.push_back(p_path);
}

void VST_SoundPluginSource::scan_plugins() {
	
	for (int i=0;i<path_list.size();i++) {
		
		scan_path( path_list[i] );
	}
	
	Sorter sorter;
	std::sort(plugin_list.begin(),plugin_list.end(),sorter);	
	
	for (int i=0;i<plugin_list.size();i++) {
		
		SoundPluginList::get_singleton()->add_info(&plugin_list[i]->plugin_info);		
	}	
}


VST_SoundPluginSource::VST_SoundPluginSource() {
	singleton=this;
	
}


VST_SoundPluginSource::~VST_SoundPluginSource()
{
}


}

#endif
