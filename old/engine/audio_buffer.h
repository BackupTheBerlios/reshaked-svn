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
	void clear(); ///< just clear the contents
	AudioBuffer(int p_channels,int p_size);
};


}; /* end of namespace */



#endif
