//
// C++ Interface: mididriver_alsa
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDMIDIDRIVER_ALSA_H
#define RESHAKEDMIDIDRIVER_ALSA_H


#include "version.h"

#ifdef DRIVER_ALSA_ENABLED

#include "engine/midi_driver.h"
#include <alsa/asoundlib.h>
#include "engine/song_playback.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class MidiDriver_Alsa : public MidiDriver {
	

	String client_name;

	enum {
		
		RING_SIZE=512 //512 events in the ring should be fine..
	};

	
	snd_seq_t *handle;	/* The snd_seq handle to /dev/snd/seq */
	
	int client; /* The client associated with this context */	
	int read_port; //port to read events */
	bool active;		/* */
		
	SongPlayback *song_playback;
	
	struct Input {
		
		struct Sync {
			bool subthread_exit_request;		
			bool subthread_exited;
			struct pollfd *pfd;
			int npfd;		
		} sync;

		struct Ring {
			DriverInputEvent buffer[RING_SIZE];
			int read_ptr;
			int write_ptr;
		} ring;
		
	} input;
	
	struct Output {
		
		
		struct AlsaPort {
			
			String identifier;
			String name;			
			snd_seq_addr_t address;
		};
		
		std::vector<AlsaPort> alsa_ports;
		
		struct Out {
			
			int alsa_port;
			snd_seq_addr_t dest_address;
			String identifier;
			bool enabled;			
			String name;
		};
		
		Out out[MAX_OUTPUTS];
		
	} output;

	String last_error;
	
	
	static void* receiver_thread_callback_internal(void*);
	void receiver_thread_callback();
	void receiver_thread_poll_events();
//	int get_file_descriptor();	
	bool connect_output_to_alsa_port(int p_output,int p_which);
	
public:
	
	String get_name();
	
	/* Output */
	
	int get_port_count();
	String get_port_name(int p_port);
	String get_port_identifier(int p_port);
	void rescan_ports();
	
	int get_output_count();
	bool connect_output(int p_output,String p_identifier); ///< p_port is 0 .. get_output_count() -1 or OUTPUT_NO_PORT (disabled)
	void disconnect_output(int p_output);
	bool is_output_connected(int p_output);
	
	String get_output_name(int p_output);
	String get_output_identifier(int p_output);
	
	void queue_event(int p_port,unsigned int p_usec_ofs,const EventMidi& p_event); //offset since next audio mixing
	void send_queued_events();
	
	
	/* Input */
	
	bool has_input_events_pending();
	bool get_input_event(DriverInputEvent* p_event);
	
	/* Settings */
	
	int get_settings_count();
	Property *get_setting(int p_setting);
	
	bool init();
	void finish();
	bool is_active();
	
	void set_song_playback(SongPlayback *p_sp);
	
	String get_last_error();	
	MidiDriver_Alsa();
	
	~MidiDriver_Alsa();

};

}

#endif
#endif
