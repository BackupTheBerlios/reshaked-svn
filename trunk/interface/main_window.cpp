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
#include "visual_settings.h"



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
			main_stack->setCurrentWidget(global_view_frame);

		} break;
		case TOP_SCREEN_EDIT_VIEW: {

			get_action(NAVIGATION_GLOBAL_VIEW)->setChecked(false);
			get_action(NAVIGATION_EDIT_VIEW)->setChecked(true);
			main_stack->setCurrentWidget(blui_list);
			blui_list->repaint_track_list();

		} break;
	}

}

void MainWindow::set_track_settings_page(TrackSettings::TrackSettingsPage p_page) {
	
	track_settings->set_page( p_page );
	
	if (p_page!=TrackSettings::TRACK_SETTINGS_PATTERN)
		get_action(TRACK_SYNTH)->setChecked(false);
	if (p_page!=TrackSettings::TRACK_SETTINGS_CONTROLS)
		get_action(TRACK_CONTROLS)->setChecked(false);
	if (p_page!=TrackSettings::TRACK_SETTINGS_AUTOMATION)
		get_action(TRACK_AUTOMATIONS)->setChecked(false);
	if (p_page!=TrackSettings::TRACK_SETTINGS_EFFECTS)
		get_action(TRACK_EFFECTS)->setChecked(false);
	if (p_page!=TrackSettings::TRACK_SETTINGS_CONNECTIONS)
		get_action(TRACK_CONNECTIONS)->setChecked(false);
	
	
}

void MainWindow::menu_action_callback(int p_action) {

	if (p_action<0 || p_action>=MAX_ITEMS)
		return;
	
	MenuItems item=(MenuItems)p_action;
	
	switch (p_action) {

		case ITEM_TRACK_ADD_PATTERN: {

			NewTrackDialog *new_dialog = new NewTrackDialog;
			if (new_dialog->exec()!=QDialog::Accepted) {
				delete new_dialog;
				return; //not accepted!
			}
			data.editor->add_track(TRACK_TYPE_PATTERN,new_dialog->get_channels(),DeQStrify(new_dialog->get_name()));
			global_view_frame->update();
			blui_list->update_track_list();
			delete new_dialog;

			if (data.song.get_track_count()) {

				get_action(TRACK_CONTROLS)->setEnabled(true);
				get_action(TRACK_AUTOMATIONS)->setEnabled(true);
				get_action(TRACK_EFFECTS)->setEnabled(true);
				get_action(TRACK_CONNECTIONS)->setEnabled(true);
			}

		} break;


		case NAVIGATION_GLOBAL_VIEW: {

			set_top_screen(TOP_SCREEN_GLOBAL_VIEW);
		} break;
		case NAVIGATION_EDIT_VIEW: {

			set_top_screen(TOP_SCREEN_EDIT_VIEW);

		} break;
		
		case TRACK_SYNTH: {
			settings_dock->setVisible( get_action( item )->isChecked() );
			track_settings->set_page( TrackSettings::TRACK_SETTINGS_PATTERN );
		} break;
		case TRACK_CONTROLS: {

			settings_dock->setVisible( get_action( item )->isChecked() );
			set_track_settings_page(TrackSettings::TRACK_SETTINGS_CONTROLS);
		} break;
		case TRACK_AUTOMATIONS: {

			settings_dock->setVisible( get_action( item )->isChecked() );
			set_track_settings_page(TrackSettings::TRACK_SETTINGS_AUTOMATION);		
		} break;
		case TRACK_EFFECTS: {

			settings_dock->setVisible( get_action( item )->isChecked() );
			set_track_settings_page(TrackSettings::TRACK_SETTINGS_EFFECTS);	
		} break;
		case TRACK_CONNECTIONS: {

			settings_dock->setVisible( get_action( item )->isChecked() );
			set_track_settings_page(TrackSettings::TRACK_SETTINGS_CONNECTIONS);	
		} break;
		
		/*
		case NAVIGATION_CONTROLS_VIEW: {
			settings_dock->setVisible(get_action(NAVIGATION_CONTROLS_VIEW)->isChecked());
		} break;
		*/
	}
}

void MainWindow::create_action(MenuItems p_item,QString p_text,QMenu *p_menu,QToolBar *p_toolbar,const QPixmap &p_pixmap) {

	ERR_FAIL_COND( action_map.find(p_item)!=action_map.end() );


	IndexedAction *q;
	QPixmap px;
	if (!p_pixmap.isNull()) {
		q = new IndexedAction(p_item,p_text,p_pixmap,this);
	} else

		q = new IndexedAction(p_item,p_text,this);

	QObject::connect(q,SIGNAL(selected_index_signal(int)),this,SLOT(menu_action_callback(int)));

	action_map[p_item]=q;
	if (p_menu)
		p_menu->addAction(q);
	if (p_toolbar) {
		
		QToolButton *tb = new QToolButton(this);
		tb->setDefaultAction(q);
		if (!p_pixmap.isNull())
			tb->setIconSize(p_pixmap.size());
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

	create_action(NAVIGATION_GLOBAL_VIEW,"Global View",NULL,navigation_toolbar,QPixmap((const char**)view_global_xpm));
	get_action(NAVIGATION_GLOBAL_VIEW)->setCheckable(true);
	get_action(NAVIGATION_GLOBAL_VIEW)->setChecked(true);
	create_action(NAVIGATION_EDIT_VIEW,"Edit View",NULL,navigation_toolbar,QPixmap((const char**)view_edit_xpm));
	navigation_toolbar->addSeparator();
	get_action(NAVIGATION_EDIT_VIEW)->setCheckable(true);
	
	track->addSeparator();
	
	create_action(TRACK_SYNTH,"Edit Synth",track,track_toolbar,GET_QPIXMAP(PIXMAP_TRACK_SETTINGS_PATTERN));
	create_action(TRACK_CONTROLS,"Edit Controls",track,track_toolbar,GET_QPIXMAP(PIXMAP_TRACK_SETTINGS_CONTROLS));
	create_action(TRACK_AUTOMATIONS,"Edit Automations",track,track_toolbar,GET_QPIXMAP(PIXMAP_TRACK_SETTINGS_AUTOMATIONS));
	create_action(TRACK_EFFECTS,"Edit Effects",track,track_toolbar,GET_QPIXMAP(PIXMAP_TRACK_SETTINGS_EFFECTS));
	create_action(TRACK_CONNECTIONS,"Edit Connections",track,track_toolbar,GET_QPIXMAP(PIXMAP_TRACK_SETTINGS_CONNECTIONS));

	get_action(TRACK_SYNTH)->setCheckable(true);
	get_action(TRACK_CONTROLS)->setCheckable(true);
	get_action(TRACK_AUTOMATIONS)->setCheckable(true);
	get_action(TRACK_EFFECTS)->setCheckable(true);
	get_action(TRACK_CONNECTIONS)->setCheckable(true);
	get_action(TRACK_SYNTH)->setEnabled(false);
	get_action(TRACK_CONTROLS)->setEnabled(false);
	get_action(TRACK_AUTOMATIONS)->setEnabled(false);
	get_action(TRACK_EFFECTS)->setEnabled(false);
	get_action(TRACK_CONNECTIONS)->setEnabled(false);


}


void MainWindow::blocklist_changed_slot() {
	
	
	Track *t=data.song.get_track( data.editor->get_current_track () );
	if (t==NULL)
		return;
	settings_dock->setWindowTitle(QStrify(t->get_name())+ " Properties");
}

MainWindow::MainWindow() {

	update_notify = new Qt_UpdateNotify(this);

	data.editor = new Editor( &data.song, update_notify );


	main_stack = new QStackedWidget(this);
	global_view_frame = new GlobalViewFrame(main_stack,data.editor);
	main_stack->addWidget(global_view_frame);
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
	track_toolbar = addToolBar("Track");
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

		snap->addItem("1/"+QString::number(divisors[i]));
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
	QObject::connect(update_notify,SIGNAL(cursor_changed_blocklist()),this,SLOT(blocklist_changed_slot()));
	QObject::connect(update_notify,SIGNAL(cursor_moved()),blui_list,SLOT(ensure_cursor_visible()));

	QObject::connect(update_notify,SIGNAL(cursor_changed_blocklist()),track_settings,SLOT(selected_track_changed_slot()));

	QObject::connect(update_notify,SIGNAL(track_list_changed()),blui_list,SLOT(update_track_list()));
	QObject::connect(update_notify,SIGNAL(track_list_changed()),global_view_frame,SLOT(update()));
	QObject::connect(update_notify,SIGNAL(editing_octave_changed()),this,SLOT(octave_changed_external_slot()));
	
	QObject::connect(track_settings,SIGNAL(update_track_names_signal()),global_view_frame,SLOT(update()));
	QObject::connect(track_settings,SIGNAL(update_track_names_signal()),blui_list,SLOT(repaint_names()));
	
	
	set_top_screen(TOP_SCREEN_GLOBAL_VIEW);
	setMinimumSize(750,550); //dont mess with my app!
}

MainWindow::~MainWindow()
{
}


}
