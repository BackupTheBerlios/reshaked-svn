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

#include "engine/editor.h"


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
	
		ACTION_DELETE,
	};
			    
	Track *track;
	Editor *editor;

	QMenu *menu;
	void paintEvent(QPaintEvent *e);
	
	void mousePressEvent(QMouseEvent *e);
	void rename();
	
protected slots:
			
	void action_slot(QAction *p_action);
public:
	TrackTop(QWidget *p_parent,Track *p_track,Editor *p_editor);
	~TrackTop();

};

}

#endif
