//
// C++ Interface: menu_box
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PIGUIMENU_BOX_H
#define PIGUIMENU_BOX_H

#include "widgets/button.h"
#include "bundles/popup_menu.h"

namespace GUI {

class MenuBox : public Button {
	
	virtual const StyleBox& stylebox(int p_which);
	virtual FontID font(int p_which);
	virtual BitmapID bitmap(int p_which);
	virtual int constant(int p_which);
	virtual const Color& color(int p_which);
	
	virtual String get_type();
	
	PopUpMenu *popup;


	void set_in_window();
	
	void item_activated(int p_which);
	
	virtual void pressed();
	Size size;
	void resize(const Size& p_new_size);

	struct DeferredAdd {

		DeferredAdd *next;
		String text;
		int id;
		BitmapID bitmap;
		bool is_sep;

		DeferredAdd( String p_text, int p_id, BitmapID p_bid) { next=0; text=p_text; id=p_id; bitmap=p_bid; is_sep=false; }
		DeferredAdd() { next=0; is_sep=true; }
	};

	DeferredAdd *deferred_add_list;
	void add_to_defered_list( DeferredAdd *p_elem );
	void clear_deferred_list();
public:

	Signal< Method1<int> > item_selected_signal;

	void add_item(const String& p_str,int p_id=-1);
	void add_item(BitmapID p_bitmap_ID,const String& p_str="",int p_id=-1);
	void add_separator();
	
	void clear(); ///< Clear the list

	MenuBox(String p_text="",BitmapID p_bitmap_ID=-1);
	~MenuBox();
	
};

};

#endif
