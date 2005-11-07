//
// C++ Interface: blocklist_ui_pattern
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDBLOCKLIST_UI_PATTERN_H
#define RESHAKEDBLOCKLIST_UI_PATTERN_H


#include "interface/blocklist_ui_base.h"
#include "engine/song.h"
#include "engine/track_pattern.h"
namespace ReShaked {

/**
	@author red <red@killy>
*/
class BlockListUI_Pattern : public BlockListUI_Base {

	Song *song;
	Track_Pattern *track;
	
	
	void paint_frames(QPainter& p);
	void paint_row_lines(QPainter &p);
	void paintEvent(QPaintEvent *e);
	
public:
	
	BlockListUI_Pattern(QWidget *p_parent,Song *p_song, int p_track);
	~BlockListUI_Pattern();

};

}

#endif
