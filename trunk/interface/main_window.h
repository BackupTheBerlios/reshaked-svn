//
// C++ Interface: main_window
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDMAIN_WINDOW_H
#define RESHAKEDMAIN_WINDOW_H


#include "containers/box_container.h"
#include "containers/stack_container.h"
#include "widgets/label.h"
#include "widgets/line_edit.h"

#include "widgets/tab_bar.h"
#include "widgets/menu_button.h"

#include "editor/editor.h"
#include "editor/keyboard_input.h"
#include "editor/midi_input_handler.h"

#include "interface/gui_update_notify.h"
#include "interface/global_view_frame.h"
#include "interface/edit_view_frame.h"
#include "interface/new_track_dialog.h"
#include "interface/track_dialog.h"
#include "interface/automation_dialog.h"
#include "interface/automation_editor.h"

#include "engine/property_edit_updater.h"
#include "engine/sound_driver_list.h"
#include "engine/midi_driver_list.h"


using namespace GUI;

namespace ReShaked {

/**
	@author Juan Linietsky <reshaked@gmail.com>
*/
class MainWindow : public VBoxContainer {
	
	enum MenuOption {
		
		FILE_NEW_SONG,
  		FILE_OPEN_SONG,
    		FILE_SAVE_SONG,
      		FILE_SAVE_SONG_AS,
      		FILE_IMPORT_MIDI,
      		FILE_EXPORT_MIDI,
      		FILE_EXPORT_WAV,
		FILE_QUIT,
  		TRACK_ADD_PATTERN,
  		TRACK_ADD_AUDIO,
    		TRACK_MIXER,
      		EDIT_UNDO,
      		EDIT_REDO,
		CONTROL_PLAY,
		CONTROL_LOOP,
		CONTROL_PAUSE,
		CONTROL_STOP,
		CONTROL_FF,
		CONTROL_RW,
		CONTROL_REC,
		CONTROL_REC_AUTO,
      		TOOL_SETTINGS,
		TOOL_MIDI_OUTPUT_ASSIGN,
  		HELP_HELP,
    		HELP_ABOUT,
      		
	};
	
	
	void menu_callback(int p_option);
		
	
	TabBar * tab_bar;
	
	StackContainer * main_stack;
	
	LineEdit *info_line;
	
	LineEdit *time_line;
	
	
	/* Pages */
	
	GlobalViewFrame *global_view_frame;
	EditViewFrame *edit_view_frame;
	
	/* Work */
	
	NewTrackDialog *new_track_dialog;
	
	/* popups */
	
	TrackDialog *track_dialog;
	AutomationEditor *automation_editor;
	
	String last_saved_file;
	
	String settings_dir;
	String settings_file;
	
	GUI_UpdateNotify * update_notify;
	
	struct Data {

		Keyboard_Input keyboard_codes;
		Song song;
		Editor *editor;
		PropertyEditUpdater property_edit_updater;

	} data;
	
	String current_file;
	void create_keybindings();
	
	void new_track_callback();
	void set_in_window();
	
	void rebuild_track_lists();
	
	
	void show_automation_editor(Track *p_track);
protected: //slots	
	
	void notify_action_slot(String p_action);
public:
	
	bool must_quit();
	void quit_request();
	
	void initialize();
	
	MainWindow(String p_settings_dir,String p_settings_file);
	~MainWindow();

};

}

#endif
