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
#include "visual_settings.h"
#include "error_macros.h"
#include <Qt/qframe.h>
#include <Qt/qlabel.h>
namespace ReShaked {

void TrackSettings::add_button(TrackSettingsPage p_page, QPixmap p_pixmap,const char *p_slot) {
	
	/*
	buttons[p_page] = new QPushButton(p_pixmap,"",hbox_top);
	buttons[p_page]->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	buttons[p_page]->setIconSize(p_pixmap.size());
	buttons[p_page]->setCheckable(true);
	QObject::connect(buttons[p_page],SIGNAL(clicked()),this,p_slot);
	buttons[p_page]->setFocusPolicy(Qt::NoFocus);
	*/
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
			
		//buttons[i]->setChecked(i==p_page);
		if (i==p_page && widgets[i])
			stack->setCurrentWidget(widgets[i]);
	}
	
	page=p_page;
	
}

TrackSettings::TrackSettingsPage TrackSettings::get_page() {
	
	return page;	
}



void TrackSettings::track_changed_slot() {
	
	int track_idx=editor->get_current_track();
	if (track_idx==-1)
		return;
	updating=true;
	
	Track * track = editor->get_song()->get_track( track_idx );
	control_settings->set_track( track );
	automation_settings->set_track( track );
	
	bool is_track=(track==editor->get_blocklist( editor->get_current_blocklist() ));
	
	updating=false;
	
	
}


QPushButton *TrackSettings::setup_button(QPushButton *p_button,QPixmap p_pixmap,const char *p_slot) {
	
	p_button->setIcon(p_pixmap);
	p_button->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	p_button->setIconSize(p_pixmap.size());
	if (p_slot!=NULL)
		QObject::connect(p_button,SIGNAL(clicked()),this,p_slot);
	p_button->setFocusPolicy(Qt::NoFocus);
	
	return p_button;
}
void TrackSettings::automation_add_slot(String p_path) {

	editor->show_automation( p_path );

}
void TrackSettings::automation_remove_slot(String p_path) {
 
	editor->hide_automation( p_path );
	
}

void TrackSettings::automation_update_status() {
	
	automation_settings->update_item_status();
}

void TrackSettings::connection_rack_changed() {
	
	tracks_rack->update();
}

TrackSettings::TrackSettings(QWidget *p_parent,Editor * p_editor) :CVBox(p_parent) {
	
	
	
	hbox = new CHBox(this);
	vb= new CVBox(hbox);
	vb->layout()->setMargin(0);
	vb->layout()->setSpacing(0);
	
	memset(widgets,0,sizeof(QWidget*)*TRACK_SETTINGS_MAX); //nullify them
	
	stack = new QStackedWidget(hbox);
		
	control_settings = new TrackControlSettings(stack);
	widgets[TRACK_SETTINGS_CONTROLS]=control_settings;
	stack->addWidget(control_settings);
	
	automation_settings = new AutomationTree(stack);
	widgets[TRACK_SETTINGS_AUTOMATION]=automation_settings;
	stack->addWidget(automation_settings);
	
	tracks_rack = new ConnectionRackTracks(stack,p_editor);
	widgets[TRACK_SETTINGS_TRACKLIST_CONNECTIONS]=tracks_rack;
	stack->addWidget(tracks_rack);
	tracks_rack->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	hbox->layout()->setSpacing(20);
	editor=p_editor;
	
	/*
	hbox_top = new CHBox(this);
	add_button(TRACK_SETTINGS_PATTERN,VisualSettings::get_singleton()->get_pixmap(PIXMAP_TRACK_SETTINGS_PATTERN),SLOT(button_1_checked()));
	add_button(TRACK_SETTINGS_CONTROLS,VisualSettings::get_singleton()->get_pixmap(PIXMAP_TRACK_SETTINGS_CONTROLS),SLOT(button_2_checked()));
	add_button(TRACK_SETTINGS_EFFECTS,VisualSettings::get_singleton()->get_pixmap(PIXMAP_TRACK_SETTINGS_EFFECTS),SLOT(button_3_checked()));
	add_button(TRACK_SETTINGS_CONNECTIONS,VisualSettings::get_singleton()->get_pixmap(PIXMAP_TRACK_SETTINGS_CONNECTIONS),SLOT(button_4_checked()));
	*/
	QObject::connect(automation_settings,SIGNAL(attempt_automation_add_signal( String )),this,SLOT(automation_add_slot( String )));
	
	QObject::connect(automation_settings,SIGNAL(attempt_automation_remove_signal( String )),this,SLOT(automation_remove_slot( String )));
	
	updating=false;
	//stack->hide();
	//stack_visible=false;
	page=TRACK_SETTINGS_MAX;

}


TrackSettings::~TrackSettings()
{
}


}
