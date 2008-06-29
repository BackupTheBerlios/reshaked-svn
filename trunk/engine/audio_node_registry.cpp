//
// C++ Implementation: audio_node_registry
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "audio_node_registry.h"


std::vector<const AudioNodeInfo*> AudioNodeRegistry::node_info_array;

void AudioNodeRegistry::add_node_info(const AudioNodeInfo *p_info) {

	node_info_array.push_back(p_info);
}
int AudioNodeRegistry::get_node_info_count() {

	return node_info_array.size();
}
const AudioNodeInfo *AudioNodeRegistry::get_node_info(int p_index) {

	ERR_FAIL_INDEX_V( p_index, node_info_array.size(), NULL );
	return node_info_array[ p_index ];

}
