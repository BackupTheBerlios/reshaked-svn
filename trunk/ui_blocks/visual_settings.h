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
	SKINBOX_EDITING_PATTERN_SHARED,
	SKINBOX_EDITING_PATTERN_SELECTED,
	SKINBOX_EDITING_AUTOMATION,
	SKINBOX_EDITING_AUTOMATION_SHARED,
	SKINBOX_EDITING_AUTOMATION_SELECTED,
	SKINBOX_EDITING_RESIZABLE_CURSOR,
		
	SKINBOX_TRACK_TOP_BG,
	
	SKINBOX_LINKED_BLOCK,
	
	SKINBOX_RACK_NORMAL,
	SKINBOX_RACK_SYSTEM,
		
	SKINBOX_THEME_SCROLLBAR_GRABBER,
	SKINBOX_THEME_SCROLLBAR_H_BG,
	SKINBOX_THEME_SCROLLBAR_V_BG,
	
};

enum ColorList {


	COLORLIST_GLOBAL_VIEW_BAR_LINE,
	COLORLIST_GLOBAL_VIEW_BEAT_LINE,
	COLORLIST_TRACK_TITLE,
	COLORLIST_TRACK_TITLE_MUTE,
	COLORLIST_PATTERN_EDIT_NOTE,
	COLORLIST_PATTERN_EDIT_VOL,
	COLORLIST_PATTERN_EDIT_FOCUS_RECT,
	COLORLIST_PATTERN_EDIT_BEAT_LINE,
	COLORLIST_PATTERN_EDIT_SUBBEAT_LINE,
	COLORLIST_PATTERN_EDIT_BAR,
	COLORLIST_AUTOMATION_ENVELOPE,
	COLORLIST_AUTOMATION_EDIT_BEAT_LINE,
	COLORLIST_AUTOMATION_EDIT_SUBBEAT_LINE,
	COLORLIST_AUTOMATION_EDIT_BAR,
	COLORLIST_EDITOR_SELECTION_AUTOMATION,
	COLORLIST_EDITOR_SELECTION_PATTERN,
	COLORLIST_RACK_CABLE,
	COLORLIST_RACK_CABLE_DARK,

	COLORLIST_TIME_LABEL,
	COLORLIST_TIME_LABEL_PLAY,

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
	
	THEME_TOP__BPM_BOTTOM,
	THEME_TOP__BPM_DISPLAY,
	THEME_TOP__BPM_END,
	THEME_TOP__BPM_TOP,
	THEME_TOP__BPM_UPDOWN,
	THEME_TOP__BUTTON_FASTFORWARD_ACTIVE,
	THEME_TOP__BUTTON_FASTFORWARD,
	THEME_TOP__BUTTON_PAUSE_ACTIVE,
	THEME_TOP__BUTTON_PAUSE,
	THEME_TOP__BUTTON_PLAY_ACTIVE,
	THEME_TOP__BUTTON_PLAY,
	THEME_TOP__BUTTON_LOOP_ACTIVE,
	THEME_TOP__BUTTON_LOOP,
	THEME_TOP__BUTTON_RECORD_ACTIVE,
	THEME_TOP__BUTTON_RECORD_AUTOMATIONS_ACTIVE,
	THEME_TOP__BUTTON_RECORD_AUTOMATIONS,
	THEME_TOP__BUTTON_RECORD,
	THEME_TOP__BUTTON_REWIND_ACTIVE,
	THEME_TOP__BUTTON_REWIND,
	THEME_TOP__BUTTONS_BOTTOM,
	THEME_TOP__BUTTONS_BPM_SEPARATOR,
	THEME_TOP__BUTTON_STOP_ACTIVE,
	THEME_TOP__BUTTON_STOP,
	THEME_TOP__BUTTONS_TOP,
	THEME_TOP__MARGIN_BOTTOM,
	THEME_TOP__MARGIN_LEFT,
	THEME_TOP__MARGIN_RIGHT,
	THEME_TOP__TAB_EDIT_ACTIVE,
	THEME_TOP__TAB_EDIT,
	THEME_TOP__TAB_MIX_ACTIVE,
	THEME_TOP__TAB_MIX,
	THEME_TOP__TABS_BUTTONS_SEPARATOR,
	THEME_TOP__TAB_SONG_ACTIVE,
	THEME_TOP__TAB_SONG,
	THEME_TOP__CONTROLS_END,
	
	THEME_TOP__ICON_FILE_ACTIVATED,
	THEME_TOP__ICON_FILE,
	THEME_TOP__ICON_ADD_ACTIVATED,
	THEME_TOP__ICON_ADD,
	THEME_TOP__ICON_HELP_ACTIVATED,
	THEME_TOP__ICON_HELP,
	THEME_TOP__ICON_REDO_ACTIVATED,
	THEME_TOP__ICON_REDO_DISABLED,
	THEME_TOP__ICON_REDO,
	THEME_TOP__ICON_SETTINGS_ACTIVATED,
	THEME_TOP__ICON_SETTINGS,
	THEME_TOP__ICON_UNDO_ACTIVATED,
	THEME_TOP__ICON_UNDO_DISABLED,
	THEME_TOP__ICON_UNDO,
	THEME_LEFT__MARGIN,
	THEME_RIGHT__MARGIN,
	THEME_BOTTOM__MARGIN,
	THEME_BOTTOM_LEFT__CORNER,
	THEME_BOTTOM_RIGHT__CORNER,

	THEME_BOTTOM__BEGIN,
	THEME_BOTTOM__END,
	THEME_BOTTOM__INFORMATION_BEGIN,
	THEME_BOTTOM__INFORMATION_END,
	THEME_BOTTOM__INFORMATION_LABEL_BOTTOM,
	THEME_BOTTOM__INFORMATION_LABEL_END,
	THEME_BOTTOM__INFORMATION_LABEL_TOP,
	THEME_BOTTOM__INFORMATION_LABEL,
	THEME_BOTTOM__OCTAVE_BOTTOM,
	THEME_BOTTOM__OCTAVE_LABEL,
	THEME_BOTTOM__OCTAVE_SPIN,
	THEME_BOTTOM__OCTAVE_TOP,
	THEME_BOTTOM__RACK_ACTIVE,
	THEME_BOTTOM__RACK,
	THEME_BOTTOM__SEPARATOR,
	THEME_BOTTOM__TIME_BEGIN,
	THEME_BOTTOM__TIME_END,
	THEME_BOTTOM__TIME_LABEL_BOTTOM,
	THEME_BOTTOM__TIME_LABEL_TOP,
	THEME_BOTTOM__TIME_LABEL,
	THEME_BOTTOM__VPIANO_BEGIN,
	THEME_BOTTOM__VPIANO_BLACK_KEY_PRESSED,
	THEME_BOTTOM__VPIANO_BLACK_KEY,
	THEME_BOTTOM__VPIANO_OCTAVE_SEPARATOR,
	THEME_BOTTOM__VPIANO_TOP,
	THEME_BOTTOM__VPIANO_WHITE_KEY_PRESSED,
	THEME_BOTTOM__VPIANO_WHITE_KEY,
	
	
	THEME_GLOBAL_TOOLBAR__ADD_ACTIVE,
	THEME_GLOBAL_TOOLBAR__ADD,
	THEME_GLOBAL_TOOLBAR__COPY_ACTIVE,
	THEME_GLOBAL_TOOLBAR__COPY_LINK_ACTIVE,
	THEME_GLOBAL_TOOLBAR__COPY_LINK,
	THEME_GLOBAL_TOOLBAR__COPY,
	THEME_GLOBAL_TOOLBAR__ERASER_PUSHED,
	THEME_GLOBAL_TOOLBAR__ERASER,
	THEME_GLOBAL_TOOLBAR__HSPACER,
	THEME_GLOBAL_TOOLBAR__LEFT,
	THEME_GLOBAL_TOOLBAR__RIGHT,
	THEME_GLOBAL_TOOLBAR__SELECT_ACTIVE,
	THEME_GLOBAL_TOOLBAR__SELECT_LINKED_PUSHED,
	THEME_GLOBAL_TOOLBAR__SELECT_LINKED,
	THEME_GLOBAL_TOOLBAR__SELECT,
	THEME_GLOBAL_TOOLBAR__UNLINK_SELECTED_PUSHED,
	THEME_GLOBAL_TOOLBAR__UNLINK_SELECTED,
	THEME_GLOBAL_TOOLBAR__ZOOM_BG,
	THEME_GLOBAL_TOOLBAR__ZOOM_FG,
	THEME_GLOBAL_TOOLBAR__ZOOM_GRABBER,
	THEME_GLOBAL_TOOLBAR__ZOOM_ICON,
	THEME_GLOBAL_TOOLBAR__BOTTOM_RIGHT_PIXMAP,
	
	THEME_EDIT_TOOLBAR__AUTOMATION_MODE_ENVELOPE_ACTIVE,
	THEME_EDIT_TOOLBAR__AUTOMATION_MODE_ENVELOPE,
	THEME_EDIT_TOOLBAR__AUTOMATION_MODE_STREAM_ACTIVE,
	THEME_EDIT_TOOLBAR__AUTOMATION_MODE_STREAM,
	THEME_EDIT_TOOLBAR__VOLUME_MASK,
	THEME_EDIT_TOOLBAR__VOLUME_MASK_ACTIVE,
	THEME_EDIT_TOOLBAR__VOLUME_MASK_LABEL,
	THEME_EDIT_TOOLBAR__VOLUME_MASK_LABEL_TOP,
	THEME_EDIT_TOOLBAR__VOLUME_MASK_LABEL_BOTTOM,
	THEME_EDIT_TOOLBAR__BEGIN,
	THEME_EDIT_TOOLBAR__CURSOR_FOLLOWS_PLAYBACK_PUSHED,
	THEME_EDIT_TOOLBAR__CURSOR_FOLLOWS_PLAYBACK,
	THEME_EDIT_TOOLBAR__CURSOR_STEPPING_DROPDOWN,
	THEME_EDIT_TOOLBAR__CURSOR_STEPPING_DROPDOWN_TOP,
	THEME_EDIT_TOOLBAR__CURSOR_STEPPING_DROPDOWN_BOTTOM,
	THEME_EDIT_TOOLBAR__CURSOR_STEPPING_ICON,
	THEME_EDIT_TOOLBAR__EDIT_MENU,
	THEME_EDIT_TOOLBAR__EDIT_MENU_TOP,
	THEME_EDIT_TOOLBAR__EDIT_MENU_BOTTOM,
	THEME_EDIT_TOOLBAR__END,
	THEME_EDIT_TOOLBAR__MIDI_IN_CONFIG_PUSHED,
	THEME_EDIT_TOOLBAR__MIDI_IN_CONFIG,
	THEME_EDIT_TOOLBAR__PLAY_CURRENT_BLOCK_PUSHED,
	THEME_EDIT_TOOLBAR__PLAY_CURRENT_BLOCK,
	THEME_EDIT_TOOLBAR__PLAY_FROM_CURSOR_PUSHED,
	THEME_EDIT_TOOLBAR__PLAY_FROM_CURSOR,
	THEME_EDIT_TOOLBAR__SNAP_DROPDOWN,
	THEME_EDIT_TOOLBAR__SNAP_DROPDOWN_TOP,
	THEME_EDIT_TOOLBAR__SNAP_DROPDOWN_BOTTOM,
	THEME_EDIT_TOOLBAR__SPACER,
	THEME_EDIT_TOOLBAR__SEPARATOR,
	
	THEME_MIDDLE__SEPARATOR_BEGIN,
	THEME_MIDDLE__SEPARATOR_CENTER,
	THEME_MIDDLE__SEPARATOR_END,
	
	
	THEME_RACK_TOOLBAR__RACK_FOLLOW_CURSOR,
	THEME_RACK_TOOLBAR__RACK_FOLLOW_CURSOR_ACTIVE,
	THEME_RACK_TOOLBAR__ADD_PLUGIN_PUSHED,
	THEME_RACK_TOOLBAR__ADD_PLUGIN,
	THEME_RACK_TOOLBAR__BEGIN,
	THEME_RACK_TOOLBAR__END,
	THEME_RACK_TOOLBAR__PRESET_FILE_PUSHED,
	THEME_RACK_TOOLBAR__PRESET_FILE,
	THEME_RACK_TOOLBAR__PRESET_ICON,
	THEME_RACK_TOOLBAR__PRESET_LABEL_BOTTOM,
	THEME_RACK_TOOLBAR__PRESET_LABEL_TOP,
	THEME_RACK_TOOLBAR__PRESET_LABEL,
	THEME_RACK_TOOLBAR__SEPARATOR,
	THEME_RACK_TOOLBAR__SHOW_BACK_ACTIVE,
	THEME_RACK_TOOLBAR__SHOW_BACK,
	THEME_RACK_TOOLBAR__SHOW_FRONT_ACTIVE,
	THEME_RACK_TOOLBAR__SHOW_FRONT,
	THEME_RACK_TOOLBAR__SPACER,
	THEME_RACK_TOOLBAR__TRACK_DROPDOWN_BOTTOM,
	THEME_RACK_TOOLBAR__TRACK_DROPDOWN_TOP,
	THEME_RACK_TOOLBAR__TRACK_DROPDOWN,
	
	THEME_RACK_PANEL__JACK_IN,
	THEME_RACK_PANEL__JACK_OUT,
	THEME_RACK_PANEL__JACK_FORBIDDEN,
		
	
	THEME_RACK_PLUGIN_TOP__BEGIN,
	THEME_RACK_PLUGIN_TOP__END,
	THEME_RACK_PLUGIN_TOP__INFO_PUSHED,
	THEME_RACK_PLUGIN_TOP__INFO,
	THEME_RACK_PLUGIN_TOP__MOVE_LEFT_PUSHED,
	THEME_RACK_PLUGIN_TOP__MOVE_LEFT,
	THEME_RACK_PLUGIN_TOP__MOVE_RIGHT_PUSHED,
	THEME_RACK_PLUGIN_TOP__MOVE_RIGHT,
	THEME_RACK_PLUGIN_TOP__PRESET_NAME,
	THEME_RACK_PLUGIN_TOP__PRESET_PUSHED,
	THEME_RACK_PLUGIN_TOP__PRESET,
	THEME_RACK_PLUGIN_TOP__REMOVE_PUSHED,
	THEME_RACK_PLUGIN_TOP__REMOVE,
	THEME_RACK_PLUGIN_TOP__SEPARATOR,
	THEME_RACK_PLUGIN_TOP__SKIP_ACTIVE,
	THEME_RACK_PLUGIN_TOP__SKIP,
	
	
	THEME_SCROLLBAR_ARROW_DOWN_PUSHED,
	THEME_SCROLLBAR_ARROW_DOWN,
	THEME_SCROLLBAR_ARROW_LEFT_PUSHED,
	THEME_SCROLLBAR_ARROW_LEFT,
	THEME_SCROLLBAR_ARROW_RIGHT_PUSHED,
	THEME_SCROLLBAR_ARROW_RIGHT,
	THEME_SCROLLBAR_ARROW_UP_PUSHED,
	THEME_SCROLLBAR_ARROW_UP,

	
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
	ICON_EDIT_VIEW_ROW_LOOP_END,
			
	ICON_FILE_OPEN,
	ICON_FILE_SAVE,
	ICON_FILE_SAVEAS,
	ICON_FILE_FOLDER,
	ICON_FILE_FOLDER_NEW,
	
	ICON_MUTE,
	ICON_SOLO,
			
		
};

enum ConstantList {
	
	CONSTANT_EDIT_TOOLBAR_FONT_HEIGHT,
	CONSTANT_RACK_MINIMUM_HEIGHT,
	CONSTANT_PLUGIN_CHOOSER_ITEM_MARGIN
	
};

#define GET_QCOLOR(m_col) VisualSettings::get_singleton()->get_color( m_col )
#define GET_QPIXMAP(m_pix) VisualSettings::get_singleton()->get_pixmap( m_pix )
#define GET_SKIN(m_skin) VisualSettings::get_singleton()->get_skin_box( m_skin )
#define GET_CONSTANT(m_const) VisualSettings::get_singleton()->get_constant( m_const )

class VisualSettings{

	static VisualSettings * singleton;

	typedef std::map<SkinBoxList,SkinBox*> SkinBoxMap;
	typedef std::map<ColorList,QColor> ColorMap;
	typedef std::map<PixmapList,QPixmap> PixmapMap;
	typedef std::map<ConstantList,int> ConstantMap;

	SkinBoxMap skin_box_map;
	ColorMap color_map;
	PixmapMap pixmap_map;
	ConstantMap constant_map;
			
	PixmapFont pattern_font;
	PixmapFont pattern_font_nofit;
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
	PixmapFont* get_pattern_font_nofit();
	PixmapFont* get_pattern_vol_font();
	PixmapFont* get_rowcount_font();
	PixmapFont* get_rowcount_subbeat_font();
	PixmapFont* get_global_bar_font();
	PixmapFont* get_global_beat_font();
	static VisualSettings * get_singleton();

	SkinBox * get_skin_box(SkinBoxList p_which);
	QColor get_color(ColorList p_color);
	QPixmap get_pixmap(PixmapList p_pixmap);
	int get_constant(ConstantList p_constant);

	int get_selection_h_margin();
	int get_rack_panel_h_margin();
	
	VisualSettings();
	~VisualSettings();

};

}

#endif
