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

#include "engine/editor.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
//@ TODO Add Vu/Pan/Trackname!
				    
class TrackTop : public QWidget {
			    
	Editor *editor;
	int track_idx;
	void paintEvent(QPaintEvent *e);
	
public:
	TrackTop(QWidget *p_parent,Editor *p_editor,int p_track_idx);
	~TrackTop();

};

}

#endif
