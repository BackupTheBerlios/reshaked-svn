//
// C++ Interface: qt_updatenotify
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDQT_UPDATENOTIFY_H
#define RESHAKEDQT_UPDATENOTIFY_H

#include <Qt/qobject.h>
#include "engine/ui_updatenotify.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class Qt_UpdateNotify : public QObject, public UI_UpdateNotify  {
			    
	Q_OBJECT
			
signals:			
			
	void cursor_moved();
	void edit_window_changed();
	void cursor_changed_blocklist();
	void track_list_changed();
	void edit_window_moved();
	
public:
	
	
	Qt_UpdateNotify(QObject *p_parent);
	~Qt_UpdateNotify();
	

};

}

#endif
