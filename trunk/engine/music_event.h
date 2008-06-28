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
