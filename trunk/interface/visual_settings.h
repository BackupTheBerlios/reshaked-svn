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
};

enum ColorList {


	COLORLIST_PATTERN_EDIT_NOTE,
	COLORLIST_PATTERN_EDIT_VOL,
	COLORLIST_PATTERN_EDIT_FOCUS_RECT,
	COLORLIST_PATTERN_EDIT_BEAT_LINE,
	COLORLIST_PATTERN_EDIT_SUBBEAT_LINE,


};


#define GET_QCOLOR(m_col) VisualSettings::get_singleton()->get_color( m_col )

class VisualSettings{

	static VisualSettings * singleton;

	typedef std::map<SkinBoxList,SkinBox*> SkinBoxMap;
	typedef std::map<ColorList,QColor> ColorMap;

	SkinBoxMap skin_box_map;
	ColorMap color_map;
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

	VisualSettings();
	~VisualSettings();

};

}

#endif
