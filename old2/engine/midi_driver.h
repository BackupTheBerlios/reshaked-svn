//
// C++ Interface: midi_driver
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDMIDI_DRIVER_H
#define RESHAKEDMIDI_DRIVER_H


#include "engine/event.h"
#include "property.h"
#include "typedefs.h"
#include "engine/song_playback.h"
#include "engine/midi_device_description.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/

struct DriverInputEvent {
	
	EventMidi midi; //midi event
	Tick tick; // if song was playing, tick of the song when event was received, otherwise <0
};


class MidiDriver {
public:	
	
	enum {
		MAX_OUTPUTS=16,
		OUTPUT_NO_PORT=-1
	};
	
private:	
	
	MidiDeviceDescription output_device_description[MAX_OUTPUTS];
	
public:
	
	
	enum OutputMethod {
		
		OUTPUT_METHOD_FRAMESTAMP,
		OUTPUT_METHOD_TIMESTAMP
	};
	
	virtual String get_name()=0;
	
	/* Output */
	
	virtual int get_port_count()=0;
	virtual String get_port_name(int p_port)=0;
	virtual String get_port_identifier(int p_port)=0;
	virtual void rescan_ports()=0;
	
	virtual int get_output_count()=0;
	virtual bool connect_output(int p_output,String p_identifier)=0; ///< p_port is 0 .. get_output_count() -1 or OUTPUT_NO_PORT (disabled)
	virtual void disconnect_output(int p_output)=0;
	virtual bool is_output_connected(int p_output)=0;
	virtual String get_output_name(int p_output)=0;
	virtual String get_output_identifier(int p_output)=0;
	MidiDeviceDescription *get_output_device_description(int p_output);
	virtual void queue_event(int p_port,unsigned int p_usec_ofs,const EventMidi& p_event)=0; //offset since next audio mixing
	virtual void send_queued_events()=0;
	
		
	/* Input */
	
	virtual bool has_input_events_pending()=0;
	virtual bool get_input_event(DriverInputEvent* p_event)=0;
	
	/* Settings */
	
	virtual int get_settings_count()=0;
	virtual Property *get_setting(int p_setting)=0;
	
	virtual bool init()=0;
	virtual void finish()=0;
	virtual bool is_active()=0;
	
	virtual void set_song_playback(SongPlayback*)=0;
	
	virtual String get_last_error()=0;
	
	MidiDriver();
	virtual ~MidiDriver();
};

}

#endif
