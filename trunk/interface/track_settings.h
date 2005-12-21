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
#include <Qt/qlineedit.h>
#include "interface/track_control_settings.h"
#include "interface/automation_tree.h"
#include "interface/connection_rack.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class TrackSettings : public CVBox {

	Q_OBJECT
public:			    
	enum TrackSettingsPage {
		TRACK_SETTINGS_PATTERN,	
		//TRACK_SETTINGS_AUDIO,	
		TRACK_SETTINGS_CONTROLS,	
		TRACK_SETTINGS_AUTOMATION,
		TRACK_SETTINGS_EFFECTS,	
		TRACK_SETTINGS_CONNECTIONS,	
		
		TRACK_SETTINGS_TRACKLIST_CONNECTIONS,	
		TRACK_SETTINGS_MAX,	
	};	
private:			    
	TrackSettingsPage page;
	QStackedWidget *stack;
	
	TrackControlSettings *control_settings;
	AutomationTree *automation_settings;
	ConnectionRackTracks *tracks_rack; //inter track connections
	QWidget *widgets[TRACK_SETTINGS_MAX];
	CVBox *vb;
	CHBox *hbox;
	CHBox *hbox_top;
			    
	Editor *editor;
	
	void add_button(TrackSettingsPage p_page, QPixmap p_pixmap,const char*);
	
	
	QPushButton *setup_button(QPushButton *p_button,QPixmap p_pixmap,const char *p_slot=NULL);
	
	bool stack_visible;
	bool updating;
protected slots:
			
	/* Qt doesnt support signal binding */
	void button_1_checked();
	void button_2_checked();
	void button_3_checked();
	void button_4_checked();
	
	
public slots:
			
	
	void track_changed_slot();
	void automation_add_slot(String p_path);
	void automation_remove_slot(String p_path);
	void automation_update_status();
	void connection_rack_changed();
	
signals:
	void update_track_names_signal();	
	
public:
		
	void set_page(TrackSettingsPage p_page);
	TrackSettingsPage get_page();
	TrackSettings(QWidget *p_parent,Editor *p_editor);
	~TrackSettings();

};

}

#endif
