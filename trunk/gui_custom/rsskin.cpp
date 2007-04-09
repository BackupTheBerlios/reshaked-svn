

//
// C++ Implementation: rsskin
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "rsskin.h"
#include "pixmap_data.h"

namespace ReShaked {

#define SET_STYLEBOX( m_which , m_style )\
set_stylebox_name(m_which,#m_which);\
set_stylebox(m_which,m_style);

#define SET_CONSTANT( m_which , m_constant )\
set_constant_name(m_which,#m_which);\
set_constant(m_which,m_constant);

#define SET_FONT( m_which , m_font )\
set_font_name(m_which,#m_which);\
set_font(m_which,m_font);

#define SET_BITMAP( m_which , m_bitmap )\
set_bitmap_name(m_which,#m_which);\
set_bitmap(m_which,m_bitmap);

#define SET_COLOR( m_which , m_color )\
set_color_name(m_which,#m_which);\
set_color(m_which,m_color);
	
	
void RSSkin::set_default_extra() {
		
	/** MAIN WINDOW **/
	SET_BITMAP(BITMAP_CONTROL_FF,PixmapData::get_pixmap(                PIXMAP_CONTROL_FF));
	SET_BITMAP(BITMAP_CONTROL_LOOP,PixmapData::get_pixmap(              	PIXMAP_CONTROL_LOOP));
	SET_BITMAP(BITMAP_CONTROL_PAUSE,PixmapData::get_pixmap(             	PIXMAP_CONTROL_PAUSE));
	SET_BITMAP(BITMAP_CONTROL_PLAY,PixmapData::get_pixmap(              	PIXMAP_CONTROL_PLAY));
	SET_BITMAP(BITMAP_CONTROL_REC,PixmapData::get_pixmap(               	PIXMAP_CONTROL_REC));
	SET_BITMAP(BITMAP_CONTROL_REC_AUTO,PixmapData::get_pixmap(          	PIXMAP_CONTROL_REC_AUTO));
	SET_BITMAP(BITMAP_CONTROL_RW,PixmapData::get_pixmap(                	PIXMAP_CONTROL_RW));
	SET_BITMAP(BITMAP_CONTROL_STOP,PixmapData::get_pixmap(              	PIXMAP_CONTROL_STOP));
	SET_BITMAP(BITMAP_ICON_AUDIO,PixmapData::get_pixmap(                	PIXMAP_ICON_AUDIO));
	SET_BITMAP(BITMAP_ICON_CURSOR_STEP,PixmapData::get_pixmap(          	PIXMAP_ICON_CURSOR_STEP));
	SET_BITMAP(BITMAP_ICON_DELETE,PixmapData::get_pixmap(               	PIXMAP_ICON_DELETE));
	SET_BITMAP(BITMAP_ICON_EDIT,PixmapData::get_pixmap(                 	PIXMAP_ICON_EDIT));
	SET_BITMAP(BITMAP_ICON_INFO,PixmapData::get_pixmap(                 	PIXMAP_ICON_INFO));
	SET_BITMAP(BITMAP_ICON_PLAY_CURRENT_BLOCK,PixmapData::get_pixmap(PIXMAP_ICON_PLAY_CURRENT_BLOCK));
	SET_BITMAP(BITMAP_ICON_PLAY_FROM_CURSOR,PixmapData::get_pixmap(PIXMAP_ICON_PLAY_FROM_CURSOR));
	SET_BITMAP(BITMAP_ICON_REDO,PixmapData::get_pixmap(PIXMAP_ICON_REDO));
	SET_BITMAP(BITMAP_ICON_SWING,PixmapData::get_pixmap(PIXMAP_ICON_SWING));
	SET_BITMAP(BITMAP_ICON_TEMPO,PixmapData::get_pixmap(PIXMAP_ICON_TEMPO));
	SET_BITMAP(BITMAP_ICON_TIME,PixmapData::get_pixmap(PIXMAP_ICON_TIME));
	SET_BITMAP(BITMAP_ICON_UNDO,PixmapData::get_pixmap(PIXMAP_ICON_UNDO));
	/* GLOBAL VIEW */
	
	SET_FONT(FONT_GLOBAL_VIEW,0);
	SET_COLOR(COLOR_GLOBAL_VIEW_FONT,Color(111,111,111));
	SET_COLOR(COLOR_GLOBAL_VIEW_BAR_LINE,Color(80,120,140));
	SET_COLOR(COLOR_GLOBAL_VIEW_BEAT_LINE,Color(50,60,80));
	SET_COLOR(COLOR_GLOBAL_VIEW_SEPARATOR,Color(90,90,90));
	SET_COLOR(COLOR_GLOBAL_VIEW_AUTOMATION,Color(0));
	SET_COLOR(COLOR_GLOBAL_VIEW_PATTERN_NOTE,Color(0));
	
	SET_STYLEBOX(SB_GLOBAL_VIEW_PATTERN,StyleBox(2,Color(0x81,0xAB,0xB3),Color(0xEE,0xFF,0xEE),Color(0xDE,0xEF,0xDE)));
	SET_STYLEBOX(SB_GLOBAL_VIEW_PATTERN_SELECTED,StyleBox(2,Color(0xB3,0x88,0x81),Color(0xFF,0xEE,0xEE),Color(0xEF,0xDE,0xDE)));
	SET_STYLEBOX(SB_GLOBAL_VIEW_AUTOMATION,StyleBox(2,Color(0xAC,0xB3,0x81),Color(0xEE,0xFF,0xFF),Color(0xDE,0xEE,0xEE)));
	SET_STYLEBOX(SB_GLOBAL_VIEW_AUTOMATION_SELECTED,StyleBox(2,Color(0xB3,0x88,0x81),Color(0xFF,0xEE,0xEE),Color(0xEF,0xDE,0xDE)));
	SET_STYLEBOX(SB_GLOBAL_VIEW_REPEAT,StyleBox(2,Color(0xCC),Color(0xFF),Color(0xEE)));
 	SET_STYLEBOX(SB_GLOBAL_VIEW_LINKED,StyleBox(2,Color(0xAA),Color(0xFF),Color(0xFF)));
  	SET_STYLEBOX(SB_GLOBAL_VIEW_FORBIDDEN,StyleBox(2,Color(0xAA,0xEE,0X44),Color(0xFF),Color(0xFF)));
	   	
	/* Global View ToolBar */

	SET_BITMAP(BITMAP_GLOBAL_TOOLBAR__SELECT,PixmapData::get_pixmap(PIXMAP_ICON_SELECT));
	SET_BITMAP(BITMAP_GLOBAL_TOOLBAR__ADD,PixmapData::get_pixmap(PIXMAP_ICON_EDIT));
	SET_BITMAP(BITMAP_GLOBAL_TOOLBAR__COPY,PixmapData::get_pixmap(PIXMAP_ICON_COPY));
	SET_BITMAP(BITMAP_GLOBAL_TOOLBAR__COPY_LINK,PixmapData::get_pixmap(PIXMAP_ICON_COPY_LINK));
	SET_BITMAP(BITMAP_GLOBAL_TOOLBAR__DELETE_SELECTED,PixmapData::get_pixmap(PIXMAP_ICON_DELETE));
	SET_BITMAP(BITMAP_GLOBAL_TOOLBAR__SELECT_LINKED,PixmapData::get_pixmap(PIXMAP_ICON_SHOW_LINKS));
	SET_BITMAP(BITMAP_GLOBAL_TOOLBAR__UNLINK_SELECTED,PixmapData::get_pixmap(PIXMAP_ICON_UNLINK));
	SET_BITMAP(BITMAP_GLOBAL_TOOLBAR__ZOOM,PixmapData::get_pixmap(PIXMAP_ICON_ZOOM));
		
	
}
	
RSSkin::RSSkin() : Skin(RS_SB_MAX,RS_C_MAX,RS_BITMAP_MAX,RS_FONT_MAX,RS_COLOR_MAX) {
	
	
	set_default_extra();
}


RSSkin::~RSSkin()
{
}


}
