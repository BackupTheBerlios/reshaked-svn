//
// C++ Interface: main_window
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

//#include "engine/audio_plugin_registry.h"

#include "base/window.h"
#include "containers/box_container.h"
#include "containers/stack_container.h"
#include "widgets/label.h"
#include "widgets/tab_bar.h"
#include "widgets/line_edit.h"
#include "bundles/menu_box.h"

#include "widgets/tab_bar.h"
#include "widgets/menu_button.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class MainWindow : public GUI::Window {

	enum FileMenu {
		
		FILE_NEW_SONG,
		FILE_OPEN_SONG,
		FILE_SAVE_SONG,
		FILE_SAVE_SONG_AS,
		FILE_IMPORT_MIDI,
		FILE_EXPORT_MIDI,
		FILE_EXPORT_WAV,
		FILE_QUIT,	
	};

	enum ControlButton {
	
		CONTROL_PLAY,
		CONTROL_LOOP,
		CONTROL_PAUSE,
		CONTROL_STOP,
		CONTROL_FF,
		CONTROL_RW,
		CONTROL_REC,
		CONTROL_REC_AUTO,		
	};
	
	GUI::MenuBox *file_menu;	
	
	GUI::TabBar * tab_bar;
	
	GUI::StackContainer * main_stack;
	
	GUI::LineEdit *info_line;	
	GUI::LineEdit *time_line;
	
	void file_menu_callback(int p_option);
	void control_callback(int p_option);
		
	bool quit;
	
public:
	
	void init(String p_config_file="");
	
	void quit_request();
	bool must_quit();
	
	MainWindow(GUI::Painter *p_painter,GUI::Timer *p_timer,GUI::Skin *p_skin);	
	~MainWindow();

};

#endif
