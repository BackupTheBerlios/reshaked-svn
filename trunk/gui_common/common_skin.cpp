

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
	
	SET_BITMAP(BITMAP_ICON_PREFERENCES,PixmapData::get_pixmap(	PIXMAP_ICON_PREFERENCES));
	
// tired of that
#define SET_BM( m_name )\
	SET_BITMAP( BITMAP_##m_name,PixmapData::get_pixmap( PIXMAP_##m_name ));
	
	SET_BM(GRAPH_JACK);
	SET_BM(GRAPH_NODE_EDIT);
	SET_BM(GRAPH_NODE_EDIT_HOVER);
	SET_BM(GRAPH_NODE_CLOSE);
	SET_BM(GRAPH_NODE_CLOSE_HOVER);
	SET_BM(GRAPH_NODE_OPTIONS);
	SET_BM(GRAPH_NODE_OPTIONS_HOVER);
	SET_BM(GRAPH_NODE_PRESET);
	SET_BM(GRAPH_NODE_RENAME);
	SET_BM(GRAPH_NODE_CONTROL_EDIT);
	SET_BM(GRAPH_NODE_SKIP);
	SET_BM(GRAPH_NODE_LAYERS);

	/* NODE CHOOSER */
	
	SET_FONT( FONT_NODE_CHOOSER_NAME, 0 );
	SET_FONT( FONT_NODE_CHOOSER_DESCRIPTION, 0 );
	SET_CONSTANT( C_NODE_CHOOSER_ITEM_MARGIN, 4 );
	SET_CONSTANT( C_NODE_CHOOSER_ITEM_INTERNAL_SEPARATION, 2 );
	SET_CONSTANT( C_NODE_CHOOSER_ITEM_DESCRIPTION_MARGIN, 2 );
	SET_COLOR( COLOR_NODE_CHOOSER_BG, GUI::Color(255) );
	SET_COLOR( COLOR_NODE_CHOOSER_BG_SELECTED,GUI::Color( 70,100,255 ) );
	SET_COLOR( COLOR_NODE_CHOOSER_FONT_NAME,GUI::Color(0) );
	SET_COLOR( COLOR_NODE_CHOOSER_FONT_DESCRIPTION,GUI::Color(100 ) );
	SET_COLOR( COLOR_NODE_CHOOSER_SEPARATOR, GUI::Color (190 ) );
	

	/* GRAPH VIEW */
	
#define MAX_LOCAL_COLORS 8
	static const GUI::Color layer_colors[MAX_LOCAL_COLORS]={
		GUI::Color(0xc9,0x5f,0x5f),
		GUI::Color(0xf0,0x96,0xd8),
		GUI::Color(0x61,0x5f,0xc9),
		GUI::Color(0x5f,0xbb,0xc9),
		GUI::Color(0x5f,0xc9,0x5f),
		GUI::Color(0xc9,0xc6,0x5f),
		GUI::Color(0xc9,0x91,0x5f),
		GUI::Color(0xaf,0x96,0xf0)
	};
	
	static const GUI::Color gray_color(0xc9,0xc9,0xc9);
	
	static const int button_pixmap_id[MAX_LOCAL_COLORS]={
	
		PixmapData::get_pixmap( PIXMAP_LAYER_BUTTON_0),
		PixmapData::get_pixmap( PIXMAP_LAYER_BUTTON_1),
		PixmapData::get_pixmap( PIXMAP_LAYER_BUTTON_2),
		PixmapData::get_pixmap( PIXMAP_LAYER_BUTTON_3),
		PixmapData::get_pixmap( PIXMAP_LAYER_BUTTON_4),
		PixmapData::get_pixmap( PIXMAP_LAYER_BUTTON_5),
		PixmapData::get_pixmap( PIXMAP_LAYER_BUTTON_6),
		PixmapData::get_pixmap( PIXMAP_LAYER_BUTTON_7)
	};
	
	static const int button_pixmap_hover_id[MAX_LOCAL_COLORS]={
	
		PixmapData::get_pixmap( PIXMAP_LAYER_BUTTON_0_HOVER),
		PixmapData::get_pixmap( PIXMAP_LAYER_BUTTON_1_HOVER),
		PixmapData::get_pixmap( PIXMAP_LAYER_BUTTON_2_HOVER),
		PixmapData::get_pixmap( PIXMAP_LAYER_BUTTON_3_HOVER),
		PixmapData::get_pixmap( PIXMAP_LAYER_BUTTON_4_HOVER),
		PixmapData::get_pixmap( PIXMAP_LAYER_BUTTON_5_HOVER),
		PixmapData::get_pixmap( PIXMAP_LAYER_BUTTON_6_HOVER),
		PixmapData::get_pixmap( PIXMAP_LAYER_BUTTON_7_HOVER)
	};
	
	static const int button_pixmap_pressed_id[MAX_LOCAL_COLORS]={
	
		PixmapData::get_pixmap( PIXMAP_LAYER_BUTTON_0_PRESSED),
		PixmapData::get_pixmap( PIXMAP_LAYER_BUTTON_1_PRESSED),
		PixmapData::get_pixmap( PIXMAP_LAYER_BUTTON_2_PRESSED),
		PixmapData::get_pixmap( PIXMAP_LAYER_BUTTON_3_PRESSED),
		PixmapData::get_pixmap( PIXMAP_LAYER_BUTTON_4_PRESSED),
		PixmapData::get_pixmap( PIXMAP_LAYER_BUTTON_5_PRESSED),
		PixmapData::get_pixmap( PIXMAP_LAYER_BUTTON_6_PRESSED),
		PixmapData::get_pixmap( PIXMAP_LAYER_BUTTON_7_PRESSED)
	};
	
#define COL( m_col ) m_col
#define COL_DARK( m_col ) GUI::Color( m_col.r/2, m_col.g/2, m_col.b/2 )
#define COL_LIGHT( m_col ) GUI::Color( (((int)m_col.r)*2 > 255?255:m_col.r*2), (((int)m_col.g)*2 > 255?255:m_col.g*2), (((int)m_col.b)*2 > 255?255:m_col.b*2) )


	for(int i=0;i<MAX_LAYER_COLORS;i++) {
	
		set_stylebox( SB_LAYER_STYLE_NODE_0+i, GUI::StyleBox( 1, COL( layer_colors[i%MAX_LOCAL_COLORS] ), COL_LIGHT( layer_colors[i%MAX_LOCAL_COLORS]), COL_LIGHT( layer_colors[i%MAX_LOCAL_COLORS] ) ) );
		set_stylebox_name( SB_LAYER_STYLE_NODE_0+i,"SB_LAYER_STYLE_NODE_"+String::num(i) );
		
		set_stylebox( SB_LAYER_STYLE_BG_0+i, GUI::StyleBox( 1, COL_DARK( layer_colors[i%MAX_LOCAL_COLORS] ), COL( layer_colors[i%MAX_LOCAL_COLORS]), COL( layer_colors[i%MAX_LOCAL_COLORS] ) ) );
		set_stylebox_name( SB_LAYER_STYLE_BG_0+i,"SB_LAYER_STYLE_BG_"+String::num(i) );
		
		set_bitmap( BITMAP_LAYER_BUTTON_0+i, button_pixmap_id[i%MAX_LOCAL_COLORS] );
		set_bitmap_name( BITMAP_LAYER_BUTTON_0+i, "BITMAP_LAYER_BUTTON_"+String::num(i) );
		
		set_bitmap( BITMAP_LAYER_BUTTON_0_HOVER+i, button_pixmap_hover_id[i%MAX_LOCAL_COLORS] );
		set_bitmap_name( BITMAP_LAYER_BUTTON_0_HOVER+i, "BITMAP_LAYER_BUTTON_"+String::num(i)+"_HOVER" );
			
		set_bitmap( BITMAP_LAYER_BUTTON_0_PRESSED+i, button_pixmap_pressed_id[i%MAX_LOCAL_COLORS] );
		set_bitmap_name( BITMAP_LAYER_BUTTON_0_PRESSED+i, "BITMAP_LAYER_BUTTON_"+String::num(i)+"_PRESSED" );
	
	}	

	SET_BM(LAYER_BUTTON_ALL);
	SET_BM(LAYER_BUTTON_ALL_HOVER);
	SET_BM(LAYER_BUTTON_ALL_PRESSED);
	SET_STYLEBOX( SB_LAYER_STYLE_NODE_ALL, GUI::StyleBox( 1, COL( gray_color ), COL_LIGHT( gray_color ), COL_LIGHT( gray_color ) ) );
	SET_STYLEBOX( SB_LAYER_STYLE_BG_ALL, GUI::StyleBox( 1, COL( gray_color ), COL_LIGHT( gray_color ), COL_LIGHT( gray_color ) ) );

	GUI::StyleBox sb;
	sb = GUI::StyleBox( 1, GUI::Color(0x99,0x5c,0x5c),GUI::Color(0xF1,0x6c,0x6c),GUI::Color(0xF1,0x6c,0x6c)); 
	sb.margins[0]=2; sb.margins[1]=2; sb.margins[2]=2; sb.margins[3]=2;
     	SET_STYLEBOX( SB_GRAPH_AUDIO_PORT, sb );
	sb = GUI::StyleBox( 1, GUI::Color(0x5c,0x5c,0x99), GUI::Color(0x6c,0x6c,0xF1), GUI::Color(0x6c,0x6c,0xF1) );
	sb.margins[0]=2; sb.margins[1]=2; sb.margins[2]=2; sb.margins[3]=2;
     	SET_STYLEBOX( SB_GRAPH_EVENT_PORT, sb);
	sb = GUI::StyleBox( 1, GUI::Color(0x99,0x98,0x5c),GUI::Color(0xf1,0xef,0x6c),GUI::Color(0xf1,0xef,0x6c)); 
	sb.margins[0]=2; sb.margins[1]=2; sb.margins[2]=2; sb.margins[3]=2;
     	SET_STYLEBOX( SB_GRAPH_CONTROL_PORT, GUI::StyleBox( 1, GUI::Color(0x99,0x98,0x5c),GUI::Color(0xf1,0xef,0x6c),GUI::Color(0xf1,0xef,0x6c)) );
	
	
	SET_FONT(FONT_GRAPH_NODE_PORT,0);
	SET_FONT(FONT_GRAPH_NODE_NAME,0);
	SET_COLOR(COLOR_GRAPH_NODE_AUDIO_PORT_FONT,GUI::Color(0));
	SET_COLOR(COLOR_GRAPH_NODE_EVENT_PORT_FONT,GUI::Color(0));
	SET_COLOR(COLOR_GRAPH_NODE_CONTROL_PORT_FONT,GUI::Color(0));
	SET_CONSTANT(C_GRAPH_NODE_VSPACING,1);
	SET_CONSTANT(C_GRAPH_NODE_HSPACING,1);


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



