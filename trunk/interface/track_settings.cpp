//
// C++ Implementation: track_settings
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "track_settings.h"
#include "ui_blocks/visual_settings.h"
#include "error_macros.h"

namespace ReShaked {

void TrackSettings::add_button(TrackSettingsPage p_page, QPixmap p_pixmap,const char *p_slot) {
	
	buttons[p_page] = new QPushButton(p_pixmap,"",vb);
	buttons[p_page]->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	buttons[p_page]->setIconSize(QSize(32,32));
	buttons[p_page]->setCheckable(true);
	QObject::connect(buttons[p_page],SIGNAL(clicked()),this,p_slot);
	buttons[p_page]->setFocusPolicy(Qt::NoFocus);
	
}

void TrackSettings::button_1_checked() {
	
	set_page(TRACK_SETTINGS_PATTERN);
}
void TrackSettings::button_2_checked() {
	
	set_page(TRACK_SETTINGS_CONTROLS);
	
}
void TrackSettings::button_3_checked() {
	
	set_page(TRACK_SETTINGS_EFFECTS);
	
}
void TrackSettings::button_4_checked() {
	
	set_page(TRACK_SETTINGS_CONNECTIONS);
	
}

void TrackSettings::set_page(TrackSettingsPage p_page) {
	
	ERR_FAIL_COND(p_page==TRACK_SETTINGS_MAX);
	
	for(int i=0;i<TRACK_SETTINGS_MAX;i++) {
			
		buttons[i]->setChecked(i==p_page);
		if (i==p_page && widgets[i])
			stack->setCurrentWidget(widgets[i]);
	}
}



void TrackSettings::selected_track_changed_slot() {
	
	int track_idx=editor->get_current_track();
	printf("track idx %i\n",track_idx);
	if (track_idx==-1)
		return;
	Track * track = editor->get_song()->get_track( track_idx );
	control_settings->set_track( track );
	
}

void TrackSettings::automation_add_slot(String p_path) {
	
	editor->add_automation( p_path );
	
}
void TrackSettings::automation_remove_slot(String p_path) {
	
}


TrackSettings::TrackSettings(QWidget *p_parent,Editor * p_editor) :CHBox(p_parent) {
	
	vb= new CVBox(this);
	add_button(TRACK_SETTINGS_PATTERN,VisualSettings::get_singleton()->get_pixmap(PIXMAP_TRACK_SETTINGS_PATTERN),SLOT(button_1_checked()));
	add_button(TRACK_SETTINGS_CONTROLS,VisualSettings::get_singleton()->get_pixmap(PIXMAP_TRACK_SETTINGS_CONTROLS),SLOT(button_2_checked()));
	add_button(TRACK_SETTINGS_EFFECTS,VisualSettings::get_singleton()->get_pixmap(PIXMAP_TRACK_SETTINGS_EFFECTS),SLOT(button_3_checked()));
	add_button(TRACK_SETTINGS_CONNECTIONS,VisualSettings::get_singleton()->get_pixmap(PIXMAP_TRACK_SETTINGS_CONNECTIONS),SLOT(button_4_checked()));
	vb->layout()->setMargin(0);
	vb->layout()->setSpacing(0);
	
	memset(widgets,0,sizeof(QWidget*)*TRACK_SETTINGS_MAX); //nullify them
	
	stack = new QStackedWidget(this);
		
	control_settings = new TrackControlSettings(stack);
	widgets[TRACK_SETTINGS_CONTROLS]=control_settings;
	
	stack->addWidget(control_settings);
	layout()->setSpacing(20);
	editor=p_editor;
	
	QObject::connect(control_settings->get_automation_tree(),SIGNAL(attempt_automation_add_signal( String )),this,SLOT(automation_add_slot( String )));
	
	QObject::connect(control_settings->get_automation_tree(),SIGNAL(attempt_automation_remove_signal( String )),this,SLOT(automation_remove_slot( String )));
	
}


TrackSettings::~TrackSettings()
{
}


}
