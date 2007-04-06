//
// C++ Implementation: menu_box
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "menu_box.h"
#include "base/skin.h"


namespace GUI {


String MenuBox::get_type() {
	
	return "MenuBox";
}
const StyleBox& MenuBox::stylebox(int p_which) {
	
	switch(p_which) {
		case SB_BUTTON_NORMAL: return Frame::stylebox( SB_MENUBOX_NORMAL ); break;
		case SB_BUTTON_PRESSED: return Frame::stylebox( SB_MENUBOX_PRESSED ); break;
		case SB_BUTTON_HOVER: return Frame::stylebox( SB_MENUBOX_HOVER ); break;
		case SB_BUTTON_FOCUS: return Frame::stylebox( SB_MENUBOX_FOCUS ); break;
		case SB_BUTTON_CHECKED: return Frame::stylebox( -1 ); break;
		case SB_BUTTON_UNCHECKED: return Frame::stylebox( -1 ); break;
	}
		
	return Frame::stylebox(0); //should never reach here
}
FontID MenuBox::font(int p_which) {
	
	return Frame::font( FONT_MENUBOX );
}
BitmapID MenuBox::bitmap(int p_which) {

	return -1;
}

const Color& MenuBox::color(int p_which) {
	
	switch (p_which) {
		
		case COLOR_BUTTON_FONT: return Frame::color(COLOR_MENUBOX_FONT); break;
		
	}
	
	return Frame::color(0);
}

int MenuBox::constant(int p_which) {
	
	switch( p_which ) {
		case C_BUTTON_SEPARATION: return Frame::constant( C_MENUBOX_SEPARATION ); break;
		case C_BUTTON_EXTRA_MARGIN: return Frame::constant( C_MENUBOX_EXTRA_MARGIN ); break;
		case C_BUTTON_DISPLACEMENT: return Frame::constant( C_MENUBOX_DISPLACEMENT ); break;
		case C_BUTTON_CHECKBOX_SIZE: return 0; break;
		case C_BUTTON_HAS_CHECKBOX: return 0; break;
		case C_BUTTON_LABEL_ALIGN_CENTER: return 0; break;
	}
	
	return 0; //should never reach here
}

void MenuBox::resize(const Size& p_new_size) {
	
	size=p_new_size;
}

void MenuBox::pressed() {
	
	popup->set_size( Size( size.width, 0 ) );
	popup->popup( get_global_pos()+Point( 0, size.height) );
}
void MenuBox::item_activated(int p_which) {
	
	item_selected_signal.call(p_which);
	popup->hide(); //this at end of function to reproduce bug
	
}

void MenuBox::add_item(const String& p_str,int p_id) {

	if (!popup) {

		add_to_defered_list( new DeferredAdd( p_str, p_id, -1 ) );
		return;

	}
	if (p_id!=-1)
		popup->add_item( p_str,p_id );
	else
		popup->add_item( p_str );
	
}
void MenuBox::add_item(BitmapID p_ID,const String& p_str,int p_id) {
	
	if (!popup) {


		add_to_defered_list( new DeferredAdd( p_str, p_id, p_ID ) );
		return;

	}
	if (p_id!=-1)
		popup->add_item( p_ID, p_str);
	else
		popup->add_item( p_ID, p_str,p_id );

}
void MenuBox::add_separator() {

	if (!popup) {

		add_to_defered_list( new DeferredAdd );
		return;

	}

	popup->add_separator();
}

void MenuBox::add_to_defered_list( DeferredAdd *p_elem ) {
	
	if ( deferred_add_list ) {
		DeferredAdd *last=deferred_add_list;
		while (last->next) {
				
			last=last->next;
		}
		last->next = p_elem;
	} else {
		
		deferred_add_list = p_elem;

	}
	
}
void MenuBox::clear_deferred_list() {


	while (deferred_add_list) {

		DeferredAdd *aux=deferred_add_list;

		deferred_add_list=deferred_add_list->next;
		delete aux;
	}
	
}

void MenuBox::clear() {
	
	popup->clear();
	clear_deferred_list();

}

void MenuBox::set_in_window() {
	
	popup = new PopUpMenu( get_window() );
	popup->selected_id_signal.connect( this, &MenuBox::item_activated );
	
	DeferredAdd *l=deferred_add_list;
	while (l) {
		
		if (l->is_sep)
			add_separator();
		else if (l->bitmap<0)
			add_item( l->text, l->id );
		else
			add_item( l->bitmap, l->text, l->id );
		
		l=l->next;		
	}
	
	clear_deferred_list();
}

MenuBox::MenuBox(String p_text,BitmapID p_bitmap_ID) : Button(p_text,p_bitmap_ID) {

	deferred_add_list=0;
	popup=0;
}


MenuBox::~MenuBox() {

	if (popup)
		delete popup;
	clear_deferred_list();
}


}
