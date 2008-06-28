//
// C++ Implementation: audio_connection
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "audio_connection.h"


bool AudioConnection::operator==(const AudioConnection& p_to) const {

	return (type==p_to.type && from_node==p_to.from_node && to_node==p_to.to_node && from_port==p_to.from_port && to_port==p_to.to_port);
}


AudioConnection::AudioConnection(AudioNode::PortType p_type, int p_from_node, int p_from_port,int p_to_node, int p_to_port ) {

	type=p_type;
	from_node=p_from_node;
	from_port=p_from_port;
	to_node=p_to_node;
	to_port=p_to_port;

}

AudioConnection::AudioConnection() {

	type=AudioNode::PORT_AUDIO;
	from_node=-1;
	from_port=-1;
	to_node=-1;
	to_port=-1;	
}


