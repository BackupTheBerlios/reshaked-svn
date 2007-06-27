//
// C++ Implementation: global_view_cursor
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "global_view_cursor.h"
#include "gui_custom/rsskin.h"
#include "widgets/separator.h"
#include "widgets/icon.h"

namespace ReShaked {

	
void GlobalViewCursor::add_button(GlobalView::EditMode p_mode, int p_bitmap_list_idx,String p_hint) {

	mode_button[p_mode]= add( new MenuButton( bitmap(p_bitmap_list_idx) ) );
	 
	mode_button[p_mode]->pressed_signal.connect( Method( Method1<int>( this, &GlobalViewCursor::mode_selected), (int)p_mode ) );
	
	group.add_button(mode_button[p_mode]);
	//tooltip!
}

void GlobalViewCursor::mode_selected(int p_mode) {
	
	if (!inited)
		return;
	
	group.set_current(p_mode);
	mode=(GlobalView::EditMode)p_mode;
	edit_mode_changed_signal.call(mode);
}


void GlobalViewCursor::zoom_changed(double p_zoom) {
	
	zoom_changed_signal.call(p_zoom);
}

void GlobalViewCursor::set_in_window() {
	
	if (inited) return;
	
	inited=true;
	
	add_button(GlobalView::EDIT_MODE_SELECT,BITMAP_GLOBAL_TOOLBAR__SELECT,"Select (Click)/Add(Ctrl+Click)/Move(Drag)/Copy(Shift+Drag)/Copy-Link(Ctrl+Shift+Drag)");
	add_button(GlobalView::EDIT_MODE_ADD_BLOCK,BITMAP_GLOBAL_TOOLBAR__ADD,"Add Blocks");
	add_button(GlobalView::EDIT_MODE_COPY_BLOCK,BITMAP_GLOBAL_TOOLBAR__COPY,"Copy Blocks");
	add_button(GlobalView::EDIT_MODE_COPY_LINK_BLOCK,BITMAP_GLOBAL_TOOLBAR__COPY_LINK,"Copy Link Blocks");
	
	add ( new VSeparator );
	
	delete_selected=add( new MenuButton(  bitmap( BITMAP_GLOBAL_TOOLBAR__DELETE_SELECTED )) );
	 
	add ( new VSeparator );
	
	select_linked=add( new MenuButton(  bitmap(BITMAP_GLOBAL_TOOLBAR__SELECT_LINKED )) );
	
	unlink_selected=add( new MenuButton(  bitmap(BITMAP_GLOBAL_TOOLBAR__UNLINK_SELECTED )) );
	
	add (new Widget,1);
	
	zoom = add( new HSlider );
	zoom->set_minimum_size( Size(150,-1) );
	
	zoom->get_range()->set_step(0.001);
	zoom->get_range()->set_max(1.0);
	zoom->get_range()->set(0.35);
	zoom->get_range()->value_changed_signal.connect( this, &GlobalViewCursor::zoom_changed );
	
	add ( new Icon( bitmap(BITMAP_GLOBAL_TOOLBAR__ZOOM) ) );
	
}

GlobalViewCursor::GlobalViewCursor() {
	
	inited=false;

}


GlobalViewCursor::~GlobalViewCursor()
{
}


}
