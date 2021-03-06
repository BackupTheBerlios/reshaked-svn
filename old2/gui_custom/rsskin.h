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
#include "base/painter.h"
#include "pixmap_data.h"

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
   
   	SB_EDIT_VIEW_CURSOR,
		
    	SB_AUTOMATION,
     	SB_AUTOMATION_SHARED,
      
	SB_PATTERN,
     	SB_PATTERN_SHARED,
      
	RS_SB_MAX
};
	
enum ColorList {
		
	COLOR_GLOBAL_VIEW_FONT=COLOR_MAX,
 	COLOR_GLOBAL_VIEW_FONT_SHADOW,
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
    	COLOR_GLOBAL_VIEW_MARKER_FONT,
     	COLOR_AUTOMATION_FONT_SHADOW,
     	COLOR_AUTOMATION_FONT,
      	COLOR_AUTOMATION_BEAT_LINE,
      	COLOR_AUTOMATION_BAR_LINE,
      	COLOR_AUTOMATION_SUBBEAT_LINE,
       	COLOR_AUTOMATION_BG,
      	COLOR_AUTOMATION_ENVELOPE,
      	COLOR_AUTOMATION_ENVELOPE_REPEAT,
      	COLOR_AUTOMATION_SELECTION,
       	COLOR_PATTERN_FONT,
       	COLOR_PATTERN_FONT_REPEAT,
       	COLOR_PATTERN_FONT_VOLUME,
       	COLOR_PATTERN_FONT_NOFIT,
	COLOR_PATTERN_VOLUME_BG,
	COLOR_PATTERN_NOTE_BG,
	COLOR_PATTERN_VOLUME_BAR,
	COLOR_PATTERN_NOTE_BAR,
 	COLOR_PATTERN_SUBBEAT_LINE,
 	COLOR_PATTERN_BEAT_LINE,
 	COLOR_PATTERN_BAR_LINE,
  	COLOR_PATTERN_SELECTION,
   	COLOR_PATTERN_BG,
	COLOR_EDIT_VIEW_ROW_BAR_FONT,
	COLOR_EDIT_VIEW_ROW_BEAT_FONT,
	COLOR_EDIT_VIEW_ROW_SUBBEAT_FONT,
 	COLOR_EDIT_VIEW_MARKER_FONT,
 	COLOR_EDITOR_BG,
       	COLOR_EDIT_VIEW_SIDE_WIDGET_AUTOMATION,
       	COLOR_EDIT_VIEW_SIDE_WIDGET_PATTERN,
  	COLOR_PLUGIN_CHOOSER_FONT_NAME,
  	COLOR_PLUGIN_CHOOSER_FONT_DESCRIPTION,
  	COLOR_PLUGIN_CHOOSER_BG,
  	COLOR_PLUGIN_CHOOSER_BG_SELECTED,
  	COLOR_PLUGIN_CHOOSER_SEPARATOR,
  	COLOR_PLUGIN_CHOOSER_SECTION,
	
	RS_COLOR_MAX
};

enum ConstantList {

	C_GLOBAL_VIEW_LOOP_COLUMN_WIDTH=C_MAX,
 	C_GLOBAL_VIEW_MARKER_COLUMN_MARGIN,
  	C_AUTOMATION_MARGIN_WIDTH,
   	C_AUTOMATION_SIZE_SMALL,
   	C_AUTOMATION_SIZE_MEDIUM,
   	C_AUTOMATION_SIZE_BIG,
    	C_AUTOMATION_SELECTION_MARGIN,
     	C_PATTERN_VOL_NOTE_BAR_HEIGHT,
    	C_PATTERN_SELECTION_MARGIN,
     	C_EDITOR_ROW_MARGIN,
     	C_EDIT_VIEW_SIDE_WIDGET_MARGIN,
	C_PLUGIN_CHOOSER_ITEM_MARGIN,
	C_PLUGIN_CHOOSER_ITEM_INTERNAL_SEPARATION,
	C_PLUGIN_CHOOSER_ITEM_DESCRIPTION_MARGIN,
	RS_C_MAX
};
	
enum FontList {
	FONT_GLOBAL_VIEW=FONT_MAX,
	FONT_GLOBAL_VIEW_BAR,
	FONT_GLOBAL_VIEW_BEAT,
	FONT_GLOBAL_VIEW_MARKER,
	FONT_AUTOMATION_VALUE,
	FONT_PATTERN,
	FONT_EDIT_VIEW_SIDE_WIDGET,
	FONT_EDIT_VIEW_MARKER_FONT,
	FONT_PLUGIN_CHOOSER_SECTION,
	FONT_PLUGIN_CHOOSER_NAME,
	FONT_PLUGIN_CHOOSER_DESCRIPTION,
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
	BITMAP_ICON_MUTE,
 	BITMAP_ICON_SOLO,
 	BITMAP_ICON_COLUMN_ADD,
 	BITMAP_ICON_COLUMN_REMOVE,
 	BITMAP_ICON_AUTOMATION,
 	BITMAP_ICON_LEFT,
 	BITMAP_ICON_RIGHT,
 	BITMAP_ICON_RENAME,
 	BITMAP_ICON_REMOVE,
 	BITMAP_ICON_ADD,
 	BITMAP_ICON_FILE_DISK,
 	BITMAP_ICON_FILE_FOLDER,
	BITMAP_GLOBAL_TOOLBAR__SELECT,
	BITMAP_GLOBAL_TOOLBAR__ADD,
	BITMAP_GLOBAL_TOOLBAR__COPY,
	BITMAP_GLOBAL_TOOLBAR__COPY_LINK,
	BITMAP_GLOBAL_TOOLBAR__DELETE_SELECTED,
	BITMAP_GLOBAL_TOOLBAR__SELECT_LINKED,
	BITMAP_GLOBAL_TOOLBAR__UNLINK_SELECTED,
	BITMAP_GLOBAL_TOOLBAR__ZOOM,
	BITMAP_AUTOMATION_POINT,
 	BITMAP_EDIT_VIEW_SIDE_WIDGET_ARROW_PATTERN,
 	BITMAP_EDIT_VIEW_SIDE_WIDGET_ARROW_AUTOMATION,
	RS_BITMAP_MAX
};

/**
	@author Juan Linietsky <reshaked@gmail.com>
*/
class RSSkin : public Skin {
	
	Painter *painter;
	void set_default_extra();
	
	FontID bar_font;
	FontID beat_font;
	
	FontID load_number_font(PixmapDataList p_bitmap);
public:
	
	
    RSSkin(Painter *p_painter);
    ~RSSkin();

};

}

#endif
