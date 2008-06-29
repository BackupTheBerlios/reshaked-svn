//
// C++ Interface: audio_driver_node
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef AUDIO_DRIVER_NODE_H
#define AUDIO_DRIVER_NODE_H

#include "engine/audio_node.h"
#include <list>

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

class AudioDriverNode : public AudioNode {
public:

	virtual void get_connectable_list( std::list<String>* p_list ) const=0;
	virtual String get_connected_to() const=0;
	virtual Error connect(String p_to)=0;

	AudioDriverNode();
	~AudioDriverNode();

};

#endif
