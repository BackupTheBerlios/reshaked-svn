//
// C++ Interface: audio_connection
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef AUDIO_CONNECTION_H
#define AUDIO_CONNECTION_H

#include "engine/audio_node.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

struct AudioConnection {

		
	AudioNode::PortType type;
		
	int from_node;
	int from_port;
	int to_node;
	int to_port;
	
	bool operator==(const AudioConnection& p_to) const;
	
	AudioConnection(AudioNode::PortType p_type, int p_from_node, int p_from_port,int p_to_node, int p_to_port );
	AudioConnection();
};
	
#endif
