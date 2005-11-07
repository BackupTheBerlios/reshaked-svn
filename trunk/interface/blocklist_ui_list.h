//
// C++ Interface: blocklist_ui_list
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDBLOCKLIST_UI_LIST_H
#define RESHAKEDBLOCKLIST_UI_LIST_H

#include "interface/blocklist_ui_base.h"
#include "ui_blocks/helpers.h"
#include <Qt/qscrollarea.h>
#include <Qt/qlayout.h>
#include "engine/song.h"


namespace ReShaked {

/**
	@author red <red@killy>
*/
class BlockListUIList : public QWidget {
	
	Q_OBJECT			    
			    		    
	std::vector<BlockListUI_Base*> block_list_ui_list;
	QFrame *spacer;
	QWidget *hbox;
	QHBoxLayout *hbox_layout;
	QScrollArea *scrollarea;
	
	Song *song;
	
public:
	void update_track_list();
	
	BlockListUIList(QWidget *p_parent,Song *p_song);
	~BlockListUIList();

};

}

#endif
