//
// C++ Interface: blocklist_ui_automation
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDBLOCKLIST_UI_AUTOMATION_H
#define RESHAKEDBLOCKLIST_UI_AUTOMATION_H

#include "interface/blocklist_ui_base.h"
#include "engine/song.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class BlockListUI_Automation : public BlockListUI_Base {
public:
    BlockListUI_Automation(QWidget *p_parent, Song *p_song, int p_track,int p_automation);
    ~BlockListUI_Automation();

};

}

#endif
