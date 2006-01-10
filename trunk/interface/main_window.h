//
// C++ Interface: main_window
//
// Description:
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDMAIN_WINDOW_H
#define RESHAKEDMAIN_WINDOW_H


#include <Qt/qmainwindow.h>
#include <Qt/qsplitter.h>
#include <Qt/qaction.h>
#include "editor/editor.h"
#include "interface/global_view_frame.h"
#include "interface/blocklist_ui_list.h"

#include <map> // std::map
#include <Qt/qstackedwidget.h>
#include <Qt/qdockwidget.h>
#include <Qt/qtimer.h>

#include "ui_blocks/visual_settings.h"
#include <Qt/qtoolbar.h>
#include <Qt/qspinbox.h>
#include <Qt/qcombobox.h>
#include "editor/keyboard_input.h"
#include "interface/qt_updatenotify.h"
#include "interface/rackui.h"
#include "ui_blocks/qaction_keybind.h"

namespace ReShaked {

/**
@author Juan Linietsky
*/

class MainWindow : public QMainWindow {

	Q_OBJECT

	enum TopScreenList {
          	TOP_SCREEN_GLOBAL_VIEW,
		TOP_SCREEN_EDIT_VIEW,
	};

	enum MenuItems {
		ITEM_SONG_NEW,
		ITEM_SONG_OPEN,
		ITEM_SONG_SAVE,
		ITEM_SONG_SAVE_AS,

		ITEM_SONG_EXIT,

		ITEM_EDIT_UNDO,
		ITEM_EDIT_REDO,

		ITEM_TRACK_ADD_PATTERN,
		ITEM_TRACK_ADD_AUDIO,

		ITEM_TRACK_REMOVE,
		ITEM_TRACK_MOVE_LEFT,
		ITEM_TRACK_MOVE_RIGHT,

		NAVIGATION_GLOBAL_VIEW,
		NAVIGATION_EDIT_VIEW,
			
		CONTROL_RW,
		CONTROL_PLAY,
		CONTROL_PLAY_LOOP,
		CONTROL_PAUSE,
		CONTROL_STOP,
		CONTROL_FF,
		CONTROL_RECORD,
			
		/*		
		TRACK_SYNTH,
		TRACK_CONTROLS,
		TRACK_AUTOMATIONS,
		TRACK_EFFECTS,
		TRACK_CONNECTIONS,
		TRACK_LIST_CONNECTIONS,
		*/
		MAX_ITEMS
	};



	struct Data {

		Keyboard_Input keyboard_codes;
		Song song;
		Editor *editor;
		PropertyEditUpdater property_edit_updater;

	} data;

	std::map<int,QAction*> action_map;

	void create_keybindings();
	void create_action(MenuItems p_item,QString p_text,String p_kb_path,QMenu *p_menu,QToolBar *p_toolbar,const QPixmap &p_pixmap = QPixmap());
	
	QAction *get_action(MenuItems p_item);
	void add_menus();

	GlobalViewFrame *global_view_frame;
	QDockWidget *settings_dock;

	QStackedWidget *main_stack;
	BlockListUIList *blui_list;

	QToolBar *navigation_toolbar;
	QToolBar *track_toolbar;
	QToolBar *editing_toolbar;
	QToolBar *playback_toolbar;
	
	QSpinBox *octave;

	QTimer *ui_updater;
	
	VisualSettings visual_settings;

	Qt_UpdateNotify *update_notify;
	
	//TrackSettings *track_settings;
	RackUI *rack;

	void set_top_screen(TopScreenList p_list);
	//void set_track_settings_page(TrackSettings::TrackSettingsPage p_page);

	bool updating_octave;
	
public slots:
	void menu_action_callback(int p_action);

	void octave_changed_slot(int p_to_idx);
	void octave_changed_external_slot();
	void blocklist_changed_slot();
	void track_list_changed_slot();
	
	void ui_update_slot();
public:

	MainWindow();
	~MainWindow();

};

}

#endif
