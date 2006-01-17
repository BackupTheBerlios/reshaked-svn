//
// C++ Implementation: top_bar_controls
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "top_bar_controls.h"
#include "ui_blocks/visual_settings.h"
#include "ui_blocks/pixmap_label.h"

#include <Qt/qinputdialog.h>

namespace ReShaked {


void TopBarControls::show_settings_menu() {
	
	settings_menu->adjustSize();
	int ofs=settings_menu->width()-icon_menu_settings->width();
	settings_menu->popup(icon_menu_settings->mapToGlobal( QPoint(-ofs,icon_menu_settings->height()) ) );
}
void TopBarControls::show_help_menu() {
	
	help_menu->adjustSize();
	int ofs=help_menu->width()-icon_menu_help->width();
	help_menu->popup(icon_menu_help->mapToGlobal( QPoint(-ofs,icon_menu_help->height()) ) );
	
}

void TopBarControls::show_file_menu() {
	
	file_menu->adjustSize();
	file_menu->popup(icon_menu_file->mapToGlobal( QPoint(0,icon_menu_file->height()) ) );
	
}

void TopBarControls::show_add_menu() {
	
	add_menu->adjustSize();
	add_menu->popup(icon_menu_add->mapToGlobal( QPoint(0,icon_menu_add->height()) ) );
	
}

void TopBarControls::undo_pressed() {
	
	
}

void TopBarControls::redo_pressed() {
	
	
}

void TopBarControls::playback_play_slot(bool p_selected) {
	
	editor->get_song()->play(0);
}

void TopBarControls::playback_loop_slot(bool p_selected) {
	
	editor->get_song()->loop();
}

void TopBarControls::playback_pause_slot(bool p_selected) {
	
	
}
void TopBarControls::playback_stop_slot(bool p_selected) {
	
	editor->get_song()->stop();	
}


void TopBarControls::record_automations_toggle(bool p_record) {
	
	
	editor->get_song()->get_song_playback().set_recording_automations( p_record );
}


void TopBarControls::update_playback_indicator() {
	
	switch (editor->get_song()->get_song_playback().get_status()) {
		
		case SongPlayback::STATUS_PLAY: {
			
			
			
			control_button_play->set_pressed( true );			
			control_button_loop->set_pressed( editor->get_song()->get_song_playback().is_loop_active() );
			control_button_pause->set_pressed( false );
			control_button_stop->set_pressed( false );
			
		} break;
		case SongPlayback::STATUS_PAUSE: {
			
			control_button_loop->set_pressed( false );
			control_button_play->set_pressed( false );
			control_button_pause->set_pressed( true );
			control_button_stop->set_pressed( false );
		} break;
		case SongPlayback::STATUS_STOP: {
			
			control_button_loop->set_pressed( false );
			control_button_play->set_pressed( false );
			control_button_pause->set_pressed( false );
			control_button_stop->set_pressed( true );
		} break;
		
	}
	
	control_button_record_auto->set_pressed(editor->get_song()->get_song_playback().is_recording_automations() );
}


void TopBarControls::song_screen_selected(bool p_selected) {
	
	set_screen(SCREEN_SONG);
}
void TopBarControls::edit_screen_selected(bool p_selected) {
	
	set_screen(SCREEN_EDIT);
	
}
void TopBarControls::mix_screen_selected(bool p_selected) {
	
	set_screen(SCREEN_MIX);
	
}


void TopBarControls::set_screen(ScreenList p_screen) {
	
	switch (p_screen) {
			
		case SCREEN_SONG: {
			
			song_view->set_pressed( true );
			edit_view->set_pressed( false );
			mix_view->set_pressed( false );
		} break;
		case SCREEN_EDIT: {
			
			song_view->set_pressed( false );
			edit_view->set_pressed( true );
			mix_view->set_pressed( false );

		} break;
		case SCREEN_MIX: {
			
			song_view->set_pressed( false );
			edit_view->set_pressed( false );
			mix_view->set_pressed( true );
			
		} break;
	}
	
	screen_changed_signal( p_screen );
}

void TopBarControls::octave_edit_popup() {
	bool ok;
	
	int o=QInputDialog::getInteger ( this, "Editing Octave", "Octave:", editor->get_editing_octave(), 0, 8,1,&ok);
	if (ok)
		editor->set_editing_octave( o );
	
	octave_changed_slot();
}

void TopBarControls::octave_raise() {
	
	editor->set_editing_octave( editor->get_editing_octave() + 1 );
	octave_changed_slot();
}
void TopBarControls::octave_lower() {
	
	editor->set_editing_octave( editor->get_editing_octave() - 1 );
	octave_changed_slot();
	
}


void TopBarControls::octave_changed_slot() {
	
	label_octave->set_text( QString::number( editor->get_editing_octave() ) );			
}

QMenu * TopBarControls::get_file_menu() {
	
	return file_menu;
}
QMenu * TopBarControls::get_add_menu() {
	
	return add_menu;
}
QMenu * TopBarControls::get_settings_menu() {
	
	return settings_menu;
	
}
QMenu * TopBarControls::get_help_menu() {
	
	return help_menu;
	
}


TopBarControls::TopBarControls(QWidget *p_parent,Editor *p_editor,PropertyEditUpdater *p_property_edit_updater) : CHBox(p_parent){
	
	editor=p_editor;
	new PixmapLabel(this,GET_QPIXMAP(THEME_TOP__MARGIN_LEFT));
	
	
	song_view = new PixmapButton(this,PixmapButton::Skin(GET_QPIXMAP(THEME_TOP__TAB_SONG),GET_QPIXMAP(THEME_TOP__TAB_SONG_ACTIVE)),PixmapButton::TYPE_TOGGLE);
	song_view->set_pressed( true );
	QObject::connect(song_view,SIGNAL(mouse_toggled_signal(bool)),this,SLOT(song_screen_selected( bool )));
	edit_view = new PixmapButton(this,PixmapButton::Skin(GET_QPIXMAP(THEME_TOP__TAB_EDIT),GET_QPIXMAP(THEME_TOP__TAB_EDIT_ACTIVE)),PixmapButton::TYPE_TOGGLE);
	QObject::connect(edit_view,SIGNAL(mouse_toggled_signal(bool)),this,SLOT(edit_screen_selected( bool )));
	mix_view = new PixmapButton(this,PixmapButton::Skin(GET_QPIXMAP(THEME_TOP__TAB_MIX),GET_QPIXMAP(THEME_TOP__TAB_MIX_ACTIVE)),PixmapButton::TYPE_TOGGLE);
	QObject::connect(mix_view,SIGNAL(mouse_toggled_signal(bool)),this,SLOT(mix_screen_selected( bool )));

	
	
	
	CVBox *vb = new CVBox(this);
	CHBox *controls_hbox = new CHBox(vb);
	
	icon_menu_file = new PixmapButton(controls_hbox,PixmapButton::Skin(GET_QPIXMAP(THEME_TOP__ICON_FILE),GET_QPIXMAP(THEME_TOP__ICON_FILE_ACTIVATED)));
	QObject::connect(icon_menu_file,SIGNAL(mouse_pressed_signal()),this,SLOT(show_file_menu()));
	
	icon_menu_add = new PixmapButton(controls_hbox,PixmapButton::Skin(GET_QPIXMAP(THEME_TOP__ICON_ADD),GET_QPIXMAP(THEME_TOP__ICON_ADD_ACTIVATED)));
	QObject::connect(icon_menu_add,SIGNAL(mouse_pressed_signal()),this,SLOT(show_add_menu()));
	
	icon_undo = new PixmapButton(controls_hbox,PixmapButton::Skin(GET_QPIXMAP(THEME_TOP__ICON_UNDO),GET_QPIXMAP(THEME_TOP__ICON_UNDO_ACTIVATED)));
	QObject::connect(icon_undo,SIGNAL(mouse_pressed_signal()),this,SLOT(undo_pressed()));
	
	icon_redo = new PixmapButton(controls_hbox,PixmapButton::Skin(GET_QPIXMAP(THEME_TOP__ICON_REDO),GET_QPIXMAP(THEME_TOP__ICON_REDO_ACTIVATED)));
	QObject::connect(icon_redo,SIGNAL(mouse_pressed_signal()),this,SLOT(redo_pressed()));
	
	
	
	/** CONTROLS **/
	
	CVBox *buttons_vb = new CVBox(controls_hbox);
	new PixmapLabel(buttons_vb,GET_QPIXMAP(THEME_TOP__BUTTONS_TOP));
	CHBox *buttons_hb = new CHBox(buttons_vb);
	
	control_button_rw = new PixmapButton(buttons_hb,PixmapButton::Skin(GET_QPIXMAP(THEME_TOP__BUTTON_REWIND),GET_QPIXMAP(THEME_TOP__BUTTON_REWIND_ACTIVE)));
	
	control_button_play = new PixmapButton(buttons_hb,PixmapButton::Skin(GET_QPIXMAP(THEME_TOP__BUTTON_PLAY),GET_QPIXMAP(THEME_TOP__BUTTON_PLAY_ACTIVE)),PixmapButton::TYPE_TOGGLE);
	QObject::connect(control_button_play,SIGNAL(mouse_toggled_signal( bool )),this,SLOT(playback_play_slot( bool )));
	
	control_button_loop = new PixmapButton(buttons_hb,PixmapButton::Skin(GET_QPIXMAP(THEME_TOP__BUTTON_LOOP),GET_QPIXMAP(THEME_TOP__BUTTON_LOOP_ACTIVE)),PixmapButton::TYPE_TOGGLE);
	QObject::connect(control_button_loop,SIGNAL(mouse_toggled_signal( bool )),this,SLOT(playback_loop_slot( bool )));
	
	control_button_pause = new PixmapButton(buttons_hb,PixmapButton::Skin(GET_QPIXMAP(THEME_TOP__BUTTON_PAUSE),GET_QPIXMAP(THEME_TOP__BUTTON_PAUSE_ACTIVE)),PixmapButton::TYPE_TOGGLE);
	QObject::connect(control_button_pause,SIGNAL(mouse_toggled_signal( bool )),this,SLOT(playback_pause_slot( bool )));
	
	control_button_stop = new PixmapButton(buttons_hb,PixmapButton::Skin(GET_QPIXMAP(THEME_TOP__BUTTON_STOP),GET_QPIXMAP(THEME_TOP__BUTTON_STOP_ACTIVE)),PixmapButton::TYPE_TOGGLE);
	QObject::connect(control_button_stop,SIGNAL(mouse_toggled_signal( bool )),this,SLOT(playback_stop_slot( bool )));
	
	control_button_ff = new PixmapButton(buttons_hb,PixmapButton::Skin(GET_QPIXMAP(THEME_TOP__BUTTON_FASTFORWARD),GET_QPIXMAP(THEME_TOP__BUTTON_FASTFORWARD_ACTIVE)));
	
	control_button_record = new PixmapButton(buttons_hb,PixmapButton::Skin(GET_QPIXMAP(THEME_TOP__BUTTON_RECORD),GET_QPIXMAP(THEME_TOP__BUTTON_RECORD_ACTIVE)),PixmapButton::TYPE_TOGGLE);
	
	control_button_record_auto = new PixmapButton(buttons_hb,PixmapButton::Skin(GET_QPIXMAP(THEME_TOP__BUTTON_RECORD_AUTOMATIONS),GET_QPIXMAP(THEME_TOP__BUTTON_RECORD_AUTOMATIONS_ACTIVE)),PixmapButton::TYPE_TOGGLE);
	QObject::connect(control_button_record_auto,SIGNAL(mouse_toggled_signal( bool )),this,SLOT(record_automations_toggle( bool )));
	
	buttons_hb->layout()->setSpacing(0);
	buttons_hb->layout()->setMargin(0);
	buttons_vb->setFixedWidth(GET_QPIXMAP(THEME_TOP__BUTTONS_BOTTOM).width());
	
	new PixmapLabel(buttons_vb,GET_QPIXMAP(THEME_TOP__BUTTONS_BOTTOM));
	
	new PixmapLabel(controls_hbox,GET_QPIXMAP(THEME_TOP__BUTTONS_BPM_SEPARATOR));
	
	CVBox *bpm_vb = new CVBox(controls_hbox);
	new PixmapLabel(bpm_vb,GET_QPIXMAP(THEME_TOP__BPM_TOP));
	CHBox *bpm_hb = new CHBox(bpm_vb);

	label_prop_bpm = new PropertyEditLabel(bpm_hb,GET_QPIXMAP(THEME_TOP__BPM_DISPLAY));
	label_prop_bpm->set_property( &p_editor->get_song()->get_global_properties().get_tempo());
	p_property_edit_updater->add_editor( label_prop_bpm );
	updown_prop_bpm = new PropertyEditUpDown(bpm_hb,GET_QPIXMAP(THEME_TOP__BPM_UPDOWN));
	updown_prop_bpm->set_property( label_prop_bpm->get_property() );
	updown_prop_bpm->add_to_group(label_prop_bpm);
	updown_prop_bpm->set_step(1);
	
	new PixmapLabel(bpm_vb,GET_QPIXMAP(THEME_TOP__BPM_BOTTOM));
	
	new PixmapLabel(controls_hbox,GET_QPIXMAP(THEME_TOP__BPM_OCTAVE_SEPARATOR));
	
	CVBox *octave_vb = new CVBox(controls_hbox);
	new PixmapLabel(octave_vb,GET_QPIXMAP(THEME_TOP__OCTAVE_TOP));
	CHBox *octave_hb = new CHBox(octave_vb);
	
	label_octave = new PixmapLabel(octave_hb,GET_QPIXMAP(THEME_TOP__OCTAVE_DISPLAY));
	QObject::connect(label_octave,SIGNAL(clicked_signal()),this,SLOT(octave_edit_popup()));
	updown_octave = new PixmapUpDown(octave_hb,GET_QPIXMAP(THEME_TOP__OCTAVE_UPDOWN));
	QObject::connect(updown_octave,SIGNAL(up_pressed_signal()),this,SLOT(octave_raise()));
	QObject::connect(updown_octave,SIGNAL(down_pressed_signal()),this,SLOT(octave_lower()));
	octave_changed_slot();
	
	new PixmapLabel(octave_vb,GET_QPIXMAP(THEME_TOP__OCTAVE_BOTTOM));

	new PixmapLabel(controls_hbox,GET_QPIXMAP(THEME_TOP__OCTAVE_END));
	
	
	
	/** END OF CONTROLS */
	
	(new PixmapLabel(controls_hbox,GET_QPIXMAP(THEME_TOP__CONTROLS_END),PixmapLabel::EXPAND_TILE_H))->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	
	icon_menu_settings = new PixmapButton(controls_hbox,PixmapButton::Skin(GET_QPIXMAP(THEME_TOP__ICON_SETTINGS),GET_QPIXMAP(THEME_TOP__ICON_SETTINGS_ACTIVATED)));
	QObject::connect(icon_menu_settings,SIGNAL(mouse_pressed_signal()),this,SLOT(show_settings_menu()));
	icon_menu_help= new PixmapButton(controls_hbox,PixmapButton::Skin(GET_QPIXMAP(THEME_TOP__ICON_HELP),GET_QPIXMAP(THEME_TOP__ICON_HELP_ACTIVATED)));
	QObject::connect(icon_menu_help,SIGNAL(mouse_pressed_signal()),this,SLOT(show_help_menu()));

	
	new PixmapLabel(vb,GET_QPIXMAP(THEME_TOP__MARGIN_BOTTOM),PixmapLabel::EXPAND_TILE_H);
	new PixmapLabel(this,GET_QPIXMAP(THEME_TOP__MARGIN_RIGHT));
	
	file_menu = new QMenu(this);
	settings_menu = new QMenu(this);
	help_menu = new QMenu(this);
	add_menu = new QMenu(this);

	
}


TopBarControls::~TopBarControls()
{
}


}
