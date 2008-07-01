//
// C++ Implementation: register_nodes
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "register_nodes.h"
#include "engine/audio_node_registry.h"


#include "amp_ratio_node.h"

void register_nodes() {

	AudioNodeRegistry::add_node_info( AmpRatioNode::get_creation_info() );
}

