//
// C++ Interface: visual_settings
//
// Description:
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDVISUAL_SETTINGS_H
#define RESHAKEDVISUAL_SETTINGS_H

#include "ui_blocks/skin_box.h"
#include "ui_blocks/pixmap_font.h"
#include <map>

namespace ReShaked {

/**
	@author red <red@killy>
*/



enum SkinBoxList {

	SKINBOX_GLOBALVIEW_PATTERN,
	SKINBOX_GLOBALVIEW_AUTOMATION,
	SKINBOX_GLOBALVIEW_AUDIO,
	SKINBOX_GLOBALVIEW_PATTERN_SELECTED,
	SKINBOX_GLOBALVIEW_AUTOMATION_SELECTED,
	SKINBOX_GLOBALVIEW_AUDIO_SELECTED,
	SKINBOX_EDITING_PATTERN,
	SKINBOX_EDITING_PATTERN_SELECTED,
	SKINBOX_EDITING_AUTOMATION,
	SKINBOX_EDITING_AUTOMATION_SELECTED,
	SKINBOX_EDITING_RESIZABLE_CURSOR,
		
	SKINBOX_TRACK_TOP_BG,
	
	SKINBOX_RACK_NORMAL,
	SKINBOX_RACK_SYSTEM,
		
};

enum ColorList {


	COLORLIST_GLOBAL_VIEW_BAR_LINE,
	COLORLIST_GLOBAL_VIEW_BEAT_LINE,
	COLORLIST_PATTERN_EDIT_BAR,
	COLORLIST_PATTERN_EDIT_NOTE,
	COLORLIST_PATTERN_EDIT_VOL,
	COLORLIST_PATTERN_EDIT_FOCUS_RECT,
	COLORLIST_PATTERN_EDIT_BEAT_LINE,
	COLORLIST_PATTERN_EDIT_SUBBEAT_LINE,
	COLORLIST_AUTOMATION_ENVELOPE,
	COLORLIST_EDITOR_SELECTION_AUTOMATION,
	COLORLIST_EDITOR_SELECTION_PATTERN,
	COLORLIST_RACK_CABLE,
	COLORLIST_RACK_CABLE_DARK,


};

enum PixmapList {
	
	/* Pixmaps */
	
	PIXMAP_TRACK_OPTIONS,
	PIXMAP_KNOB_BASE,
	PIXMAP_KNOB_HANDLE,
	PIXMAP_TRACK_SETTINGS_PATTERN,
	PIXMAP_TRACK_SETTINGS_AUTOMATIONS,
	PIXMAP_TRACK_SETTINGS_CONTROLS,
	PIXMAP_TRACK_SETTINGS_EFFECTS,
	PIXMAP_TRACK_SETTINGS_CONNECTIONS,
	PIXMAP_ENVELOPE_POINT,
	PIXMAP_RACK_JACK_HOLE,
	PIXMAP_TRACK_SLIDER_VU_BG,		
	PIXMAP_TRACK_SLIDER_VU_FG,
	PIXMAP_TRACK_SLIDER_VU_GRABBER,
	PIXMAP_TRACK_SLIDER_VU_ICON,
	PIXMAP_TRACK_SLIDER_SWING_BG,		
	PIXMAP_TRACK_SLIDER_SWING_FG,
	PIXMAP_TRACK_SLIDER_SWING_GRABBER,
	PIXMAP_TRACK_SLIDER_SWING_ICON,
	
	
	/* Icons */
	
	ICON_GLOBAL_VIEW_SELECT_BLOCK,
	ICON_GLOBAL_VIEW_ADD_BLOCK,
	ICON_GLOBAL_VIEW_COPY_BLOCK,
	ICON_GLOBAL_VIEW_COPY_LINK_BLOCK,
	ICON_GLOBAL_VIEW_ERASE_BLOCK,
	
	ICON_CURSOR_ADD_BLOCK,
	
	ICON_TRACK_MOVE_LEFT,
	ICON_TRACK_MOVE_RIGHT,
	ICON_TRACK_DELETE,
	ICON_TRACK_EDIT,
	ICON_AUTOMATION_MOVE_LEFT,
	ICON_AUTOMATION_MOVE_RIGHT,
	ICON_COLUMN_ADD,
	ICON_COLUMN_REMOVE,
	ICON_ZOOM_SMALL,
	 
	ICON_CONTROL_RW,
	ICON_CONTROL_PLAY,
	ICON_CONTROL_PLAY_LOOP,
	ICON_CONTROL_PLAY_BLOCK,
	ICON_CONTROL_PLAY_CURSOR,
	ICON_CONTROL_PAUSE,
	ICON_CONTROL_STOP,
	ICON_CONTROL_FF,
	ICON_CONTROL_RECORD,
	ICON_CONTROL_RECORD_AUTOMATIONS,
	
	ICON_EDIT_VIEW_ROW_PLAYED,
	ICON_EDIT_VIEW_ROW_LOOP_BEGIN,
	ICON_EDIT_VIEW_ROW_LOOP_END
		
};

#define GET_QCOLOR(m_col) VisualSettings::get_singleton()->get_color( m_col )
#define GET_QPIXMAP(m_pix) VisualSettings::get_singleton()->get_pixmap( m_pix )
#define GET_SKIN(m_skin) VisualSettings::get_singleton()->get_skin_box( m_skin )

class VisualSettings{

	static VisualSettings * singleton;

	typedef std::map<SkinBoxList,SkinBox*> SkinBoxMap;
	typedef std::map<ColorList,QColor> ColorMap;
	typedef std::map<PixmapList,QPixmap> PixmapMap;

	SkinBoxMap skin_box_map;
	ColorMap color_map;
	PixmapMap pixmap_map;
			
	PixmapFont pattern_font;
	PixmapFont pattern_vol_font;
	PixmapFont rowcount_font;
	PixmapFont rowcount_subbeat_font;
	PixmapFont global_bar_font;
	PixmapFont global_beat_font;
	QPixmap pattern_cursor;
	int editing_row_height;
	QPoint pattern_cursor_offset;
public:

	
	QPixmap get_pattern_cursor_pixmap();
	QPoint get_pattern_cursor_offset();
	int get_editing_row_height();
	PixmapFont* get_pattern_font();
	PixmapFont* get_pattern_vol_font();
	PixmapFont* get_rowcount_font();
	PixmapFont* get_rowcount_subbeat_font();
	PixmapFont* get_global_bar_font();
	PixmapFont* get_global_beat_font();
	static VisualSettings * get_singleton();

	SkinBox * get_skin_box(SkinBoxList p_which);
	QColor get_color(ColorList p_color);
	QPixmap get_pixmap(PixmapList p_pixmap);

	int get_selection_h_margin();
	int get_rack_panel_h_margin();
	
	VisualSettings();
	~VisualSettings();

};

}

#endif
