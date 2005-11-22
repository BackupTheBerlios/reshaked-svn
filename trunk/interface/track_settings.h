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
#include "track_control_settings.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class TrackSettings : public CVBox {

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
	CHBox *hbox;
	CHBox *hbox_top;
			    
	Editor *editor;
	
	void add_button(TrackSettingsPage p_page, QPixmap p_pixmap,const char*);
	
	QLineEdit *track_name;
	
	QPushButton *track_edit;
	QPushButton *track_move_automation_left;
	QPushButton *track_move_automation_right;
	QPushButton *track_column_add;
	QPushButton *track_column_remove;
	
	QPushButton *setup_button(QPushButton *p_button,QPixmap p_pixmap,const char *p_slot=NULL);
	
	bool stack_visible;
	bool updating;
protected slots:
			
	/* Qt doesnt support signal binding */
	void button_1_checked();
	void button_2_checked();
	void button_3_checked();
	void button_4_checked();
	
	void track_move_left_slot();
	void track_move_right_slot();
	void track_delete_slot();
	void track_edit_slot(bool p_on);
	void track_move_automation_left_slot();
	void track_move_automation_right_slot();
	void track_column_add_slot();
	void track_column_remove_slot();
	
	
	void automation_add_slot(String p_path);
	void automation_remove_slot(String p_path);
	
	void track_name_changed(const QString &p_new_name);
	
public slots:
			
	
	void selected_track_changed_slot();
	
signals:
	void update_track_names_signal();	
	
public:
		
	void set_page(TrackSettingsPage p_page);
	TrackSettings(QWidget *p_parent,Editor *p_editor);
	~TrackSettings();

};

}

#endif
