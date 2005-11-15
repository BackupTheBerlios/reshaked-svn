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

namespace ReShaked {

void TrackSettings::add_button(TrackSettingsPage p_page, QPixmap p_pixmap,const char *p_slot) {
	
	buttons[p_page] = new QPushButton(p_pixmap,"",vb);
	buttons[p_page]->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	buttons[p_page]->setIconSize(QSize(32,32));
	buttons[p_page]->setCheckable(true);
	QObject::connect(buttons[p_page],SIGNAL(clicked()),this,p_slot);
	
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
	}
}

TrackSettings::TrackSettings(QWidget *p_parent) :CHBox(p_parent) {
	
	vb= new CVBox(this);
	add_button(TRACK_SETTINGS_PATTERN,VisualSettings::get_singleton()->get_pixmap(PIXMAP_TRACK_SETTINGS_PATTERN),SLOT(button_1_checked()));
	add_button(TRACK_SETTINGS_CONTROLS,VisualSettings::get_singleton()->get_pixmap(PIXMAP_TRACK_SETTINGS_CONTROLS),SLOT(button_2_checked()));
	add_button(TRACK_SETTINGS_EFFECTS,VisualSettings::get_singleton()->get_pixmap(PIXMAP_TRACK_SETTINGS_EFFECTS),SLOT(button_3_checked()));
	add_button(TRACK_SETTINGS_CONNECTIONS,VisualSettings::get_singleton()->get_pixmap(PIXMAP_TRACK_SETTINGS_CONNECTIONS),SLOT(button_4_checked()));
	vb->layout()->setMargin(0);
	vb->layout()->setSpacing(0);
}


TrackSettings::~TrackSettings()
{
}


}
