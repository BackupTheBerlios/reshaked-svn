//
// C++ Interface: track_settings
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDTRACK_SETTINGS_H
#define RESHAKEDTRACK_SETTINGS_H

#include "ui_blocks/helpers.h"
#include <Qt/qstackedwidget.h>
#include <Qt/qpushbutton.h>
#include "track_control_settings.h"
namespace ReShaked {

/**
	@author red <red@killy>
*/
class TrackSettings : public CHBox {

	Q_OBJECT
			    
	enum TrackSettingsPage {
		TRACK_SETTINGS_PATTERN,	
		//TRACK_SETTINGS_AUDIO,	
		TRACK_SETTINGS_CONTROLS,	
		TRACK_SETTINGS_EFFECTS,	
		TRACK_SETTINGS_CONNECTIONS,	
		TRACK_SETTINGS_MAX,	
	};			    
	QStackedWidget *stack;
	TrackControlSettings *control_settings;
	QWidget *widgets[TRACK_SETTINGS_MAX];
	QPushButton *buttons[TRACK_SETTINGS_MAX];
	CVBox *vb;
			    
	Editor *editor;
	
	void add_button(TrackSettingsPage p_page, QPixmap p_pixmap,const char*);
	
protected slots:
			
	/* Qt doesnt support signal binding */
	void button_1_checked();
	void button_2_checked();
	void button_3_checked();
	void button_4_checked();
	
	void automation_add_slot(String p_path);
	void automation_remove_slot(String p_path);
	
public slots:
			
	
	void selected_track_changed_slot();
public:
		
	void set_page(TrackSettingsPage p_page);
	TrackSettings(QWidget *p_parent,Editor *p_editor);
	~TrackSettings();

};

}

#endif
