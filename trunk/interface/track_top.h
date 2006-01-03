//
// C++ Interface: track_top
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDTRACK_TOP_H
#define RESHAKEDTRACK_TOP_H

#include <Qt/qwidget.h>
#include <Qt/qmenu.h>

#include "editor/editor.h"
#include "interface/automation_dialog.h"


namespace ReShaked {

/**
	@author red <red@killy>
*/
//@ TODO Add Vu/Pan/Trackname!
				    
class TrackTop : public QWidget {
			    
	Q_OBJECT
			    
	enum TrackActions {
		/* for pattern tracks */
		ACTION_PATTERN_ADD_COLUMN,
		ACTION_PATTERN_REMOVE_COLUMN,
		
		/* track generic */
		ACTION_MOVE_TRACK_LEFT,
		ACTION_MOVE_TRACK_RIGHT,
		
		ACTION_RENAME,
		ACTION_DUPLICATE,
			
		ACTION_EFFECTS,
			
		ACTION_DELETE,
	};
			    
	Track *track;
	Editor *editor;

	AutomationPopup *automation_menu;
	QMenu *menu;
	void paintEvent(QPaintEvent *e);
	
	void mousePressEvent(QMouseEvent *e);
	void rename();
	bool can_rename;
	bool can_synths;
	
protected slots:
			
	void action_slot(QAction *p_action);
	void automation_add_slot(String p_path);
	void automation_remove_slot(String p_path);
	
public:
	enum TrackType {
		TYPE_PATTERN,
		TYPE_GLOBAL,
	};
	
	TrackTop(QWidget *p_parent,Track *p_track,Editor *p_editor,TrackType p_type);
	~TrackTop();

};

}

#endif
