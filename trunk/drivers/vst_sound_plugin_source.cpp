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
		return new VST_Plugin(	p_info, singleton->plugin_list[i]->path, p_channels);

		
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
		
		int vstchans=1;
		
		//printf("VSTPLUG: IN %i, OUT %i\n",ptrPlug->numInputs,ptrPlug->numOutputs);
		/* Hackish thing to determine port grouping */
		if (ptrPlug->numInputs%4==0 &&  ptrPlug->numOutputs%4==0) {
			
			vstchans=4;
			
		} else if (ptrPlug->numInputs%2==0 &&  ptrPlug->numOutputs%2==0) {
			
			vstchans=2;
		} else {
			
			vstchans=1;
		}
		
		VST_Struct *plugin_data= new VST_Struct;
		plugin_data->path=lib_name;
		SoundPluginInfo &info=plugin_data->plugin_info;

		String name=dirent->d_name;
		name=name.substr(0,name.find("."));
		info.caption=name;
		info.description="VST Plugin";
		info.long_description="VST Info:\n Name: "+info.caption +"\n ID: "+ String::num(ptrPlug->uniqueID) + "\n Version: " + String(ptrPlug->version);
		info.unique_ID="VST_"+String::num(ptrPlug->uniqueID);
		info.category="VST"; 
		info.can_custom_channels=true;
		info.custom_channels.push_back(vstchans);			
		info.has_internal_UI=(ptrPlug->flags & effFlagsHasEditor);
		printf("internal UI: %i\n",ptrPlug->flags & effFlagsHasEditor);
		info.is_synth=(ptrPlug->dispatcher(ptrPlug,effGetVstVersion,0,0,NULL,0.0f)==2 && ptrPlug->flags & effFlagsIsSynth);
		info.xpm_preview=(const char**)vst_xpm;
		info.creation_func=&VST_SoundPluginSource::create_vst_plugin;
		info.version=ptrPlug->version;		

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


int VST_SoundPluginSource::get_path_count() {
	
	return path_list.size();	
}

void VST_SoundPluginSource::erase_path(int p_index) {
	
	ERR_FAIL_INDEX(p_index,path_list.size());
	path_list.erase( path_list.begin() + p_index );
}
String VST_SoundPluginSource::get_path(int p_index) {
	
	ERR_FAIL_INDEX_V(p_index,path_list.size(),"");
	return path_list[p_index];
	
}
void VST_SoundPluginSource::add_path(String p_path) {
	
	path_list.push_back(p_path);
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
	path_list.push_back("C:\\Documents and Settings\\goruka\\Desktop\\VSTS");
}


VST_SoundPluginSource::~VST_SoundPluginSource()
{
}


}

#endif
