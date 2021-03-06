

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

#include <stdio.h>

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
	SET_BITMAP(BITMAP_ICON_FILE_FOLDER,PixmapData::get_pixmap(	PIXMAP_ICON_FILE_FOLDER));
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
	
	SET_BM( ICON_TEMPO );
	SET_BM( ICON_OCTAVE );

	SET_BM( ICON_FAVORITE );

	SET_BM( NODE_HW_AUDIO_IN );
	SET_BM( NODE_HW_AUDIO_OUT );
	SET_BM( NODE_HW_EVENT_IN ) ;
	SET_BM( NODE_HW_EVENT_OUT );
	SET_BM( NODE_AMP );
	SET_BM( NODE_REVERB );

	SET_BM( ICON_ENUM_LEFT );
	SET_BM( ICON_ENUM_LEFT_ACTIVE );
	SET_BM( ICON_ENUM_RIGHT );
	SET_BM( ICON_ENUM_RIGHT_ACTIVE );

	SET_BM( ICON_UNMUTE );
	SET_BM( ICON_MUTE );
	SET_BM( ICON_SOLO );
	SET_BM( ICON_COLUMN_ADD );
	SET_BM( ICON_COLUMN_REMOVE );
	SET_BM( ICON_MOVE_TRACK_LEFT );
	SET_BM( ICON_MOVE_TRACK_RIGHT );
	SET_BM( ICON_RENAME_TRACK );
	SET_BM( ICON_REMOVE_TRACK );

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
	
#define GRAY( m_col ) 	(((int)m_col.r + (int)m_col.g + (int) m_col.b)/3)
#define GRAYED( m_col,m_comp ) ((GRAY(m_col) + (int)m_col.m_comp)/3)
#define COL( m_col ) GUI::Color( GRAYED(m_col,r), GRAYED(m_col,g), GRAYED(m_col,b) )
#define COL_DARK( m_col ) GUI::Color( m_col.r/2, m_col.g/2, m_col.b/2 )
#define COL_LIGHT( m_col ) GUI::Color( (((int)m_col.r)*2 > 255?255:m_col.r*2), (((int)m_col.g)*2 > 255?255:m_col.g*2), (((int)m_col.b)*2 > 255?255:m_col.b*2) )


	for(int i=0;i<MAX_LAYER_COLORS;i++) {
	
		GUI::StyleBox sb = GUI::StyleBox( 1, COL( layer_colors[i%MAX_LOCAL_COLORS] ), COL_LIGHT( layer_colors[i%MAX_LOCAL_COLORS]), COL_LIGHT( layer_colors[i%MAX_LOCAL_COLORS] ) );
		sb.margins[0]=3; sb.margins[1]=3; sb.margins[2]=3; sb.margins[3]=3;
		
		set_stylebox( SB_LAYER_STYLE_NODE_0+i, sb );
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
	SET_STYLEBOX( SB_GRAPH_CONTROL_PORT, GUI::StyleBox( 1, GUI::Color(0x79,0x78,0x3c),GUI::Color(0xf1,0xef,0x6c),GUI::Color(0xf1,0xef,0x6c)) );
	
	
	SET_FONT(FONT_GRAPH_NODE_PORT,0);
	SET_FONT(FONT_GRAPH_NODE_NAME,0);
	SET_COLOR(COLOR_GRAPH_NODE_AUDIO_PORT_FONT,GUI::Color(255));
	SET_COLOR(COLOR_GRAPH_NODE_EVENT_PORT_FONT,GUI::Color(255));
	SET_COLOR(COLOR_GRAPH_NODE_CONTROL_PORT_FONT,GUI::Color(255));
	SET_COLOR(COLOR_GRAPH_NODE_NAME_FONT,GUI::Color(255));
	SET_COLOR(COLOR_GRAPH_AUDIO_CABLE, GUI::Color(0xF1,0x6c,0x6c) );
	SET_COLOR(COLOR_GRAPH_EVENT_CABLE, GUI::Color(0x6c,0x6c,0xF1) );
	SET_COLOR(COLOR_GRAPH_CONTROL_CABLE, GUI::Color(0xf1,0xef,0x6c) );
	SET_COLOR(COLOR_GRAPH_CONNECTING_CABLE, GUI::Color(0xff,0xff,0xff) );
	
	SET_CONSTANT(C_GRAPH_NODE_VSPACING,1);
	SET_CONSTANT(C_GRAPH_NODE_HSPACING,1);
	
	/* nodeui label */
	
	SET_STYLEBOX( SB_NODEUI_LABEL, GUI::StyleBox() );
	SET_STYLEBOX( SB_NODEUI_VLABEL, GUI::StyleBox() );
	SET_FONT( FONT_NODEUI_LABEL, 0 );
	SET_COLOR( COLOR_NODEUI_LABEL_FONT, GUI::Color(0) );

	/* nodeui value */
	
	SET_STYLEBOX( SB_NODEUI_VALUE, GUI::StyleBox() );
	SET_FONT( FONT_NODEUI_VALUE, 0 );
	SET_COLOR( COLOR_NODEUI_VALUE_FONT, GUI::Color(0) );
	 
	/* nodeui enum */
	
	SET_STYLEBOX( SB_NODEUI_ENUM, GUI::StyleBox() );
	SET_FONT( FONT_NODEUI_ENUM, 0 );
	SET_COLOR( COLOR_NODEUI_ENUM_FONT, GUI::Color(0) );
	SET_CONSTANT( C_GENERIC_NODE_UI_ENUM_MIN_WIDTH, 80 );
	 
	/* generic nodeui */
	
	SET_CONSTANT(C_GENERIC_NODE_UI_ROW_SIZE,6);
	SET_CONSTANT(C_GENERIC_NODE_UI_ROW_MAX,4);	
	SET_CONSTANT(C_GENERIC_NODE_UI_KNOB_MARGIN,6);
	SET_CONSTANT(C_GENERIC_NODE_UI_SCROLL_HEIGHT,400);
	SET_CONSTANT(C_GENERIC_NODE_UI_VSLIDER_MIN_HEIGHT,100);
	SET_CONSTANT(C_GENERIC_NODE_UI_VSLIDER_MARGIN,5);
	
	/* reverb nodeui */
	
	SET_STYLEBOX( SB_NODEUI_REVERB_IR_BG, GUI::StyleBox( 1, GUI::Color( 0 ), GUI::Color( 50,50,50 ), GUI::Color( 250,250,250 ) )) ;
	SET_COLOR( COLOR_NODEUI_REVERB_IR_IMPULSE, GUI::Color( 255,150,100 ) );
	SET_CONSTANT( C_NODEUI_REVERB_IR_HEIGHT, 50 );

	/* compressor curve */
	
	SET_CONSTANT( C_NODEUI_COMPRESSOR_CURVE_SIZE, 120 );
	SET_STYLEBOX( SB_NODEUI_COMRESSOR_CURVE, GUI::StyleBox( 1, GUI::Color( 0 ), GUI::Color( 50,50,50 ), GUI::Color( 250,250,250 ) ) );
	SET_COLOR( COLOR_NODEUI_COMPRESSOR_PLOT, GUI::Color( 255 ) );
	
	/* filterbank */
	
	SET_CONSTANT( C_NODEUI_FILTERBANK_MODE_WIDTH, 42 );
	SET_CONSTANT( C_NODEUI_FILTERBANK_STAGES_WIDTH, 10 );
	SET_STYLEBOX( SB_NODEUI_FILTER_BANK_FR_BG, GUI::StyleBox( 1, GUI::Color( 0 ), GUI::Color( 50,50,50 ), GUI::Color( 250,250,250 ) ) );
	SET_COLOR( COLOR_NODEUI_FILTER_BANK_FREQ, GUI::Color( 100,150,255 ) );
	SET_CONSTANT( C_NODEUI_FILTERBANK_FR_MIN_WIDTH, 200 );
	
	GUI::StyleBox sbsb = GUI::StyleBox( 2, GUI::Color(200,200,200),GUI::Color(250,250,250),GUI::Color(90,90,90) );
	for (int i=0;i<4;i++)
		sbsb.margins[i]=4;
	
	SET_STYLEBOX(SB_NODEUI_FILTER_BANK_SECTION_BOX, sbsb );
	SET_COLOR( COLOR_NODEUI_FILTER_BANK_CUTOFF, GUI::Color( 230 ) );
	SET_COLOR( COLOR_NODEUI_FILTER_BANK_SELECTED_CUTOFF, GUI::Color( 160 ) );
	SET_COLOR( COLOR_NODEUI_FILTER_BANK_FR_FONT, GUI::Color(255) );
	SET_COLOR( COLOR_NODEUI_FILTER_BANK_FR_FONT_SHADOW, GUI::Color(0) );
	SET_FONT( FONT_NODEUI_FILTER_BANK_FR_FONT, 0 );
		
	/* track editor */
	
	SET_FONT( FONT_TRACK_EDITOR, track_font );
	SET_CONSTANT( C_TRACK_EDITOR_VOL_NOTE_BAR_HEIGHT, 2 );
	SET_CONSTANT( C_TRACK_EDITOR_VOL_NOTE_BAR_SEPAATION,0 );
	SET_CONSTANT( C_TRACK_EDITOR_ROW_MARGIN, 2 );
	SET_CONSTANT( C_TRACK_EDITOR_SELECTION_MARGIN,2 );	
	SET_STYLEBOX( SB_TRACK_EDITOR_CURSOR,	GUI::StyleBox(1,GUI::Color(255),GUI::Color(255)));
	
	SET_BM( CONTROL_PLAY_PATTERN );
	SET_BM( CONTROL_PLAY_CURSOR );
	SET_BM( ICON_VOLUME_MASK );
	SET_BM( ICON_CURSOR_STEP );
	SET_BM( ICON_MIDI );
	SET_BM( ICON_ZOOM );
	SET_BM( ICON_TRACK_VISIBLE );
	SET_BM( ICON_TRACK_VISIBLE_HL );
	SET_BM( ICON_TRACK_COLLAPSED );
	SET_BM( ICON_TRACK_COLLAPSED_HL );
	SET_BM( ICON_TRACK_OPTIONS );
	SET_BM( ICON_TRACK_OPTIONS_HL );
	
	SET_COLOR(COLOR_TRACK_EDITOR_BG,GUI::Color(0));
	SET_COLOR(COLOR_BARBEAT_BG,GUI::Color(0));		
	SET_COLOR(COLOR_BARBEAT_BEAT_LINE,GUI::Color(0x7a,0x90,0x9c));
	SET_COLOR(COLOR_BARBEAT_BAR_LINE,GUI::Color(0xe2,0xf6,0xff));
	SET_COLOR(COLOR_BARBEAT_SUBBEAT_LINE,GUI::Color(0x4f,0x5b,0x65));
	SET_COLOR(COLOR_BARBEAT_BEAT_FONT,GUI::Color(0x8a,0xa0,0xac));
	SET_COLOR(COLOR_BARBEAT_BAR_FONT,GUI::Color(0xae,0xe9,0xf8));
	SET_COLOR(COLOR_BARBEAT_SUBBEAT_FONT,GUI::Color(0x6b,0x6b,0x6b));
	SET_COLOR(COLOR_BARBEAT_MARKER_FONT,GUI::Color(200));
	SET_COLOR(COLOR_BARBEAT_LOOP,GUI::Color(170,180,210));
	SET_FONT( FONT_TRACK_EDITOR_TRACK_NAME, track_font );
	SET_STYLEBOX( SB_TRACK_EDITOR_TRACK_NAME, GUI::StyleBox(1,GUI::Color(0),GUI::Color(0),GUI::Color(0) ) );
	SET_CONSTANT( C_BARBEAT_MARKER_OFFSET, 4 );
		
	/* pattern editor */
	
	SET_COLOR(COLOR_PATTERN_FONT,GUI::Color(0xae,0xe9,0xf8));
	SET_COLOR(COLOR_PATTERN_FONT_VOLUME,GUI::Color(0xf2,0xae,0xbd));
	SET_COLOR(COLOR_PATTERN_FONT_NOFIT,GUI::Color(0x81,0x81,0x81));
	SET_COLOR(COLOR_PATTERN_FONT_REPEAT,GUI::Color(0x81,0x81,0x81));
	SET_COLOR(COLOR_PATTERN_VOLUME_BG,GUI::Color(0));
	SET_COLOR(COLOR_PATTERN_NOTE_BG,GUI::Color(0));
	SET_COLOR(COLOR_PATTERN_VOLUME_BAR,GUI::Color(0xff,0xbe,0xaa));
	SET_COLOR(COLOR_PATTERN_NOTE_BAR,GUI::Color(0xc8,0xdc,0xff));
	SET_COLOR(COLOR_PATTERN_BEAT_LINE,GUI::Color(0x7a,0x90,0x9c));
	SET_COLOR(COLOR_PATTERN_BAR_LINE,GUI::Color(0xe2,0xf6,0xff));
	SET_COLOR(COLOR_PATTERN_SUBBEAT_LINE,GUI::Color(0x4f,0x5b,0x65));
	SET_COLOR(COLOR_PATTERN_SELECTION,GUI::Color(0x28,0x5C,0x7C));
	SET_COLOR(COLOR_PATTERN_BG,GUI::Color(0));	

	
	SET_STYLEBOX(SB_PATTERN_BLOCK,GUI::StyleBox(3,GUI::Color(0x2d,0x27,0x39),GUI::Color(0xbd,0xd3,0xd7),GUI::Color(0xbd,0xd3,0xd7)));
     	//SET_STYLEBOX(SB_PATTERN_BLOCK_SHARED,get_stylebox(SB_GLOBAL_VIEW_LINKED)); //use the same

	/* automation editor */
	
	SET_STYLEBOX(SB_AUTOMATION_BLOCK,GUI::StyleBox(3,GUI::Color(0x2f,0x37,0x1e),GUI::Color(0xcd,0xd7,0xbc),GUI::Color(0xcd,0xd7,0xbc)));

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
	
GUI::FontID CommonSkin::load_fixed_font(PixmapDataList p_pixmap,int p_from,int p_to) {
		
	
	int font_h=painter->get_bitmap_size(PixmapData::get_pixmap(p_pixmap)).height;
	int font_w=painter->get_bitmap_size(PixmapData::get_pixmap(p_pixmap)).width/(p_to-p_from);
	
	GUI::FontID font = painter->create_font(font_h,font_h-1);
	
	for (int i=0;i<(p_to-p_from);i++) {
		
		painter->font_add_char(font,p_from+i,PixmapData::get_pixmap(p_pixmap),GUI::Rect(GUI::Point(i*(font_w),0),GUI::Size(font_w,font_h)));
	}	
	
	return font;
}

CommonSkin::CommonSkin(GUI::Painter *p_painter) : Skin(COMMON_SB_MAX,COMMON_C_MAX,COMMON_BITMAP_MAX,COMMON_FONT_MAX,COMMON_COLOR_MAX) {
	
	painter=p_painter;
	
//	bar_font=load_number_font(PIXMAP_GLOBAL_VIEW_BAR_FONT);
//	beat_font=load_number_font(PIXMAP_GLOBAL_VIEW_BEAT_FONT);
	small_font = load_fixed_font( PIXMAP_FONT_SMALL, 32, 127 );
	track_font = load_fixed_font( PIXMAP_FONT_FIXED, 32, 127 );
	set_default_extra();
}


CommonSkin::~CommonSkin()
{
}



