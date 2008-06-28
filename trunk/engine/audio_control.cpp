
#include "audio_control.h"
#include "error_macros.h"
//#include "sound_driver_list.h"

void value_stream_global_lock() {
	
//	ReShaked::AudioControl::mutex_lock();
}

void value_stream_global_unlock() {
	
//	ReShaked::AudioControl::mutex_unlock();
}



//MutexLock *AudioControl::audio_mutex=0;



int AudioControl::get_output_latency() {
	
	return 0;
//	return SoundDriverList::get_singleton()->get_output_latency();
}
float AudioControl::get_mix_rate() {
	
	
//	return SoundDriverList::get_singleton()->get_mix_rate();	
	
}

/* Audio Mutex */
void AudioControl::mutex_lock() {
	
//	audio_mutex->grab();
	
}
bool AudioControl::mutex_try_lock() {
	
//	return audio_mutex->try_grab();
	return true;
}
void AudioControl::mutex_unlock() {
	
//	audio_mutex->release();
	
	
}

void AudioControl::reset() {
	
//	SoundDriverList::get_singleton()->reset_driver();
}

void AudioControl::init() {
	
//	audio_mutex=MutexLock::create_mutex();
}
	
