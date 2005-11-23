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
#include "engine/editor.h"
#include "interface/global_view.h"
#include "interface/blocklist_ui_list.h"

#include <map> // std::map
#include <Qt/qstackedwidget.h>
#include <Qt/qdockwidget.h>

#include "ui_blocks/visual_settings.h"
#include <Qt/qtoolbar.h>
#include "engine/keyboard_input.h"
#include "interface/qt_updatenotify.h"
#include "interface/track_settings.h"

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
		NAVIGATION_CONTROLS_VIEW

	};



	struct Data {

		Keyboard_Input keyboard_codes;
		Song song;
		Editor *editor;

	} data;

	std::map<int,QAction*> action_map;

	void create_keybindings();
	void create_action(MenuItems p_item,QString p_text,QMenu *p_menu,QToolBar *p_toolbar,const char **p_xpm=NULL);
	QAction *get_action(MenuItems p_item);
	void add_menus();

	GlobalView *global_view;
	QDockWidget *settings_dock;

	QStackedWidget *main_stack;
	BlockListUIList *blui_list;

	QToolBar *navigation_toolbar;

	VisualSettings visual_settings;

	Qt_UpdateNotify *update_notify;
	
	TrackSettings *track_settings;

	void set_top_screen(TopScreenList p_list);

public slots:
	void menu_action_callback(int p_action);
public:

	MainWindow();
	~MainWindow();

};

}

#endif
