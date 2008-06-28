//
// C++ Interface: bottom_bar_controls
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDBOTTOM_BAR_CONTROLS_H
#define RESHAKEDBOTTOM_BAR_CONTROLS_H

#include "ui_blocks/helpers.h"
#include "ui_blocks/pixmap_button.h"
#include "ui_blocks/pixmap_label.h"
#include "ui_blocks/pixmap_updown.h"
#include "editor/editor.h"
#include "interface/piano_keys.h"

#include <Qt/qtimer.h>

namespace ReShaked {

/**
	@author red <red@killy>
*/
class BottomBarControls : public CHBox {
	
	Q_OBJECT
	
	enum {
		
		MSG_NOTIFY_TIME_MS=3000,
		UPDATE_INTERVAL_MS=80
	};
			
	Editor *editor;
	
	QTimer *time_updater;	
	PixmapLabel *time_label;
	PixmapLabel *info_label;
	PixmapLabel *label_octave;
	PixmapUpDown *updown_octave;
	friend class MainWindow;	
	
	PixmapButton *rack_toggle;
	
	Piano_Keys *vpiano;
	
	QString last_time;
	QString last_msg;
		
	int tmp_msg_time;
	
	
	void update_time_label(QString p_str,bool p_play);
	
public slots:
			
	
	
	void action_notify(String p_action);
	void time_update_slot();

	void octave_edit_popup();
	void octave_raise();
	void octave_lower();
	void octave_changed_slot();
		
public:
	BottomBarControls(QWidget *p_parent,Editor *p_editor);
	
	~BottomBarControls();

};

}

#endif
