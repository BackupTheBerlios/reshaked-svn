

#include "audio_buffer.h"
#include <math.h>
namespace ReShaked {

float AudioBuffer::get_highest_energy(int p_frames) {
	
	if (p_frames<0 || p_frames>size)
		p_frames=size;
	
	float max=0;
	for (int i=0;i<(int)data.size();i++) {
		float *ptr=&data[i][0];
		for (int j=0;j<p_frames;j++) {
			
			float nrg=fabsf(ptr[j]);
			if (nrg>max)
				max=nrg;
		}
	}
		
	return max;
}
/*
float *AudioBuffer::get_buffer(int p_channel) {

	ERR_FAIL_INDEX_V(p_channel,(int)data.size(),NULL);
	
	return &data[p_channel][0];

}*/

int AudioBuffer::get_size() {

	return size;
}


void AudioBuffer::clear(int p_frames) { ///< just clear the contents

	if (p_frames==-1 || p_frames>size)
		p_frames=size;
	
	for (int i=0;i<(int)data.size();i++)
		memset(&data[i][0],0,sizeof(float)*p_frames);
		 
}



void AudioBuffer::copy_to_interleaved(float *p_dst,int p_frames) {
	
	ERR_FAIL_COND(p_frames>size);///< check if frames are valid
		
	int skip=data.size();
	
	for (int i=0;i<skip;i++) {
		
		float *src=&data[i][0];
		float *dst=&p_dst[0];
	
		int dst_idx=i;
		for (int j=0;j<p_frames;j++) {
			dst[dst_idx]=src[j];
			dst_idx+=skip;
			
		}
			
	}
	
	
}

void AudioBuffer::copy_from_interleaved(float *p_src,int p_frames) {
	
	
	ERR_FAIL_COND(p_frames>size);///< check if frames are valid
		
	int skip=data.size();
	
	for (int i=0;i<skip;i++) {
		
		float *dst=&data[i][0];
		float *src=&p_src[i];
	
		
		for (int j=0,k=0;j<p_frames;j++,k+=skip) {
			dst[j]=src[k];
			
		}
			
	}
	
	
}
void AudioBuffer::copy_from(AudioBuffer *p_src,int p_frames) {


	ERR_FAIL_COND(p_frames>size);///< check if frames are valid
	ERR_FAIL_COND(p_src->data.size()!=data.size()); ///check if channels are valid
	
	for (int i=0;i<data.size();i++) {
		
		float *dst=&data[i][0];
		float *src=&p_src->data[i][0];
		
		memcpy(dst,src,sizeof(float)*p_frames);
		//for (int j=0;j<p_frames;j++)
		//	dst[j]=src[j];
	}
	
}
void AudioBuffer::add_from(AudioBuffer *p_src,int p_frames) {
	
	ERR_FAIL_COND(p_frames>size);///< check if frames are valid
	ERR_FAIL_COND(p_src->data.size()!=data.size()); ///check if channels are valid
	
	for (int i=0;i<data.size();i++) {
		
		float *dst=&data[i][0];
		float *src=&p_src->data[i][0];
		
		int todo=p_frames>>2;
		
		while (todo--) {
			
			*(dst++)+=*(src++);
			*(dst++)+=*(src++);
			*(dst++)+=*(src++);
			*(dst++)+=*(src++);
		}
		
		todo=p_frames&3;
		
		while (todo--)
			*(dst++)+=*(src++);
		
	}
	
}

void AudioBuffer::mult_from(AudioBuffer *p_src,int p_frames) {
	
	ERR_FAIL_COND(p_frames>size);///< check if frames are valid
	ERR_FAIL_COND(p_src->data.size()!=data.size()); ///check if channels are valid
	
	for (int i=0;i<data.size();i++) {
		
		float *dst=&data[i][0];
		float *src=&p_src->data[i][0];
		
		int todo=p_frames;
		
		while (todo--) {
			
			dst[todo]*=src[todo];
		}
		
	}
	
}

void AudioBuffer::multiply_by(float p_val,int p_frames) {
	
	ERR_FAIL_COND(p_frames>size);///< check if frames are valid
	
	for (int i=0;i<data.size();i++) {
		
		float *buff=&data[i][0];
		
		for (int j=0;j<p_frames;j++)
			buff[j]*=p_val;
		
	}
	
}
void AudioBuffer::setup(int p_channels,int p_size) {
	
	size=0;

	ERR_FAIL_COND(p_channels==0);

	ERR_FAIL_COND(p_size==0);

	size=p_size;
	
	data.resize(p_channels);
	
	for (int i=0;i<(int)data.size();i++) {
		
		data[i].resize(size);
		
		for (int j=0;j<size;j++)
			data[i][j]=0;
	}
	
	
}

AudioBuffer::AudioBuffer() {
	
	
}
AudioBuffer::AudioBuffer(int p_channels,int p_size) {

	setup(p_channels,p_size);

}

}; /* end of namespace */
