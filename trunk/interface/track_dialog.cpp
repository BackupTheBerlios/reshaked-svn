//
// C++ Implementation: track_dialog
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "track_dialog.h"
#include "gui_custom/rsskin.h"

namespace ReShaked {

	
void TrackDialog::action(int p_action) {
		
	ERR_FAIL_COND(!t);
	switch(p_action) {
		
		case ITEM_MUTE: {
			
			editor->set_track_mute(t,!t->is_mute());
	
		} break;
		case ITEM_SOLO: {
			
			editor->set_track_solo(t_idx);

		} break;
		case ITEM_ADD_COLUMN: {
			
			editor->track_pattern_add_column(t_idx);

		} break;
		case ITEM_REMOVE_COLUMN: {
			
			editor->track_pattern_remove_column(t_idx);
			
		} break;
		case ITEM_AUTOMATIONS: {
			
			request_automation_editor_signal.call(t);			
		} break;
		case ITEM_MOVE_TRACK_LEFT: {
			
			editor->move_track_left(t_idx);
			
		} break;
		case ITEM_MOVE_TRACK_RIGHT: {
			
			editor->move_track_right(t_idx);
		} break;
		case ITEM_RENAME: {
			
			track_name->show("Rename Track: ("+t->get_name()+")",t->get_name());
		
		} break;
		case ITEM_REMOVE: {
			
			editor->remove_track(t_idx);
			
		} break;
		
	}
}
	
void TrackDialog::show(const Point& p_pos,int p_track) {
		
	clear();
		
	t = (p_track==-1)?&editor->get_song()->get_global_track():editor->get_song()->get_track(p_track);
	ERR_FAIL_COND(!t);
	t_idx=p_track;

	
	bool track_is_mute=editor->get_song()->get_track(p_track);
	
	if (p_track!=-1) {
		add_check_item("Mute",ITEM_MUTE,t->is_mute());
		add_item(get_skin()->get_bitmap(BITMAP_ICON_SOLO),"Solo",ITEM_SOLO); 
		add_separator();
		add_item(get_skin()->get_bitmap(BITMAP_ICON_COLUMN_ADD),"Add Column",ITEM_ADD_COLUMN); 
		add_item(get_skin()->get_bitmap(BITMAP_ICON_COLUMN_REMOVE),"Remove Column",ITEM_REMOVE_COLUMN); 
		add_separator();
	}
	
 	add_item(get_skin()->get_bitmap(BITMAP_ICON_AUTOMATION),"Automations..",ITEM_AUTOMATIONS); 
	
	if (p_track!=-1) {
		add_separator();
		add_item(get_skin()->get_bitmap(BITMAP_ICON_LEFT),"Move Left",ITEM_MOVE_TRACK_LEFT); 
		add_item(get_skin()->get_bitmap(BITMAP_ICON_RIGHT),"Move Right",ITEM_MOVE_TRACK_RIGHT); 
		add_separator();
		add_item(get_skin()->get_bitmap(BITMAP_ICON_RENAME),"Rename",ITEM_RENAME); 
		add_separator();
		add_item(get_skin()->get_bitmap(BITMAP_ICON_REMOVE),"Remove",ITEM_REMOVE); 
	}
			
	popup(p_pos);
	
}
	
	
void TrackDialog::track_renamed_callback(String p_name) {
	ERR_FAIL_COND(!t);
	
	editor->set_track_name(t_idx,p_name);

}
TrackDialog::TrackDialog(Window *p_parent,Editor *p_editor) : PopUpMenu(p_parent)
{
	editor=p_editor;
	selected_id_signal.connect(this,&TrackDialog::action); ///< Selected, return ID , for all items
	t=0;
	
	track_name = new StringInputDialog(p_parent);
	track_name->entered_string_signal.connect(this,&TrackDialog::track_renamed_callback);
	
}


TrackDialog::~TrackDialog()
{
	
}


}
