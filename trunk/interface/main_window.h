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
#include "interface/global_view.h"

#include <map> // std::map

namespace ReShaked {

/**
@author Juan Linietsky
*/

class MainWindow : public QMainWindow {

	Q_OBJECT
	
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
		
	};
	
	
	struct Data {
		
		Song song;
		
	} data;
	
	std::map<int,QAction*> action_map;

	void create_action(MenuItems p_item,QString p_text,QMenu *p_menu,QToolBar *p_toolbar,const char **p_xpm=NULL);
	QAction *get_action(MenuItems p_item);
	void add_menus();
	
	GlobalView *global_view;
	QSplitter *splitter;
public slots:	
	void menu_action_callback(int p_action);
public:
	MainWindow();
	
	~MainWindow();

};

}

#endif
