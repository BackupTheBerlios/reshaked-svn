//
// C++ Implementation: sound_driver_portaudio
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sound_driver_portaudio.h"

#include "version.h"

#ifdef DRIVER_PORTAUDIO_ENABLED

#include "typedefs.h"
#include <math.h>
#include "engine/audio_control.h"

namespace ReShaked {


#define MAX_BUFFER_SIZES 10
static int buffer_sizes[MAX_BUFFER_SIZES]={64,128,256,512,1024,2048,4096,8192,16384,32768};



int SoundDriver_PortAudio::get_settings_count() {
	
	return settings.size();
	
}
Property *SoundDriver_PortAudio::get_setting(int p_setting) {
	
	ERR_FAIL_INDEX_V(p_setting,settings.size(),NULL);
	return settings[p_setting];
}

bool SoundDriver_PortAudio::uses_file() {
	
	return false;
}
String SoundDriver_PortAudio::get_file() {
	
	return "";
}
void SoundDriver_PortAudio::set_file(String p_file) {
	
	
	
}

String SoundDriver_PortAudio::get_last_error() {
	
	return last_error;
}
String SoundDriver_PortAudio::get_name() {
	
	return driver_name;
}
float SoundDriver_PortAudio::get_mix_rate() {
	
	
	return mixing_frequency;
}

 int SoundDriver_PortAudio::static_callback( const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData ) {
	 
	 
	 SoundDriver_PortAudio *_this=(SoundDriver_PortAudio*)userData;
	 _this->callback((float*)input,(float*)output,frameCount);
	 
	 return 0;
 }


void SoundDriver_PortAudio::callback(float *in_buffer,float*out_buffer, int p_bufferSize) {
	

	if (AudioControl::mutex_try_lock())
		return;
	
	process_offset=0;
	current_input=in_buffer;
	current_output=out_buffer;
	
	process_graph(p_bufferSize);
	
	AudioControl::mutex_unlock();

	
}

void SoundDriver_PortAudio::write_to_inputs(int p_frames) {
	
	if (!get_input_count())
		return;
	if (current_inputs==0)
		return;
	
	get_input_plug(0)->get_buffer()->copy_from_interleaved(&current_input[process_offset*current_inputs],p_frames);
	
}
void SoundDriver_PortAudio::read_from_outputs(int p_frames) {
	

	if (!get_output_count())
		return;
	if (current_outputs==0)
		return;

	get_output_plug(0)->get_buffer()->copy_to_interleaved(&current_output[process_offset*current_outputs],p_frames);
	
	process_offset+=p_frames;
	
	
}
bool SoundDriver_PortAudio::is_input_enabled(int p_input) {
	
	return (p_input==0 && current_inputs>0);
}
bool SoundDriver_PortAudio::is_output_enabled(int p_output) {
	
	return (p_output==0 && current_outputs>0);
	
}

bool SoundDriver_PortAudio::init() {
	
	last_error="";
	
	if (active)
		return true;
	
	
	current_inputs = get_input_count()?get_input_plug(0)->get_channels():0;
	current_outputs = get_output_count()?get_output_plug(0)->get_channels():0;
	
	if (current_inputs && (input_devices.get_current()<0 || input_devices.get_current()>=input_device_index.size())) {
		
		last_error="No Input Devices";
		return true;
	}
	if (current_outputs && (output_devices.get_current()<0 || output_devices.get_current()>=output_device_index.size())) {
		
		last_error="No Output Devices";
		return true;
	}
	
	int in_device=input_device_index[ input_devices.get_current() ]; 
	int out_device=output_device_index[ output_devices.get_current() ];
	
	if (current_inputs==0 && current_outputs==0) {
		last_error="No Inputs/Outputs!";
		return true;
	}

	if (sampling_rate_values.empty()) {
		
		last_error="wtf? BUG!";
		return true;
	}
	
	int buffsize=buffer_sizes[buffer_size.get_current()];
//	int buffcount=(int)fragments.get();
	mixing_frequency=sampling_rate_values[mix_rate.get_current()];
	
	PaStreamParameters in_params;
	in_params.device=in_device;
	in_params.channelCount=current_inputs;
	in_params.sampleFormat=paFloat32;
	in_params.suggestedLatency=0; //die
	in_params.hostApiSpecificStreamInfo=NULL;
	PaStreamParameters out_params;
	out_params.device=out_device;
	out_params.channelCount=current_outputs;
	out_params.sampleFormat=paFloat32;
	out_params.suggestedLatency=0; //die
	out_params.hostApiSpecificStreamInfo=NULL;
	
	
	PaError error = Pa_OpenStream  	(&stream,&in_params,&out_params,mixing_frequency,buffsize,paNoFlag,&SoundDriver_PortAudio::static_callback, this);
			
	if (error<0) {
		
		last_error=Pa_GetErrorText(error);		
		return true;
	}
			
	error=Pa_StartStream(stream);
	
	if (error<0) {
		
		last_error=Pa_GetErrorText(error);		
		return true;
	}
	
	printf("OPENING PORTAUDIO\n");
	
	active=true;
	return false;
					
	
}
void SoundDriver_PortAudio::finish() {
	if (!active)
		return;
	Pa_CloseStream(stream);	
	active=false;
}
String SoundDriver_PortAudio::get_error() {
	
	return last_error;
	
}

int SoundDriver_PortAudio::get_output_latency() {
	
	return 0; // :(
		
}

SoundDriver::Status SoundDriver_PortAudio::get_status() {
	
	return active?STATUS_ACTIVE:STATUS_DISABLED;
}


SoundDriver_PortAudio::SoundDriver_PortAudio() {
	
	mixing_frequency=1;
	

	driver_name="PortAudio v19";
	
	active=false;

	
	int devices=Pa_GetDeviceCount();
		
	int current_input=0;
	int current_output=0;
	
	std::vector<String> output_names;
	std::vector<String> input_names;
	
	for (int i=0;i<devices;i++) {
		const PaDeviceInfo * dinfo=Pa_GetDeviceInfo (i);
		
		
		if (dinfo->maxOutputChannels) {
			
			if (i==Pa_GetDefaultOutputDevice())
				current_output=output_device_index.size();
			output_device_index.push_back(i);
			output_names.push_back(dinfo->name);
		}
		if (dinfo->maxInputChannels) {
			
			if (i==Pa_GetDefaultInputDevice())
				current_input=input_device_index.size();
			input_device_index.push_back(i);
			input_names.push_back(dinfo->name);
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

	
	settings.push_back(&input_devices);
	settings.push_back(&output_devices);
	settings.push_back(&buffer_size);
	settings.push_back(&mix_rate);
	
	stream=NULL;
	
}

SoundDriver_PortAudio::~SoundDriver_PortAudio(){


}


bool SoundDriver_PortAudio::initialize_portaudio() {
	
	
	PaError error=Pa_Initialize();
	if (error<0) {
		printf("PORTAUDIO INITIALIZE ERROR: %s\n",Pa_GetErrorText(error));
		return true;
	}
	
	
	
}
void SoundDriver_PortAudio::finalize_portaudio() {
	
	Pa_Terminate();
	
	
}


}
#endif