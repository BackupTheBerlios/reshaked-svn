#ifndef EVENT_H
#define EVENT_H

#include <string.h>
namespace ReShaked {



/**
 * This set of structs may seem a bit hackish, but they are created with the aim that
 * can be used reliably in a realtime environment, without any memory allocation.
 * The idea is that you can just assign easily this instead of copying pointers,
 * given the "smallness" of it.
 */



struct EventSeqTempo  {
	
	float bpm;
	
};

struct EventSeqSignature  {
	
	unsigned char numerator;
	unsigned char denominator;
	
};

struct EventSeqBeat  {

	unsigned char beat; /* beat inside the bar */
	
};

struct EventSeqBar  {

	long int bar; /* bar # */
	
};

struct EventSeqScale  {

	
	enum ScaleType {
		SCALE_MAJOR,
		SCALE_MINOR,
		/* Will have add more later */
	};
	
	ScaleType scale_type;
	char key_note; /* 0 .. 11 */
	
};

struct EventMidi {

	enum MidiType {
		MIDI_NONE, //event to ignore
		MIDI_NOTE_ON,
		MIDI_NOTE_OFF,
		MIDI_NOTE_PRESSURE,
		MIDI_CONTROLLER,
		MIDI_PATCH_SELECT,
		MIDI_AFTERTOUCH, //channel pressure
		MIDI_PITCH_BEND,
		MIDI_SYSEX, //this will not be used here for now anway
	};

	

	MidiType midi_type;
	unsigned char channel; /* 0-15 */

	union Data {

		struct Raw {
			unsigned char param1;
			unsigned char param2;
		} raw;
		struct Raw2 { /* raw, 2 bytes */
			unsigned short param;
		} raw2;

		struct Note { /* Note On / Note Off / Note Pressure */

			unsigned char note;
			unsigned char velocity;
		} note;

		struct Control { /* Controller */

			unsigned char index;
			unsigned char parameter;
		} control;

		struct Aftertouch { /* Channel Pressure */

			unsigned char pressure;
		} aftertouch;

		struct Patch { /* Patch */

			unsigned char index;
		} patch;

		struct Pitch_Bend { /* Pitch Bend */

			unsigned short bend; /* 0 - 0x3999 */
		} pitch_bend;


	} data;
		
};



struct Event {
	
	enum Type {
		TYPE_NONE, /* nothing */
		TYPE_MIDI,
		TYPE_SEQ_TEMPO,
		TYPE_SEQ_BEAT,
		TYPE_SEQ_BAR,
		TYPE_SEQ_SCALE,
		
	};
	
	Type type;

	unsigned long int frame_offset; /* offset in audio frame */
	
	union {
		
		EventMidi midi;
		EventSeqTempo tempo;
		EventSeqSignature signature;
		EventSeqBeat beat;
		EventSeqBar bar;
		EventSeqScale scale;
		
	} param;
	
	Event() { memset(this,0,sizeof(this)); }; /* only reliable initializable way */
};


#define SET_EVENT_MIDI(m_e,m_type,m_chan,m_param_1,m_param_2) {\
	m_e.type=Event::TYPE_MIDI;			       \
	m_e.param.midi.midi_type=m_type;		       \
	m_e.param.midi.channel=m_chan;                         \
	m_e.param.midi.data.raw.param1=m_param_1;		       \
	m_e.param.midi.data.raw.param2=m_param_2;		       \
}

};


#endif
