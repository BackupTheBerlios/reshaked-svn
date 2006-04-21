//
// C++ Interface: track_rack
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDTRACK_RACK_H
#define RESHAKEDTRACK_RACK_H

#include <Qt/qscrollarea.h>
#include "editor/editor.h"
#include "ui_blocks/helpers.h"
#include "ui_blocks/pixmap_vu.h"
#include "engine/property_edit_updater.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class TrackRack : public QScrollArea {
	
	Q_OBJECT
	
	Editor *editor;
	CHBox *rack_box;
	
	void add_label(QWidget *p_parent,QString p_name);
	void add_track(Track *p_track);
	
	PropertyEditUpdater *updater;
	std::vector<PropertyEditor*> editor_list;
	
	void add_editor(PropertyEditor *p_editor);
	
	static void property_changed(void *instance,PropertyEditor *p_prop,double p_old_val);

	std::vector< std::vector<PixmapVU*> > vu_list;
	
	
public slots:	
	void rebuild_track_rack();
	void rack_height_chanegd_slot( int );
	
	void update_VUs();
public:
	
	TrackRack(QWidget *p_parent,Editor *p_editor,PropertyEditUpdater *p_updater);
	~TrackRack();

};

}

#endif
