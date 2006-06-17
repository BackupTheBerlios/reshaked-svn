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

#include "interface/visual_settings.h"
#include <Qt/qtoolbar.h>
#include <Qt/qspinbox.h>
#include <Qt/qcombobox.h>
#include "editor/keyboard_input.h"
#include "interface/qt_updatenotify.h"
#include "interface/rackui.h"
#include "ui_blocks/qaction_keybind.h"
#include "interface/top_bar_controls.h"
#include "interface/bottom_bar_controls.h"
#include "interface/settings_dialog.h"

namespace ReShaked {

/**
@author Juan Linietsky
*/

class MainWindow : public QWidget {

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

		ITEM_SONG_INFO,
		
		ITEM_SONG_EXIT,
		

		ITEM_EDIT_UNDO,
		ITEM_EDIT_REDO,

		ITEM_TRACK_ADD,

		NAVIGATION_GLOBAL_VIEW,
		NAVIGATION_EDIT_VIEW,
			
		CONTROL_RW,
		CONTROL_PLAY,
		CONTROL_PLAY_LOOP,
		CONTROL_PLAY_FROM_CURSOR,
		CONTROL_PLAY_BLOCK,
		CONTROL_PAUSE,
		CONTROL_STOP,
		CONTROL_FF,
		CONTROL_RECORD,
		CONTROL_RECORD_AUTOMATIONS,
		
		SETTINGS_CONFIG,
		SETTINGS_MIDI_OUTPUT,
		HELP_HELP,
		HELP_ABOUT,
				
		DELETE_BLOCKS, //you.. just need it :(
		RACK_TOGGLE,
			
		MAX_ITEMS
	};


	QString last_saved_file;
	
	QString settings_dir;
	QString settings_file;

	struct Data {

		Keyboard_Input keyboard_codes;
		Song song;
		Editor *editor;
		PropertyEditUpdater property_edit_updater;

	} data;

	QString current_file;
	
	std::map<int,QAction*> action_map;

	void create_keybindings();
	void create_action(MenuItems p_item,QString p_text,String p_kb_path,QMenu *p_menu,QWidget *p_widget=NULL,const QPixmap &p_pixmap = QPixmap());
	
	QAction *get_action(MenuItems p_item);
	void add_menus();

	GlobalViewFrame *global_view_frame;

	QStackedWidget *main_stack;
	BlockListUIList *blui_list;


	QTimer *ui_updater;
	
	VisualSettings visual_settings;

	Qt_UpdateNotify *update_notify;
	
	TopBarControls *top_bar;
	BottomBarControls *bottom_bar;
	//TrackSettings *track_settings;
	RackUI *rack;
	CVBox *rack_vbox;
	
	PropertyEditSliderVU *main_vol;
	
	SettingsDialog *settings;

	void set_top_screen(TopScreenList p_list);
	//void set_track_settings_page(TrackSettings::TrackSettingsPage p_page);

	static void global_property_editor_property_edited_callback(void *_this,Property* p_property,double p_old_val);
	
	
	void update_titlebar();
	
	void closeEvent ( QCloseEvent * e );
	
	void save_settings();
	
public slots:
	
	void property_options_requested(Property *p_property);
	
	void ui_update_interval_changed(int p_to_value);
	
	void screen_changed_slot(int);
	void menu_action_callback(int p_action);
	
	void ui_update_slot();
	
	/* MVC Hacks (read QT_UpdateNotify) */
	void current_track_add_column_slot();	
	void current_track_remove_column_slot();
	void automation_editor_popup_slot(int p_track);
	void automation_options(int p_blocklist);
	
	void toggle_rack_visibility_slot(bool p_visible);
public:

	bool load_settings();
	
	MainWindow(QString p_settings_dir,QString p_settings_file);
	~MainWindow();

};

}

#endif
