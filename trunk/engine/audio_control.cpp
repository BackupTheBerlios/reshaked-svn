
#include "audio_control.h"


namespace ReShaked {

AudioControl* AudioControl::singleton_instance=0;

AudioControl *AudioControl::singleton() {
	
	return singleton_instance;
}



float AudioControl::get_sample_rate() {
	
	return 0;	
	
}

/* Audio Mutex */
void AudioControl::mutex_lock() {
	
	
	
}
bool AudioControl::mutex_try_lock() {
	
	
	return false;	
}
void AudioControl::mutex_unlock() {
	
	
	
}

	
	
};
