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
#include "pixmaps/pattern_bg.xpm"
#include "pixmaps/automation_bg.xpm"
#include "pixmaps/pattern_bg_sel.xpm"
#include "pixmaps/pattern_cursor.xpm"
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
	skin_box_map[SKINBOX_EDITING_PATTERN]=new SkinBox((const char**)pattern_bg_xpm,9,9,9,9);
	skin_box_map[SKINBOX_EDITING_PATTERN_SELECTED]=new SkinBox((const char**)pattern_bg_sel_xpm,9,9,9,9);
	skin_box_map[SKINBOX_EDITING_AUTOMATION]=new SkinBox((const char**)automation_bg_xpm,9,9,9,9);
	
	
	color_map[COLORLIST_PATTERN_EDIT_NOTE]=QColor(0x9d,0xda,0x82);
	color_map[COLORLIST_PATTERN_EDIT_VOL]=QColor(0xda,0x9d,0x82);
	color_map[COLORLIST_PATTERN_EDIT_FOCUS_RECT]=QColor(255,0,0);

	
	pattern_font.set_font( QPixmap((const char**)pattern_font_xpm), "ABCDEFG#01234567890-*"); //DAMN, ADDED ZERO TWICE!
	pattern_vol_font.set_font( QPixmap((const char**)pattern_vol_font_xpm), "ABCDEFG#01234567890-*"); //DAMN, ADDED ZERO TWICE!
	editing_row_height=pattern_font.get_height()+8;
	pattern_cursor=QPixmap(pattern_cursor_xpm);
	pattern_cursor_offset=QPoint(-3,-3);
	
	
}


VisualSettings::~VisualSettings()
{
}


}
