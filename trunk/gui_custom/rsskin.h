//
// C++ Interface: rsskin
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDRSSKIN_H
#define RESHAKEDRSSKIN_H

#include "base/skin.h"

using namespace GUI;

namespace ReShaked {
enum StyleBoxList{
	
	SB_GLOBAL_VIEW_PATTERN=SB_MAX,
	SB_GLOBAL_VIEW_PATTERN_SELECTED,
	SB_GLOBAL_VIEW_AUTOMATION,
	SB_GLOBAL_VIEW_AUTOMATION_SELECTED,
	SB_GLOBAL_VIEW_REPEAT,
 	SB_GLOBAL_VIEW_LINKED,
  	SB_GLOBAL_VIEW_FORBIDDEN,
		
	RS_SB_MAX
};
	
enum ColorList {
		
	COLOR_GLOBAL_VIEW_FONT=COLOR_MAX,
 	COLOR_GLOBAL_VIEW_BG,
	COLOR_GLOBAL_VIEW_BAR_LINE,
	COLOR_GLOBAL_VIEW_BEAT_LINE,
	COLOR_GLOBAL_VIEW_SEPARATOR,
	COLOR_GLOBAL_VIEW_AUTOMATION,
	COLOR_GLOBAL_VIEW_PATTERN_NOTE,
 	COLOR_GLOBAL_VIEW_BAR_FONT,
 	COLOR_GLOBAL_VIEW_BEAT_FONT,
	COLOR_GLOBAL_VIEW_LOOP,
 	COLOR_GLOBAL_VIEW_MOUSE_OVER_BEAT,
  	COLOR_GLOBAL_VIEW_BARBEAT_CHANGE,
   	COLOR_GLOBAL_VIEW_LOOP_MARKER,
	
	RS_COLOR_MAX
};

enum ConstantList {

	C_GLOBAL_VIEW_LOOP_COLUMN_WIDTH=C_MAX,
	RS_C_MAX
};
	
enum FontList {
	FONT_GLOBAL_VIEW=FONT_MAX,
 	FONT_GLOBAL_VIEW_BAR,
 	FONT_GLOBAL_VIEW_BEAT,
	RS_FONT_MAX
};

enum BitmapList {
	BITMAP_CONTROL_FF=BITMAP_MAX,
	BITMAP_CONTROL_LOOP,
	BITMAP_CONTROL_PAUSE,
	BITMAP_CONTROL_PLAY,
	BITMAP_CONTROL_REC,
	BITMAP_CONTROL_REC_AUTO,
	BITMAP_CONTROL_RW,
	BITMAP_CONTROL_STOP,
	BITMAP_ICON_AUDIO,
	BITMAP_ICON_CURSOR_STEP,
	BITMAP_ICON_DELETE,
	BITMAP_ICON_EDIT,
	BITMAP_ICON_INFO,
	BITMAP_ICON_PLAY_CURRENT_BLOCK,
	BITMAP_ICON_PLAY_FROM_CURSOR,
	BITMAP_ICON_REDO,
	BITMAP_ICON_SWING,
	BITMAP_ICON_TEMPO,
	BITMAP_ICON_TIME,
	BITMAP_ICON_UNDO,
	BITMAP_GLOBAL_TOOLBAR__SELECT,
	BITMAP_GLOBAL_TOOLBAR__ADD,
	BITMAP_GLOBAL_TOOLBAR__COPY,
	BITMAP_GLOBAL_TOOLBAR__COPY_LINK,
	BITMAP_GLOBAL_TOOLBAR__DELETE_SELECTED,
	BITMAP_GLOBAL_TOOLBAR__SELECT_LINKED,
	BITMAP_GLOBAL_TOOLBAR__UNLINK_SELECTED,
	BITMAP_GLOBAL_TOOLBAR__ZOOM,
 	
	RS_BITMAP_MAX
};

/**
	@author Juan Linietsky <reshaked@gmail.com>
*/
class RSSkin : public Skin {
	
	void set_default_extra();
public:
	
	
    RSSkin();
    ~RSSkin();

};

}

#endif
