//
// C++ Implementation: preset_browser
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "preset_browser.h"
#include "containers/box_container.h"
#include "widgets/separator.h"
#include "widgets/check_button.h"
#include "bundles/scroll_box.h"
#include "widgets/window_top.h"
#include "gui_common/default_paths.h"
void PresetBrowser::edit_preset( AudioNode *p_node ) {

	window_box->get_window_top()->set_text("Preset Editor - "+p_node->get_name() );
	
	DefaultPaths::ensure_node_preset_dir_exists(p_node->get_info()->unique_ID);
	
	
	show();
}

PresetBrowser::PresetBrowser(GUI::Window *p_parent) : GUI::Window(p_parent, GUI::Window::MODE_POPUP, GUI::Window::SIZE_TOPLEVEL_CENTER ) {

	window_box = new GUI::WindowBox("Preset Editor");
	set_root_frame( window_box );
	GUI::HBoxContainer *hbc = window_box->add( new GUI::HBoxContainer , 1 );
	
	GUI::ScrollBox *sbox = hbc->add( new GUI::ScrollBox, 1 );
	 
	tree = sbox->set( new GUI::Tree(1) );
	
	sbox->set_scroll_v( true );
	sbox->set_scroll_h( true );
	sbox->set_expand_v( true );
	sbox->set_expand_h( true );
	
	GUI::VBoxContainer *vbc = hbc->add( new GUI::VBoxContainer );
	
	GUI::Button * b = vbc->add( new GUI::Button( "Open" ) );
	b = vbc->add( new GUI::Button( "Save" ) );
	GUI::CheckButton * cb = vbc->add( new GUI::CheckButton( "Reference" ) );
	vbc->add( new GUI::HSeparator );
	vbc->add( new GUI::Widget,1 );
	
	b = vbc->add( new GUI::Button( "Make Dir" ) );
	b = vbc->add( new GUI::Button( "Rename" ) );
	b = vbc->add( new GUI::Button( "Erase" ) );
	

}


PresetBrowser::~PresetBrowser()
{
}


