//
// C++ Interface: sound_driver
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSOUND_DRIVER_H
#define RESHAKEDSOUND_DRIVER_H

#include "typedefs.h"
#include "engine/audio_node.h"
#include "engine/audio_port_layout.h"
#include "engine/audio_graph.h"
#include "engine/audio_process_base.h"
#include "engine/midi_input_handler_base.h"

#include "property.h"
#include <vector>

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class SoundDriver{
		
	class InputNode : public AudioNode {
	friend class SoundDriver;
		SoundDriver *driver;
		std::vector<AudioPlug*> input_plugs;
	public:
		int get_input_plug_count();
		int get_output_plug_count();
		AudioPlug *get_input_plug(int p_index);
		AudioPlug *get_output_plug(int p_index);
		
		bool is_input_plug_enabled(int p_idx);
		bool is_output_plug_enabled(int p_idx);
		String get_input_plug_caption(int p_which);
		String get_output_plug_caption(int p_which);
		bool is_system() { return true; }
		String get_caption();
		
		void process(int p_frames);
		
	};
		
	class OutputNode : public AudioNode {
	friend class SoundDriver;
		SoundDriver *driver;
		std::vector<AudioPlug*> output_plugs;
	public:	
		int get_input_plug_count();
		int get_output_plug_count();
		AudioPlug *get_input_plug(int p_index);
		AudioPlug *get_output_plug(int p_index);
		
		bool is_input_plug_enabled(int p_idx);
		bool is_output_plug_enabled(int p_idx);
		String get_input_plug_caption(int p_which);
		String get_output_plug_caption(int p_which);
		bool is_system() { return true; }
		String get_caption();
		
		void process(int p_frames);

	};
	
	InputNode input_node;
	OutputNode output_node;
	const AudioPortLayout *port_layout;
	AudioGraph *graph;
	AudioProcessBase *audio_process_base;
	MidiInputHandlerBase *midi_input_handler;
protected:	

	/* INTERNAL API */
	int get_input_count();
	AudioPlug *get_input_plug(int p_input);
	int get_output_count();
	AudioPlug *get_output_plug(int p_output);
	const AudioPortLayout *get_port_layout();
	
	/**
	 * Ask to process a given amount of frames.
	 * @param p_frames Amount of frames to process
	 * @return Frames actually processed
	 */
	int process_graph(int p_frames);
	/* OVERRIDES */
	
	virtual void write_to_inputs(int p_frames)=0;
	virtual void read_from_outputs(int p_frames)=0;
	virtual bool is_input_enabled(int p_input)=0;
	virtual bool is_output_enabled(int p_output)=0;
	
public:
	
	
	enum Status {
		STATUS_DISABLED,
		STATUS_IDLE,  
		STATUS_ACTIVE
	};
	
	virtual String get_last_error()=0;
	
	
	virtual int get_settings_count()=0;
	virtual Property *get_setting(int p_setting)=0;
	
	virtual bool uses_file()=0;
	virtual String get_file()=0;
	virtual void set_file(String p_file)=0;
	
	virtual String get_name()=0;
	
	virtual bool init()=0;
	virtual void finish()=0;
	
	virtual Status get_status()=0;
	
	virtual float get_mix_rate()=0;	
	
	virtual int get_output_latency()=0; //get output latency in frames
	
	void set_port_layout(const AudioPortLayout *p_layout);
	void set_audio_graph(AudioGraph *p_graph);
	void set_audio_process_base(AudioProcessBase *p_base);
	void set_midi_input_handler(MidiInputHandlerBase *p_handler);
	
	AudioNode *get_input_node();
	AudioNode *get_output_node();
	
	
	virtual ~SoundDriver();

};

}

#endif
