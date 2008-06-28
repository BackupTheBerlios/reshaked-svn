//
// C++ Interface: audio_process_base
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDAUDIO_PROCESS_BASE_H
#define RESHAKEDAUDIO_PROCESS_BASE_H

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class AudioProcessBase{
public:
	
	virtual int process(int p_frames)=0;	
	AudioProcessBase();
	virtual ~AudioProcessBase();

};

}

#endif
