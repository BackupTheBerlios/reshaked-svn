//
// C++ Implementation: mididriver_alsa
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "mididriver_alsa.h"

#ifdef DRIVER_ALSA_ENABLED

#include <unistd.h>
#include <pthread.h>
#include "misc_funcs.h"
#include "engine/audio_control.h"

//#include "components/data/timer.h"
namespace ReShaked {

String MidiDriver_Alsa::get_name() {
	
	return "ALSA";
	
}

/* Output */


int MidiDriver_Alsa::get_port_count() {
	
	return output.alsa_ports.size();
}
String MidiDriver_Alsa::get_port_name(int p_port) {
	
	ERR_FAIL_INDEX_V(p_port,output.alsa_ports.size(),"");
	return output.alsa_ports[p_port].name;
	
}
String MidiDriver_Alsa::get_port_identifier(int p_port) {
	
	ERR_FAIL_INDEX_V(p_port,output.alsa_ports.size(),"");
	return output.alsa_ports[p_port].identifier;
	
}
void MidiDriver_Alsa::rescan_ports() {
	
	AudioControl::mutex_lock();
	
	snd_seq_client_info_t *cinfo;

	snd_seq_client_info_alloca(&cinfo);
	snd_seq_client_info_set_client(cinfo, -1);

	while (snd_seq_query_next_client(handle, cinfo) >= 0) { //iterate through clients

		snd_seq_port_info_t *pinfo;
		snd_seq_port_info_alloca(&pinfo);
		snd_seq_port_info_set_client(pinfo, snd_seq_client_info_get_client(cinfo));
		snd_seq_port_info_set_port(pinfo, -1);

		while (snd_seq_query_next_port(handle, pinfo) >= 0) { //and now through ports

			unsigned int capability = snd_seq_port_info_get_capability(pinfo);
			Output::AlsaPort new_device;
		

			if ((capability & SND_SEQ_PORT_CAP_SUBS_WRITE) == 0)
				continue;

			
			snd_seq_addr_t address= *snd_seq_port_info_get_addr(pinfo);
			if (address.client==client)
				continue; //we cant allow connecting to ourselves
			new_device.name=snd_seq_port_info_get_name(pinfo);
			new_device.identifier="ALSA|" + String::num(address.client) + "|" + String::num(address.port) + "|" + new_device.name;
			new_device.address=address;		
			output.alsa_ports.push_back(new_device);
		}

	}
	
	AudioControl::mutex_unlock();
	
}

int MidiDriver_Alsa::get_output_count() {
	
	return MAX_OUTPUTS;	
}

void MidiDriver_Alsa::disconnect_output(int p_output) {
	
	
	ERR_FAIL_INDEX(p_output,MAX_OUTPUTS);
	ERR_FAIL_COND(!output.out[p_output].enabled);

	AudioControl::mutex_lock();
	
	snd_seq_port_subscribe_t* subs;

	snd_seq_port_subscribe_alloca(&subs);

	snd_seq_addr_t src;
	src.client=client;
	src.port=output.out[p_output].alsa_port;
	snd_seq_port_subscribe_set_sender(subs, &src);
	snd_seq_port_subscribe_set_dest(subs, &output.out[p_output].dest_address);

	if ( snd_seq_unsubscribe_port(handle, subs) ) {
	
		ERR_PRINT("Couldn't unsubscribe port!");		
	}

	output.out[p_output].enabled=false;
	AudioControl::mutex_unlock();
	
	
}
bool MidiDriver_Alsa::is_output_connected(int p_output) {
	
	ERR_FAIL_INDEX_V(p_output,MAX_OUTPUTS,false);
	
	return output.out[p_output].enabled;
}


bool MidiDriver_Alsa::connect_output_to_alsa_port(int p_output,int p_which) {
	
	
	ERR_FAIL_INDEX_V(p_which,output.alsa_ports.size(),true);
	ERR_FAIL_INDEX_V(p_output,MAX_OUTPUTS,true);
	
	ERR_FAIL_COND_V(output.out[p_output].enabled,true);
	
	AudioControl::mutex_lock();

	snd_seq_port_subscribe_t* subs;

	snd_seq_port_subscribe_alloca(&subs);

	snd_seq_addr_t src;
	src.client=client;
	src.port=output.out[p_output].alsa_port;
	snd_seq_port_subscribe_set_sender(subs, &src);
	snd_seq_port_subscribe_set_dest(subs, &output.alsa_ports[p_which].address);

	if (snd_seq_subscribe_port(handle, subs)) {
		
		AudioControl::mutex_unlock();

		return true;
	}
	output.out[p_output].enabled=true;
	output.out[p_output].dest_address=output.alsa_ports[p_which].address;
	output.out[p_output].name=output.alsa_ports[p_which].name;
	output.out[p_output].identifier=output.alsa_ports[p_which].identifier;

	AudioControl::mutex_unlock();
	
	return false;	
	
}

bool MidiDriver_Alsa::connect_output(int p_output,String p_identifier) {
	
	
	if (is_output_connected(p_output))
		   disconnect_output(p_output);
	/* FIRST try to find if the exact thing is found */
	
	for (int i=0;i<output.alsa_ports.size();i++) {
		
		if (output.alsa_ports[i].identifier==p_identifier)
			return connect_output_to_alsa_port(p_output,i);

	}
	
	/* THEN, for next attempts, we need the individual identifier information */
	
	std::vector<String> result=split_str('|',p_identifier);
	if (result.size()!=4)
		return true;
	
	if (result[0]!="ALSA")
		return true;
	
//	int alsa_client=atoi(result[1].ascii().get_data());
	int alsa_port=atoi(result[2].ascii().get_data());
	
	String name=result[3];
	
	/* SECOND try to find name:port matching */
	
	for (int i=0;i<output.alsa_ports.size();i++) {
	
		if (output.alsa_ports[i].address.port==alsa_port && output.alsa_ports[i].name==name) {
			
			return connect_output_to_alsa_port(p_output,i);
		}
	}
		
		
	return true; //no success!	
	
	
}
String MidiDriver_Alsa::get_output_name(int p_output) {
	
	ERR_FAIL_INDEX_V(p_output,MAX_OUTPUTS,"");
	
	return output.out[p_output].name;
	
}

String MidiDriver_Alsa::get_output_identifier(int p_output) {
	
	ERR_FAIL_INDEX_V(p_output,MAX_OUTPUTS,"");
	
	return output.out[p_output].identifier;
	
}

void MidiDriver_Alsa::queue_event(int p_port,unsigned int p_usec_ofs,const EventMidi& p_event) {
	
}

void MidiDriver_Alsa::send_queued_events() {
	
	
	
}

/* Input */


void* MidiDriver_Alsa::receiver_thread_callback_internal(void* p_this) {

	nice(-20); //grab as much priority as possible
	MidiDriver_Alsa *m_this=(MidiDriver_Alsa*)p_this;
	m_this->receiver_thread_callback();
	return NULL;	
}



void MidiDriver_Alsa::receiver_thread_callback() {
	
	input.sync.npfd = snd_seq_poll_descriptors_count(handle, POLLIN);
	input.sync.pfd = (struct pollfd *)malloc(input.sync.npfd * sizeof(struct pollfd));
	snd_seq_poll_descriptors(handle, input.sync.pfd, input.sync.npfd, POLLIN);	
	
	while (!input.sync.subthread_exit_request) {
		
		receiver_thread_poll_events();			
	}
	
	free(input.sync.pfd);
	input.sync.subthread_exited=true;	
	
}

void MidiDriver_Alsa::receiver_thread_poll_events() {
	
	
	
	if (snd_seq_event_input_pending(handle, 0) <= 0) {
		if (poll(input.sync.pfd, input.sync.npfd, 1000) <= 0)  {
			//sleep(1);
			return; //no events for a while, check if we must exit
		}
	}
	
	
	snd_seq_event_t *event;
	int er;

	er = snd_seq_event_input(handle, &event);

	ERR_FAIL_COND(er < 0 || event == NULL);
	
	if ( ((input.ring.write_ptr+1)%RING_SIZE)==input.ring.read_ptr) {
		ERR_PRINT("RINGBUFFER FULL! Dropping event..");
		snd_seq_free_event(event);
		return; //rungbuffer full
	}
		
	DriverInputEvent *dst_event=&input.ring.buffer[input.ring.write_ptr];
	*dst_event=DriverInputEvent(); //clear
	
	if (song_playback && song_playback->get_status()==SongPlayback::STATUS_PLAY) {
		
		dst_event->tick=song_playback->get_playback_pos_tick();
		
	} else {
		
		dst_event->tick=-1;
		
	}
		
		
	bool recognized=true;
	
	switch(event->type) {

		case SND_SEQ_EVENT_NOTEON: {


			dst_event->midi.midi_type=EventMidi::MIDI_NOTE_ON;
			dst_event->midi.channel=event->data.note.channel;
			dst_event->midi.data.note.note=event->data.note.note;
			dst_event->midi.data.note.velocity=event->data.note.velocity;
		} break;


		case SND_SEQ_EVENT_NOTEOFF: {

			dst_event->midi.midi_type=EventMidi::MIDI_NOTE_OFF;
			dst_event->midi.channel=event->data.note.channel;
			dst_event->midi.data.note.note=event->data.note.note;
			dst_event->midi.data.note.velocity=event->data.note.velocity;

		} break;

		case SND_SEQ_EVENT_KEYPRESS: {

			dst_event->midi.midi_type=EventMidi::MIDI_NOTE_PRESSURE;
			dst_event->midi.channel=event->data.note.channel;
			dst_event->midi.data.note.note=event->data.note.note;
			dst_event->midi.data.note.velocity=event->data.note.velocity;
		} break;

		case SND_SEQ_EVENT_PGMCHANGE: {

			dst_event->midi.midi_type=EventMidi::MIDI_PATCH_SELECT;
			dst_event->midi.channel=event->data.control.channel;
			dst_event->midi.data.patch.index=event->data.control.value;
		} break;

		case SND_SEQ_EVENT_CONTROLLER: {

			dst_event->midi.midi_type=EventMidi::MIDI_CONTROLLER;
			dst_event->midi.channel=event->data.control.channel;
			dst_event->midi.data.control.index=event->data.control.param;
			dst_event->midi.data.control.parameter=event->data.control.value;
		} break;

		case SND_SEQ_EVENT_CONTROL14: {
		} break;

		case SND_SEQ_EVENT_PITCHBEND: {

			dst_event->midi.midi_type=EventMidi::MIDI_PITCH_BEND;
			dst_event->midi.channel=event->data.control.channel;
			dst_event->midi.data.pitch_bend.bend=event->data.control.value+0x2000; //evil alsa

		} break;

		case SND_SEQ_EVENT_CHANPRESS: {

			dst_event->midi.midi_type=EventMidi::MIDI_AFTERTOUCH;
			dst_event->midi.channel=event->data.control.channel;
			dst_event->midi.data.aftertouch.pressure=event->data.control.value;

		} break;
		default: {
	//		dst_event->midi.midi_type=EventMidi::MIDI_SYSEX;
			recognized=false;
		} break;
	}


	if (recognized) {

		input.ring.write_ptr=(input.ring.write_ptr+1) % RING_SIZE;				
	}
	
	snd_seq_free_event(event);	
	
		
}

bool MidiDriver_Alsa::is_active() {

	return active;

}

bool MidiDriver_Alsa::init() {
	
	last_error="";
	
	if (active) {
		last_error="Driver Already Active";
		return true;
	}
	
	
	
	/* open that bitch */
	if (snd_seq_open(&handle, "hw", SND_SEQ_OPEN_DUPLEX, 0)<0) {
		
		last_error="Can't open ALSA sequencer.";
		return true;
	}

	snd_seq_set_client_name(handle, client_name.ascii().get_data());

	client = snd_seq_client_id(handle); //get client?
	//fd = get_file_descriptor();
	snd_seq_set_client_pool_input(handle, 1000); /* enough? */

	/* Port for reading */
	read_port = snd_seq_create_simple_port(handle, "Input",
				SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
				SND_SEQ_PORT_TYPE_MIDI_GENERIC);
	if (read_port < 0) {
		last_error="Can't create input port.";
		snd_seq_close(handle);
		return true;
	}

	for (int i=0;i<MAX_OUTPUTS;i++) {
		
		String outname="Output "+String::num(i);
		
		output.out[i].alsa_port=snd_seq_create_simple_port(handle, outname.ascii().get_data(),
				SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_NO_EXPORT,
				SND_SEQ_PORT_TYPE_APPLICATION);
		
		if (output.out[i].alsa_port < 0) {
			last_error="Can't Create Output Port " + String(i+1);
			snd_seq_close(handle);
			return true;
		}
		
	}
	
	input.ring.read_ptr=0;
	input.ring.write_ptr=0; //reset ringbuffer


	active=true;
	
	/* Thread initialization */	
	
	pthread_t midi_thread;
	pthread_attr_t thread_attr;
	
	pthread_attr_init(&thread_attr);
	sched_param thread_sched_param;
	memset(&thread_sched_param,0,sizeof(thread_sched_param));
	int priority=sched_get_priority_max(SCHED_FIFO);
	thread_sched_param.sched_priority=priority;
	pthread_attr_setschedparam (&thread_attr,&thread_sched_param);
	
	input.sync.subthread_exit_request=false;
	input.sync.subthread_exited=false;
	
	pthread_create(&midi_thread,&thread_attr,&receiver_thread_callback_internal,this);

	
	return false;
}

void MidiDriver_Alsa::finish() {

	if (!active)
		return;
	
	input.sync.subthread_exit_request=true;
	while(!input.sync.subthread_exited)
		usleep(50000);

	snd_seq_close(handle);
	active=false;

}

bool MidiDriver_Alsa::has_input_events_pending() {

	if (!active) return false;

	return  (input.ring.read_ptr!=input.ring.write_ptr);
   

  
}

bool MidiDriver_Alsa::get_input_event(DriverInputEvent* p_event) {

	if (!active)
		return true;
	if (input.ring.read_ptr==input.ring.write_ptr)
		return true;
	
	*p_event=input.ring.buffer[input.ring.read_ptr];
	input.ring.read_ptr=(input.ring.read_ptr+1) % RING_SIZE;
	
	return false;
}


/* Settings */


void MidiDriver_Alsa::set_song_playback(SongPlayback *p_sp) {
	
	song_playback=p_sp;
	
	
}

int MidiDriver_Alsa::get_settings_count() {
	
	return 0;	
}
Property *MidiDriver_Alsa::get_setting(int p_setting) {
	
	return NULL;
}

String MidiDriver_Alsa::get_last_error() {
	
	
	return last_error;
}


MidiDriver_Alsa::MidiDriver_Alsa() {
	
	active=false;
	handle=NULL;
	client=0;
	read_port=0;
	
	input.ring.read_ptr=0;
	input.ring.write_ptr=0;
	
	input.sync.subthread_exit_request=false;
	input.sync.subthread_exited=false;
	
	client_name="ReShaked";
	song_playback=NULL;
	
	for (int i=0;i<MAX_OUTPUTS;i++) {
		
		output.out[i].alsa_port=-1;
		output.out[i].enabled=false;
		output.out[i].identifier="";
	}
	
}


MidiDriver_Alsa::~MidiDriver_Alsa() {
	
}


}

#endif
