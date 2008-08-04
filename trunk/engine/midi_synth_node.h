//
// C++ Interface: midi_synth_node
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MIDI_SYNTH_NODE_H
#define MIDI_SYNTH_NODE_H


#include "engine/hl_audio_node.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class MIDI_SynthNode : public HL_AudioNode {
protected:
class Voice;
private:
	char _cc[MIDI_CC_MAX]; // midi controls
 	float _pitch_bend; // pitch bend		
	void process(const ProcessInfo& p_info);
	
	std::vector<Voice*> voices;
		
	HL_ControlPort voice_level;
		
	void reset();
protected:
	
	class Voice {
		friend class MIDI_SynthNode;
		
		struct Internal {
		
			char event_note; // Note from the midi event, for reference (note off tracking)
			char event_velocity; // velocity from the midi event
			
			float note; // curent note (may have been transposed)
			
			bool active; // in use
			bool off; // note off off
			
			MIDI_SynthNode * synth;
			
			void reset() { off=false; active=false; note=0; event_note=0; event_velocity=0; }
			
			Internal() { reset(); }
		} _internal;		
	protected:		
		
		inline bool is_off() const { return _internal.off; }
		float get_amplitude() const; // voice amp, doesn't include expression and velocity
		float get_total_amplitude() const; // voice amp, includes expression and velocity
		inline int get_event_note() const { return _internal.event_note; }
		inline int get_event_velocity() const { return _internal.event_velocity; }
		inline int get_note() const { return _internal.note; }
		inline int get_CC(int p_cc_idx) const { if (p_cc_idx<0 || p_cc_idx>127) { return 0; } else return _internal.synth->_cc[p_cc_idx]; }
		inline int get_channels() const { return _internal.synth->get_channels(); }
		inline float get_mix_rate() const { return _internal.synth->get_mix_rate(); }
		
		inline MIDI_SynthNode *get_synth() const { return _internal.synth; }
	public:	
		
		enum Event {
			NOTE_ON,
			NOTE_OFF,
			TERMINATE,
		};
		
		virtual void event(Event p_event)=0;	
		virtual void process(int p_frames,sample_t **p_out_buffers)=0;	
		
		virtual float get_priority() const=0; ///< 1 is max priority, 0 means it finished
		
		virtual void mix_rate_changed() {}
		
		virtual ~Voice() {}
		
	};
		

	void register_voices(std::vector<Voice*>& p_voices);
	virtual void event_cc_user(char p_cc, char p_param) {} // cc (user)
	virtual void event_pch_user(char p_program) {} // program change (user)
	
	
public:

    MIDI_SynthNode(int p_channels,const AudioNodeInfo *p_info);
    ~MIDI_SynthNode();

};

#endif
