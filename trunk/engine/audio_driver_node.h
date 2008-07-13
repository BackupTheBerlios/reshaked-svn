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
	
	enum ConnectionType {
	
		CONNECTION_FIXED,
		CONNECTION_NAMED
	};


	virtual void get_named_connectable_list( std::list<String>* p_list ) const=0;
	virtual void get_fixed_connectable_list( std::list<String>* p_list ) const=0;
	
	virtual void connect_fixed(int p_index,int p_channel=-1)=0; // channel for audioports
	virtual void connect_named(String p_name,int p_channel=-1)=0; // channel for audioports
	
	virtual void disconnect()=0;
	
	virtual ConnectionType get_connection_type() const=0;
	
	virtual String get_named_connection() const=0;
	virtual int get_fixed_connection() const=0;
	virtual int get_fixed_connection_channel() const=0; // for audio ports
	
	virtual PortType get_driver_node_type() const=0;
	virtual PortFlow get_driver_node_flow() const=0;

	AudioDriverNode();
	~AudioDriverNode();

};

#endif
