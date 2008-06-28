//
// C++ Interface: saver
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSAVER_H
#define RESHAKEDSAVER_H


#include "tree_saver.h"
#include "engine/song.h"
#include "engine/track_pattern.h"
namespace ReShaked {

/**
	@author red <red@killy>
*/
class Saver{
	
	std::vector<Automation::AutomationBlock*> shared_automation_blocks;
	std::vector<Track_Pattern::PatternBlock*> shared_pattern_blocks;
	

	void save_track(Track *p_track,TreeSaver *p_saver);
	void save_track_pattern(Track_Pattern *p_pattern_track,TreeSaver *p_saver);
		
	int index_pattern_block(Track_Pattern::PatternBlock *p_block);
	int index_automation_block(Automation::AutomationBlock *p_block);
	
	void save_automation_block(Automation::AutomationBlock *p_block,TreeSaver *p_saver);
	void save_pattern_block(Track_Pattern::PatternBlock *p_block,TreeSaver *p_saver);
	
	bool self_contained;
	
public:
	
	
	static void save_graph(AudioGraph *p_graph,const std::vector<int> &p_node_remap,TreeSaver *p_saver);
	static void save_track_rack(Track *p_track,TreeSaver *p_saver,bool p_self_contained=false);
	
	void save_song(Song *p_song,TreeSaver *p_saver,bool p_self_contained=false);
	Saver();
	~Saver();

};

}

#endif
