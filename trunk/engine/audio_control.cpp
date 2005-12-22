
#include "audio_control.h"
#include "error_macros.h"


void value_stream_global_lock() {
	
	ReShaked::AudioControl::mutex_lock();
}

void value_stream_global_unlock() {
	
	ReShaked::AudioControl::mutex_unlock();
}

namespace ReShaked {


MutexLock *AudioControl::audio_mutex=0;



float AudioControl::get_sample_rate() {
	
	return 0;	
	
}

/* Audio Mutex */
void AudioControl::mutex_lock() {
	
	audio_mutex->grab();
	
}
bool AudioControl::mutex_try_lock() {
	
	return audio_mutex->try_grab();

}
void AudioControl::mutex_unlock() {
	
	audio_mutex->release();
	
	
}

void AudioControl::init() {
	
	audio_mutex=MutexLock::create_mutex();
}
	
};
