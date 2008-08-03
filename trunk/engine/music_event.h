//
// C++ Interface: music_event
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MUSIC_EVENT_H
#define MUSIC_EVENT_H

/**
	@author Juan Linietsky <reduzio@gmail.com>
 * Music Event Structure.
 * Must fit inside 32 bits
*/

enum MIDI_CC_List {

	MIDI_CC_BANK_SELECT_MSB=0,
	MIDI_CC_MODULATION=1,
	MIDI_CC_BREATH=2,
	MIDI_CC_FOOT=4,
	MIDI_CC_PORTAMENTO_TIME=5,
	MIDI_CC_DATA_ENTRY_MSB=6,
	MIDI_CC_MAIN_VOLUME=7,
	MIDI_CC_PAN=10,
	MIDI_CC_EXPRESSION=11,
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

};


struct MusicEvent {
public:

	enum Type {
		
		NONE=0x0,
		SEQ_TEMPO=0x1,
		SEQ_SIGNATURE=0x2,
		SEQ_BAR=0x3,
		SEQ_BEAT=0x4,
		SEQ_SCALE=0x5,
		STREAM_TAIL=0x7, // end of stream, for stream buffers
		MIDI_NOTE_OFF=0x8,
		MIDI_NOTE_ON=0x9,
		MIDI_NOTE_PRESSURE=0xA,
		MIDI_CONTROLLER=0xB,
		MIDI_PATCH_SELECT=0xC,
		MIDI_AFTERTOUCH=0xD, //channel pressure
		MIDI_PITCH_BEND=0xE,
		MIDI_SYSEX=0xF, //this will not be used here for now anway
	};

	unsigned int frame;
	
	unsigned char type;
	unsigned char channel; // 0 - 15

	union {

		struct {
			unsigned char param1;
			unsigned char param2;
		} raw;
		struct { /* raw, 2 bytes */
			unsigned short param;
		} raw2;

		struct { /* Note On / Note Off / Note Pressure */

			unsigned char note;
			unsigned char velocity;
		} note;

		struct { /* Controller */

			unsigned char index;
			unsigned char parameter;
		} control;

		struct { /* Channel Pressure */

			unsigned char pressure;
		} aftertouch;

		struct { /* Patch */

			unsigned char index;
		} patch;

		struct { /* Pitch Bend */

			unsigned short bend; /* 0 - 0x3999 */
		} pitch_bend;

		struct {
		
			unsigned short tempo;
		} tempo;
		
		struct {
		
			unsigned char num;
			unsigned char denom;
		} signature;
		
		struct {
		
			unsigned short bar; // warning, max is  65535, It's a high number but may roll around
		} bar;
		struct {
		
			unsigned char beat;
		} beat;
		struct {
		
			enum ScaleType {
				SCALE_MAJOR,
				SCALE_MINOR,
				/* Will have add more later */
			};
	
			unsigned char scale_type;
			char key_note; /* 0 .. 11 */
		} scale;

	};	

	MusicEvent();
	MusicEvent(Type p_type, unsigned char p_chan,unsigned char data1, unsigned char data2 );
	
	~MusicEvent();

};

#endif
