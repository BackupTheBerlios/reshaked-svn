//
// C++ Implementation: main_window
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "main_window.h"

#include "widgets/separator.h"
#include "widgets/icon.h"
#include "bundles/menu_box.h"
#include "gui_custom/rsskin.h"

#include <stdio.h>

namespace ReShaked {

	
bool MainWindow::must_quit() {
	
	return false;	
}

	
void MainWindow::quit_request() {
		

}
	
void MainWindow::new_track_callback() {
	
	data.editor->add_track(TRACK_TYPE_PATTERN,new_track_dialog->get_channels(),new_track_dialog->get_name()); 

 	global_view_frame->update_all();

}
	
void MainWindow::menu_callback(int p_option) {
		
	
	switch (p_option) {
		
		case FILE_NEW_SONG:  {} break;
  		case FILE_OPEN_SONG:  {} break;
    		case FILE_SAVE_SONG:  {} break;
      		case FILE_SAVE_SONG_AS:  {} break;
      		case FILE_IMPORT_MIDI:  {} break;
      		case FILE_EXPORT_MIDI:  {} break;
      		case FILE_EXPORT_WAV:  {} break;
		case FILE_QUIT:  {} break;
  		case TRACK_ADD_PATTERN:  {
		
			new_track_dialog->show("New Track");
		
		} break;
  		case TRACK_ADD_AUDIO:  {} break;
    		case TRACK_MIXER:  {} break;
      		case EDIT_UNDO:  {} break;
      		case EDIT_REDO:  {} break;
		case CONTROL_PLAY:  {} break;
		case CONTROL_LOOP:  {} break;
		case CONTROL_PAUSE:  {} break;
		case CONTROL_STOP:  {} break;
		case CONTROL_FF:  {} break;
		case CONTROL_RW:  {} break;
		case CONTROL_REC:  {} break;
		case CONTROL_REC_AUTO:  {} break;
      		case TOOL_SETTINGS:  {} break;
		case TOOL_MIDI_OUTPUT_ASSIGN:  {} break;
  		case HELP_HELP:  {} break;
    		case HELP_ABOUT:  {} break;		
		
	}
	
	
}
	
void MainWindow::rebuild_track_lists() {
		
	edit_view_frame->rebuild();
	global_view_frame->update_all();
}
	
	
void MainWindow::notify_action_slot(String p_action) {
		
	if (info_line->get_text()!=p_action)
		info_line->set_text(p_action);
	
	
}
	
void MainWindow::show_automation_editor(Track *p_track) {
	
	automation_editor->show(p_track);
}
void MainWindow::set_in_window() {
	
	new_track_dialog = new NewTrackDialog(get_window());
	new_track_dialog->track_create_signal.connect(this,&MainWindow::new_track_callback);
	track_dialog = new TrackDialog(get_window(),data.editor);
	track_dialog->request_automation_editor_signal.connect(this,&MainWindow::show_automation_editor);
	automation_editor = new AutomationEditor(get_window(),data.editor);	
}
	
	
void MainWindow::initialize() {
		
	HBoxContainer *hb = add(new HBoxContainer);
	
	tab_bar = hb->add( new TabBar );
	
	tab_bar->add_tab("Song");
	tab_bar->add_tab("Edit");
	tab_bar->add_tab("Rack");
	
	hb->add( new VSeparator );
	
	MenuBox * file_menu = hb->add( new MenuBox("File") );
	
	file_menu->item_selected_signal.connect( this, &MainWindow::menu_callback );
	
	file_menu->add_item("New Song",FILE_NEW_SONG);
	file_menu->add_item("Open Song",FILE_OPEN_SONG);
	file_menu->add_item("Save Song",FILE_SAVE_SONG);
	file_menu->add_item("Save Song As",FILE_SAVE_SONG_AS);
	file_menu->add_separator();
	file_menu->add_item("Import MIDI File",FILE_IMPORT_MIDI);
	file_menu->add_item("Export MIDI File",FILE_EXPORT_MIDI);
	file_menu->add_separator();
	file_menu->add_item("Export WAV",FILE_EXPORT_WAV);
	file_menu->add_separator();
	file_menu->add_item("Quit",FILE_QUIT);
	
	MenuBox * track_menu = hb->add( new MenuBox("Track") );
	
	track_menu->item_selected_signal.connect( this, &MainWindow::menu_callback );
	
	track_menu->add_item("Add Pattern Track",TRACK_ADD_PATTERN);
	track_menu->add_item("Add Audio Track",TRACK_ADD_AUDIO);
	track_menu->add_separator();
	track_menu->add_item("Track Mixer",TRACK_MIXER);
	
	hb->add( new VSeparator );
	
	printf("bitmap for undo: %i\n",bitmap(BITMAP_ICON_UNDO));
	hb->add( new MenuButton(bitmap(BITMAP_ICON_UNDO)) )->pressed_signal.connect( Method( Method1<int>(this, &MainWindow::menu_callback), (int)EDIT_UNDO ));
	hb->add( new MenuButton(bitmap(BITMAP_ICON_REDO)) )->pressed_signal.connect( Method(Method1<int>(this, &MainWindow::menu_callback), (int)EDIT_REDO ));	
	
	hb->add( new VSeparator );

	hb->add( new MenuButton(bitmap(BITMAP_CONTROL_RW)) )->pressed_signal.connect( Method( Method1<int>(this, &MainWindow::menu_callback), (int)CONTROL_RW ));
	hb->add( new MenuButton(bitmap(BITMAP_CONTROL_PLAY)) )->pressed_signal.connect( Method( Method1<int>(this, &MainWindow::menu_callback), (int)CONTROL_PLAY ));
	hb->add( new MenuButton(bitmap(BITMAP_CONTROL_LOOP)) )->pressed_signal.connect( Method( Method1<int>(this, &MainWindow::menu_callback), (int)CONTROL_LOOP ));
	hb->add( new MenuButton(bitmap(BITMAP_CONTROL_PAUSE)) )->pressed_signal.connect( Method( Method1<int>(this, &MainWindow::menu_callback), (int)CONTROL_PAUSE ));
	hb->add( new MenuButton(bitmap(BITMAP_CONTROL_STOP)) )->pressed_signal.connect( Method( Method1<int>(this, &MainWindow::menu_callback), (int)CONTROL_STOP ));
	hb->add( new MenuButton(bitmap(BITMAP_CONTROL_FF)) )->pressed_signal.connect( Method( Method1<int>(this, &MainWindow::menu_callback), (int)CONTROL_FF ));
	hb->add( new MenuButton(bitmap(BITMAP_CONTROL_REC)) )->pressed_signal.connect( Method( Method1<int>(this, &MainWindow::menu_callback), (int)CONTROL_REC ));
	hb->add( new MenuButton(bitmap(BITMAP_CONTROL_REC_AUTO)) )->pressed_signal.connect( Method( Method1<int>(this, &MainWindow::menu_callback), (int)CONTROL_REC_AUTO ));
	
	hb->add( new VSeparator );
	
	hb->add( new Widget, 1 ); //expand
	
	hb->add( new VSeparator );
	
	MenuBox * tools_menu = hb->add( new MenuBox("Config") );
	
	tools_menu->item_selected_signal.connect( this, &MainWindow::menu_callback );
	
	tools_menu->add_item("Settings",TOOL_SETTINGS);
	tools_menu->add_item("Midi Output Assign",TOOL_MIDI_OUTPUT_ASSIGN);
	
	MenuBox * help_menu = hb->add( new MenuBox("Help") );	
	
	help_menu->item_selected_signal.connect( this, &MainWindow::menu_callback );
	
	help_menu->add_item("Help!",HELP_HELP);
	help_menu->add_item("About",HELP_ABOUT);
	
	main_stack = add( new StackContainer, 1 );
	
	hb = add( new HBoxContainer );
	
	hb->add( new Icon( bitmap( BITMAP_ICON_INFO ) ) );
	
	info_line = hb->add( new LineEdit, 4 );
	
	hb->add( new Icon( bitmap( BITMAP_ICON_TIME ) ) );
	
	time_line = hb->add( new LineEdit );
	time_line->set_minimum_size( Size(150,-1));
	
	
	/* Pages */
	
	
	global_view_frame =main_stack->add( new GlobalViewFrame(data.editor) );
	edit_view_frame =main_stack->add( new EditViewFrame(update_notify,data.editor) );
	rack_view_frame =main_stack->add( new RackViewFrame(update_notify, data.editor) );
	
	tab_bar->tab_changed_signal.connect( main_stack, &StackContainer::raise );
	
	/* UPDATE NOTIFY ASSIGN */
	update_notify->notify_action_signal.connect(this, &MainWindow::notify_action_slot);
	update_notify->track_list_changed_signal.connect(this,&MainWindow::rebuild_track_lists);
	
	edit_view_frame->show_track_dialog_signal.connect( track_dialog, &TrackDialog::show );
	
	create_keybindings();	
}
	
MainWindow::MainWindow(String p_settings_dir,String p_settings_file) 
{
	
	settings_dir=p_settings_dir;
	settings_file=p_settings_file;

	SoundDriverList::get_singleton()->set_song(&data.song);
	MidiDriverList::get_singleton()->set_song_playback(&data.song.get_song_playback());
	update_notify = new GUI_UpdateNotify;
	
	data.editor = new Editor( &data.song, update_notify );
	
	MidiInputHandler::get_singleton()->set_editor( data.editor );
	
	
}


MainWindow::~MainWindow()
{
}


}
