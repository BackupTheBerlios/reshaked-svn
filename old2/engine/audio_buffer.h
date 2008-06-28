#ifndef AUDIO_BUFFER_H
#define AUDIO_BUFFER_H


#include <vector>
#include "typedefs.h"
#include "error_macros.h"

namespace ReShaked {

class AudioBuffer {

	std::vector<std::vector<float> > data;
	int size;
public:
	inline float *get_buffer(int p_channel) { ///< buffer for a defined channel
		
		ERR_FAIL_INDEX_V(p_channel,(int)data.size(),NULL);
	
		return &data[p_channel][0];
	}
	int get_size(); ///< size in frames
	inline int get_channels()  ///< channels the buffer uses
		{ return data.size(); }
	void clear(int p_frames=-1); ///< just clear the contents, or specify how much
	void add_from(AudioBuffer *p_src,int p_frames);
	void mult_from(AudioBuffer *p_src,int p_frames);
	void copy_from(AudioBuffer *p_src,int p_frames);
	void copy_from_interleaved(float *p_src,int p_frames);
	void copy_to_interleaved(float *p_dst,int p_frames);
	void setup(int p_channels,int p_size);
	void multiply_by(float p_val,int p_frames);
	AudioBuffer(int p_channels,int p_size);
	AudioBuffer();
	/* helpers */
	
	float get_highest_energy(int p_frames=-1);
	
};


}; /* end of namespace */



#endif
