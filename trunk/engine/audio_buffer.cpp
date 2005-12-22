

#include "audio_buffer.h"

namespace ReShaked {


float *AudioBuffer::get_buffer(int p_channel) {

	ERR_FAIL_INDEX_V(p_channel,(int)data.size(),NULL);
	
	return &data[p_channel][0];

}

int AudioBuffer::get_size() {

	return size;
}
int AudioBuffer::get_channels() {

	return data.size();
}


void AudioBuffer::clear(int p_frames) { ///< just clear the contents

	if (p_frames==-1 || p_frames>size)
		p_frames=size;
	
	for (int i=0;i<(int)data.size();i++)
		memset(&data[i][0],0,sizeof(float)*p_frames);
		 
}

AudioBuffer::AudioBuffer(int p_channels,int p_size) {


	size=0;

	ERR_FAIL_COND(p_channels==0);

	ERR_FAIL_COND(p_size==0);

	size=p_size;
	
	data.resize(p_channels);
	
	for (int i=0;i<(int)data.size();i++) 
		data[i].resize(size);

}

}; /* end of namespace */
