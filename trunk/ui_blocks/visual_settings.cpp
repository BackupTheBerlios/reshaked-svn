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
#include "pixmaps/pattern_bg.xpm"
#include "pixmaps/automation_bg.xpm"
#include "pixmaps/automation_bg_sel.xpm"
#include "pixmaps/pattern_bg_sel.xpm"
#include "pixmaps/pattern_cursor.xpm"
#include "pixmaps/track_options.xpm"
#include "pixmaps/knob_base.xpm"
#include "pixmaps/knob_handle.xpm"
#include "pixmaps/track_settings_pattern.xpm"
#include "pixmaps/track_settings_controls.xpm"
#include "pixmaps/track_settings_effects.xpm"
#include "pixmaps/track_settings_connections.xpm"
#include "pixmaps/envelope_point.xpm"

#include "pixmaps/track_move_left.xpm"
#include "pixmaps/track_move_right.xpm"
#include "pixmaps/track_delete.xpm"
#include "pixmaps/track_edit.xpm"
#include "pixmaps/track_move_automation_left.xpm"
#include "pixmaps/track_move_automation_right.xpm"
#include "pixmaps/track_column_add.xpm"
#include "pixmaps/track_column_rem.xpm"
#include "pixmaps/zoom.xpm"


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
PixmapFont* VisualSettings::get_pattern_font() {

	return &pattern_font;
}
PixmapFont* VisualSettings::get_pattern_vol_font() {

	return &pattern_vol_font;
}

PixmapFont* VisualSettings::get_rowcount_font() {

	return &rowcount_font;
}

SkinBox * VisualSettings::get_skin_box(SkinBoxList p_which) {

	SkinBoxMap::iterator I=skin_box_map.find(p_which);

	ERR_FAIL_COND_V( I==skin_box_map.end() , NULL );

	return I->second;

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

	skin_box_map[SKINBOX_GLOBALVIEW_PATTERN]=new SkinBox((const char**)pattern_glob_bg_xpm,2,2,2,2);
	skin_box_map[SKINBOX_GLOBALVIEW_AUTOMATION]=new SkinBox((const char**)automation_glob_bg_xpm,2,2,2,2);
	skin_box_map[SKINBOX_GLOBALVIEW_PATTERN_SELECTED]=new SkinBox((const char**)pattern_glob_bg_sel_xpm,2,2,2,2);
	skin_box_map[SKINBOX_GLOBALVIEW_AUTOMATION_SELECTED]=new SkinBox((const char**)automation_glob_bg_sel_xpm,2,2,2,2);
	skin_box_map[SKINBOX_EDITING_PATTERN]=new SkinBox((const char**)pattern_bg_xpm,4,4,4,4);
	skin_box_map[SKINBOX_EDITING_PATTERN_SELECTED]=new SkinBox((const char**)pattern_bg_sel_xpm,4,4,4,4);
	skin_box_map[SKINBOX_EDITING_AUTOMATION]=new SkinBox((const char**)automation_bg_xpm,4,4,4,4);
	skin_box_map[SKINBOX_EDITING_AUTOMATION_SELECTED]=new SkinBox((const char**)automation_bg_sel_xpm,4,4,4,4);


	color_map[COLORLIST_PATTERN_EDIT_NOTE]=QColor(0xca,0xfd,0xc0);
	color_map[COLORLIST_PATTERN_EDIT_VOL]=QColor(0xda,0x9d,0x82);
	color_map[COLORLIST_PATTERN_EDIT_FOCUS_RECT]=QColor(255,0,0);
	color_map[COLORLIST_PATTERN_EDIT_BEAT_LINE]=QColor(200,255,200,128);
	color_map[COLORLIST_PATTERN_EDIT_SUBBEAT_LINE]=QColor(150,200,150,64);
	color_map[COLORLIST_AUTOMATION_ENVELOPE]=QColor(255,255,200,200);
	
	pixmap_map[PIXMAP_TRACK_OPTIONS]=QPixmap((const char**)track_options_xpm);
	pixmap_map[PIXMAP_KNOB_BASE]=QPixmap((const char**)knob_base_xpm);
	pixmap_map[PIXMAP_KNOB_HANDLE]=QPixmap((const char**)knob_handle_xpm);
	pixmap_map[PIXMAP_TRACK_SETTINGS_PATTERN]=QPixmap((const char**)track_settings_pattern_xpm);
	pixmap_map[PIXMAP_TRACK_SETTINGS_CONTROLS]=QPixmap((const char**)track_settings_controls_xpm);
	pixmap_map[PIXMAP_TRACK_SETTINGS_EFFECTS]=QPixmap((const char**)track_settings_effects_xpm);
	pixmap_map[PIXMAP_TRACK_SETTINGS_CONNECTIONS]=QPixmap((const char**)track_settings_connections_xpm);
	pixmap_map[PIXMAP_ENVELOPE_POINT]=QPixmap((const char**)envelope_point_xpm);


	pixmap_map[ICON_TRACK_MOVE_LEFT]=QPixmap((const char**)track_move_left_xpm);
	pixmap_map[ICON_TRACK_MOVE_RIGHT]=QPixmap((const char**)track_move_right_xpm);
	pixmap_map[ICON_TRACK_DELETE]=QPixmap((const char**)track_delete_xpm);
	pixmap_map[ICON_TRACK_EDIT]=QPixmap((const char**)track_edit_xpm);
	pixmap_map[ICON_AUTOMATION_MOVE_LEFT]=QPixmap((const char**)track_move_automation_left_xpm);
	pixmap_map[ICON_AUTOMATION_MOVE_RIGHT]=QPixmap((const char**)track_move_automation_right_xpm);
	pixmap_map[ICON_COLUMN_ADD]=QPixmap((const char**)track_column_add_xpm);
	pixmap_map[ICON_COLUMN_REMOVE]=QPixmap((const char**)track_column_rem_xpm);
	pixmap_map[ICON_ZOOM_SMALL]=QPixmap((const char**)zoom_xpm);
			
	pattern_font.set_font( QPixmap((const char**)pattern_font_xpm), "ABCDEFG#0123456789-*");
	pattern_vol_font.set_font( QPixmap((const char**)pattern_vol_font_xpm), "ABCDEFG#0123456789-*"); 

	rowcount_font.set_font( QPixmap((const char**)rowcount_font_xpm), "ABCDEFG#0123456789-*"); 
	editing_row_height=pattern_font.get_height()+8;
	pattern_cursor=QPixmap(pattern_cursor_xpm);
	pattern_cursor_offset=QPoint(-3,-3); 



}


VisualSettings::~VisualSettings()
{
}


}
  