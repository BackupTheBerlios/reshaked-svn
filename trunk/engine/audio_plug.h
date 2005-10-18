#ifndef AUDIO_PLUG_H
#define AUDIO_PLUG_H

#include "engine/audio_buffer.h"
namespace ReShaked {


/**
 * This is a plug buffer, it manages inputs an output buffers for a plug/
 *
 */

class AudioNode; /* forward declaration */
 
class AudioPlug {
public:

	enum Type {
		TYPE_INPUT,
		TYPE_OUTPUT
	};
	
private:
	AudioBuffer* buffer;
	AudioNode *owner; /* audionodes always own the audioplugs */
	int channels;
	Type type;
	
public:
	void set_buffer(AudioBuffer* p_buff);
	AudioBuffer* get_buffer();
	int get_channels();
	Type get_type();
	AudioNode *get_owner();
	
	AudioPlug(int p_plug_channels,Type p_type,AudioNode* p_owner);
};


};

#endif
