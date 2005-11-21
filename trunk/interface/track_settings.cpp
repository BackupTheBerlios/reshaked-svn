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
#include <Qt/qframe.h>
#include <Qt/qlabel.h>
namespace ReShaked {

void TrackSettings::add_button(TrackSettingsPage p_page, QPixmap p_pixmap,const char *p_slot) {
	
	buttons[p_page] = new QPushButton(p_pixmap,"",hbox_top);
	buttons[p_page]->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	buttons[p_page]->setIconSize(p_pixmap.size());
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

QPushButton *TrackSettings::setup_button(QPushButton *p_button,QPixmap p_pixmap,const char *p_slot) {
	
	p_button->setIcon(p_pixmap);
	p_button->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	p_button->setIconSize(p_pixmap.size());
	if (p_slot!=NULL)
		QObject::connect(p_button,SIGNAL(clicked()),this,p_slot);
	p_button->setFocusPolicy(Qt::NoFocus);
	
	return p_button;
}

void TrackSettings::track_move_left_slot() {
	
	
}
void TrackSettings::track_move_right_slot(){
	
	
}
void TrackSettings::track_delete_slot(){
	
	
}
void TrackSettings::track_edit_slot(bool p_on){
	
	
}
void TrackSettings::track_move_automation_left_slot(){
	
	
}
void TrackSettings::track_move_automation_right_slot(){
	
	
}
void TrackSettings::track_column_add_slot(){
	
	
}
void TrackSettings::track_column_remove_slot(){
	
	
}


TrackSettings::TrackSettings(QWidget *p_parent,Editor * p_editor) :CVBox(p_parent) {
	
	
	hbox_top = new CHBox(this);
	

	setup_button(new QPushButton(hbox_top),GET_QPIXMAP(ICON_TRACK_MOVE_LEFT),SLOT(track_move_left_slot()));
	setup_button(new QPushButton(hbox_top),GET_QPIXMAP(ICON_TRACK_MOVE_RIGHT),SLOT(track_move_right_slot()));
	setup_button(new QPushButton(hbox_top),GET_QPIXMAP(ICON_TRACK_DELETE),SLOT(track_delete_slot()));
	track_edit=setup_button(new QPushButton(hbox_top),GET_QPIXMAP(ICON_TRACK_EDIT));
	track_edit->setCheckable(true);
	
	new QLabel(" Track: ",hbox_top);
	
	track_name = new QLineEdit( hbox_top );
	track_name->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
		
	track_emove_automation_left=setup_button(new QPushButton(hbox_top),GET_QPIXMAP(ICON_AUTOMATION_MOVE_LEFT),SLOT(track_move_automation_left_slot()));
	track_emove_automation_right=setup_button(new QPushButton(hbox_top),GET_QPIXMAP(ICON_AUTOMATION_MOVE_RIGHT),SLOT(track_move_automation_right_slot()));
		
	track_column_add=setup_button( new QPushButton(hbox_top), GET_QPIXMAP(ICON_COLUMN_ADD), SLOT(track_column_add_slot()));
	track_column_remove=setup_button( new QPushButton(hbox_top), GET_QPIXMAP(ICON_COLUMN_REMOVE), SLOT(track_column_remove_slot()));
	
	QFrame *fr=new QFrame(hbox_top);
	fr->setFrameStyle(QFrame::VLine+QFrame::Raised);
	fr->setFixedWidth(20);
	
	hbox = new CHBox(this);
	vb= new CVBox(hbox);
	add_button(TRACK_SETTINGS_PATTERN,VisualSettings::get_singleton()->get_pixmap(PIXMAP_TRACK_SETTINGS_PATTERN),SLOT(button_1_checked()));
	add_button(TRACK_SETTINGS_CONTROLS,VisualSettings::get_singleton()->get_pixmap(PIXMAP_TRACK_SETTINGS_CONTROLS),SLOT(button_2_checked()));
	add_button(TRACK_SETTINGS_EFFECTS,VisualSettings::get_singleton()->get_pixmap(PIXMAP_TRACK_SETTINGS_EFFECTS),SLOT(button_3_checked()));
	add_button(TRACK_SETTINGS_CONNECTIONS,VisualSettings::get_singleton()->get_pixmap(PIXMAP_TRACK_SETTINGS_CONNECTIONS),SLOT(button_4_checked()));
	vb->layout()->setMargin(0);
	vb->layout()->setSpacing(0);
	
	memset(widgets,0,sizeof(QWidget*)*TRACK_SETTINGS_MAX); //nullify them
	
	stack = new QStackedWidget(hbox);
		
	control_settings = new TrackControlSettings(stack);
	widgets[TRACK_SETTINGS_CONTROLS]=control_settings;
	
	stack->addWidget(control_settings);
	hbox->layout()->setSpacing(20);
	editor=p_editor;
	
	QObject::connect(control_settings->get_automation_tree(),SIGNAL(attempt_automation_add_signal( String )),this,SLOT(automation_add_slot( String )));
	
	QObject::connect(control_settings->get_automation_tree(),SIGNAL(attempt_automation_remove_signal( String )),this,SLOT(automation_remove_slot( String )));
	
}


TrackSettings::~TrackSettings()
{
}


}
