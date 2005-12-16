//
// C++ Implementation: visual_settings
//   
// Description:
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "visual_settings.h"
#include "pixmaps/pattern_glob_bg.xpm"
#include "pixmaps/automation_glob_bg.xpm"
#include "pixmaps/pattern_glob_bg_sel.xpm"
#include "pixmaps/automation_glob_bg_sel.xpm"
#include "pixmaps/pattern_font.xpm"
#include "pixmaps/pattern_vol_font.xpm"
#include "pixmaps/rowcount_font.xpm"
#include "pixmaps/rowcount_font_subbeat.xpm"
#include "pixmaps/pattern_bg.xpm"
#include "pixmaps/automation_bg.xpm"
#include "pixmaps/automation_bg_sel.xpm"
#include "pixmaps/pattern_bg_sel.xpm"
#include "pixmaps/pattern_cursor.xpm"
#include "pixmaps/track_options.xpm"
#include "pixmaps/knob_base.xpm"
#include "pixmaps/knob_handle.xpm"
#include "pixmaps/track_settings_pattern.xpm"
#include "pixmaps/track_settings_automations.xpm"
#include "pixmaps/track_settings_controls.xpm"
#include "pixmaps/track_settings_effects.xpm"
#include "pixmaps/track_settings_connections.xpm"
#include "pixmaps/envelope_point.xpm"

#include "pixmaps/font_global_bar.xpm"
#include "pixmaps/font_global_beat.xpm"

#include "pixmaps/track_move_left.xpm"
#include "pixmaps/track_move_right.xpm"
#include "pixmaps/track_delete.xpm"
#include "pixmaps/track_edit.xpm"
#include "pixmaps/track_move_automation_left.xpm"
#include "pixmaps/track_move_automation_right.xpm"
#include "pixmaps/track_column_add.xpm"
#include "pixmaps/track_column_rem.xpm"
#include "pixmaps/zoom.xpm"

#include "pixmaps/global_edit_add_block.xpm"
#include "pixmaps/global_edit_copy_block_linked.xpm"
#include "pixmaps/global_edit_copy_block.xpm"
#include "pixmaps/global_edit_delete_block.xpm"
#include "pixmaps/global_edit_select_block.xpm"

#include "pixmaps/cursor_add_block.xpm"
#include "pixmaps/connection_panel_bg.xpm"
#include "pixmaps/connection_panel_system.xpm"

#include "pixmaps/jack_hole.xpm"

#include <iostream>

#include "error_macros.h"
namespace ReShaked {

VisualSettings * VisualSettings::singleton=NULL;
 
VisualSettings * VisualSettings::get_singleton() {

	return singleton;


}

int VisualSettings::get_editing_row_height() {

	return editing_row_height;
}

PixmapFont* VisualSettings::get_global_bar_font() {
	
	return &global_bar_font;
}

PixmapFont* VisualSettings::get_global_beat_font() {
	
	return &global_beat_font;
}

PixmapFont* VisualSettings::get_pattern_font() {

	return &pattern_font;
}
PixmapFont* VisualSettings::get_pattern_vol_font() {

	return &pattern_vol_font;
}

PixmapFont* VisualSettings::get_rowcount_font() {

	return &rowcount_font;
}

PixmapFont* VisualSettings::get_rowcount_subbeat_font() {

	return &rowcount_subbeat_font;
}

SkinBox * VisualSettings::get_skin_box(SkinBoxList p_which) {

	SkinBoxMap::iterator I=skin_box_map.find(p_which);

	ERR_FAIL_COND_V( I==skin_box_map.end() , NULL );

	return I->second;

}

int VisualSettings::get_selection_h_margin() {
	
	return 3;
}

QColor VisualSettings::get_color(ColorList p_color) {

	ColorMap::iterator I=color_map.find(p_color);

	ERR_FAIL_COND_V( I==color_map.end() ,QColor(0,0,0) );

	return I->second;

}

QPixmap VisualSettings::get_pixmap(PixmapList p_pixmap) {
	
	PixmapMap::iterator I=pixmap_map.find(p_pixmap);

	ERR_FAIL_COND_V( I==pixmap_map.end() ,QPixmap() );

	return I->second;
	
}

QPixmap VisualSettings::get_pattern_cursor_pixmap() {

	return pattern_cursor;
}

QPoint VisualSettings::get_pattern_cursor_offset() {

	return pattern_cursor_offset;
}

VisualSettings::VisualSettings() {

	singleton=this;    

	skin_box_map[SKINBOX_GLOBALVIEW_PATTERN]=new SkinBox((const char**)pattern_glob_bg_xpm,6,6,6,6,true);
	skin_box_map[SKINBOX_GLOBALVIEW_AUTOMATION]=new SkinBox((const char**)automation_glob_bg_xpm,6,6,6,6,true);
	skin_box_map[SKINBOX_GLOBALVIEW_PATTERN_SELECTED]=new SkinBox((const char**)pattern_glob_bg_sel_xpm,6,6,6,6,true);
	skin_box_map[SKINBOX_GLOBALVIEW_AUTOMATION_SELECTED]=new SkinBox((const char**)automation_glob_bg_sel_xpm,6,6,6,6,true);
	skin_box_map[SKINBOX_EDITING_PATTERN]=new SkinBox((const char**)pattern_bg_xpm,8,8,8,8);
	skin_box_map[SKINBOX_EDITING_PATTERN_SELECTED]=new SkinBox((const char**)pattern_bg_sel_xpm,4,4,4,4);
	skin_box_map[SKINBOX_EDITING_AUTOMATION]=new SkinBox((const char**)automation_bg_xpm,4,4,4,4);
	skin_box_map[SKINBOX_EDITING_AUTOMATION_SELECTED]=new SkinBox((const char**)automation_bg_sel_xpm,4,4,4,4);
	skin_box_map[SKINBOX_EDITING_RESIZABLE_CURSOR]=new SkinBox((const char**)pattern_cursor_xpm,3,3,3,3,true); 
	skin_box_map[SKINBOX_RACK_NORMAL]=new SkinBox((const char**)connection_panel_bg_xpm,26,31,20,33,true);
	skin_box_map[SKINBOX_RACK_SYSTEM]=new SkinBox((const char**)connection_panel_system_xpm,26,31,20,33,true);
	
	color_map[COLORLIST_PATTERN_EDIT_NOTE]=QColor(0,33,0);
	color_map[COLORLIST_PATTERN_EDIT_VOL]=QColor(255,190,170);
	color_map[COLORLIST_PATTERN_EDIT_FOCUS_RECT]=QColor(255,0,0);
	color_map[COLORLIST_PATTERN_EDIT_BEAT_LINE]=QColor(200,255,200,128);
	color_map[COLORLIST_PATTERN_EDIT_SUBBEAT_LINE]=QColor(150,200,150,64);
	color_map[COLORLIST_AUTOMATION_ENVELOPE]=QColor(255,255,200,200);
	color_map[COLORLIST_GLOBAL_VIEW_BAR_LINE]=QColor(150,200,150);
	color_map[COLORLIST_GLOBAL_VIEW_BEAT_LINE]=QColor(50,80,60);
	color_map[COLORLIST_PATTERN_EDIT_BAR]=QColor(222,255,242,40);
	color_map[COLORLIST_EDITOR_SELECTION_AUTOMATION]=QColor(0x75,0x7f,0x09);
	color_map[COLORLIST_EDITOR_SELECTION_PATTERN]=QColor(0x28,0x7c,0x46);
	
	pixmap_map[PIXMAP_TRACK_OPTIONS]=QPixmap((const char**)track_options_xpm);
	pixmap_map[PIXMAP_KNOB_BASE]=QPixmap((const char**)knob_base_xpm);
	pixmap_map[PIXMAP_KNOB_HANDLE]=QPixmap((const char**)knob_handle_xpm);
	pixmap_map[PIXMAP_TRACK_SETTINGS_PATTERN]=QPixmap((const char**)track_settings_pattern_xpm);
	pixmap_map[PIXMAP_TRACK_SETTINGS_AUTOMATIONS]=QPixmap((const char**)track_settings_automations_xpm); 
	pixmap_map[PIXMAP_TRACK_SETTINGS_CONTROLS]=QPixmap((const char**)track_settings_controls_xpm);
	pixmap_map[PIXMAP_TRACK_SETTINGS_EFFECTS]=QPixmap((const char**)track_settings_effects_xpm);
	pixmap_map[PIXMAP_TRACK_SETTINGS_CONNECTIONS]=QPixmap((const char**)track_settings_connections_xpm);
	pixmap_map[PIXMAP_ENVELOPE_POINT]=QPixmap((const char**)envelope_point_xpm);
	pixmap_map[PIXMAP_RACK_JACK_HOLE]=QPixmap((const char**)jack_hole_xpm);


	pixmap_map[ICON_TRACK_MOVE_LEFT]=QPixmap((const char**)track_move_left_xpm);
	pixmap_map[ICON_TRACK_MOVE_RIGHT]=QPixmap((const char**)track_move_right_xpm);
	pixmap_map[ICON_TRACK_DELETE]=QPixmap((const char**)track_delete_xpm);
	pixmap_map[ICON_TRACK_EDIT]=QPixmap((const char**)track_edit_xpm);
	pixmap_map[ICON_AUTOMATION_MOVE_LEFT]=QPixmap((const char**)track_move_automation_left_xpm);
	pixmap_map[ICON_AUTOMATION_MOVE_RIGHT]=QPixmap((const char**)track_move_automation_right_xpm);
	pixmap_map[ICON_COLUMN_ADD]=QPixmap((const char**)track_column_add_xpm);
	pixmap_map[ICON_COLUMN_REMOVE]=QPixmap((const char**)track_column_rem_xpm);
	pixmap_map[ICON_ZOOM_SMALL]=QPixmap((const char**)zoom_xpm);
			
	pixmap_map[ICON_GLOBAL_VIEW_SELECT_BLOCK]=QPixmap((const char**)global_edit_select_block_xpm);
	pixmap_map[ICON_GLOBAL_VIEW_ADD_BLOCK]=QPixmap((const char**)global_edit_add_block_xpm);
	pixmap_map[ICON_GLOBAL_VIEW_COPY_BLOCK]=QPixmap((const char**)global_edit_copy_block_xpm);
	pixmap_map[ICON_GLOBAL_VIEW_COPY_LINK_BLOCK]=QPixmap((const char**)global_edit_copy_block_linked_xpm);
	pixmap_map[ICON_GLOBAL_VIEW_ERASE_BLOCK]=QPixmap((const char**)global_edit_delete_block_xpm);
	
	pixmap_map[ICON_CURSOR_ADD_BLOCK]=QPixmap((const char**)cursor_add_block_xpm);
	
	pattern_font.set_font( QPixmap((const char**)pattern_font_xpm), "ABCDEFG#0123456789-*");
	pattern_vol_font.set_font( QPixmap((const char**)pattern_vol_font_xpm), "ABCDEFG#0123456789-*"); 

	rowcount_font.set_font( QPixmap((const char**)rowcount_font_xpm), "ABCDEFG#0123456789-*"); 
	rowcount_subbeat_font.set_font( QPixmap((const char**)rowcount_font_subbeat_xpm), "ABCDEFG#0123456789-*"); 
	global_bar_font.set_font( QPixmap((const char**)font_global_bar_xpm), "0123456789BEATR"); 
	global_beat_font.set_font( QPixmap((const char**)font_global_beat_xpm), "0123456789BEATR"); 
	editing_row_height=pattern_font.get_height()+5;
	pattern_cursor=QPixmap(pattern_cursor_xpm);
	pattern_cursor_offset=QPoint(-3,-3); 



}


VisualSettings::~VisualSettings()
{
}


}
  