//
// C++ Implementation: sound_driver_jack
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sound_driver_jack.h"

#ifdef DRIVER_JACK_ENABLED

#include "engine/audio_control.h"

namespace ReShaked {

/**** STATIC FUNCS *****/

int SoundDriver_JACK::process_jack_callback(jack_nframes_t nframes,void *arg) {
	
	SoundDriver_JACK* _this=(SoundDriver_JACK*)arg;
	_this->process(nframes);
	
}
int SoundDriver_JACK::jack_set_mixfreq(jack_nframes_t nframes,void *arg) {
	
	/* todo */
}
int SoundDriver_JACK::jack_set_buffsize(jack_nframes_t nframes,void *arg) {
	
	/* todo*/
}
void SoundDriver_JACK::jack_shutdown(void *arg) {
	
	SoundDriver_JACK* _this=(SoundDriver_JACK*)arg;
	_this->finish();
}

/* PROCESS Functions */

void SoundDriver_JACK::process(int p_frames) {
	
	if (AudioControl::mutex_try_lock())
		return;
	
	process_offset=0;
	process_nframes=p_frames;
	
	process_graph(p_frames);
	
	AudioControl::mutex_unlock();
}

void SoundDriver_JACK::write_to_inputs(int p_frames) {
	
	ERR_FAIL_COND( get_input_count()!=input_port_list.size() );
	for(int i=0;i<get_input_count();i++) {
		
		AudioBuffer *buff=get_input_plug(i)->get_buffer();
		
		ERR_CONTINUE(buff==NULL);
		ERR_CONTINUE(buff->get_channels()!=input_port_list[i].ports.size() );
		
		for(int j=0;j<buff->get_channels();j++) {
			
			float *audio_ptr=buff->get_buffer(j);
			
			/* this condition will disapear in compile time */
			/* this is done in case jack audio sample is not float anymore */
			
			jack_default_audio_sample_t*jack_ptr = (jack_default_audio_sample_t *)jack_port_get_buffer(input_port_list[i].ports[j], process_nframes); 
			
			if (sizeof(jack_default_audio_sample_t)==sizeof(float)) { 
				
				memcpy(audio_ptr,&jack_ptr[process_offset],sizeof(float)*p_frames);
			} else {
				
				for (int k=0;k<p_frames;k++)
					audio_ptr[k]=jack_ptr[k+process_offset];
			}

		}			
	}
}
			    
void SoundDriver_JACK::read_from_outputs(int p_frames) {
	
	
	ERR_FAIL_COND( get_output_count()!=output_port_list.size() );
	for(int i=0;i<get_output_count();i++) {
		
		AudioBuffer *buff=get_output_plug(i)->get_buffer();
		
		ERR_CONTINUE(buff==NULL);
		ERR_CONTINUE(buff->get_channels()!=output_port_list[i].ports.size() );
		
		for(int j=0;j<buff->get_channels();j++) {
			
			float *audio_ptr=buff->get_buffer(j);
			
			/* this condition will disapear in compile time */
			/* this is done in case jack audio sample is not float anymore */
			
			jack_default_audio_sample_t*jack_ptr = (jack_default_audio_sample_t *)jack_port_get_buffer(output_port_list[i].ports[j], process_nframes); 
			
			if (sizeof(jack_default_audio_sample_t)==sizeof(float)) { 
				
				
				memcpy(&jack_ptr[process_offset],audio_ptr,sizeof(float)*p_frames);
			} else {
				
				for (int k=0;k<p_frames;k++)
					jack_ptr[k+process_offset]=audio_ptr[k];
			}
		}			
	}	
}

/* Regular Functions */



int SoundDriver_JACK::get_settings_count() {
	
	return 0;
}

Property *SoundDriver_JACK::get_setting() {
	
	return NULL;
}

bool SoundDriver_JACK::uses_file() {
	
	return false;
}

String SoundDriver_JACK::get_file() {
	
	return "";
}

void SoundDriver_JACK::set_file(String p_file) {
	
	
}

String SoundDriver_JACK::get_name() {
	
	return "Jack";	
}

SoundDriver::Status SoundDriver_JACK::get_status() {
	
	return active?STATUS_ACTIVE:STATUS_DISABLED;
}

float SoundDriver_JACK::get_mix_rate() {
	
	return mixing_frequency;
}

bool SoundDriver_JACK::init() {
	
	if (active) {
		last_error="Driver Already Active";
		ERR_PRINT("Driver Already Active");
		return true;
	}
	if (get_port_layout()==NULL)
		return true;
	
	if ((client = jack_client_new ("ReShaked")) == 0) {
		last_error="JACK server doesn't seem to be running";
		ERR_PRINT("JACK server doesn't seem to be running");
		return true;
	}
	jack_set_process_callback (client, process_jack_callback, this);
	jack_set_buffer_size_callback (client, jack_set_buffsize, this);
	jack_set_sample_rate_callback (client, jack_set_mixfreq, this);
	jack_on_shutdown (client, jack_shutdown, this);
	mixing_frequency=jack_get_sample_rate (client);
	
	
	input_port_list.clear();
	
	
	for (int i=0;i<get_input_count();i++) {
		
		PortList pl;
		std::string name=(i==0)?"_MainIn":"_AuxIn";
		
		AudioPlug *out=get_input_plug(i);
		
		for (int j=0;j<out->get_channels();j++) {
			std::string post;
			
			if (out->get_channels()==2 &&j==0)
				post="_L";
			else if (out->get_channels()==2 &&j==1)
				post="_R";
			else if (out->get_channels()>2) {
				char postdigit[2]={0,0};
				postdigit[0]='0'+j+1;
				post+="_";
				post+=postdigit;
			}
			name+=post;
			jack_port_t *p=jack_port_register (client, name.c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
			pl.ports.push_back(p);
		}
		
		input_port_list.push_back(pl);
	}
	
	output_port_list.clear();
	
	for (int i=0;i<get_output_count();i++) {
		
		PortList pl;
		std::string name=(i==0)?"_MainOut":"_AuxOut";
		
		AudioPlug *out=get_output_plug(i);
		
		for (int j=0;j<out->get_channels();j++) {
			std::string post;
			
			if (out->get_channels()==2 &&j==0)
				post="_L";
			else if (out->get_channels()==2 &&j==1)
				post="_R";
			else if (out->get_channels()>2) {
				char postdigit[2]={0,0};
				postdigit[0]='0'+j+1;
				post+="_";
				post+=postdigit;
			}
			name+=post;
			jack_port_t *p=jack_port_register (client, name.c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
			pl.ports.push_back(p);
		}
		
		output_port_list.push_back(pl);
	}
	
	
	/* tell the JACK server that we are ready to roll */
	
	if (jack_activate (client)) {
		ERR_PRINT("Cannot activate client");
		last_error="Cannot activate client";
		jack_client_close (client);
		return true;
	}
	
	/* connect the ports. Note: you can't do this before
	   the client is activated, because we can't allow
	   connections to be made to clients that aren't
	   running.
	*/
	
	/*
	
	if (jack_connect (client, jack_port_name (output_port_l), 	"alsa_pcm:playback_1")) {
	ERROR("cannot connect output ports\n");
		//jack_client_close (client);
		//return FUNCTION_FAILED;
	}
	if (jack_connect (client, jack_port_name (output_port_r), "alsa_pcm:playback_2")) {
	ERROR("cannot connect output ports\n");
		//jack_client_close (client);
		//return FUNCTION_FAILED;
	}
	*/
	
	return false;
}
void SoundDriver_JACK::finish() {
	
	ERR_FAIL_COND(!active);
	
	for (int i=0;i<input_port_list.size();i++) 
		for (int j=0;j<input_port_list[i].ports.size();j++) 
			jack_port_unregister(client,input_port_list[i].ports[j]);
			
	input_port_list.clear();
	
	for (int i=0;i<output_port_list.size();i++) 
		for (int j=0;j<output_port_list[i].ports.size();j++) 
			jack_port_unregister(client,output_port_list[i].ports[j]);
	
	output_port_list.clear();
	
	jack_client_close(client);
	
	active=false;
	last_error="";
}

/********* VIRTUALS ***********/

bool SoundDriver_JACK::is_input_enabled(int p_input) {
	
	/* everything can be enabled under jack */
	return true;
}
bool SoundDriver_JACK::is_output_enabled(int p_output) {
	
	/* everything can be enabled under jack */
	return true;
}


SoundDriver_JACK::SoundDriver_JACK() {
	
	active=false;
	client=NULL;
}


SoundDriver_JACK::~SoundDriver_JACK() {
	
	if (active)
		finish();
}



}


#endif
