//
// C++ Interface: global_track
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDGLOBAL_TRACK_H
#define RESHAKEDGLOBAL_TRACK_H


#include "engine/track.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class GlobalTrack : public Track {
			    
	/* Useles Overrides */
	Block *create_duplicate_block(Block *p_block);
	Block *create_link_block(Block *p_block);
	String get_type_name();
	Block* create_block(BlockCreationData *p_creation_data);
	bool shares_block_data(Block *p_block);
	bool accepts_block(Block *p_block);
	BlockCreationBehavior get_block_creation_behavior();
	bool can_resize_from_begining();
	void process_track(AudioBuffer *p_in_buf,AudioBuffer *p_out_buf,int p_frames);	
	
public:
	
	GlobalTrack(GlobalProperties *p_global_props,SongPlayback *p_song_playback);
	~GlobalTrack();

};

}

#endif
