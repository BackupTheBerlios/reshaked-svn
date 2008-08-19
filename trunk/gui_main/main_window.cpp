//
// C++ Implementation: main_window
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "main_window.h"

#include "widgets/separator.h"
#include "widgets/icon.h"
#include "bundles/menu_box.h"
#include "gui_common/common_skin.h"

#include "nodes/register_nodes.h"
#include "gui_nodes/register_node_uis.h"
#include "editor/edit_commands.h"

void MainWindow::init(String p_config_file) {


}

void MainWindow::initial_tempo_changed() {

	tempo_spin->get_range()->set( song.get_initial_tempo() );
}

void MainWindow::tempo_spin_changed(double p_bpm) {

	static bool updating=false;
	
	if (updating)
		return;
		
	updating=true;
		
	EditCommands::get_singleton()->song_change_initial_tempo(&song,(int)p_bpm);
	
	updating=false;
}

void MainWindow::octave_spin_changed(double p_to_octave) {

	editor->set_octave((int)p_to_octave);
}

void MainWindow::unhandled_key(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask) {


	if (!p_press)
		return;
		
	unsigned int code = p_scan_code | p_modifier_mask;
	//printf("fuck.. unhandled key code %i, trackview %i\n",code,KEYBIND("global/track_view"));

	SWITCH( code )
	
		CASE( KEYBIND("global/raise_octave") )
		

			octave_spin->get_range()->step_increment();
		CASE( KEYBIND("global/lower_octave") )

			octave_spin->get_range()->step_decrement();
			
		CASE( KEYBIND("global/add_node") )
		
			add_node_callback();
			
		CASE( KEYBIND("global/song_view") )
		
			tab_bar->select_tab( TAB_GLOBAL );
			
		CASE( KEYBIND("global/track_view") )
		
			tab_bar->select_tab( TAB_TRACKS );
			track_editor_screen->notify_enter();
			
		CASE( KEYBIND("global/graph_view") )
		
			tab_bar->select_tab( TAB_GRAPH );
						
	END_SWITCH		
}
void MainWindow::tab_changed(int p_tab) {

	
}

void MainWindow::pre_tab_changed(int p_tab) {

	if (tab_bar->get_selected_tab()==TAB_GRAPH)
		last_graph_add_category=add_node_dialog->get_current_category();
		
}

void MainWindow::file_menu_callback(int p_option) {

}

void MainWindow::control_callback(int p_option) {

}

void MainWindow::add_node_callback() {

	if (tab_bar->get_selected_tab()!=TAB_GRAPH)
		add_node_dialog->show("Tracks");
	else
		add_node_dialog->show(last_graph_add_category);
	
}

void MainWindow::quit_request() {

	quit=true;
}

bool MainWindow::must_quit() {

	return quit;
}


MainWindow::MainWindow(GUI::Painter *p_painter,GUI::Timer *p_timer,GUI::Skin *p_skin) : GUI::Window(p_painter,p_timer,p_skin) {

	init_key_bindings();
	editor = new Editor(&song);

	register_nodes();
	register_node_uis();


	GUI::VBoxContainer * main_vbc = new GUI::VBoxContainer;
	set_root_frame(main_vbc);
	
	GUI::HBoxContainer *hb = main_vbc->add(new GUI::HBoxContainer);
	
	
	GUI::MenuBox * file_menu = hb->add( new GUI::MenuBox( "", p_skin->get_bitmap( BITMAP_ICON_FILE_DISK) ) );
	
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
	
	file_menu->item_selected_signal.connect( this, &MainWindow::file_menu_callback );
	
	tab_bar = hb->add( new GUI::TabBar );
	
	tab_bar->add_tab("Song");
	tab_bar->add_tab("Tracks");
	tab_bar->add_tab("Graph");
		
	hb->add( new GUI::VSeparator );
	
	GUI::MenuButton * add_button = hb->add( new GUI::MenuButton( p_skin->get_bitmap( BITMAP_ICON_ADD) )  );
	add_button->set_tooltip("Add Node: \n From here, you can add any\n kind of nodes to the song:\n\n   -Tracks (Pattern/Automation)\n   -Synths\n   -Audio FX\n   -Hardware In/Out (Audio/MIDI).");
	add_button->pressed_signal.connect( this, &MainWindow::add_node_callback );

	hb->add( new GUI::VSeparator );

	hb->add( new GUI::MenuButton(get_skin()->get_bitmap(BITMAP_CONTROL_RW)) )->pressed_signal.connect( GUI::Method( GUI::Method1<int>(this, &MainWindow::control_callback), (int)CONTROL_RW ));
	hb->add( new GUI::MenuButton(get_skin()->get_bitmap(BITMAP_CONTROL_PLAY)) )->pressed_signal.connect( GUI::Method( GUI::Method1<int>(this, &MainWindow::control_callback), (int)CONTROL_PLAY ));
	hb->add( new GUI::MenuButton(get_skin()->get_bitmap(BITMAP_CONTROL_LOOP)) )->pressed_signal.connect( GUI::Method( GUI::Method1<int>(this, &MainWindow::control_callback), (int)CONTROL_LOOP ));
	hb->add( new GUI::MenuButton(get_skin()->get_bitmap(BITMAP_CONTROL_PAUSE)) )->pressed_signal.connect( GUI::Method( GUI::Method1<int>(this, &MainWindow::control_callback), (int)CONTROL_PAUSE ));
	hb->add( new GUI::MenuButton(get_skin()->get_bitmap(BITMAP_CONTROL_STOP)) )->pressed_signal.connect( GUI::Method( GUI::Method1<int>(this, &MainWindow::control_callback), (int)CONTROL_STOP ));
	hb->add( new GUI::MenuButton(get_skin()->get_bitmap(BITMAP_CONTROL_FF)) )->pressed_signal.connect( GUI::Method( GUI::Method1<int>(this, &MainWindow::control_callback), (int)CONTROL_FF ));
	hb->add( new GUI::MenuButton(get_skin()->get_bitmap(BITMAP_CONTROL_REC)) )->pressed_signal.connect( GUI::Method( GUI::Method1<int>(this, &MainWindow::control_callback), (int)CONTROL_REC ));
	hb->add( new GUI::MenuButton(get_skin()->get_bitmap(BITMAP_CONTROL_REC_AUTO)) )->pressed_signal.connect( GUI::Method( GUI::Method1<int>(this, &MainWindow::control_callback), (int)CONTROL_REC_AUTO ));
	
//	hb->add( new GUI::VSeparator );
	
	hb->add( new GUI::Widget, 1 ); //expand
	
	hb->add( new GUI::VSeparator );
		
	hb->add( new GUI::Icon )->add_bitmap_override( GUI::BITMAP_ICON_DEFAULT, BITMAP_ICON_TEMPO );
	tempo_spin = hb->add(new GUI::SpinBox);
	tempo_spin->get_range()->config(32,255,125,1);
	tempo_spin->get_range()->value_changed_signal.connect( this, &MainWindow::tempo_spin_changed );
	
	hb->add( new GUI::VSeparator );
	hb->add( new GUI::Icon )->add_bitmap_override( GUI::BITMAP_ICON_DEFAULT, BITMAP_ICON_OCTAVE );	
	octave_spin = hb->add(new GUI::SpinBox);
	octave_spin->get_range()->config(0,9,4,1);
	octave_spin->get_range()->value_changed_signal.connect( this, &MainWindow::octave_spin_changed );
		
	hb->add( new GUI::VSeparator );
		
	GUI::MenuButton * preferences = hb->add( new GUI::MenuButton( p_skin->get_bitmap( BITMAP_ICON_PREFERENCES) )  );
	GUI::MenuButton * help = hb->add( new GUI::MenuButton( p_skin->get_bitmap( BITMAP_ICON_HELP) )  );
		
	main_stack = main_vbc->add( new GUI::StackContainer, 1 );
	
	global_screen = main_stack->add(new GlobalScreen);
	track_editor_screen = main_stack->add(new TrackEditorScreen(editor,&gui_update_notify));
	audio_graph_screen = main_stack->add(new AudioGraphScreen(&gui_update_notify,&song) );	
	
	tab_bar->tab_changed_signal.connect( main_stack, &GUI::StackContainer::raise );
	
	hb = main_vbc->add( new GUI::HBoxContainer );
	
	hb->add( new GUI::Icon( get_skin()->get_bitmap( BITMAP_ICON_INFO ) ) );
	
	info_line = hb->add( new GUI::LineEdit, 4 );
	
	hb->add( new GUI::Icon( get_skin()->get_bitmap( BITMAP_ICON_TIME ) ) );
	
	time_line = hb->add( new GUI::LineEdit );
	time_line->set_minimum_size( GUI::Size(150,-1));
	
	/* Pages */

	add_node_dialog = new AddNodeDialog(this,&song);
	add_node_dialog->hide();
	quit=false;
	key_unhandled_signal.connect( this, &MainWindow::unhandled_key);
	gui_update_notify.song_initial_tempo_changed_signal.connect( this, &MainWindow::initial_tempo_changed );
	last_graph_add_category="Hardware";
}


MainWindow::~MainWindow()
{

	delete editor;
	
}


