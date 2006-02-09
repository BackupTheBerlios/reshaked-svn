//
// C++ Implementation: sound_driver_rtaudio
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sound_driver_rtaudio.h"
#include "typedefs.h"
#include "engine/audio_control.h"
#include <math.h>
#ifdef DRIVER_RTAUDIO_ENABLED

namespace ReShaked {




#define MAX_BUFFER_SIZES 10
static int buffer_sizes[MAX_BUFFER_SIZES]={64,128,256,512,1024,2048,4096,8192,16384,32768};



int SoundDriver_RtAudio::get_settings_count() {
	
	return settings.size();
	
}
Property *SoundDriver_RtAudio::get_setting(int p_setting) {
	
	ERR_FAIL_INDEX_V(p_setting,settings.size(),NULL);
	return settings[p_setting];
}

bool SoundDriver_RtAudio::uses_file() {
	
	return false;
}
String SoundDriver_RtAudio::get_file() {
	
	return "";
}
void SoundDriver_RtAudio::set_file(String p_file) {
	
	
	
}

String SoundDriver_RtAudio::get_last_error() {
	
	return last_error;
}
String SoundDriver_RtAudio::get_name() {
	
	return driver_name;
}
float SoundDriver_RtAudio::get_mix_rate() {
	
	
	return mixing_frequency;
}


int SoundDriver_RtAudio::static_callback(char *buffer, int bufferSize, void *p_instance) {
	
	SoundDriver_RtAudio *_this=(SoundDriver_RtAudio*)p_instance;
	_this->callback(buffer,bufferSize);
	
	return 0;
}

void SoundDriver_RtAudio::callback(char *p_buffer, int p_bufferSize) {
	

	if (AudioControl::mutex_try_lock())
		return;
	
	process_offset=0;
	current_input=(float*)p_buffer;
	
	process_graph(p_bufferSize);

	
	float *buff_ptr=(float *)(p_buffer);
	
	for (int i=0;i<(p_bufferSize*current_outputs);i++) {
		
		buff_ptr[i]=output_aux[i];
	}
	
	AudioControl::mutex_unlock();
	

	
}

void SoundDriver_RtAudio::write_to_inputs(int p_frames) {
	
	if (!get_input_count())
		return;
	if (current_inputs==0)
		return;
	
	get_input_plug(0)->get_buffer()->copy_from_interleaved(&current_input[process_offset*current_inputs],p_frames);
	
}
void SoundDriver_RtAudio::read_from_outputs(int p_frames) {
	

	if (!get_output_count())
		return;
	if (current_outputs==0)
		return;
	ERR_FAIL_COND( ((process_offset+p_frames)*current_outputs)>output_aux.size() );
	get_output_plug(0)->get_buffer()->copy_to_interleaved(&output_aux[process_offset*current_outputs],p_frames);
	
	process_offset+p_frames;
	
	
}
bool SoundDriver_RtAudio::is_input_enabled(int p_input) {
	
	return (p_input==0 && current_inputs>0);
}
bool SoundDriver_RtAudio::is_output_enabled(int p_output) {
	
	return (p_output==0 && current_outputs>0);
	
}

bool SoundDriver_RtAudio::init() {
	
	last_error="";
	
	if (active)
		return true;
	
	ERR_FAIL_COND_V(rtaudio!=NULL,true);
	
	current_inputs = get_input_count()?get_input_plug(0)->get_channels():0;
	current_outputs = get_output_count()?get_output_plug(0)->get_channels():0;
	
	int in_device=input_device_index[ input_devices.get_current() ]+1; //rtaudio uses 1..devices
	int out_device=output_device_index[ output_devices.get_current() ]+1;
	
	if (current_inputs==0 && current_outputs==0) {
		last_error="No Inputs/Outputs!";
		return true;
	}

	if (sampling_rate_values.empty()) {
		
		last_error="wtf? BUG!";
		return true;
	}
	
	int buffsize=buffer_sizes[buffer_size.get_current()];
	int buffcount=(int)fragments.get();
	mixing_frequency=sampling_rate_values[mix_rate.get_current()];
	
	try {
	
		rtaudio = new RtAudio::RtAudio( in_device,
						current_outputs,
						out_device,
						current_inputs,
						RTAUDIO_FLOAT32,
						(int)mixing_frequency,
						&buffsize,
						&buffcount,
						api);

	} catch (RtError &error) {
		
		last_error=error.getMessage().c_str();
		rtaudio=NULL;
		return true;
	}
	
	printf("OPENING RTAUDIO - INs %i, OUTs %i, mixing_freq %i, buffersize %i, buffers %i\n",current_inputs,current_outputs,(int)mixing_frequency,buffsize,buffcount);
	
	try {
		rtaudio->setStreamCallback(&SoundDriver_RtAudio::static_callback, this);
		rtaudio->startStream();
	}
	catch (RtError &error) {
		delete rtaudio;
		rtaudio=NULL;
		last_error=error.getMessage().c_str();
		return true;
	}

	output_aux.resize(buffsize*buffcount*current_outputs); //max possible buffer
	active=true;
	return false;
					
	
}
void SoundDriver_RtAudio::finish() {
	
	if (!active)
		return;
	ERR_FAIL_COND(rtaudio==NULL);
	rtaudio->stopStream();
	rtaudio->closeStream();
	delete rtaudio;
	rtaudio=NULL;
	active=false;
}
String SoundDriver_RtAudio::get_error() {
	
	return last_error;
	
}

int SoundDriver_RtAudio::get_output_latency() {
	
	return 0; // :(
		
}

SoundDriver::Status SoundDriver_RtAudio::get_status() {
	
	return active?STATUS_ACTIVE:STATUS_DISABLED;
}


SoundDriver_RtAudio::SoundDriver_RtAudio(RtAudio::RtAudioApi p_api) {
	
	mixing_frequency=1;
	
	api=p_api;
	active=false;

	rtaudio=NULL;
	
	//stupid exceptions....
	try {
		rtaudio = new RtAudio(p_api);
	}
	catch (RtError &error) {
		
		return;
	}
	
	switch (p_api) {
		case RtAudio::LINUX_ALSA: driver_name="Alsa"; break;
		case RtAudio::LINUX_OSS: driver_name="Open Sound System (OSS)"; break;
		case RtAudio::LINUX_JACK: driver_name="Jack"; break;
		case RtAudio::MACOSX_CORE: driver_name="Core Audio"; break;
		case RtAudio::WINDOWS_ASIO: driver_name="ASIO"; break;
		case RtAudio::WINDOWS_DS: driver_name="DirectSound"; break;
		default: driver_name="RtAudio Unknown"; break;
	}
	
	int devices=0;
	try {
		devices=rtaudio->getDeviceCount();
	}
	catch (RtError &error) {
		
		last_error=String("CANT GET DEVICES:")+error.getMessage().c_str();
		return;
		
	}
		
	int current_input=0;
	int current_output=0;
	
	std::vector<String> output_names;
	std::vector<String> input_names;
	
	for (int i=0;i<devices;i++) {
		
		RtAudioDeviceInfo dinfo;
		try {
			dinfo=rtaudio->getDeviceInfo(i+1);
		}
		catch (RtError &error) {
			
			last_error=String("CANT GET DEVICE INFO: ")+error.getMessage().c_str();
		}
		
		if (dinfo.outputChannels) {
			
			if (output_device_index.empty())
				current_output=i;
			output_device_index.push_back(i);
			output_names.push_back(String(dinfo.name.c_str()));
		}
		if (dinfo.inputChannels) {
			
			if (input_device_index.empty())
				current_input=i;
			input_device_index.push_back(i);
			input_names.push_back(String(dinfo.name.c_str()));
		}

	}	
	
	input_devices.set_all("input_devices","Input Devices",input_names,current_input);
	output_devices.set_all("output_devices","Output Devices",output_names,current_output);
	

	
	std::vector<String> buffer_sizes_text;
	for (int i=0;i<MAX_BUFFER_SIZES;i++) {
		
		buffer_sizes_text.push_back( String::num(buffer_sizes[i]) );
	}
	
	buffer_size.set_all("buffer_size","Buffer Size",buffer_sizes_text,6,"frames");
	
	
	sampling_rate_values.push_back(4000);
	sampling_rate_values.push_back(5512);
	sampling_rate_values.push_back(8000);
	sampling_rate_values.push_back(9600);
	sampling_rate_values.push_back(11025);
	sampling_rate_values.push_back(16000);
	sampling_rate_values.push_back(22050);
	sampling_rate_values.push_back(22050);
	sampling_rate_values.push_back(32000);
	sampling_rate_values.push_back(44100);
	sampling_rate_values.push_back(48000);
	sampling_rate_values.push_back(88200);
	sampling_rate_values.push_back(96000);
	sampling_rate_values.push_back(192000);
	
	std::vector<String> sampling_rates_text;
	for (int i=0;i<sampling_rate_values.size();i++) {
		
		sampling_rates_text.push_back( String::num(sampling_rate_values[i]) );
	}
	
	int default_sampling_rate=0;
	for (int i=0;i<sampling_rate_values.size();i++) {
		default_sampling_rate=i;
		if (sampling_rate_values[i]>=48000)
			break;
		
		
	}
	
	mix_rate.set_all("sampling_rate","Sampling Rate",sampling_rates_text,default_sampling_rate,"hz");

	fragments.set_all(4,2,16,4,1,Property::DISPLAY_SPIN,"fragments","Fragments");
	
	settings.push_back(&input_devices);
	settings.push_back(&output_devices);
	settings.push_back(&buffer_size);
	settings.push_back(&fragments);
	settings.push_back(&mix_rate);
	delete rtaudio;
	rtaudio=NULL;
	
	
}


SoundDriver_RtAudio::~SoundDriver_RtAudio()
{
	if (rtaudio)
		delete rtaudio;
}


}
#endif