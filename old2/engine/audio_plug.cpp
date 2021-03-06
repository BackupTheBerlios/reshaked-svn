
#include "audio_plug.h"


namespace ReShaked {


String AudioPlug::get_name() {
	
	return name;
}
void AudioPlug::set_name(String p_name) {
	
	name=p_name;
}

void AudioPlug::set_buffer(AudioBuffer *p_buff) {

	ERR_FAIL_COND(channels!=p_buff->get_channels());
			
	buffer=p_buff;

}

AudioBuffer * AudioPlug::get_buffer() {

	return buffer;

}

int AudioPlug::get_channels() {

	return channels;
}

AudioPlug::Type AudioPlug::get_type() {
	
	return type;
}
AudioPlug::AudioPlug(int p_plug_channels,Type p_type,AudioNode* p_owner) {

	channels=p_plug_channels;
	buffer=NULL;
	type=p_type;
	owner=p_owner;

}

};
