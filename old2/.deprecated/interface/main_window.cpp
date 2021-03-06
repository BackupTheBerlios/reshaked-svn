
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
#include <Qt/qevent.h>
#include <Qt/qtoolbar.h>
#include <Qt/qtoolbutton.h>
#include <Qt/qmenubar.h>
#include <Qt/qlabel.h>
#include "indexed_action.h"
#include "typedefs.h"
#include "interface/new_track_dialog.h"
#include "pixmaps/view_global.xpm"
#include "pixmaps/view_edit.xpm"
#include "pixmaps/view_controls.xpm"
#include "visual_settings.h"
#include "engine/sound_driver_list.h"
#include "engine/midi_driver_list.h"
#include "editor/midi_input_handler.h"
#include "ui_blocks/vu_scale.h"
#include "tree_saver_disk.h"
#include "tree_loader_disk.h"
#include "engine/saver.h"
#include "engine/loader.h"
#include "engine/audio_control.h"

#include "interface/automation_tree.h"
#include <Qt/qfiledialog.h>
#include <Qt/qmessagebox.h>

#include "interface/info_editor.h"
#include "interface/midi_output_editor.h"
#include "interface/blocklist_ui_automation.h"
#include "tree_container.h"
#include "config_handler.h"

namespace ReShaked {


void MainWindow::update_titlebar() {
	
	if (data.song.get_info_title()!="") {
		
		setWindowTitle(QStrify(data.song.get_info_title())+" - Reshaked v"+QString(VERSION_STRING));
	} else {
		
		setWindowTitle("Reshaked v"+QString(VERSION_STRING)+" - (c) 2006 Juan Linietsky");
	}
	
}

void MainWindow::menu_action_callback(int p_action) {

	if (p_action<0 || p_action>=MAX_ITEMS)
		return;
	
	MenuItems item=(MenuItems)p_action;
	
	switch (p_action) {

		case ITEM_SONG_NEW : {
			
			if (QMessageBox::question ( this, "New Song", "Create a New Song?\n (THIS HAS NO UNDO!)", QMessageBox::Yes, QMessageBox::No)!=QMessageBox::Yes)
				break;
			
			AudioControl::mutex_lock();
			data.song.stop();
			data.song.clear(true);
			AudioControl::mutex_unlock();
			data.editor->reset();
			AudioControl::reset();
			data.editor->get_ui_update_notify()->track_list_changed();
			
			
		} break;
		case ITEM_SONG_OPEN : {
		
			QString open_file= QFileDialog::getOpenFileName ( this, "Open Song","","ReShaked Songs (*.rsh)");			
			if (open_file=="")
				break;
			TreeLoaderDisk tld("RESHAKED",5,5);
			
			TreeLoaderDisk::ErrorReading err=tld.open_file(DeQStrify(open_file));
			
			if (err==TreeLoaderDisk::ERROR_CANT_OPEN_FILE) 
				QMessageBox::critical ( this, "Error", "Cant Open File", QMessageBox::Ok, QMessageBox::NoButton);
			else if (err==TreeLoaderDisk::ERROR_FILE_UNRECOGNIZED) 
				QMessageBox::critical ( this, "Error", "File Unrecognized", QMessageBox::Ok, QMessageBox::NoButton);
			else if (err==TreeLoaderDisk::ERROR_FILE_CORRUPTED) 
				QMessageBox::critical ( this, "Error", "File is Corrupted", QMessageBox::Ok, QMessageBox::NoButton);
			else if (err==TreeLoaderDisk::ERROR_VERSION_TOO_NEW) 
				QMessageBox::critical ( this, "Error", "You need a newer version to open this file", QMessageBox::Ok, QMessageBox::NoButton);
			else if (err==TreeLoaderDisk::ERROR_VERSION_TOO_OLD) 
				QMessageBox::critical ( this, "Error", "File format too old, not supported.", QMessageBox::Ok, QMessageBox::NoButton);
				
			if (err)
				break;
			Loader load;
			AudioControl::mutex_lock();
			load.load_song( &data.song, &tld );
			AudioControl::mutex_unlock();
			tld.close_file();
			data.editor->reset();
			data.editor->get_ui_update_notify()->track_list_changed();
			update_titlebar();
			
			current_file=open_file;
			
		} break;
		case ITEM_SONG_SAVE: {
			
			if (current_file!="") {

				TreeSaverDisk tsd("RESHAKED",5);
				if (tsd.open_file(DeQStrify(current_file))) {
				
					QMessageBox::critical( this, "Error", "Unable to Save File", QMessageBox::Ok, QMessageBox::NoButton);

					break;
				}
				Saver saver;
		
				saver.save_song(&data.song,&tsd);
		
				tsd.close_file();
				break;				
						
			}
			
		}
		case ITEM_SONG_SAVE_AS : {
			
					
			QString save_file=QFileDialog::getSaveFileName ( this, "Save Song As..",current_file, "ReShaked Songs (*.rsh)");
			
			if (save_file!="" && save_file.indexOf(".rsh")==-1)
				save_file+=".rsh";
			
			if (save_file=="")
				break;
			
			
			/* temp */
			TreeSaverDisk tsd("RESHAKED",5);
			
			
			if (tsd.open_file(DeQStrify(save_file))) {
				
				QMessageBox::critical( this, "Error", "Unable to Save File", QMessageBox::Ok, QMessageBox::NoButton);

				break;
			}

			Saver saver;
			
			saver.save_song(&data.song,&tsd);
			
			tsd.close_file();
			current_file=save_file;
	
		} break;
		
		case ITEM_SONG_INFO: {
			
			InfoEditor *ie=new InfoEditor(this,&data.song);
			ie->exec();
			data.song.set_info_title( DeQStrify( ie->get_title() ));
			data.song.set_info_author( DeQStrify( ie->get_author() ));
			data.song.set_info_notes( DeQStrify( ie->get_notes() ));
			delete ie;
			update_titlebar();
			
		} break;
		case ITEM_SONG_EXIT: {
			
			close();
		} break;
		
		case ITEM_TRACK_ADD: {

			NewTrackDialog *new_dialog = new NewTrackDialog(this);
			if (new_dialog->exec()!=QDialog::Accepted) {
				delete new_dialog;
				return; //not accepted!
			}
			data.editor->add_track(TRACK_TYPE_PATTERN,new_dialog->get_channels(),DeQStrify(new_dialog->get_name()));
			global_view_frame->update();
			blui_list->update_track_list();
			delete new_dialog;

			/* make it ready for adding a new plugin */
			rack->select_track(data.song.get_track_count() -1 );
			rack->rack_front_selected();
			/*
			if (data.song.get_track_count()) {

				get_action(TRACK_CONTROLS)->setEnabled(true);
				get_action(TRACK_AUTOMATIONS)->setEnabled(true);
				get_action(TRACK_EFFECTS)->setEnabled(true);
				get_action(TRACK_CONNECTIONS)->setEnabled(true);
				get_action(TRACK_LIST_CONNECTIONS)->setEnabled(true);
			}
			*/
		} break;

		case ITEM_EDIT_UNDO: {
			
			data.editor->undo();
		} break;
		case ITEM_EDIT_REDO: {
			
			data.editor->redo();
		} break;

		case NAVIGATION_GLOBAL_VIEW: {

			top_bar->set_screen( TopBarControls::SCREEN_SONG );
			get_action(DELETE_BLOCKS)->setEnabled(true);
		} break;
		case NAVIGATION_EDIT_VIEW: {

			top_bar->set_screen( TopBarControls::SCREEN_EDIT );
			get_action(DELETE_BLOCKS)->setEnabled(false);

		} break;
		case RACK_TOGGLE: {

			bottom_bar->rack_toggle->set_pressed( !bottom_bar->rack_toggle->is_pressed() );
			toggle_rack_visibility_slot( bottom_bar->rack_toggle->is_pressed() );
		} break;	
		case DELETE_BLOCKS: {

			if (main_stack->currentWidget()==global_view_frame)
				global_view_frame->delete_blocks_slot();
		} break;	
		
		/*		
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
		case TRACK_LIST_CONNECTIONS: {

			settings_dock->setVisible( get_action( item )->isChecked() );
			set_track_settings_page(TrackSettings::TRACK_SETTINGS_TRACKLIST_CONNECTIONS);	
		} break;
		*/
		/*
		case NAVIGATION_CONTROLS_VIEW: {
			settings_dock->setVisible(get_action(NAVIGATION_CONTROLS_VIEW)->isChecked());
		} break;
		*/
		
		case CONTROL_PLAY_FROM_CURSOR: {
			
			if (data.editor->get_blocklist_count())
				data.song.play(data.editor->get_cursor().get_tick_pos());
			data.editor->midi_reset();
			
		} break;
		case CONTROL_PLAY_BLOCK: {
			
			if (data.editor->get_blocklist_count()==0)
				break;
			BlockList *bl=data.editor->get_blocklist( data.editor->get_current_blocklist());
			
			if (bl==NULL)
				break;
			int block_idx= bl->get_block_idx_at_pos(data.editor->get_cursor().get_tick_pos());
			if (block_idx<0)
				break;
			
			data.song.loop( bl->get_block_pos( block_idx ), bl->get_block_pos( block_idx )+bl->get_block( block_idx )->get_length());
			data.editor->midi_reset();
		} break;
		
		case CONTROL_PLAY: {
			
			data.song.play(0);
		} break;
		case CONTROL_PLAY_LOOP: {
		
			printf("loopy?\n");
			data.song.loop();
			
		} break;
		case CONTROL_STOP: {
			
			song_stop();	
		} break;
		
		case CONTROL_RECORD_AUTOMATIONS: {
		
			data.song.get_song_playback().set_recording_automations( get_action(CONTROL_RECORD_AUTOMATIONS)->isChecked() );
		
		} break;
		
		case SETTINGS_CONFIG: {
			
			settings->exec();
		} break;
		case SETTINGS_MIDI_OUTPUT: {
			
			MidiOutputEditor *moe = new MidiOutputEditor(this);
			moe->exec();
			delete moe;
		} break;
		case HELP_ABOUT: {
			
			QMessageBox::about ( this, "About Reshaked","Reshaked v"+QString(VERSION_STRING)+"\n(c) 2006 Juan Linietsky\nEmail: reshaked@gmail.com" );
		} break;
		
	}
}

void MainWindow::toggle_rack_visibility_slot(bool p_visible) {
	
	if (p_visible)
		rack_vbox->show();
	else
		rack_vbox->hide();
}

void MainWindow::create_action(MenuItems p_item,QString p_text,String p_kb_path,QMenu *p_menu,QWidget *p_widget,const QPixmap &p_pixmap) {

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
	else 
		addAction(q);
	
	
	if (p_widget) {
		p_widget->addAction(q);
		//printf("action at %p\n",q);
		
	}

		
	QWidget_KeybindDescription *widget_desc=NULL;
	if (p_widget)
		widget_desc=new QWidget_KeybindDescription(p_widget);
	data.keyboard_codes.add_key_bind("actions/"+p_kb_path,DeQStrify(p_text),Keyboard_Input::NO_KEY,false,new QAction_Keybind(q),widget_desc);
	
}

QAction *MainWindow::get_action(MenuItems p_item) {

	std::map<int,QAction*>::iterator I=action_map.find(p_item);
	if (I==action_map.end())
		return NULL;

	return I->second;
}


void MainWindow::add_menus() {



	create_action(ITEM_SONG_NEW,"New Song","new",top_bar->get_file_menu());
	create_action(ITEM_SONG_OPEN,"Open Song","open",top_bar->get_file_menu());
	create_action(ITEM_SONG_SAVE,"Save Song","save",top_bar->get_file_menu());
	create_action(ITEM_SONG_SAVE_AS,"Save song As..","save_as",top_bar->get_file_menu());
	top_bar->get_file_menu()->addSeparator();
	create_action(ITEM_SONG_INFO,"Edit Song Info..","edit_song_info",top_bar->get_file_menu());
	top_bar->get_file_menu()->addSeparator();
	create_action(ITEM_SONG_EXIT,"Quit!","quit",top_bar->get_file_menu());

	create_action(ITEM_EDIT_UNDO,"Undo","undo",NULL,top_bar->icon_undo);
	create_action(ITEM_EDIT_REDO,"Redo","redo",NULL,top_bar->icon_redo);

	create_action(ITEM_TRACK_ADD,"Add Track","add_track",NULL,top_bar->icon_menu_add);
	//create_action(ITEM_TRACK_ADD_AUDIO,"Add Audio Track",track,NULL);

	create_action(NAVIGATION_GLOBAL_VIEW,"Global View","song_view",NULL,top_bar->song_view);
	create_action(NAVIGATION_EDIT_VIEW,"Edit View","edit_view",NULL,top_bar->edit_view);
	
	create_action(CONTROL_RW,"Rewind","rewind",NULL,top_bar->control_button_rw);
	create_action(CONTROL_PLAY,"Play","play",NULL,top_bar->control_button_play);
	create_action(CONTROL_PLAY_LOOP,"Play Loop","play_loop",NULL,top_bar->control_button_loop);
	create_action(CONTROL_PAUSE,"Pause","pause",NULL,top_bar->control_button_pause);
	create_action(CONTROL_STOP,"Stop","stop",NULL,top_bar->control_button_stop);
	create_action(CONTROL_FF,"FastForward","fast_forward",NULL,top_bar->control_button_ff);
	create_action(CONTROL_RECORD,"Record","record",NULL,top_bar->control_button_record);
	create_action(CONTROL_RECORD_AUTOMATIONS,"Record Automations","record_automations",NULL,top_bar->control_button_record_auto);
	create_action(CONTROL_PLAY_BLOCK,"Play Block","play_block",NULL,blui_list->play_block);
	create_action(CONTROL_PLAY_FROM_CURSOR,"Play From Cursor","play_from_cursor",NULL,blui_list->play_cursor);

	
	
	create_action(RACK_TOGGLE,"Toggle Rack Visibility","toggle_rack",NULL,bottom_bar->rack_toggle);
	
	create_action(DELETE_BLOCKS,"Delete Blocks","delete_blocks",NULL,NULL);
	
	create_action(SETTINGS_CONFIG,"Settings","settings",top_bar->get_settings_menu());
	create_action(SETTINGS_MIDI_OUTPUT,"Midi Output Assign","midi_output_assign",top_bar->get_settings_menu());
	create_action(HELP_HELP,"Help","help",top_bar->get_help_menu());
	create_action(HELP_ABOUT,"About","about",top_bar->get_help_menu());
	
	

}

void MainWindow::property_options_requested(Property *p_property) {
	
	Track *track=&data.song.get_global_track();
	int prop_idx=-1;
	
	for (int j=0;j<track->get_property_count();j++) {
		
		if (track->get_property(j)==p_property) {
			
			prop_idx=j;
			break;
		}
	}
	
	ERR_FAIL_COND(prop_idx==-1);
	
	bool is_automated=track->has_property_visible_automation(prop_idx);
	
	QList<QAction*> ac_list;
	QAction *op_auto = new QAction(GET_QPIXMAP(PIXMAP_TRACK_SETTINGS_AUTOMATIONS),is_automated?"Hide Automation":"Show Automation",this);
	ac_list.push_back(op_auto);
	QAction *res = QMenu::exec(ac_list,QCursor::pos());
	
	if (res==op_auto) {
		
		if (is_automated)
			data.editor->hide_automation( prop_idx, track );
		else
			data.editor->show_automation( prop_idx, track );
	}
	
	foreach(I,ac_list) {
		delete *I;
	}
	
}

void MainWindow::global_property_editor_property_edited_callback(void *_this,Property* p_property,double p_old_val) {
	
	MainWindow *instance=(MainWindow*)_this;
	instance->data.editor->property_changed(p_property,p_old_val,&instance->data.song.get_global_track());
	
}


void MainWindow::screen_changed_slot(int p_screen) {
	
	switch(p_screen) {
		
		case TopBarControls::SCREEN_SONG: main_stack->setCurrentWidget(global_view_frame); break;
		case TopBarControls::SCREEN_EDIT: main_stack->setCurrentWidget(blui_list); break;

	} 
}


void MainWindow::closeEvent ( QCloseEvent * e ) {
	
	if (QMessageBox::question ( this, "Exit?", "Are you sure?", QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes)
		e->accept();
	else
		e->ignore();

	
}
void MainWindow::ui_update_slot() {
	//printf("update!\n");
	//if (data.song.get_song_playback().get_status()==SongPlayback::STATUS_PLAY)
//		printf("play pos at %f\n",(float)data.song.get_song_playback().get_current_tick_from()/(float)TICKS_PER_BEAT);
	
	MidiInputHandler::get_singleton()->gui_thread_callback();
		
	data.property_edit_updater.update_editors();
	blui_list->update_play_position();
	blui_list->update_vus();
	top_bar->update_playback_indicator();
	rack->ui_update_callback();	
	
	float global_db=data.song.grab_accumulated_max_gain();
	if (global_db>=0 && !peaked) {
		
		vu_peak->set_bg( GET_QPIXMAP( THEME_MAIN_VU_PEAK_ACTIVE ) );
	}
	global_db+=60;
	global_db/=60.0+24.0;
	
	main_vol->set_value( global_db );
	
}

/* MVC Hacks (read QT_UpdateNotify) */
void MainWindow::current_track_add_column_slot() {
	
	data.editor->track_pattern_add_column();
}
void MainWindow::current_track_remove_column_slot() {
	
	data.editor->track_pattern_remove_column();
	
}

void MainWindow::automation_editor_popup_slot(int p_track) {
	Track *t;
	if (p_track==-1)
		t=&data.song.get_global_track();
	else
		t=data.song.get_track(p_track);
	
	if (!t)
		return;
	
	AutomationTreeeDialog *atd = new AutomationTreeeDialog(this,t,data.editor);
	atd->exec();
	delete atd;
	
}

void MainWindow::automation_options(int p_blocklist) {
	
	BlockListUI_Automation *auto_ui=dynamic_cast<BlockListUI_Automation*>(blui_list->get_blocklist_ui( p_blocklist ));
	ERR_FAIL_COND(auto_ui==NULL);
	
	auto_ui->show_popup();
	
}

void MainWindow::ui_update_interval_changed(int p_to_value) {
	
	ui_updater->setInterval(p_to_value);
}

void MainWindow::song_stop() {

	data.song.stop();
	data.editor->midi_reset();
	
	clear_peaked();
}

void MainWindow::clear_peaked() {
	
	peaked=false;
	vu_peak->set_bg( GET_QPIXMAP( THEME_MAIN_VU_PEAK ) );
}

void MainWindow::save_settings() {

	
	QString settings_path=settings_dir+"/"+settings_file;
	
	printf("Saving settings to %s\n",settings_path.toAscii().data());
	TreeContainer tc;
	settings->save( &tc );
	
	ConfigHandler ch;
	ch.set_header_check( "ReShaked Configuration" );
	ch.save( DeQStrify(settings_path), &tc );
	

}

bool MainWindow::load_settings() {
	
	QString settings_path=settings_dir+"/"+settings_file;
	
	printf("Loading settings to %s\n",settings_path.toAscii().data());
	TreeContainer tc;

	ConfigHandler ch;
	ch.set_header_check( "ReShaked Configuration" );
	if (ch.load( DeQStrify(settings_path), &tc )) {
		return true;
	}
	
	settings->load( &tc );	
	
	
	ch.save(DeQStrify(settings_path+".test"),&tc);
	
	return false;
}


MainWindow::MainWindow(QString p_settings_dir,QString p_settings_file) {

	settings_dir=p_settings_dir;
	settings_file=p_settings_file;
	
	SoundDriverList::get_singleton()->set_song(&data.song);
	MidiDriverList::get_singleton()->set_song_playback(&data.song.get_song_playback());
	update_notify = new Qt_UpdateNotify(this);

	data.editor = new Editor( &data.song, update_notify );

	MidiInputHandler::get_singleton()->set_editor( data.editor );

	setLayout(new QVBoxLayout(this));
	CVBox *main_vbox = new CVBox(this);
	layout()->addWidget(main_vbox);
	
	top_bar = new TopBarControls(main_vbox,data.editor,&data.property_edit_updater);
	top_bar->label_prop_bpm->set_changed_by_editor_callback( this, &MainWindow::global_property_editor_property_edited_callback );
	top_bar->updown_prop_bpm->set_changed_by_editor_callback( this, &MainWindow::global_property_editor_property_edited_callback );
	top_bar->knob_swing->set_changed_by_editor_callback( this, &MainWindow::global_property_editor_property_edited_callback );
	
	QObject::connect(top_bar->label_prop_bpm,SIGNAL(external_edit_signal( Property* )),this,SLOT(property_options_requested( Property* )));
	QObject::connect(top_bar->knob_swing,SIGNAL(external_edit_signal( Property* )),this,SLOT(property_options_requested( Property* )));
	
	CHBox *stack_hbox = new CHBox(main_vbox);
			
	new PixmapLabel(stack_hbox,GET_QPIXMAP(THEME_LEFT__MARGIN),PixmapLabel::EXPAND_TILE_V);
	main_stack = new QStackedWidget(stack_hbox);
	
	CVBox *vu_vbox = new CVBox(stack_hbox);

	vu_peak = new PixmapLabel(vu_vbox,GET_QPIXMAP(THEME_MAIN_VU_PEAK));
	peaked=false;
	QObject::connect(vu_peak,SIGNAL(clicked_signal()),this,SLOT(clear_peaked()));
	
	CHBox *vu_hbox = new CHBox(vu_vbox);
	
	VUScale * vu_scale = new VUScale(vu_hbox,GET_QPIXMAP(THEME_MAIN_VU),VUScale::EXPAND_STRETCH_V);
	vu_scale->set_min(-60);
	vu_scale->set_max(24);
	vu_scale->set_color(QColor(0xc1,0xef,0xec));
	vu_scale->set_zero_color(QColor(255,255,255));
	main_vol = new PropertyEditSliderVU(vu_hbox,GET_QPIXMAP(THEME_MAIN_VU_GRABBER));
	main_vol->set_fall_time( 1.0 );
	main_vol->set_property(&data.song.get_global_properties().get_volume());
	main_vol->set_changed_by_editor_callback( this, &MainWindow::global_property_editor_property_edited_callback );
	QObject::connect(main_vol,SIGNAL(external_edit_signal( Property* )),this,SLOT(property_options_requested( Property* )));
	
	data.property_edit_updater.add_editor(main_vol);
	
	new PixmapLabel(stack_hbox,GET_QPIXMAP(THEME_RIGHT__MARGIN),PixmapLabel::EXPAND_TILE_V);
	
	global_view_frame = new GlobalViewFrame(main_stack,data.editor);
	main_stack->addWidget(global_view_frame);
	
	blui_list = new BlockListUIList(main_stack,data.editor,&data.property_edit_updater);
			
	main_stack->addWidget(blui_list);
	main_stack->setCurrentWidget(global_view_frame);


	
	//settings_dock->setLayout(new QHBoxLayout(settings_dock));
	//track_settings = new TrackSettings(settings_dock,data.editor);
	//settings_dock->layout()->addWidget(track_settings);
	rack_vbox = new CVBox(main_vbox);
	
	CHBox *middle_hbox = new CHBox(rack_vbox);
	
	new PixmapLabel(middle_hbox,GET_QPIXMAP(THEME_BOTTOM_LEFT__CORNER));
	new PixmapLabel(middle_hbox,GET_QPIXMAP(THEME_BOTTOM__MARGIN),PixmapLabel::EXPAND_TILE_H);
	new PixmapLabel(middle_hbox,GET_QPIXMAP(THEME_BOTTOM_RIGHT__CORNER));
	
	CHBox *rack_top_hbox = new CHBox(rack_vbox);
	
	new PixmapLabel(rack_top_hbox,GET_QPIXMAP(THEME_MIDDLE__SEPARATOR_BEGIN));
	new PixmapLabel(rack_top_hbox,GET_QPIXMAP(THEME_MIDDLE__SEPARATOR_CENTER),PixmapLabel::EXPAND_TILE_H);
	new PixmapLabel(rack_top_hbox,GET_QPIXMAP(THEME_MIDDLE__SEPARATOR_END));
	
	CHBox *rack_middle_hbox = new CHBox(rack_vbox);
	new PixmapLabel(rack_middle_hbox,GET_QPIXMAP(THEME_LEFT__MARGIN),PixmapLabel::EXPAND_TILE_V);
	
	rack = new RackUI(rack_middle_hbox,data.editor,&data.property_edit_updater);
	rack->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	new PixmapLabel(rack_middle_hbox,GET_QPIXMAP(THEME_RIGHT__MARGIN),PixmapLabel::EXPAND_TILE_V);
	
	
	bottom_bar = new BottomBarControls(main_vbox,data.editor);
	bottom_bar->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);	
	QObject::connect(bottom_bar->rack_toggle,SIGNAL(mouse_toggled_signal( bool )),this,SLOT(toggle_rack_visibility_slot(bool)));
	
	add_menus();
	create_keybindings();

	QObject::connect(update_notify,SIGNAL(edit_window_changed()),blui_list,SLOT(repaint_track_list()));

	QObject::connect(update_notify,SIGNAL(edit_window_moved()),blui_list,SLOT(vscroll_track_list()));
	
	
	QObject::connect(update_notify,SIGNAL(cursor_changed_blocklist()),blui_list,SLOT(cursor_changed_blocklist()));
	QObject::connect(update_notify,SIGNAL(cursor_changed_blocklist()),rack,SLOT(blocklist_changed_slot()));
	QObject::connect(update_notify,SIGNAL(cursor_moved()),blui_list,SLOT(ensure_cursor_visible()));

	//QObject::connect(update_notify,SIGNAL(cursor_changed_blocklist()),track_settings,SLOT(track_changed_slot()));
	//QObject::connect(update_notify,SIGNAL(current_automation_status_changed()),track_settings,SLOT(automation_update_status()),Qt::QueuedConnection);
	QObject::connect(update_notify,SIGNAL(current_automation_status_changed()),blui_list,SLOT(update_track_list()));
	QObject::connect(update_notify,SIGNAL(current_automation_status_changed()),global_view_frame,SLOT(update()));

	QObject::connect(update_notify,SIGNAL(track_list_changed()),blui_list,SLOT(update_track_list()));
	QObject::connect(update_notify,SIGNAL(track_list_changed()),global_view_frame,SLOT(update()));
	QObject::connect(update_notify,SIGNAL(track_list_changed()),rack,SLOT(update_rack()));
	QObject::connect(update_notify,SIGNAL(editing_octave_changed()),bottom_bar,SLOT(octave_changed_slot()));
	
	QObject::connect(update_notify,SIGNAL(track_names_changed()),global_view_frame,SLOT(update()));
	QObject::connect(update_notify,SIGNAL(track_names_changed()),blui_list,SLOT(repaint_names()));
	
	QObject::connect(global_view_frame,SIGNAL(global_view_changed_blocks_signal()),blui_list,SLOT(update_h_scroll()));
	QObject::connect(update_notify,SIGNAL(update_blocklist_list( const std::list< int >& )),blui_list,SLOT(update_blocklist_list(const std::list<int>&)));
	

	QObject::connect(update_notify,SIGNAL(block_layout_changed()),global_view_frame->get_global_view(),SLOT(block_layout_changed_slot()));
	
	QObject::connect(update_notify,SIGNAL(block_layout_changed()),blui_list,SLOT(repaint_track_list()));
	QObject::connect(update_notify,SIGNAL(rack_changed()),rack,SLOT(update_rack()));
	QObject::connect(update_notify,SIGNAL(rack_repaint()),rack,SLOT(repaint_rack()));
	QObject::connect(update_notify,SIGNAL(track_names_changed()),rack,SLOT(update_rack_combo_names_slot()));
		
	QObject::connect(top_bar,SIGNAL(screen_changed_signal( int )),this,SLOT(screen_changed_slot( int )));
	
	QObject::connect(update_notify,SIGNAL(current_track_add_column()),this,SLOT(current_track_add_column_slot()),Qt::QueuedConnection);
	QObject::connect(update_notify,SIGNAL(current_track_remove_column()),this,SLOT(current_track_remove_column_slot()),Qt::QueuedConnection);
	QObject::connect(update_notify,SIGNAL(automation_editor_popup( int )),this,SLOT(automation_editor_popup_slot( int )),Qt::QueuedConnection);
	QObject::connect(update_notify,SIGNAL(automation_options( int )),this,SLOT(automation_options( int )),Qt::QueuedConnection);
	
	QObject::connect(update_notify,SIGNAL(volume_mask_changed()),blui_list,SLOT(update_top_bar()));
	QObject::connect(update_notify,SIGNAL(cursor_step_changed()),blui_list,SLOT(update_top_bar()));
	
	QObject::connect(update_notify,SIGNAL(notify_action( String )),bottom_bar,SLOT(action_notify( String )));
	QObject::connect(update_notify,SIGNAL(editing_octave_changed()),bottom_bar->vpiano,SLOT(octave_changed_slot()));
	
	QObject::connect(bottom_bar->vpiano,SIGNAL(key_pressed_signal( int )),rack,SLOT(test_note( int)));
	QObject::connect(bottom_bar->vpiano,SIGNAL(key_released_signal( int )),rack,SLOT(test_note_off( int)));
	
	QObject::connect(update_notify,SIGNAL(editor_marker_edit_request()),blui_list,SLOT(edit_marker_slot()));
	QObject::connect(update_notify,SIGNAL(editor_volume_scale_request()),blui_list,SLOT(scale_volume_slot()));
	
	QObject::connect(top_bar,SIGNAL(song_stop_signal()),this,SLOT(song_stop()));
	
	ui_updater = new QTimer(this);
	QObject::connect(ui_updater,SIGNAL(timeout()),this,SLOT(ui_update_slot()));
	ui_updater->start(50);

	setMinimumSize(750,550); //dont mess with my app!
	setBackgroundRole(QPalette::NoRole);
	QPalette p=palette();
	p.setColor(QPalette::Background,QColor(0,0,0));
	setPalette(p);
	rack_vbox->hide();

	settings = new SettingsDialog(this);
	
	QObject::connect(settings->get_settings_interface(),SIGNAL(ui_update_interval_changed( int )),this,SLOT(ui_update_interval_changed( int )));
	
	update_titlebar();
}

MainWindow::~MainWindow() {

	/* Save settings */
	printf("mywindow\n");
	save_settings();
}


}
