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
#include "pixmaps/pattern_bg.xpm"
#include "pixmaps/automation_bg.xpm"

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
SkinBox * VisualSettings::get_skin_box(SkinBoxList p_which) {
	
	SkinBoxMap::iterator I=skin_box_map.find(p_which);
	if (I==skin_box_map.end())
		return NULL;
	
	return I->second;
	
}

VisualSettings::VisualSettings() {
	
	singleton=this;
	
	skin_box_map[SKINBOX_GLOBALVIEW_PATTERN]=new SkinBox((const char**)pattern_glob_bg_xpm,2,2,2,2);
	skin_box_map[SKINBOX_GLOBALVIEW_AUTOMATION]=new SkinBox((const char**)automation_glob_bg_xpm,2,2,2,2);
	skin_box_map[SKINBOX_GLOBALVIEW_PATTERN_SELECTED]=new SkinBox((const char**)pattern_glob_bg_sel_xpm,2,2,2,2);
	skin_box_map[SKINBOX_GLOBALVIEW_AUTOMATION_SELECTED]=new SkinBox((const char**)automation_glob_bg_sel_xpm,2,2,2,2);
	skin_box_map[SKINBOX_EDITING_PATTERN]=new SkinBox((const char**)pattern_bg_xpm,9,9,9,9);
	skin_box_map[SKINBOX_EDITING_AUTOMATION]=new SkinBox((const char**)automation_bg_xpm,9,9,9,9);
	
	pattern_font.set_font( QPixmap((const char**)pattern_font_xpm), "ABCDEFG#01234567890-*"); //DAMN, ADDED ZERO TWICE!
	editing_row_height=pattern_font.get_height()+8;
}


VisualSettings::~VisualSettings()
{
}


}
