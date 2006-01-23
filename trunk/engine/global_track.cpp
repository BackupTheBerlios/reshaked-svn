//
// C++ Implementation: global_track
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "global_track.h"

namespace ReShaked {


bool GlobalTrack::can_use_synths() {
	
	
	return false;
}

BlockList::Block *GlobalTrack::create_duplicate_block(Block *p_block) {
	
	return NULL;
}
BlockList::Block *GlobalTrack::create_link_block(Block *p_block) {
	
	return NULL;
}
String GlobalTrack::get_type_name() {
	
	
	return "global";	
}
BlockList::Block* GlobalTrack::create_block(BlockCreationData *p_creation_data) {
	
	return NULL;
}
bool GlobalTrack::shares_block_data(Block *p_block) {
	
	return false;
}
bool GlobalTrack::accepts_block(Block *p_block) {
	
	
	return false;	
}
BlockList::BlockCreationBehavior GlobalTrack::get_block_creation_behavior() {
	
	
	return BLOCK_CREATE_AND_RESIZE; //useles	
	
}

bool GlobalTrack::can_resize_from_begining() {
	
	return false;
}

void GlobalTrack::process_track(AudioBuffer *p_in_buf,AudioBuffer *p_out_buf,int p_frames) {
	
	
	
}

GlobalTrack::GlobalTrack(GlobalProperties *p_global_props,SongPlayback *p_song_playback) : Track(1,BLOCK_TYPE_FIXED_TO_BEAT,p_global_props,p_song_playback) {
	
	set_name("Main");
	add_property("/",&p_global_props->get_swing());
	add_property("/",&p_global_props->get_volume());
	add_property("/",&p_global_props->get_tempo());
	
}


GlobalTrack::~GlobalTrack()
{
}


}
