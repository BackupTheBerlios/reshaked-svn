
//
// C++ Implementation: midi_synth_node
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "midi_synth_node.h"
#include "dsp/formulas.h"


float MIDI_SynthNode::Voice::get_total_amplitude() const {

	return ( get_event_velocity() / 127.0 ) * ( get_CC( 7 ) / 127.0 ) * ( get_CC( 11 ) / 127.0 ) * db_to_energy( _internal.synth->voice_level() );
}

float MIDI_SynthNode::Voice::get_amplitude() const {


	return  ( get_CC( 7 ) / 127.0 ) * db_to_energy( _internal.synth->voice_level() );

}
		

void MIDI_SynthNode::process(const ProcessInfo& p_info) {

	MusicEvent *ebuff = get_event_buffer( PORT_IN, 0 );

	
	for (int e=0;e<p_info.event_buffer_size;e++) {
	
		MusicEvent *ev = &ebuff[e];
		if ( ev->type==MusicEvent::STREAM_TAIL )
			break;
	
		//printf("event type %x\n",ev->type);
		switch( ev->type ) {
		
			case MusicEvent::MIDI_NOTE_ON: {
			
				Voice *best_voice=NULL;
				float best_voice_prio=1; 
				
				for(int i=0;i<voices.size();i++) {
				
					if (!voices[i]->_internal.active) {
						best_voice=voices[i];
						break;						
					}
					if (voices[i]->get_priority()<best_voice_prio) {
						best_voice=voices[i];
						best_voice_prio=voices[i]->get_priority();
						if( best_voice_prio== 0 );
							break; // can be replaced
					}
					
				}
				
				//printf("best voice is %p\n",best_voice);
				
				if (!best_voice)
					break; // no go, out of voices
									
				best_voice->_internal.active=true;
				best_voice->_internal.note=ev->note.note;
				best_voice->_internal.event_note=ev->note.note;
				best_voice->_internal.event_velocity=ev->note.velocity;
				best_voice->_internal.off=false;
				best_voice->event( Voice::NOTE_ON );				
			} break;
			case MusicEvent::MIDI_NOTE_OFF: {
				
				for(int i=0;i<voices.size();i++) {
				
					if (!voices[i]->_internal.active)
						continue;
					if (voices[i]->_internal.event_note!=ev->note.note)
						continue;
						
					voices[i]->_internal.off=true;
					voices[i]->event( Voice::NOTE_OFF );
						
				}
			} break;			
			case MusicEvent::MIDI_NOTE_PRESSURE: {
			
			} break;
			case MusicEvent::MIDI_CONTROLLER: {
			
				int cc = ev->control.index;
				if (cc<0 || cc>127)
					break;
					
				_cc[cc]=ev->control.parameter;
				
				event_cc_user( ev->control.index, ev->control.parameter );
			} break;
			case MusicEvent::MIDI_PATCH_SELECT: {
			
				event_pch_user(  ev->patch.index );
			} break;
			case MusicEvent::MIDI_AFTERTOUCH: {
			
			} break;
			case MusicEvent::MIDI_PITCH_BEND: {
			
				_pitch_bend=(float)(0x2000-ev->pitch_bend.bend)/(float)0x2000;
			} break;
		}
	}

	/* process */
	
	sample_t *buffers[8];
	
	for (int i=0;i<get_channels();i++) {
	
		buffers[i]=get_audio_buffer( PORT_OUT, 0, i );
		memset(buffers[i],0,sizeof(sample_t)*p_info.audio_buffer_size); // clear
	}

		
	for(int i=0;i<voices.size();i++) {
	
		if (!voices[i]->_internal.active)
			continue;					
			
		if (voices[i]->get_priority()==0) {
		
			voices[i]->event( Voice::TERMINATE );
			voices[i]->_internal.active=false;
			continue;
		}
			
		voices[i]->process( p_info.audio_buffer_size, buffers );
	}

}

void MIDI_SynthNode::register_voices(std::vector<Voice*>& p_voices) {

	voices=p_voices;
	for (int i=0;i<voices.size();i++)
		voices[i]->_internal.synth=this;
}

void MIDI_SynthNode::reset() {

	for (int i=0;i<MIDI_CC_MAX;i++) {
	
		char param=0;
		switch(i) {
		
			case MIDI_CC_MAIN_VOLUME: param=127; break;
			case MIDI_CC_PAN: param=64; break;
			case MIDI_CC_EXPRESSION: param=127; break;
#if 0			
			MIDI_CC_BANK_SELECT_LSB=32,
			MIDI_CC_DATA_ENTRY_LSB=38,
			MIDI_CC_DAMPER_PEDAL_TOGGLE=64,
			MIDI_CC_PORTAMENTO_TOGGLE=65,
			MIDI_CC_SOSTENUTO_TOGGLE=66,
			MIDI_CC_SOFT_PEDAL_TOGGLE=67,
			MIDI_CC_FILTER_CUTOFF=71,
			MIDI_CC_RELEASE_TIME=72,
			MIDI_CC_ATTACK_TIME=73,
			MIDI_CC_FILTER_RESONANCE=74,
			MIDI_CC_DECAY_TIME=75,
			MIDI_CC_VIBRATO_DEPTH=76,
			MIDI_CC_VIBRATO_RATE=77,
			MIDI_CC_VIBRATO_DELAY=78,
			MIDI_CC_PORTAMENTO_CONTROL=84,
			MIDI_CC_REVERB_SEND=91,
			MIDI_CC_FX2_SEND=92,
			MIDI_CC_CHORUS_SEND=93,
			MIDI_CC_FX4_SEND=94,
			MIDI_CC_DATA_INCREMENT=96,
			MIDI_CC_DATA_DECREMENT=97,
			MIDI_CC_NRPN_LSB=98,
			MIDI_CC_NRPN_MSB=99,
			MIDI_CC_RPN_LSB=98,
			MIDI_CC_RPN_MSB=99,
			MIDI_CC_ALL_SOUNDS_OFF_CMD=120,
			MIDI_CC_RESET_ALL_CC_CMD=121,
			MIDI_CC_LOCAL_CTRL_TOGGLE=122,
			MIDI_CC_ALL_NOTES_OFF=123,
			MIDI_CC_MAX=128
#endif			
			
			default: {}
		}
		
		_cc[i]=param;
	}
}


MIDI_SynthNode::MIDI_SynthNode(int p_channels,const AudioNodeInfo *p_info)  : HL_AudioNode(p_channels,p_info) {

	add_event_port("MIDI IN",PORT_IN);
	add_audio_port("Output",PORT_OUT,p_channels);
	voice_level.set_all(-12,-60,24,0.01,ControlPort::HINT_RANGE,"VoiceLevel","dB");
	add_control_port( PORT_IN, &voice_level );
	
	_pitch_bend=0;	
	
	reset();
}


MIDI_SynthNode::~MIDI_SynthNode()
{
	
	for (int i=0;i<voices.size();i++) {
	
		delete voices[i];
	}
}


