//
// C++ Interface: ui_updatenotify
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDUI_UPDATENOTIFY_H
#define RESHAKEDUI_UPDATENOTIFY_H

namespace ReShaked {

/**
	@author red <red@killy>
*/
class UI_UpdateNotify{
public:
	
	virtual void cursor_moved()=0;
	virtual void edit_window_changed()=0;
	virtual void cursor_changed_blocklist()=0;
	virtual ~UI_UpdateNotify(){}
	
};

}

#endif
