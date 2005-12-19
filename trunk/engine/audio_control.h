#ifndef AUDIO_CONTROL_H
#define AUDIO_CONTROL_H



namespace ReShaked {


class AudioControl {
	static AudioControl *singleton_instance;

public:

	
	static AudioControl *singleton();
	
	/* Sampling Rate */
	float get_sample_rate();
	
	/* Audio Mutex */
	static void mutex_lock();
	static bool mutex_try_lock();
	static void mutex_unlock();
	

};



};

#endif
