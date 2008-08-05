
#include "audio_control.h"
#include "error_macros.h"
#include "engine/audio_driver.h"

void value_stream_global_lock() {
	
	
}

void value_stream_global_unlock() {
	
//	ReShaked::AudioControl::mutex_unlock();
}



//MutexLock *AudioControl::audio_mutex=0;



int AudioControl::get_output_latency() {
	
	return 0;
//	return SoundDriverList::get_singleton()->get_output_latency();
}

/* Audio Mutex */
void AudioControl::mutex_lock() {
	
	if (!AudioDriver::get_current_driver())
		return;
		
	AudioDriver::get_current_driver()->lock();
}
void AudioControl::mutex_unlock() {

	if (!AudioDriver::get_current_driver())
		return;
		
	AudioDriver::get_current_driver()->unlock();
	
	
}

void AudioControl::reset() {
	
	if (!AudioDriver::get_current_driver())
		return;
		
	AudioDriver::get_current_driver()->restart();
	

}

	
