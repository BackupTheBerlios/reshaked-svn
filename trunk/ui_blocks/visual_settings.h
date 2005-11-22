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
};

enum ColorList {


	COLORLIST_PATTERN_EDIT_NOTE,
	COLORLIST_PATTERN_EDIT_VOL,
	COLORLIST_PATTERN_EDIT_FOCUS_RECT,
	COLORLIST_PATTERN_EDIT_BEAT_LINE,
	COLORLIST_PATTERN_EDIT_SUBBEAT_LINE,
	COLORLIST_AUTOMATION_ENVELOPE


};

enum PixmapList {
	
	/* Pixmaps */
	
	PIXMAP_TRACK_OPTIONS,
	PIXMAP_KNOB_BASE,
	PIXMAP_KNOB_HANDLE,
	PIXMAP_TRACK_SETTINGS_PATTERN,
	PIXMAP_TRACK_SETTINGS_CONTROLS,
	PIXMAP_TRACK_SETTINGS_EFFECTS,
	PIXMAP_TRACK_SETTINGS_CONNECTIONS,
	PIXMAP_ENVELOPE_POINT,
	
	
	
	/* Icons */
	
	ICON_TRACK_MOVE_LEFT,
	ICON_TRACK_MOVE_RIGHT,
	ICON_TRACK_DELETE,
	ICON_TRACK_EDIT,
	ICON_AUTOMATION_MOVE_LEFT,
	ICON_AUTOMATION_MOVE_RIGHT,
	ICON_COLUMN_ADD,
	ICON_COLUMN_REMOVE
	
};

#define GET_QCOLOR(m_col) VisualSettings::get_singleton()->get_color( m_col )
#define GET_QPIXMAP(m_pix) VisualSettings::get_singleton()->get_pixmap( m_pix )

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
	static VisualSettings * get_singleton();

	SkinBox * get_skin_box(SkinBoxList p_which);
	QColor get_color(ColorList p_color);
	QPixmap get_pixmap(PixmapList p_pixmap);

	VisualSettings();
	~VisualSettings();

};

}

#endif