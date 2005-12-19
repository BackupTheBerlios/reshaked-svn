//
// C++ Interface: audio_port_layout
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDAUDIO_PORT_LAYOUT_H
#define RESHAKEDAUDIO_PORT_LAYOUT_H

#include <vector>

namespace ReShaked {

/**
	@author red <red@killy>
*/
class AudioPortLayout{
public:	
	std::vector<int> port_out_info;
	std::vector<int> port_in_info;

};

}

#endif
