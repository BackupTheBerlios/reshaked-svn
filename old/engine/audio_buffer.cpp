

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


void AudioBuffer::clear() { ///< just clear the contents

	for (int i=0;i<(int)data.size();i++)
		memset(&data[i][0],0,sizeof(float)*size);
		 
}

AudioBuffer::AudioBuffer(int p_channels,int p_size) {

	size=0;

	if (p_channels==0)
		return;
	if (p_size==0)
		return;

	size=p_size;
	
	data.resize(p_channels);
	
	for (int i=0;i<(int)data.size();i++) 
		data[i].resize(size);

}

}; /* end of namespace */