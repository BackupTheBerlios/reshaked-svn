#ifndef AUDIO_BUFFER_H
#define AUDIO_BUFFER_H


#include <vector>
#include "typedefs.h"

namespace ReShaked {

class AudioBuffer {

	std::vector<std::vector<float> > data;
	int size;
public:
	float *get_buffer(int p_channel); ///< buffer for a defined channel
	int get_size(); ///< size in frames
	int get_channels(); ///< channels the buffer uses
	void clear(int p_frames=-1); ///< just clear the contents, or specify how much
	void add_from(AudioBuffer *p_src,int p_frames);
	void copy_from(AudioBuffer *p_src,int p_frames);
	AudioBuffer(int p_channels,int p_size);
	
	/* helpers */
	
	float get_highest_energy(int p_frames=-1);
	
};


}; /* end of namespace */



#endif
