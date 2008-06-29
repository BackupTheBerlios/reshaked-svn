//
// C++ Interface: audio_node_registry
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef AUDIO_NODE_REGISTRY_H
#define AUDIO_NODE_REGISTRY_H

#include "engine/audio_node.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class AudioNodeRegistry {

	static std::vector<const AudioNodeInfo*> node_info_array;
public:

	static void add_node_info(const AudioNodeInfo *p_info);
	static int get_node_info_count();
	const AudioNodeInfo *get_node_info(int p_index);

};

#endif
