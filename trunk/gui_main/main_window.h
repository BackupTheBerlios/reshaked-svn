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

#include "editor/editor.h"
#include "base/window.h"
#include "containers/box_container.h"
#include "containers/stack_container.h"
#include "widgets/label.h"
#include "widgets/tab_bar.h"
#include "widgets/line_edit.h"
#include "bundles/menu_box.h"

#include "widgets/tab_bar.h"
#include "widgets/menu_button.h"
#include "engine/song.h"
#include "gui_main/gui_update_notify.h"
#include "gui_main/add_node_dialog.h"
#include "gui_main/track_editor_screen.h"
#include "gui_main/global_screen.h"
#include "gui_main/audio_graph_screen.h"
#include "editor/key_bindings.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class MainWindow : public GUI::Window {

	enum TabList {
	
		TAB_GLOBAL,
		TAB_TRACKS,
		TAB_GRAPH
	};

	enum MenuOptions {
		
		FILE_NEW_SONG,
		FILE_OPEN_SONG,
		FILE_SAVE_SONG,
		FILE_SAVE_SONG_AS,
		FILE_IMPORT_MIDI,
		FILE_EXPORT_MIDI,
		FILE_EXPORT_WAV,
		FILE_QUIT,	
	
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
	
	GUI::SpinBox *tempo_spin;
	GUI::SpinBox *octave_spin;
	
	GUI::StackContainer * main_stack;
	
	
	
	GUI::LineEdit *info_line;	
	GUI::LineEdit *time_line;
		
	void octave_spin_changed(double p_to_octave);
	void tempo_spin_changed(double p_bpm);
	void initial_tempo_changed();
	void unhandled_key(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask);
	void pre_tab_changed(int p_tab);
	
	void tab_changed(int p_tab);
	void file_menu_callback(int p_option);
	void control_callback(int p_option);
	void add_node_callback();
		
	bool quit;
	
	Song song;
	Editor *editor;
	GUI_UpdateNotify gui_update_notify;
	
	GlobalScreen *global_screen;
	TrackEditorScreen *track_editor_screen;
	AudioGraphScreen *audio_graph_screen;
	
	AddNodeDialog *add_node_dialog;
	String last_graph_add_category;
	KeyBindings key_bindings;
	
	void init_key_bindings();
public:
	
	void init(String p_config_file="");
	
	void quit_request();
	bool must_quit();
	
	MainWindow(GUI::Painter *p_painter,GUI::Timer *p_timer,GUI::Skin *p_skin);	
	~MainWindow();

};

#endif
