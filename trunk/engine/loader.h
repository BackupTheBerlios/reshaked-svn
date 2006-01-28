//
// C++ Interface: loader
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDLOADER_H
#define RESHAKEDLOADER_H


#include "tree_loader.h"
#include "engine/song.h"
#include "engine/track_pattern.h"
#include <map>
namespace ReShaked {

/**
	@author red <red@killy>
*/
class Loader{
	
	
	std::map<int,Automation::AutomationBlock*> shared_automation_blocks;
	std::map<int,Track_Pattern::PatternBlock*> shared_pattern_blocks;
	
	void load_track(Track *p_track,TreeLoader *p_loader);
	void load_track_pattern(Track_Pattern *p_pattern_track,TreeLoader *p_loader);
		
	void load_automation_block(TreeLoader *p_loader);
	void load_pattern_block(TreeLoader *p_loader);
	
public:
	

	static void load_graph(AudioGraph *p_graph,const std::vector<int> &p_node_remap,TreeLoader *p_loader);
	static void load_track_rack(Track *p_track,TreeLoader *p_loader);

	void load_song(Song *p_song,TreeLoader *p_loader);
	Loader();
	~Loader();

};

}

#endif
