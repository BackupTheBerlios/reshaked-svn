//
// C++ Interface: midi_synth
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDMIDI_SYNTH_H
#define RESHAKEDMIDI_SYNTH_H


#include <vector>
#include "property.h"
#include "prealloc_list.h"
#include <list>
namespace ReShaked {

/**
	@author red <red@killy>
*/
class MidiSynth{

	
public:	
	class Voice {
	friend class MidiSynth;		
	
		struct Internal {
			float mix_rate;
			float total_amplitude; //expression+mainvol+velocity
			char original_note; ///< Note from the midi event, for reference (note off)
			float note; ///< Note after transpose, this can be modified by portamento, too
			int velocity; ///< 0 .. 127 always
			float expression;
			float note_fine;
			std::vector<float*> *buffers;
			bool active;
			bool off; //note off happened
			float current_note; // note+porta+pitchbend+mod
		} internal;		
		void reset_internal();
	protected:		
		
		bool is_off();
		float get_mix_rate();
		float get_total_amplitude(); //expression+mainvol+velocity
		int get_original_note();
		float get_current_note();
		int get_velocity();
		float get_expression();
		int get_buffer_count();
		inline float *get_buffer(int p_buff) { return (*internal.buffers)[p_buff]; }
	
	public:	
		
		enum Event {
			NOTE_ON,
			NOTE_OFF,
			TERMINATE,
		};
		
		virtual void event(Event p_event)=0;	
		virtual void process(int p_frames)=0;	
		
		
		virtual float priority()=0; ///< 1 is max priority, 0 means it finished
		
		
		Voice() { reset_internal(); }
		virtual ~Voice() {}
		
	};
	
	
	
private:		
		
	typedef  PreallocList<int> ActiveVoiceList;	
	
	void terminate_voice(ActiveVoiceList::iterator p_it);
	
	std::vector<float*> buffers;
	
	float mix_rate;	
	std::vector<Voice*> voice_pool;
	

	Sint64 frames_mixed;
	float lfo_depth;
	float lfo_pos;
	
	int last_voice_idx;
	
	struct PortamentoData {
		
		int voice_idx;
		float target_base_note;
		
	} portamento_data;
	
	//typedef ActiveVoiceList PreallocList<int>;
	ActiveVoiceList active_voices;
	
	/* input internal properties */
	LocalProperty expression; //ok
	LocalProperty main_volume; //ok
	LocalProperty pitch_bend; //ok
	LocalProperty portamento; //ok
	LocalProperty modulation_speed; //ok
	LocalProperty modulation_depth; //ook
	LocalProperty mono_mode; //ok
	LocalProperty sustain;
	LocalProperty transpose; //ok
	LocalProperty finetune; //ok
	LocalProperty duplicate_check; //ok
	
	LocalProperty gain; //ok
	
	/* output internal properties */
	LocalProperty voice_count;
	
	void process_voice_internal(int p_voice,int p_frames); 
	
	std::vector<Property*> input_properties;
	std::vector<Property*> output_properties;
	
	void stop_all_voices();
	
public:
	
	int get_channels();
	void set_buffer(int p_channel,float *p_buffer);
	
	void control_note_on(char p_note,char p_velocity,signed char p_note_fine=0);
	void control_note_off(char p_note);
	
	void reset();
	
	void process(int p_frames);
	
	void set_mix_rate(float p_mix_rate);
	
	int get_input_property_count();
	int get_output_property_count();
	Property* get_input_property(int p_index);
	Property* get_output_property(int p_index);
	void add_input_property(Property *p_prop);
	void add_output_property(Property *p_prop);
	
	MidiSynth(int p_channels,std::vector<Voice*> p_voice_pool);
	~MidiSynth();

};

}

#endif
