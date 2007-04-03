
//
// C++ Implementation: midi_synth
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "midi_synth.h"
#include <math.h>
#include "dsp/formulas.h"
namespace ReShaked {


/* The following values are hand-tuned for ease of use */
//max semitones/sec that the portamento can do
#define PORTA_ST_SEC 160.0
//curve shape of the portamento
#define PORTA_POWER 2.5


/***************** VOICE ************************/


void MidiSynth::Voice::reset_internal() {
	
	
	internal.off=false;
	internal.active=false;
	internal.note_fine=0;
	internal.velocity=0;
	internal.note=0;
	internal.original_note=0;
	internal.total_amplitude=0;
		
}

bool MidiSynth::Voice::is_off() {
	
	return internal.off;
}

float MidiSynth::Voice::get_mix_rate() {
	
	return internal.mix_rate;
}

float MidiSynth::Voice::get_current_note() {
	
	return internal.current_note;
}
float MidiSynth::Voice::get_total_amplitude() {
	
	return internal.total_amplitude;
}
int MidiSynth::Voice::get_original_note() {
	
	return internal.original_note;	
}
int MidiSynth::Voice::get_velocity() {
	
	
	return internal.velocity;	
}
int MidiSynth::Voice::get_buffer_count() {
	
	return internal.buffers->size();
	
}

float MidiSynth::Voice::get_expression() {

	return internal.expression;

}

/*
float *MidiSynth::Voice::get_buffer(int p_buff) {
	
	ERR_FAIL_INDEX_V(p_buff,buffers->size(),NULL);
	return (*buffers)[p_buff];

}
*/



/************************************************/

void MidiSynth::process_voice_internal(int p_voice,int p_frames) {
	Voice &v=*voice_pool[p_voice];
	
	v.internal.total_amplitude=main_volume.get()*expression.get()*((float)v.internal.velocity/127.0)*db_to_energy( gain.get() );
	
	v.internal.current_note=v.internal.note;
	v.internal.current_note+=pitch_bend.get()*2.0; //should change pbdepth
	v.internal.current_note+=lfo_depth;
	v.internal.current_note+=finetune.get();
	v.internal.expression=expression.get();
	
	
}

void MidiSynth::terminate_voice(ActiveVoiceList::iterator p_it) {
	
	voice_pool[*p_it]->event(Voice::TERMINATE);	
	voice_pool[*p_it]->internal.active=false;
	active_voices.erase(p_it);	
}

int MidiSynth::get_channels() {
	
	return buffers.size();;		
}
void MidiSynth::set_buffer(int p_channel,float *p_buffer) {
	
	ERR_FAIL_INDEX(p_channel,buffers.size());
	buffers[p_channel]=p_buffer;
		
	
}

void MidiSynth::control_note_on(char p_note,char p_velocity,signed char p_note_fine) {
	
	int voice_idx=-1;
	
	if (portamento.get()>0.0001 && last_voice_idx>=0) { //portamento enabled
	
		if (voice_pool[last_voice_idx]->internal.active) {
			/* only do portamento to an active voice */
			portamento_data.voice_idx=last_voice_idx;
			portamento_data.target_base_note=p_note;
			voice_pool[last_voice_idx]->internal.original_note=p_note; //so it can be off-ed properly
			return; 
			
		}
		
	} if (mono_mode.get()>0.5) { // mono mode
	
		stop_all_voices();
		
	} else if (duplicate_check.get()>0.5) { // duplicate check, 0 means, let two of the many note happen 1 means, silence duplicate
		foreach(I,active_voices) {
				
			if (voice_pool[*I]->internal.original_note==p_note) {
				voice_idx=*I; //reuse the same voice, gain some speed
				terminate_voice(I);
				break;			
			}
		}
	}

	if (voice_idx==-1) {	
		for (int i=0;i<voice_pool.size();i++) {
			
			if (voice_pool[i]->internal.active)
				continue;
			
			voice_idx=i;
			break;
		}
	}
	
	if (voice_idx==-1) { //all voices active!?
		float min_prio=1;
		ActiveVoiceList::iterator v;
		
		foreach(I,active_voices) {
				
			float priority=voice_pool[*I]->priority();
			if (priority<min_prio) {
				
				v=I;
				min_prio=priority;
				if (min_prio==0.0f)
					break; //couldnt be less than this
			}
				
			
		}
		
		voice_idx=*v;
		terminate_voice(v);
	}
	
	

	//printf("decided on voice %i for %i:%i\n",voice_idx,p_note,p_velocity);
	voice_pool[voice_idx]->reset_internal();
	voice_pool[voice_idx]->internal.original_note=p_note;
	voice_pool[voice_idx]->internal.note=(float)p_note+transpose.get();
	voice_pool[voice_idx]->internal.velocity=int( ((float)p_velocity)*expression.get() );
	voice_pool[voice_idx]->internal.mix_rate=mix_rate;
	voice_pool[voice_idx]->internal.active=true;
	voice_pool[voice_idx]->event(Voice::NOTE_ON);
	
	active_voices.push_back(voice_idx);
	
	last_voice_idx=voice_idx;
	portamento_data.voice_idx=-1;


}
void MidiSynth::control_note_off(char p_note) {
	
	foreach(I,active_voices) {
				
		if (voice_pool[*I]->internal.original_note==p_note && !voice_pool[*I]->internal.off) {
			
			voice_pool[*I]->event(Voice::NOTE_OFF);
			voice_pool[*I]->internal.off=true;
		}
	}
		
}

void MidiSynth::stop_all_voices() {
	
	for (int i=0;i<voice_pool.size();i++) {
		
		
		voice_pool[i]->internal.active=false;
	}
	active_voices.clear();
	
	
}

void MidiSynth::reset() {
	
	stop_all_voices();
	
	pitch_bend.set(0);
	modulation_depth.set(0);
	
	last_voice_idx=-1;
	portamento_data.voice_idx=-1;	

}

void MidiSynth::process(int p_frames) {
	
		
	/* LFO computation */
			
	lfo_pos+=(float)p_frames*modulation_speed.get()/mix_rate;
	lfo_pos=fmodf(lfo_pos,1.0);
	lfo_depth=modulation_depth.get()*sin(lfo_pos*M_PI*2.0);
	
	/* Portamento computation */
	
	if (portamento.get()>0.0001 && portamento_data.voice_idx>=0 && voice_pool[portamento_data.voice_idx]->internal.active) {
		/* seems ok to use portamento */	
		
		float &src_note=voice_pool[portamento_data.voice_idx]->internal.note;
		float dst_note=portamento_data.target_base_note;
		
		float time=(float)p_frames/mix_rate; //fraction of a second
		float increment=pow(portamento.get(),PORTA_POWER)*time*PORTA_ST_SEC; //1 semitone / second max?
		
		
		if ( fabs(src_note-dst_note)<increment) { //cant portamento anymore!
			
			src_note=dst_note;
		} else if (src_note<dst_note) {
			
			src_note+=increment;
		} else if (src_note>dst_note) {
			
			src_note-=increment;
		}
	
	}

	
	/* VOICEs computation */
	
	int active_voice_count=0;
	for(ActiveVoiceList::iterator I=active_voices.begin();I!=active_voices.end();) {
				

		ActiveVoiceList::iterator N=I;
		N++;
		
		process_voice_internal(*I,p_frames);
		voice_pool[*I]->process(p_frames);
		if (voice_pool[*I]->priority()==0.0f) {
			
			terminate_voice(I);	
		}
		
		I=N;
		
		active_voice_count++;
	}
	
	
	static int last_voice_count=0;
	//if (last_voice_count!=active_voice_count) 
	//	printf("there were %i active voices, size %i\n",active_voice_count,active_voices.size());
	
	last_voice_count=active_voice_count;
	voice_count.set(active_voice_count);
	
	
	frames_mixed+=p_frames;
		
}

void MidiSynth::set_mix_rate(float p_mix_rate) {
	
	mix_rate=p_mix_rate;
}


int MidiSynth::get_input_property_count() {
	
	return input_properties.size();	
}
int MidiSynth::get_output_property_count() {
	
	return output_properties.size();	
	
}
Property* MidiSynth::get_input_property(int p_index) {
	
	ERR_FAIL_INDEX_V(p_index,input_properties.size(),NULL);
	return input_properties[p_index];
	
}
Property* MidiSynth::get_output_property(int p_index) {
	
	ERR_FAIL_INDEX_V(p_index,output_properties.size(),NULL);
	return output_properties[p_index];
	
}

void MidiSynth::add_input_property(Property *p_prop) {
	
	input_properties.push_back(p_prop);	
}

void MidiSynth::add_output_property(Property *p_prop) {
	
	output_properties.push_back(p_prop);	
	
}
	
MidiSynth::MidiSynth(int p_channels,std::vector<Voice*> p_voice_pool) /*: active_voices(p_voice_pool.size())*/ {
	
	buffers.resize(p_channels);
	voice_pool=p_voice_pool;
	for (int i=0;i<voice_pool.size();i++) 
		voice_pool[i]->internal.buffers=&buffers;
	
	
	expression.set_all(1,0,1,1,0.01,Property::DISPLAY_KNOB,"expression","Control/Expression","","Silence","Max");
	main_volume.set_all(1,0,1,1,0.01,Property::DISPLAY_KNOB,"main_volume","Control/Main Volume","","Silence","Max");
	pitch_bend.set_all(0,-1,1,0,0.01,Property::DISPLAY_KNOB,"pitch_bend","Control/Pitch Bend","","Min","Max");
	portamento.set_all(0,0,1,0,0.01,Property::DISPLAY_KNOB,"portamento","Control/Portamento","","Off");
	modulation_speed.set_all(5,0.1,10,5,0.1,Property::DISPLAY_KNOB,"modulation_speed","Control/Modulation Speed","hz");
	modulation_depth.set_all(0,0,2,0,0.1,Property::DISPLAY_KNOB,"modulation_depth","Control/Modulation Depth","st");
	mono_mode.set_all(0,0,1,0,1,Property::DISPLAY_CHECKBOX,"mono","Switch/Mono Mode","","Off","On");
	sustain.set_all(0,0,1,0,1,Property::DISPLAY_CHECKBOX,"sustain","Switch/Sustain","","Off","On");
	duplicate_check.set_all(0,0,1,0,1,Property::DISPLAY_CHECKBOX,"duplicate_check","Switch/Duplicate Check","","Off","On");
	transpose.set_all(0,-24,24,0,1,Property::DISPLAY_KNOB,"transpose","Control/Transpose","nt","-2 Oct","+2 Oct");
	finetune.set_all(0,-1,1,0,0.01,Property::DISPLAY_KNOB,"finetune","Control/Finetune","cnt","-st","+st");

	gain.set_all( -24, -60, 24, 0, 0.1, Property::DISPLAY_SLIDER, "global_gain","Gain","dB");	
	
	add_input_property(&expression);
	add_input_property(&main_volume);
	add_input_property(&pitch_bend);
	add_input_property(&portamento);
	add_input_property(&modulation_speed);
	add_input_property(&modulation_depth);
	add_input_property(&mono_mode);
	//add_input_property(&sustain); //not in here for now
	add_input_property(&transpose);
	add_input_property(&finetune);
	add_input_property(&duplicate_check);
	add_input_property(&gain);
	
	
	voice_count.set_all(0,0,p_voice_pool.size(),0,1,Property::DISPLAY_KNOB,"voice_count","Info/Voice Count","","None","Max");
	
	add_output_property(&voice_count);
	
	mix_rate=1;
	frames_mixed=0;
	
	lfo_pos=0;
	lfo_depth=0;
	
	last_voice_idx=-1;
	portamento_data.voice_idx=-1;	
	portamento_data.target_base_note=0;
}
MidiSynth::~MidiSynth() {
	
	for (int i=0;i<voice_pool.size();i++) 
		delete voice_pool[i];
		
		
}


}
