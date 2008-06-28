

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
#include "common_skin.h"



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
	
	
	
void CommonSkin::set_default_extra() {
		
	/** MAIN WINDOW **/

	SET_BITMAP(BITMAP_ICON_FILE_DISK,PixmapData::get_pixmap(	PIXMAP_ICON_FILE_DISK));
	SET_BITMAP(BITMAP_ICON_HELP,PixmapData::get_pixmap(	PIXMAP_ICON_HELP));
	SET_BITMAP(BITMAP_ICON_UNDO,PixmapData::get_pixmap(	PIXMAP_ICON_UNDO));
	SET_BITMAP(BITMAP_ICON_REDO,PixmapData::get_pixmap(	PIXMAP_ICON_REDO));
	SET_BITMAP(BITMAP_ICON_ADD,PixmapData::get_pixmap(	PIXMAP_ICON_ADD));
	SET_BITMAP(BITMAP_ICON_INFO,PixmapData::get_pixmap(	PIXMAP_ICON_INFO));
	SET_BITMAP(BITMAP_ICON_TIME,PixmapData::get_pixmap(	PIXMAP_ICON_TIME));
	
	SET_BITMAP(BITMAP_CONTROL_PLAY,PixmapData::get_pixmap(	PIXMAP_CONTROL_PLAY));
	SET_BITMAP(BITMAP_CONTROL_LOOP,PixmapData::get_pixmap(	PIXMAP_CONTROL_LOOP));
	SET_BITMAP(BITMAP_CONTROL_PAUSE,PixmapData::get_pixmap(	PIXMAP_CONTROL_PAUSE));
	SET_BITMAP(BITMAP_CONTROL_STOP,PixmapData::get_pixmap(	PIXMAP_CONTROL_STOP));
	SET_BITMAP(BITMAP_CONTROL_FF,PixmapData::get_pixmap(	PIXMAP_CONTROL_FF));
	SET_BITMAP(BITMAP_CONTROL_RW,PixmapData::get_pixmap(	PIXMAP_CONTROL_RW));
	SET_BITMAP(BITMAP_CONTROL_REC,PixmapData::get_pixmap(	PIXMAP_CONTROL_REC));
	SET_BITMAP(BITMAP_CONTROL_REC_AUTO,PixmapData::get_pixmap(	PIXMAP_CONTROL_REC_AUTO));
	
	

	/* GLOBAL VIEW */
	
	
}
	
	
GUI::FontID CommonSkin::load_number_font(PixmapDataList p_pixmap) {
		
	
	int font_h=painter->get_bitmap_size(PixmapData::get_pixmap(p_pixmap)).height-2;
	int font_w=painter->get_bitmap_size(PixmapData::get_pixmap(p_pixmap)).width-11;
	font_w/=10;
	
	GUI::FontID font = painter->create_font(font_h,font_h);
	
	for (int i=0;i<10;i++) {
		
		painter->font_add_char(font,'0'+i,PixmapData::get_pixmap(p_pixmap),GUI::Rect(GUI::Point(1+i*(font_w+1),1),GUI::Size(font_w,font_h)));
	}	
	
	return font;
}
	
CommonSkin::CommonSkin(GUI::Painter *p_painter) : Skin(COMMON_SB_MAX,COMMON_C_MAX,COMMON_BITMAP_MAX,COMMON_FONT_MAX,COMMON_COLOR_MAX) {
	
	painter=p_painter;
	
//	bar_font=load_number_font(PIXMAP_GLOBAL_VIEW_BAR_FONT);
//	beat_font=load_number_font(PIXMAP_GLOBAL_VIEW_BEAT_FONT);
	
	set_default_extra();
}


CommonSkin::~CommonSkin()
{
}



