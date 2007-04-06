//
// C++ Implementation: popup_menu
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "popup_menu.h"
#include "containers/box_container.h"
#include "widgets/separator.h"
#include "widgets/menu_button.h"

namespace GUI {

void PopUpMenu::menu_button_selected(MenuButton* p_mb) {
	
	hide();
	selected_id_userdata_signal.call( p_mb->get_id(), p_mb->get_userdata());
	selected_id_signal.call( p_mb->get_id());
}
void PopUpMenu::menu_button_toggled(MenuButton*p_mb,bool p_toggle) {
	
	hide();
	checked_id_userdata_signal.call( p_mb->get_id(), p_toggle, p_mb->get_userdata());
	checked_id_signal.call( p_mb->get_id(), p_toggle);
	
}


void PopUpMenu::add_item(String p_text, int p_ID, void *p_userdata) {
	
	MenuButton *mb  = vbc->add( new MenuButton(p_text), 0 );
	
	mb->set_id( p_ID );
	mb->set_userdata( p_userdata );
	id_count=p_ID+1;
	
	vbc->adjust_minimum_size();
	
	mb->menubutton_pressed_signal.connect( this,  &PopUpMenu::menu_button_selected );
}

void PopUpMenu::add_item(String p_text, void *p_userdata) {
	
	MenuButton *mb  = vbc->add( new MenuButton(p_text), 0 );
	
	mb->set_id( id_count++ );
	mb->set_userdata( p_userdata );

	vbc->adjust_minimum_size();
	
	mb->menubutton_pressed_signal.connect( this,  &PopUpMenu::menu_button_selected );
	
}
		
void PopUpMenu::add_item(BitmapID p_icon, String p_text, int p_ID, void *p_userdata) {
	
	
	MenuButton *mb  = vbc->add( new MenuButton(p_text,p_icon), 0 );
	
	mb->set_id( p_ID );
	mb->set_userdata( p_userdata );
	id_count=p_ID+1;
	
	vbc->adjust_minimum_size();
	
	mb->menubutton_pressed_signal.connect( this,  &PopUpMenu::menu_button_selected );
	
}
void PopUpMenu::add_item(BitmapID p_icon, String p_text, void *p_userdata) {
	
	
	MenuButton *mb  = vbc->add( new MenuButton(p_text,p_icon), 0 );
	
	mb->set_id( id_count++ );
	mb->set_userdata( p_userdata );
	
	vbc->adjust_minimum_size();
	
	mb->menubutton_pressed_signal.connect( this,  &PopUpMenu::menu_button_selected );
	
}
		
void PopUpMenu::add_item(String p_text, const Method& p_method) {
	
	MenuButton *mb  = vbc->add( new MenuButton(p_text), 0 );
	
	mb->set_id( id_count++ );
	mb->pressed_signal.connect( p_method );
	
	vbc->adjust_minimum_size();
	
	mb->menubutton_pressed_signal.connect( this,  &PopUpMenu::menu_button_selected );
	
}
void PopUpMenu::add_item(BitmapID p_icon, String p_text, const Method& p_method) {
	
	MenuButton *mb  = vbc->add( new MenuButton(p_text,p_icon), 0 );
	
	mb->set_id( id_count++ );
	mb->pressed_signal.connect( p_method );
	
	vbc->adjust_minimum_size();
	
	mb->menubutton_pressed_signal.connect( this,  &PopUpMenu::menu_button_selected );
	
}
		
void PopUpMenu::add_check_item(String p_text, int p_ID, bool p_checked,void *p_userdata) {
	
	MenuButton *mb  = vbc->add( new MenuButton(p_text,true), 0 );
	
	mb->set_id( p_ID );
	mb->set_userdata( p_userdata );
	mb->set_checked(p_checked);
	id_count=p_ID+1;	
	
	vbc->adjust_minimum_size();
	
	mb->menubutton_toggled_signal.connect( this,  &PopUpMenu::menu_button_toggled );
	
}
void PopUpMenu::add_check_item(String p_text, bool p_checked,void *p_userdata) {
	
	MenuButton *mb  = vbc->add( new MenuButton(p_text,true), 0 );
	
	mb->set_id( id_count++ );
	mb->set_userdata( p_userdata );
	mb->set_checked(p_checked);
	
	vbc->adjust_minimum_size();
	
	mb->menubutton_toggled_signal.connect( this,  &PopUpMenu::menu_button_toggled );
	
}
void PopUpMenu::add_check_item(BitmapID p_icon, String p_text, int p_ID, bool p_checked,void *p_userdata) {
	
	
	MenuButton *mb  = vbc->add( new MenuButton(p_text,p_icon,true), 0 );
	
	mb->set_id( p_ID );
	mb->set_userdata( p_userdata );
	mb->set_checked(p_checked);
	id_count=p_ID+1;
	
	vbc->adjust_minimum_size();
	
	mb->menubutton_toggled_signal.connect( this,  &PopUpMenu::menu_button_toggled );
	
}
void PopUpMenu::add_check_item(BitmapID p_icon, String p_text,bool p_checked,void *p_userdata) {
	
	
	MenuButton *mb  = vbc->add( new MenuButton(p_text,p_icon,true), 0 );
	
	mb->set_id( id_count++ );
	mb->set_userdata( p_userdata );
	mb->set_checked(p_checked);
	
	vbc->adjust_minimum_size();
	
	mb->menubutton_toggled_signal.connect( this,  &PopUpMenu::menu_button_toggled );
	
}

void PopUpMenu::add_check_item(String p_text, const Method1<bool>& p_method, bool p_checked) {
	
	MenuButton *mb  = vbc->add( new MenuButton(p_text,true), 0 );
	
	mb->set_id( id_count++ );
	mb->toggled_signal.connect( p_method );
	mb->set_checked(p_checked);
	
	vbc->adjust_minimum_size();
	
	mb->menubutton_toggled_signal.connect( this,  &PopUpMenu::menu_button_toggled );
	
}
void PopUpMenu::add_check_item(BitmapID p_icon, String p_text, const Method1<bool>& p_method, bool p_checked) {
	
	MenuButton *mb  = vbc->add( new MenuButton(p_text,p_icon,true), 0 );
	
	mb->set_id( id_count++ );
	mb->toggled_signal.connect( p_method );
	mb->set_checked(p_checked);
	
	vbc->adjust_minimum_size();
	
	mb->menubutton_toggled_signal.connect( this,  &PopUpMenu::menu_button_toggled );
	
}

void PopUpMenu::add_separator() {

	vbc->add( new HSeparator,0 );

}


String PopUpMenu::get_item_text(int p_ID) {
	
	ChildIterator I=vbc->first_child();
	
	while (!I.end()) {
		
		Frame *f=*I;
		
		if (f->get_type()!="MenuButton") {
			
			I=vbc->next_child( I );
			continue;
		}
		
		MenuButton *mb=(MenuButton*)f;
				
		if (mb->get_id()==p_ID) {
			
			
			return mb->get_text();
		}
		I=vbc->next_child( I );
		
	}
	
	return "";
}
BitmapID PopUpMenu::get_item_icon(int p_ID) {
	
	ChildIterator I=vbc->first_child();
	
	while (!I.end()) {
		
		Frame *f=*I;
		
		if (f->get_type()!="MenuButton") {
			
			I=vbc->next_child( I );
			continue;
		}
		
		MenuButton *mb=(MenuButton*)f;
				
		if (mb->get_id()==p_ID) {
			
			
			return mb->get_icon();
		}
		I=vbc->next_child( I );
	
	}
	
	return -1;
}
void* PopUpMenu::get_item_userdata(int p_ID) {
	
	ChildIterator I=vbc->first_child();
	
	while (!I.end()) {
		
		Frame *f=*I;
		
		if (f->get_type()!="MenuButton") {
			
			I=vbc->next_child( I );
			continue;
		}
		
		MenuButton *mb=(MenuButton*)f;
				
		if (mb->get_id()==p_ID) {
			
			
			return mb->get_userdata();
		}
		I=vbc->next_child( I );
		
	}
	
	return 0;
	
}

bool PopUpMenu::has_ID(int p_ID) {
	
	ChildIterator I=vbc->first_child();
	
	while (!I.end()) {
		
		Frame *f=*I;
		
		if (f->get_type()!="MenuButton") {
			
			I=vbc->next_child( I );
			continue;
		}
		
		MenuButton *mb=(MenuButton*)f;
				
		if (mb->get_id()==p_ID) {
			
			
			return true;
		}
		
		I=vbc->next_child( I );
		
	}
	
	return false;

}
	

void PopUpMenu::clear()  {
	
	
	
}

void PopUpMenu::popup() {
	

	show();	
	set_focus( 0 ); //clear focus	
}
void PopUpMenu::popup(const Point &p_pos) {
	
	set_pos(p_pos);
	show();	
	set_focus( 0 ); //clear focus	
	
}

void PopUpMenu::skin_changed() {
	
	Window::skin_changed();
	vbc->set_style( get_skin()->get_stylebox( SB_POPUP_BG ), true );
	
}

PopUpMenu::PopUpMenu(Window *p_parent) : Window(p_parent,MODE_POPUP) {
	
	id_count=0;
	vbc = new VBoxContainer;
	set_root_frame( vbc );
	vbc->set_style( get_skin()->get_stylebox( SB_POPUP_BG ), true );
	

	
}


PopUpMenu::~PopUpMenu() {
	
}


}
