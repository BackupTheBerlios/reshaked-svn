//
// C++ Interface: top_bar_controls
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDTOP_BAR_CONTROLS_H
#define RESHAKEDTOP_BAR_CONTROLS_H

#include "ui_blocks/helpers.h"
#include "ui_blocks/pixmap_button.h"
#include "ui_blocks/property_editors.h"
#include "editor/editor.h"
#include "engine/property_edit_updater.h"
#include <Qt/qmenu.h>

namespace ReShaked {

/**
	@author red <red@killy>
*/
class TopBarControls : public CHBox {
			    
Q_OBJECT
		
public:		
	
	enum ScreenList {
		SCREEN_SONG,
		SCREEN_EDIT,
		SCREEN_MIX,
	};
		
private:		
	
	Editor *editor;
friend class MainWindow;	
	PixmapButton *song_view;
	PixmapButton *edit_view;
	PixmapButton *mix_view;
	
	PixmapButton *icon_menu_file;
	PixmapButton *icon_menu_add;
	
	PixmapButton *icon_undo;
	PixmapButton *icon_redo;
	
	PixmapButton *control_button_rw;
	PixmapButton *control_button_play;
	PixmapButton *control_button_loop;
	PixmapButton *control_button_pause;
	PixmapButton *control_button_stop;
	PixmapButton *control_button_ff;
	PixmapButton *control_button_record;
	PixmapButton *control_button_record_auto;
	
	PropertyEditLabel *label_prop_bpm;
	PropertyEditUpDown *updown_prop_bpm;
	
	PixmapLabel *label_octave;
	PixmapUpDown *updown_octave;
	
	PixmapButton *icon_menu_settings;
	PixmapButton *icon_menu_help;
	
	QMenu * file_menu;
	QMenu * settings_menu;
	QMenu * help_menu;

	
public slots:
			
	void show_settings_menu();
	void show_help_menu();
	void show_file_menu();
	
	
	void octave_edit_popup();
	void octave_raise();
	void octave_lower();
	void octave_changed_slot();
	
	void song_screen_selected(bool p_selected);
	void edit_screen_selected(bool p_selected);
	void mix_screen_selected(bool p_selected);
	

	void playback_play_slot(bool p_selected);
	void playback_loop_slot(bool p_selected);
	void playback_pause_slot(bool p_selected);
	void playback_stop_slot(bool p_selected);
	
	void record_automations_toggle(bool p_record);
	
	void update_playback_indicator();
	
	void undo_pressed();
	void redo_pressed();
	
signals:	
	
	void screen_changed_signal(ScreenList p_screen);
	
public:
	
	QMenu * get_file_menu();
	QMenu * get_settings_menu();
	QMenu * get_help_menu();
	
	
	void set_screen(ScreenList p_screen);
	TopBarControls(QWidget *p_parent,Editor *p_editor,PropertyEditUpdater *p_property_edit_updater);
	~TopBarControls();

};

}

#endif
