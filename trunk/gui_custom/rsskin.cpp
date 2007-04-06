

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
		
	SET_BITMAP(BITMAP_CONTROL_FF,PixmapData::get_pixmap(                PIXMAP_CONTROL_FF));
	SET_BITMAP(BITMAP_CONTROL_LOOP,PixmapData::get_pixmap(              	PIXMAP_CONTROL_LOOP));
	SET_BITMAP(BITMAP_CONTROL_PAUSE,PixmapData::get_pixmap(             	PIXMAP_CONTROL_PAUSE));
	SET_BITMAP(BITMAP_CONTROL_PLAY,PixmapData::get_pixmap(              	PIXMAP_CONTROL_PLAY));
	SET_BITMAP(BITMAP_CONTROL_REC,PixmapData::get_pixmap(               	PIXMAP_CONTROL_REC));
	SET_BITMAP(BITMAP_CONTROL_REC_AUTO,PixmapData::get_pixmap(          	PIXMAP_CONTROL_REC_AUTO));
	SET_BITMAP(BITMAP_CONTROL_RW,PixmapData::get_pixmap(                	PIXMAP_CONTROL_RW));
	SET_BITMAP(BITMAP_CONTROL_STOP,PixmapData::get_pixmap(              	PIXMAP_CONTROL_STOP));
	SET_BITMAP(BITMAP_ICON_AUDIO,PixmapData::get_pixmap(                	PIXMAP_ICON_AUDIO));
	SET_BITMAP(BITMAP_ICON_BLOCK_REPEAT,PixmapData::get_pixmap(         	PIXMAP_ICON_BLOCK_REPEAT));
	SET_BITMAP(BITMAP_ICON_BLOCK_UNREPEAT,PixmapData::get_pixmap(       	PIXMAP_ICON_BLOCK_UNREPEAT));
	SET_BITMAP(BITMAP_ICON_COPY,PixmapData::get_pixmap(                 	PIXMAP_ICON_COPY));
	SET_BITMAP(BITMAP_ICON_COPY_LINK,PixmapData::get_pixmap(            	PIXMAP_ICON_COPY_LINK));
	SET_BITMAP(BITMAP_ICON_CURSOR_STEP,PixmapData::get_pixmap(          	PIXMAP_ICON_CURSOR_STEP));
	SET_BITMAP(BITMAP_ICON_DELETE,PixmapData::get_pixmap(               	PIXMAP_ICON_DELETE));
	SET_BITMAP(BITMAP_ICON_EDIT,PixmapData::get_pixmap(                 	PIXMAP_ICON_EDIT));
	SET_BITMAP(BITMAP_ICON_INFO,PixmapData::get_pixmap(                 	PIXMAP_ICON_INFO));
	SET_BITMAP(BITMAP_ICON_PLAY_CURRENT_BLOCK,PixmapData::get_pixmap(PIXMAP_ICON_PLAY_CURRENT_BLOCK));
	SET_BITMAP(BITMAP_ICON_PLAY_FROM_CURSOR,PixmapData::get_pixmap(PIXMAP_ICON_PLAY_FROM_CURSOR));
	SET_BITMAP(BITMAP_ICON_REDO,PixmapData::get_pixmap(PIXMAP_ICON_REDO));
	SET_BITMAP(BITMAP_ICON_SELECT,PixmapData::get_pixmap(PIXMAP_ICON_SELECT));
	SET_BITMAP(BITMAP_ICON_SHOW_LINKS,PixmapData::get_pixmap(PIXMAP_ICON_SHOW_LINKS));
	SET_BITMAP(BITMAP_ICON_SWING,PixmapData::get_pixmap(PIXMAP_ICON_SWING));
	SET_BITMAP(BITMAP_ICON_TEMPO,PixmapData::get_pixmap(PIXMAP_ICON_TEMPO));
	SET_BITMAP(BITMAP_ICON_TIME,PixmapData::get_pixmap(PIXMAP_ICON_TIME));
	SET_BITMAP(BITMAP_ICON_UNDO,PixmapData::get_pixmap(PIXMAP_ICON_UNDO));
	SET_BITMAP(BITMAP_ICON_UNLINK,PixmapData::get_pixmap(PIXMAP_ICON_UNLINK));
	SET_BITMAP(BITMAP_ICON_ZOOM,PixmapData::get_pixmap(PIXMAP_ICON_ZOOM));		   
		   	
	
	
}
	
RSSkin::RSSkin() : Skin(RS_SB_MAX,RS_C_MAX,RS_BITMAP_MAX,RS_FONT_MAX,RS_COLOR_MAX) {
	
	
	set_default_extra();
}


RSSkin::~RSSkin()
{
}


}
