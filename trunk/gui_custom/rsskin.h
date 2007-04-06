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
	
	
	RS_SB_MAX=SB_MAX
};
	
enum ColorList {
		
	RS_COLOR_MAX=COLOR_MAX
};

enum ConstantList {

	RS_C_MAX=C_MAX
};
	
enum FontList {
	
	RS_FONT_MAX=FONT_MAX
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
	BITMAP_ICON_BLOCK_REPEAT,
	BITMAP_ICON_BLOCK_UNREPEAT,
	BITMAP_ICON_COPY,
	BITMAP_ICON_COPY_LINK,
	BITMAP_ICON_CURSOR_STEP,
	BITMAP_ICON_DELETE,
	BITMAP_ICON_EDIT,
	BITMAP_ICON_INFO,
	BITMAP_ICON_PLAY_CURRENT_BLOCK,
	BITMAP_ICON_PLAY_FROM_CURSOR,
	BITMAP_ICON_REDO,
	BITMAP_ICON_SELECT,
	BITMAP_ICON_SHOW_LINKS,
	BITMAP_ICON_SWING,
	BITMAP_ICON_TEMPO,
	BITMAP_ICON_TIME,
	BITMAP_ICON_UNDO,
	BITMAP_ICON_UNLINK,
	BITMAP_ICON_ZOOM,	
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
