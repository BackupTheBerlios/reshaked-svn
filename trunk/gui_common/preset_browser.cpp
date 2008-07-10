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
#include "gui_common/common_skin.h"
#include <list>

void PresetBrowser::parse_dir( GUI::FileSystem *p_fs, GUI::TreeItem *p_item ) {

	p_fs->list_dir_begin();
	
	std::list<String> dir_list;
	std::list<String> file_list;
	
	String file;
	bool isdir;
	while( true ) {
	
		file=p_fs->get_next(&isdir);
		if (file=="")
			break;
			
		if (isdir)
			dir_list.push_back(file);
		else
			file_list.push_back(file);
	}
	
	p_fs->list_dir_end();
	
	for( std::list<String>::iterator I = dir_list.begin(); I != dir_list.end() ; I++ ) {
	
		if (*I=="." || *I=="..")
			continue;
			
		GUI::TreeItem *diritem = tree->create_item( p_item );
		diritem->set_text( 0, *I + "/" );
		diritem->set_bitmap(0, get_skin()->get_bitmap( BITMAP_ICON_FILE_FOLDER ) );
			
		bool failed = p_fs->change_dir( *I );
		ERR_CONTINUE( failed );
		parse_dir( p_fs, diritem );
		failed = p_fs->change_dir( ".." );
		ERR_FAIL_COND( failed );
	}
	
	for( std::list<String>::iterator I = file_list.begin(); I != file_list.end() ; I++ ) {
	
		GUI::TreeItem *fileitem = tree->create_item( p_item );
		fileitem->set_text( 0, *I );
	}
	
}

void PresetBrowser::rescan_tree() {

	GUI::FileSystem *fs = GUI::FileSystem::instance_func();
	
	fs->change_dir(current_path);
	
	tree->clear();
	
	GUI::TreeItem *root = tree->create_item();
	
	root->set_text(0,"/");
	root->set_bitmap(0, get_skin()->get_bitmap( BITMAP_ICON_FILE_FOLDER ) );
	
	parse_dir( fs, root );
	
	delete fs;
	
}

String PresetBrowser::get_selected() {

	GUI::TreeItem* sel=NULL;
	int col=NULL;
	if (!tree->get_selected(&sel,&col) ||  !sel)
		return "/";
		
	String path;
	
	while (sel) {
	
		path=sel->get_text(0)+path;
		sel=sel->get_parent();
	}
	
	return path;
}

void PresetBrowser::dialog_callback(String p_text) {

	if (p_text=="")
		return;

	switch( current_op ) {
	
		case OP_OPEN: {
				
		} break;
		case OP_SAVE: {
		
		} break;
		case OP_MKDIR: {
		
			if (p_text.find("/")!=-1 || p_text.find("\\")!=-1 || p_text.find(".")==0) {
			
				mbox->show("File contains invalid characters.");
				return;
			}
			
			String path=current_path+get_selected();
			if (path[ path.size() -1]=='/')
				path=path.left( path.size() -1 );
			
			GUI::FileSystem *fs = GUI::FileSystem::instance_func();
			if (!fs->make_dir( path+"/"+p_text)) {
			
				rescan_tree();
			} else {
			
				mbox->show("Failed making dir.");
			
			}
		
		} break;
		case OP_RENAME: {
		
			if (p_text.find("/")!=-1 || p_text.find("\\")!=-1 || p_text.find(".")==0) {
			
				mbox->show("File contains invalid characters.");
				return;
			}

			
			String path=current_path+get_selected();
			if (path[ path.size() -1]=='/')
				path=path.left( path.size() -1 );
				
			String new_path=path.left( path.find_last("/") )+"/"+p_text;
			
			GUI::FileSystem *fs = GUI::FileSystem::instance_func();
			
			if (fs->rename( path, new_path)) {
			
				mbox->show("Error renaming!");			
			} else {
				rescan_tree();
			}
			
			delete fs;
		
		} break;
		case OP_ERASE: {
			printf("erasing..\n");						
			String path=current_path+get_selected();
			if (path[ path.size() -1]=='/')
				path=path.left( path.size() -1 );
						
			GUI::FileSystem *fs = GUI::FileSystem::instance_func();
			
			if (fs->remove( path)) {
			
				mbox->show("Error removing!");			
			} else {
				rescan_tree();
			}
			
			delete fs;
						
		} break;
	};

}

void PresetBrowser::question_callback(int p_answer) {

	printf("answer was %i\n",p_answer);
	if (!p_answer)
		return;
	dialog_callback(".");
}

void PresetBrowser::op_callback( Operation p_op ) {
	
	current_op=p_op;

	switch( p_op ) {
	
		case OP_OPEN: {
				
		} break;
		case OP_SAVE: {
		
		} break;
		case OP_MKDIR: {
			
			string_input->show("Enter New Name:","New Dir");
		} break;
		case OP_RENAME: {
			
			String sel=get_selected();
			if  (sel=="/") {
			
				mbox->show("Not much point in renaming the root dir..");
				return;
			}
			
			if (sel[ sel.size() -1]=='/')
				sel=sel.left( sel.size() -1 );
			
		//	if (sel[ sel.size() -1]=='/') {
		// 	ok, renaming dirs allowed
	//			mbox->show("Can only rename presets.");
//				return;//
			//}/
				
			string_input->show("Enter New Name:",sel.substr( sel.find_last("/")+1,sel.size()));
				
		} break;
		case OP_ERASE: {
						
			String sel=get_selected();
			if  (sel=="/") {			
				mbox->show("Easy tiger! I want to keep this dir..");
				return;
			}
			question->show("Delete \""+sel+"\" ?");
			
		} break;
	};

}

void PresetBrowser::edit_preset( AudioNode *p_node ) {

	window_box->get_window_top()->set_text("Preset Editor - "+p_node->get_name() );
	
	DefaultPaths::ensure_node_preset_dir_exists(p_node->get_info()->unique_ID);
	
	current_path=DefaultPaths::get_presets_path()+"/"+p_node->get_info()->unique_ID;
	
	rescan_tree();
	
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
	b->pressed_signal.connect( GUI::Method( GUI::Method1<Operation>( this, &PresetBrowser::op_callback), OP_OPEN ) );	
	
	b = vbc->add( new GUI::Button( "Save" ) );
	b->pressed_signal.connect( GUI::Method( GUI::Method1<Operation>( this, &PresetBrowser::op_callback), OP_SAVE ) );			
	
	GUI::CheckButton * cb = vbc->add( new GUI::CheckButton( "Reference" ) );
	
	vbc->add( new GUI::HSeparator );
	vbc->add( new GUI::Widget,1 );
	
	b = vbc->add( new GUI::Button( "Make Dir" ) );
	b->pressed_signal.connect( GUI::Method( GUI::Method1<Operation>( this, &PresetBrowser::op_callback), OP_MKDIR ) );
	b = vbc->add( new GUI::Button( "Rename" ) );
	b->pressed_signal.connect( GUI::Method( GUI::Method1<Operation>( this, &PresetBrowser::op_callback), OP_RENAME ) );	
	b = vbc->add( new GUI::Button( "Erase" ) );
	b->pressed_signal.connect( GUI::Method( GUI::Method1<Operation>( this, &PresetBrowser::op_callback), OP_ERASE ) );	
	
	string_input = new GUI::StringInputDialog( this );
	string_input->entered_string_signal.connect( this, &PresetBrowser::dialog_callback );

	mbox = new GUI::MessageBox( this );
	question = new GUI::QuestionInputDialog( this );
	
	question->button_pressed_signal.connect( this,  &PresetBrowser::question_callback );	
	question->add_button(1,"Yes");
	question->add_button(0,"Cancel");
	
}


PresetBrowser::~PresetBrowser()
{
}


