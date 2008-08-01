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
#include "hq_reverb_node.h"
#include "filter_bank_node.h"
#include "chorus_node.h"
#include "compressor_node.h"

void register_nodes() {

	AudioNodeRegistry::add_node_info( AmpRatioNode::get_creation_info() );
	AudioNodeRegistry::add_node_info( HQ_ReverbNode::get_creation_info() );	
	AudioNodeRegistry::add_node_info( ChorusNode::get_creation_info() );	
	AudioNodeRegistry::add_node_info( FilterBankNode::get_creation_info() );	
	AudioNodeRegistry::add_node_info( CompressorNode::get_creation_info_peak() );	
	AudioNodeRegistry::add_node_info( CompressorNode::get_creation_info_rms() );	
}

