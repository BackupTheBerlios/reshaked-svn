//
// C++ Implementation: pattern_track
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pattern_track.h"



AudioNode *PatternTrack::creation_func(int p_channels,const AudioNodeInfo *p_info) {

	return new PatternTrack(p_channels,p_info);
}


const AudioNodeInfo *PatternTrack::get_creation_info() {

	static AudioNodeInfo _info;
	_info.caption="Pattern Track";
	_info.short_caption="Pattern";
	_info.description="Multicolumn Sequence Editing";
	_info.unique_ID="INTERNAL_PatternTrack";
	_info.creation_func=&PatternTrack::creation_func;
	_info.icon_string="node_pattern_track";
	_info.category="Tracks";

	return &_info;
}

void PatternTrack::process(const ProcessInfo& p_info) {



}

bool PatternTrack::accepts_block( Block *p_block ) const {

	PatternBlock *pb = dynamic_cast<PatternBlock*>(p_block);
	
	return (pb!=NULL);
}
	
Track::Block * PatternTrack::create_block(BlockCreationData *p_data) {

	return new PatternBlock;
}

Track::BlockCreationBehavior PatternTrack::get_block_creation_behavior() const {

	return BLOCK_CREATE_AND_RESIZE;
}
bool PatternTrack::is_block_motion_snapped() const {

	return true;
}


int PatternTrack::get_visible_columns() const {

	return visible_columns;
}


PatternTrack::PatternTrack(int p_instanced_channels,const AudioNodeInfo *p_info) : Track(p_instanced_channels,p_info) {

	add_event_port("Record",PORT_IN);
	add_event_port("Output",PORT_OUT);
	
	visible_columns=1;

}


PatternTrack::~PatternTrack() {


}


