//
// C++ Implementation: main_window
//
// Description:
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "main_window.h"
#include <Qt/qicon.h>
#include <Qt/qmenu.h>
#include <Qt/qtoolbar.h>
#include <Qt/qtoolbutton.h>
#include <Qt/qmenubar.h>
#include <Qt/qlabel.h>
#include  "indexed_action.h"
#include "typedefs.h"
#include "interface/new_track_dialog.h"
#include "pixmaps/view_global.xpm"
#include "pixmaps/view_edit.xpm"
#include "pixmaps/view_controls.xpm"



namespace ReShaked {

class SpinBoxNoFocus : public QSpinBox {
	
public:	
	SpinBoxNoFocus(QWidget *p_parent) : QSpinBox(p_parent) {
		setFocusPolicy(Qt::NoFocus);
		lineEdit()->setFocusPolicy(Qt::NoFocus);
	}
};

void MainWindow::snap_changed_slot(int p_to_idx) {
	
	if (p_to_idx<0 || p_to_idx>=MAX_DIVISORS)
		return;
	data.editor->set_snap(divisors[p_to_idx]);
}
void MainWindow::octave_changed_slot(int p_to_idx) {
	
	updating_octave=true;
	data.editor->set_editing_octave(p_to_idx);
	updating_octave=false;
}
void MainWindow::octave_changed_external_slot() {
	if (updating_octave)
		return;
	octave->setValue(data.editor->get_editing_octave());
}

void MainWindow::set_top_screen(TopScreenList p_list) {

	switch (p_list) {

		case TOP_SCREEN_GLOBAL_VIEW: {

			get_action(NAVIGATION_GLOBAL_VIEW)->setChecked(true);
			get_action(NAVIGATION_EDIT_VIEW)->setChecked(false);
			main_stack->setCurrentWidget(global_view);

		} break;
		case TOP_SCREEN_EDIT_VIEW: {

			get_action(NAVIGATION_GLOBAL_VIEW)->setChecked(false);
			get_action(NAVIGATION_EDIT_VIEW)->setChecked(true);
			main_stack->setCurrentWidget(blui_list);
			blui_list->repaint_track_list();

		} break;
	}

}

void MainWindow::menu_action_callback(int p_action) {


	switch (p_action) {

		case ITEM_TRACK_ADD_PATTERN: {

			NewTrackDialog *new_dialog = new NewTrackDialog;
			if (new_dialog->exec()!=QDialog::Accepted) {
				delete new_dialog;
				return; //not accepted!
			}
			data.editor->add_track(TRACK_TYPE_PATTERN,new_dialog->get_channels(),DeQStrify(new_dialog->get_name()));
			global_view->update();
			blui_list->update_track_list();
			delete new_dialog;

			if (data.song.get_track_count())
				get_action(NAVIGATION_CONTROLS_VIEW)->setEnabled(true);

		} break;


		case NAVIGATION_GLOBAL_VIEW: {

			set_top_screen(TOP_SCREEN_GLOBAL_VIEW);
		} break;
		case NAVIGATION_EDIT_VIEW: {

			set_top_screen(TOP_SCREEN_EDIT_VIEW);

		} break;
		case NAVIGATION_CONTROLS_VIEW: {
			settings_dock->setVisible(get_action(NAVIGATION_CONTROLS_VIEW)->isChecked());
		} break;
	}
}

void MainWindow::create_action(MenuItems p_item,QString p_text,QMenu *p_menu,QToolBar *p_toolbar,const char **p_xpm) {

	ERR_FAIL_COND( action_map.find(p_item)!=action_map.end() );


	IndexedAction *q;
	QPixmap px;
	if (p_xpm) {
		px=QPixmap(p_xpm);
		q = new IndexedAction(p_item,p_text,QPixmap(px),this);
	} else

		q = new IndexedAction(p_item,p_text,this);

	QObject::connect(q,SIGNAL(selected_index_signal(int)),this,SLOT(menu_action_callback(int)));

	action_map[p_item]=q;
	if (p_menu)
		p_menu->addAction(q);
	if (p_toolbar) {
		
		QToolButton *tb = new QToolButton(this);
		tb->setDefaultAction(q);
		if (p_xpm)
			tb->setIconSize(px.size());
		p_toolbar->addWidget(tb);
		
	}


}

QAction *MainWindow::get_action(MenuItems p_item) {

	std::map<int,QAction*>::iterator I=action_map.find(p_item);
	if (I==action_map.end())
		return NULL;

	return I->second;
}


void MainWindow::add_menus() {




	QMenu * file = menuBar()->addMenu("File");
	QMenu * edit = menuBar()->addMenu("Edit");
	QMenu * track = menuBar()->addMenu("Track");

	create_action(ITEM_SONG_NEW,"New",file,NULL);
	create_action(ITEM_SONG_OPEN,"Open",file,NULL);
	create_action(ITEM_SONG_SAVE,"Save",file,NULL);
	create_action(ITEM_SONG_SAVE_AS,"Save As..",file,NULL);
	file->addSeparator();
	create_action(ITEM_SONG_EXIT,"Quit",file,NULL);

	create_action(ITEM_EDIT_UNDO,"Undo",edit,NULL);
	create_action(ITEM_EDIT_REDO,"Redo",edit,NULL);

	create_action(ITEM_TRACK_ADD_PATTERN,"Add Pattern Track",track,NULL);
	create_action(ITEM_TRACK_ADD_AUDIO,"Add Audio Track",track,NULL);
	create_action(ITEM_TRACK_REMOVE,"Remove Current Track",track,NULL);
	track->addSeparator();
	create_action(ITEM_TRACK_MOVE_LEFT,"Move Curent Track Left",track,NULL);
	create_action(ITEM_TRACK_MOVE_RIGHT,"Move Curent Track Right",track,NULL);

	create_action(NAVIGATION_GLOBAL_VIEW,"Global View",NULL,navigation_toolbar,(const char**)view_global_xpm);
	get_action(NAVIGATION_GLOBAL_VIEW)->setCheckable(true);
	get_action(NAVIGATION_GLOBAL_VIEW)->setChecked(true);
	create_action(NAVIGATION_EDIT_VIEW,"Edit View",NULL,navigation_toolbar,(const char**)view_edit_xpm);
	get_action(NAVIGATION_EDIT_VIEW)->setCheckable(true);
	create_action(NAVIGATION_CONTROLS_VIEW,"Edit View",NULL,navigation_toolbar,(const char**)view_controls_xpm);
	get_action(NAVIGATION_CONTROLS_VIEW)->setCheckable(true);
	get_action(NAVIGATION_CONTROLS_VIEW)->setEnabled(false);

}

MainWindow::MainWindow() {

	update_notify = new Qt_UpdateNotify(this);

	data.editor = new Editor( &data.song, update_notify );


	main_stack = new QStackedWidget(this);
	global_view = new GlobalView(main_stack,data.editor);
	main_stack->addWidget(global_view);
	blui_list = new BlockListUIList(main_stack,data.editor);
	main_stack->addWidget(blui_list);
	main_stack->setCurrentWidget(blui_list);

	setCentralWidget(main_stack);
	settings_dock = new QDockWidget("Track Properties",this);
	settings_dock->setAllowedAreas(Qt::TopDockWidgetArea|Qt::BottomDockWidgetArea);
	settings_dock->setFeatures(QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
	addDockWidget(Qt::BottomDockWidgetArea,settings_dock);
	settings_dock->hide();
	
	//settings_dock->setLayout(new QHBoxLayout(settings_dock));
	track_settings = new TrackSettings(settings_dock,data.editor);
	settings_dock->layout()->addWidget(track_settings);
	navigation_toolbar = addToolBar("Navigation");

	editing_toolbar = addToolBar("Editing");
	editing_toolbar->addWidget(new QLabel(" Octave: ",editing_toolbar));
	octave = new SpinBoxNoFocus(editing_toolbar);
	octave->setRange(0,8);
	octave->setValue(4);
	octave->setFocusPolicy(Qt::NoFocus);
	QObject::connect(octave,SIGNAL(valueChanged(int)),this,SLOT(octave_changed_slot( int )));
	updating_octave=false;;	
	editing_toolbar->addWidget(octave);
	editing_toolbar->addWidget(new QLabel(" Snap: ",editing_toolbar));
	
	snap = new QComboBox(editing_toolbar);
	for (int i=0;i<MAX_DIVISORS;i++) {
		
		snap->addItem(QString::number(divisors[i]));
	}
	snap->setFocusPolicy(Qt::NoFocus);
	snap->setCurrentIndex(3);
	QObject::connect(snap,SIGNAL(activated(int)),this,SLOT(snap_changed_slot( int )));
	editing_toolbar->addWidget(snap);
	
	
	add_menus();
	create_keybindings();

	QObject::connect(update_notify,SIGNAL(edit_window_changed()),blui_list,SLOT(repaint_track_list()));

	QObject::connect(update_notify,SIGNAL(edit_window_moved()),blui_list,SLOT(vscroll_track_list()));
	
	QObject::connect(update_notify,SIGNAL(cursor_changed_blocklist()),blui_list,SLOT(cursor_changed_blocklist()));
	QObject::connect(update_notify,SIGNAL(cursor_moved()),blui_list,SLOT(ensure_cursor_visible()));

	QObject::connect(update_notify,SIGNAL(cursor_changed_blocklist()),track_settings,SLOT(selected_track_changed_slot()));

	QObject::connect(update_notify,SIGNAL(track_list_changed()),blui_list,SLOT(update_track_list()));
	QObject::connect(update_notify,SIGNAL(track_list_changed()),global_view,SLOT(update()));
	QObject::connect(update_notify,SIGNAL(editing_octave_changed()),this,SLOT(octave_changed_external_slot()));
	
	QObject::connect(track_settings,SIGNAL(update_track_names_signal()),global_view,SLOT(update()));
	QObject::connect(track_settings,SIGNAL(update_track_names_signal()),blui_list,SLOT(repaint_names()));
	
	set_top_screen(TOP_SCREEN_GLOBAL_VIEW);

}

MainWindow::~MainWindow()
{
}


}
