#ifndef AUDIO_NODE_H
#define AUDIO_NODE_H


#include "engine/audio_plug.h"

namespace ReShaked {


class AudioNode {

public:

	virtual int get_input_plug_count()=0;
	virtual int get_output_plug_count()=0;
	virtual AudioPlug *get_input_plug(int p_index)=0;
	virtual AudioPlug *get_output_plug(int p_index)=0;
	
	virtual bool is_input_plug_enabled(int p_idx);
	virtual bool is_output_plug_enabled(int p_idx);
	virtual String get_input_plug_caption(int p_which);
	virtual String get_output_plug_caption(int p_which);
	virtual bool is_system();	
	virtual String get_caption()=0;
	
	virtual void process(int p_frames)=0;
	virtual ~AudioNode() {}; /* here, have your virtual destructor, gcc */
};


}; /* end of namespace */

#endif



