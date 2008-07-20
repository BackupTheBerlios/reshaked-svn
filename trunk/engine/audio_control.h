#ifndef AUDIO_CONTROL_H
#define AUDIO_CONTROL_H

//#include "mutex_lock.h"


class AudioControl {
	
//	static MutexLock *audio_mutex;

public:

	
	/* Sampling Rate */
	static float get_mix_rate();
	
	/* Audio Mutex */
	static void mutex_lock();
	static void mutex_unlock();
		
	static int get_output_latency();
	
	static void reset();
	

};

// little helper to avoid making mistakes 

struct  _AudioLock {

	_AudioLock() { AudioControl::mutex_lock(); }
	~_AudioLock() { AudioControl::mutex_unlock(); }
};

#define _AUDIO_LOCK_ _AudioLock lock;


#endif
