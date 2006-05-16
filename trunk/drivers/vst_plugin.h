//
// C++ Interface: vst_plugin
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDVST_PLUGIN_H
#define RESHAKEDVST_PLUGIN_H

#include "version.h"
#ifdef VST_ENABLED

#include "engine/sound_plugin.h"
#include "engine/midi_parameters.h"

#include "typedefs.h"
#include "aeffectx.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream.h>
#include <fstream.h>
#include <string.h>
namespace ReShaked {

/**
	@author red <red@killy>
 */
class VST_Plugin : public SoundPlugin {
			    
	
	enum {
		
		MAX_INPUT_EVENTS=1024
	};
	
	
	class Parameter: public Property {
	public:		
		
		char resbuff[256];
		AEffect* ptrPlug; //plugin
		int index; //parameter index
		bool write_only;
		
		double get();
		void set(double p_val);
		double get_stepping();
		double get_min();
		double get_max();
		double get_default();
		String get_name();
		String get_caption();
		String get_postfix();
		String get_text_value(double p_for_value,bool p_no_postfix=false);
		bool has_text_value();
		bool is_write_only() { return write_only; }
	
		DisplayMode get_display_mode();
	
		/* helpers */	
		String get_text_value(bool p_no_postfix=false); 
	 
		Parameter(AEffect *p_ptrPlug,int p_index,bool p_write_only=false) { ptrPlug=p_ptrPlug; index=p_index; write_only=p_write_only; }
	};
	
	std::vector<AudioPlug*> input_plugs;
	std::vector<AudioPlug*> output_plugs;	
	
	float **input_buffers;
	float **output_buffers;
	
	std::vector<Property*> param_list;
	std::vector<Property*> property_list;
	
	bool enabled;
	HINSTANCE libhandle;
	AEffect* ptrPlug;
	
	bool off_all_notes;
	static VstIntPtr VSTCALLBACK host(AEffect *effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void *ptr, float opt);	
	
	float mix_rate;
	LocalProperty gain;
	/* input events stuff */
	
	VstMidiEvent *event_array;
	VstEvents *event_pointers;
	
	MidiParameters *midi_parameters; //midi parameters
	
	/* UI Edited notification */
	
	void *userdata;
	void (*property_changed)(void*,int);

	
	String plugin_dir;
public:

	MidiParameters *get_midi_parameters() { return midi_parameters; }
	AEffect* get_ptrPlug() { return ptrPlug; }
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
	
	void set_property_changed_callback(void *p_userdata,void (*property_changed)(void*,int));

	/* Setting up */
	void set_mixing_rate(float p_mixing_rate);
	void save(TreeSaver *p_saver);	
	void load(TreeLoader *p_saver);	
	/* Processing */
	void process(int p_frames);
	
	void reset();

	VST_Plugin(const SoundPluginInfo *p_info,String p_path,String p_dir,bool p_ports_write_only,int p_channels);
	~VST_Plugin();

};

}

#endif
#endif