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

#define MAX_LAYER_COLORS 16

enum CommonStyleBoxList{
	
	COMMON_SB_FIRST=GUI::SB_MAX,
	
	COMMON_SB_MAX
};
	
enum CommonColorList {
		
	COMMON_COLOR_FIRST=GUI::COLOR_MAX,
	COMMON_COLOR_MAX
};

enum CommonConstantList {

	COMMON_C_FIRST=GUI::C_MAX,
	COMMON_C_MAX

};
	
enum CommonFontList {
	COMMON_FONT_FIRST=GUI::FONT_MAX,
	COMMON_FONT_MAX
};

enum CommonBitmapList {

	COMMON_BITMAP_FIRST=GUI::BITMAP_MAX,
	
	BITMAP_ICON_AUDIO,
	BITMAP_ICON_FILE_DISK,
	BITMAP_ICON_HELP,
	BITMAP_ICON_UNDO,
	BITMAP_ICON_REDO,
	BITMAP_ICON_ADD,
	BITMAP_ICON_INFO,
	BITMAP_ICON_TIME,
		
	BITMAP_CONTROL_PLAY,
	BITMAP_CONTROL_LOOP,
	BITMAP_CONTROL_PAUSE,
	BITMAP_CONTROL_STOP,
	BITMAP_CONTROL_FF,
	BITMAP_CONTROL_RW,
	BITMAP_CONTROL_REC,
	BITMAP_CONTROL_REC_AUTO,
	
	COMMON_BITMAP_MAX
};

/**
	@author Juan Linietsky <reshaked@gmail.com>
*/
class CommonSkin : public GUI::Skin {
	
	GUI::Painter *painter;
	void set_default_extra();
	
	GUI::FontID bar_font;
	GUI::FontID beat_font;
	
	GUI::FontID load_number_font(PixmapDataList p_bitmap);
public:
	
	
    CommonSkin(GUI::Painter *p_painter);
    ~CommonSkin();

};



#endif
