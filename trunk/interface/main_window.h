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
	
public:
	
	bool must_quit();
	void quit_request();
	
	void initialize();
	
	MainWindow();
	~MainWindow();

};

}

#endif
