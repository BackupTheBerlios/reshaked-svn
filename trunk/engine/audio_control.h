#ifndef AUDIO_CONTROL_H
#define AUDIO_CONTROL_H

#include "mutex_lock.h"

namespace ReShaked {


class AudioControl {
	
	static AudioControl *singleton_instance;
	static MutexLock *audio_mutex;

public:

	
	/* Sampling Rate */
	static float get_mix_rate();
	
	/* Audio Mutex */
	static void mutex_lock();
	static bool mutex_try_lock();
	static void mutex_unlock();
		
	static int get_output_latency();
	
	static void reset();
	
	static void init();

};



};

#endif
