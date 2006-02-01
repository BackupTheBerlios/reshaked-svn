//
// C++ Implementation: visual_settings
//   
// Description:
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "visual_settings.h"
#include "pixmaps/pattern_glob_bg.xpm"
#include "pixmaps/automation_glob_bg.xpm"
#include "pixmaps/pattern_glob_bg_sel.xpm"
#include "pixmaps/automation_glob_bg_sel.xpm"
#include "pixmaps/pattern_font.xpm"
#include "pixmaps/pattern_vol_font.xpm"
#include "pixmaps/rowcount_font.xpm"
#include "pixmaps/rowcount_font_subbeat.xpm"
#include "pixmaps/pattern_bg.xpm"
#include "pixmaps/automation_bg.xpm"
#include "pixmaps/pattern_bg_shared.xpm"
#include "pixmaps/automation_bg_shared.xpm"
#include "pixmaps/automation_bg_sel.xpm"
#include "pixmaps/pattern_bg_sel.xpm"
#include "pixmaps/pattern_cursor.xpm"
#include "pixmaps/automation_cursor.xpm"
#include "pixmaps/track_options.xpm"
#include "pixmaps/knob_base.xpm"
#include "pixmaps/knob_handle.xpm"
#include "pixmaps/track_settings_pattern.xpm"
#include "pixmaps/track_settings_automations.xpm"
#include "pixmaps/track_settings_controls.xpm"
#include "pixmaps/track_settings_effects.xpm"
#include "pixmaps/track_settings_connections.xpm"
#include "pixmaps/envelope_point.xpm"

#include "pixmaps/font_global_bar.xpm"
#include "pixmaps/font_global_beat.xpm"

#include "pixmaps/track_move_left.xpm"
#include "pixmaps/track_move_right.xpm"
#include "pixmaps/track_delete.xpm"
#include "pixmaps/track_edit.xpm"
#include "pixmaps/track_move_automation_left.xpm"
#include "pixmaps/track_move_automation_right.xpm"
#include "pixmaps/track_column_add.xpm"
#include "pixmaps/track_column_rem.xpm"
#include "pixmaps/zoom.xpm"

#include "pixmaps/global_edit_add_block.xpm"
#include "pixmaps/global_edit_copy_block_linked.xpm"
#include "pixmaps/global_edit_copy_block.xpm"
#include "pixmaps/global_edit_delete_block.xpm"
#include "pixmaps/global_edit_select_block.xpm"

#include "pixmaps/cursor_add_block.xpm"
#include "pixmaps/connection_panel_bg.xpm"
#include "pixmaps/connection_panel_system.xpm"

#include "pixmaps/jack_hole.xpm"
#include "pixmaps/control_ff.xpm"
#include "pixmaps/control_play.xpm"
#include "pixmaps/control_play_loop.xpm"
#include "pixmaps/control_play_cursor.xpm"
#include "pixmaps/control_play_block.xpm"
#include "pixmaps/control_rw.xpm"
#include "pixmaps/control_pause.xpm"
#include "pixmaps/control_record.xpm"
#include "pixmaps/control_record_automations.xpm"
#include "pixmaps/control_stop.xpm"

#include "pixmaps/icon_solo.xpm"
#include "pixmaps/icon_mute.xpm"

#include "pixmaps/row_icon_loop_begin.xpm"
#include "pixmaps/row_icon_loop_end.xpm"
#include "pixmaps/row_icon_play.xpm"

#include "pixmaps/track_variables_volume_vu_empty.xpm"
#include "pixmaps/track_variables_volume_vu_full.xpm"
#include "pixmaps/track_variables_volume_vu_grabber.xpm"
#include "pixmaps/track_variables_volume.xpm"

#include "pixmaps/track_variables_swing_empty.xpm"
#include "pixmaps/track_variables_swing_full.xpm"
#include "pixmaps/track_variables_swing_grabber.xpm"
#include "pixmaps/track_variables_swing.xpm"

#include "pixmaps/track_top_bg.xpm"

#include "pixmaps/linked_block.xpm"

#include "pixmaps/default_skin/top__bpm_bottom.xpm"
#include "pixmaps/default_skin/top__bpm_display.xpm"
#include "pixmaps/default_skin/top__bpm_octave_separator.xpm"
#include "pixmaps/default_skin/top__bpm_top.xpm"
#include "pixmaps/default_skin/top__bpm_updown.xpm"
#include "pixmaps/default_skin/top__button_fastforward_active.xpm"
#include "pixmaps/default_skin/top__button_fastforward.xpm"
#include "pixmaps/default_skin/top__button_pause_active.xpm"
#include "pixmaps/default_skin/top__button_pause.xpm"
#include "pixmaps/default_skin/top__button_play_active.xpm"
#include "pixmaps/default_skin/top__button_play.xpm"
#include "pixmaps/default_skin/top__button_loop_active.xpm"
#include "pixmaps/default_skin/top__button_loop.xpm"
#include "pixmaps/default_skin/top__button_record_active.xpm"
#include "pixmaps/default_skin/top__button_record_automations_active.xpm"
#include "pixmaps/default_skin/top__button_record_automations.xpm"
#include "pixmaps/default_skin/top__button_record.xpm"
#include "pixmaps/default_skin/top__button_rewind_active.xpm"
#include "pixmaps/default_skin/top__button_rewind.xpm"
#include "pixmaps/default_skin/top__buttons_bottom.xpm"
#include "pixmaps/default_skin/top__buttons_bpm_separator.xpm"
#include "pixmaps/default_skin/top__button_stop_active.xpm"
#include "pixmaps/default_skin/top__button_stop.xpm"
#include "pixmaps/default_skin/top__buttons_top.xpm"
#include "pixmaps/default_skin/top__margin_bottom.xpm"
#include "pixmaps/default_skin/top__margin_left.xpm"
#include "pixmaps/default_skin/top__margin_right.xpm"
#include "pixmaps/default_skin/top__octave_bottom.xpm"
#include "pixmaps/default_skin/top__octave_display.xpm"
#include "pixmaps/default_skin/top__octave_end.xpm"
#include "pixmaps/default_skin/top__octave_top.xpm"
#include "pixmaps/default_skin/top__octave_updown.xpm"
#include "pixmaps/default_skin/top__tab_edit_active.xpm"
#include "pixmaps/default_skin/top__tab_edit.xpm"
#include "pixmaps/default_skin/top__tab_mix_active.xpm"
#include "pixmaps/default_skin/top__tab_mix.xpm"
#include "pixmaps/default_skin/top__tabs_buttons_separator.xpm"
#include "pixmaps/default_skin/top__tab_song_active.xpm"
#include "pixmaps/default_skin/top__tab_song.xpm"
#include "pixmaps/default_skin/top__controls_end.xpm"

#include "pixmaps/default_skin/top__icon_file_activated.xpm"
#include "pixmaps/default_skin/top__icon_file.xpm"
#include "pixmaps/default_skin/top__icon_add_activated.xpm"
#include "pixmaps/default_skin/top__icon_add.xpm"
#include "pixmaps/default_skin/top__icon_help_activated.xpm"
#include "pixmaps/default_skin/top__icon_help.xpm"
#include "pixmaps/default_skin/top__icon_redo_activated.xpm"
#include "pixmaps/default_skin/top__icon_redo_disabled.xpm"
#include "pixmaps/default_skin/top__icon_redo.xpm"
#include "pixmaps/default_skin/top__icon_settings_activated.xpm"
#include "pixmaps/default_skin/top__icon_settings.xpm"
#include "pixmaps/default_skin/top__icon_undo_activated.xpm"
#include "pixmaps/default_skin/top__icon_undo_disabled.xpm"
#include "pixmaps/default_skin/top__icon_undo.xpm"

#include "pixmaps/default_skin/left__margin.xpm"
#include "pixmaps/default_skin/right__margin.xpm"
#include "pixmaps/default_skin/bottom__margin.xpm"
#include "pixmaps/default_skin/bottom_left__corner.xpm"
#include "pixmaps/default_skin/bottom_right__corner.xpm"

#include "pixmaps/default_skin/global_toolbar__add_active.xpm"
#include "pixmaps/default_skin/global_toolbar__add.xpm"
#include "pixmaps/default_skin/global_toolbar__copy_active.xpm"
#include "pixmaps/default_skin/global_toolbar__copy_link_active.xpm"
#include "pixmaps/default_skin/global_toolbar__copy_link.xpm"
#include "pixmaps/default_skin/global_toolbar__copy.xpm"
#include "pixmaps/default_skin/global_toolbar__eraser_pushed.xpm"
#include "pixmaps/default_skin/global_toolbar__eraser.xpm"
#include "pixmaps/default_skin/global_toolbar__hspacer.xpm"
#include "pixmaps/default_skin/global_toolbar__left.xpm"
#include "pixmaps/default_skin/global_toolbar__right.xpm"
#include "pixmaps/default_skin/global_toolbar__select_active.xpm"
#include "pixmaps/default_skin/global_toolbar__select_linked_pushed.xpm"
#include "pixmaps/default_skin/global_toolbar__select_linked.xpm"
#include "pixmaps/default_skin/global_toolbar__select.xpm"
#include "pixmaps/default_skin/global_toolbar__unlink_selected_pushed.xpm"
#include "pixmaps/default_skin/global_toolbar__unlink_selected.xpm"
#include "pixmaps/default_skin/global_toolbar__zoom_bg.xpm"
#include "pixmaps/default_skin/global_toolbar__zoom_fg.xpm"
#include "pixmaps/default_skin/global_toolbar__zoom_grabber.xpm"
#include "pixmaps/default_skin/global_toolbar__zoom_icon.xpm"
#include "pixmaps/default_skin/global_toolbar__bottom_right_pixmap.xpm"
#include "pixmaps/default_skin/edit_toolbar__volume_mask_active.xpm"
#include "pixmaps/default_skin/edit_toolbar__volume_mask_label_bottom.xpm"
#include "pixmaps/default_skin/edit_toolbar__volume_mask_label_top.xpm"
#include "pixmaps/default_skin/edit_toolbar__volume_mask_label.xpm"
#include "pixmaps/default_skin/edit_toolbar__volume_mask.xpm"
#include "pixmaps/default_skin/edit_toolbar__automation_mode_envelope_active.xpm"
#include "pixmaps/default_skin/edit_toolbar__automation_mode_envelope.xpm"
#include "pixmaps/default_skin/edit_toolbar__automation_mode_stream_active.xpm"
#include "pixmaps/default_skin/edit_toolbar__automation_mode_stream.xpm"
#include "pixmaps/default_skin/edit_toolbar__begin.xpm"
#include "pixmaps/default_skin/edit_toolbar__cursor_follows_playback_pushed.xpm"
#include "pixmaps/default_skin/edit_toolbar__cursor_follows_playback.xpm"
#include "pixmaps/default_skin/edit_toolbar__cursor_stepping_dropdown.xpm"
#include "pixmaps/default_skin/edit_toolbar__cursor_stepping_dropdown_top.xpm"
#include "pixmaps/default_skin/edit_toolbar__cursor_stepping_dropdown_bottom.xpm"
#include "pixmaps/default_skin/edit_toolbar__cursor_stepping_icon.xpm"
#include "pixmaps/default_skin/edit_toolbar__edit_menu.xpm"
#include "pixmaps/default_skin/edit_toolbar__edit_menu_top.xpm"
#include "pixmaps/default_skin/edit_toolbar__edit_menu_bottom.xpm"
#include "pixmaps/default_skin/edit_toolbar__end.xpm"
#include "pixmaps/default_skin/edit_toolbar__midi_in_config_pushed.xpm"
#include "pixmaps/default_skin/edit_toolbar__midi_in_config.xpm"
#include "pixmaps/default_skin/edit_toolbar__play_current_block_pushed.xpm"
#include "pixmaps/default_skin/edit_toolbar__play_current_block.xpm"
#include "pixmaps/default_skin/edit_toolbar__play_from_cursor_pushed.xpm"
#include "pixmaps/default_skin/edit_toolbar__play_from_cursor.xpm"
#include "pixmaps/default_skin/edit_toolbar__snap_dropdown.xpm"
#include "pixmaps/default_skin/edit_toolbar__snap_dropdown_top.xpm"
#include "pixmaps/default_skin/edit_toolbar__snap_dropdown_bottom.xpm"
#include "pixmaps/default_skin/edit_toolbar__spacer.xpm"
#include "pixmaps/default_skin/edit_toolbar__separator.xpm"
#include "pixmaps/default_skin/scrollbar_arrow_down_pushed.xpm"
#include "pixmaps/default_skin/scrollbar_arrow_down.xpm"
#include "pixmaps/default_skin/scrollbar_arrow_left_pushed.xpm"
#include "pixmaps/default_skin/scrollbar_arrow_left.xpm"
#include "pixmaps/default_skin/scrollbar_arrow_right_pushed.xpm"
#include "pixmaps/default_skin/scrollbar_arrow_right.xpm"
#include "pixmaps/default_skin/scrollbar_arrow_up_pushed.xpm"
#include "pixmaps/default_skin/scrollbar_arrow_up.xpm"
#include "pixmaps/default_skin/scrollbar_grabber.xpm"
#include "pixmaps/default_skin/scrollbar_h_bg.xpm"
#include "pixmaps/default_skin/scrollbar_v_bg.xpm"

#include "pixmaps/default_skin/middle__separator_begin.xpm"
#include "pixmaps/default_skin/middle__separator_center.xpm"
#include "pixmaps/default_skin/middle__separator_end.xpm"


#include "pixmaps/default_skin/rack_plugin_top__begin.xpm"
#include "pixmaps/default_skin/rack_plugin_top__end.xpm"
#include "pixmaps/default_skin/rack_plugin_top__info_pushed.xpm"
#include "pixmaps/default_skin/rack_plugin_top__info.xpm"
#include "pixmaps/default_skin/rack_plugin_top__move_left_pushed.xpm"
#include "pixmaps/default_skin/rack_plugin_top__move_left.xpm"
#include "pixmaps/default_skin/rack_plugin_top__move_right_pushed.xpm"
#include "pixmaps/default_skin/rack_plugin_top__move_right.xpm"
#include "pixmaps/default_skin/rack_plugin_top__preset_name.xpm"
#include "pixmaps/default_skin/rack_plugin_top__preset_pushed.xpm"
#include "pixmaps/default_skin/rack_plugin_top__preset.xpm"
#include "pixmaps/default_skin/rack_plugin_top__remove_pushed.xpm"
#include "pixmaps/default_skin/rack_plugin_top__remove.xpm"
#include "pixmaps/default_skin/rack_plugin_top__separator.xpm"
#include "pixmaps/default_skin/rack_plugin_top__skip_active.xpm"
#include "pixmaps/default_skin/rack_plugin_top__skip.xpm"

#include "pixmaps/default_skin/rack_toolbar__add_plugin_pushed.xpm"
#include "pixmaps/default_skin/rack_toolbar__add_plugin.xpm"
#include "pixmaps/default_skin/rack_toolbar__begin.xpm"
#include "pixmaps/default_skin/rack_toolbar__end.xpm"
#include "pixmaps/default_skin/rack_toolbar__preset_file_pushed.xpm"
#include "pixmaps/default_skin/rack_toolbar__preset_file.xpm"
#include "pixmaps/default_skin/rack_toolbar__preset_icon.xpm"
#include "pixmaps/default_skin/rack_toolbar__preset_label_bottom.xpm"
#include "pixmaps/default_skin/rack_toolbar__preset_label_top.xpm"
#include "pixmaps/default_skin/rack_toolbar__preset_label.xpm"
#include "pixmaps/default_skin/rack_toolbar__separator.xpm"
#include "pixmaps/default_skin/rack_toolbar__show_back_active.xpm"
#include "pixmaps/default_skin/rack_toolbar__show_back.xpm"
#include "pixmaps/default_skin/rack_toolbar__show_front_active.xpm"
#include "pixmaps/default_skin/rack_toolbar__show_front.xpm"
#include "pixmaps/default_skin/rack_toolbar__spacer.xpm"
#include "pixmaps/default_skin/rack_toolbar__track_dropdown_bottom.xpm"
#include "pixmaps/default_skin/rack_toolbar__track_dropdown_top.xpm"
#include "pixmaps/default_skin/rack_toolbar__track_dropdown.xpm"
#include "pixmaps/default_skin/rack_panel__jack_in.xpm"
#include "pixmaps/default_skin/rack_panel__jack_out.xpm"
#include "pixmaps/default_skin/rack_panel__jack_forbidden.xpm"

#include "pixmaps/icon_file_open.xpm"
#include "pixmaps/icon_file_save.xpm"
#include "pixmaps/icon_file_saveas.xpm"
#include "pixmaps/icon_file_folder.xpm"
#include "pixmaps/icon_file_folder_new.xpm"

#include <iostream>

#include "error_macros.h"

namespace ReShaked {

VisualSettings * VisualSettings::singleton=NULL;
 
VisualSettings * VisualSettings::get_singleton() {

	return singleton;


}

int VisualSettings::get_editing_row_height() {

	return editing_row_height;
}

PixmapFont* VisualSettings::get_global_bar_font() {
	
	return &global_bar_font;
}

PixmapFont* VisualSettings::get_global_beat_font() {
	
	return &global_beat_font;
}

PixmapFont* VisualSettings::get_pattern_font() {

	return &pattern_font;
}
PixmapFont* VisualSettings::get_pattern_vol_font() {

	return &pattern_vol_font;
}

PixmapFont* VisualSettings::get_rowcount_font() {

	return &rowcount_font;
}

PixmapFont* VisualSettings::get_rowcount_subbeat_font() {

	return &rowcount_subbeat_font;
}

SkinBox * VisualSettings::get_skin_box(SkinBoxList p_which) {

	SkinBoxMap::iterator I=skin_box_map.find(p_which);

	ERR_FAIL_COND_V( I==skin_box_map.end() , NULL );

	return I->second;

}

int VisualSettings::get_selection_h_margin() {
	
	return 3;
}

int VisualSettings::get_rack_panel_h_margin() {
	
	return 22;
}
QColor VisualSettings::get_color(ColorList p_color) {

	ColorMap::iterator I=color_map.find(p_color);

	ERR_FAIL_COND_V( I==color_map.end() ,QColor(0,0,0) );

	return I->second;

}

QPixmap VisualSettings::get_pixmap(PixmapList p_pixmap) {
	
	PixmapMap::iterator I=pixmap_map.find(p_pixmap);

	ERR_FAIL_COND_V( I==pixmap_map.end() ,QPixmap() );

	return I->second;
	
}

int VisualSettings::get_constant(ConstantList p_constant) {
	
	ConstantMap::iterator I=constant_map.find(p_constant);

	ERR_FAIL_COND_V( I==constant_map.end() ,-1 );

	return I->second;
	
	
}

QPixmap VisualSettings::get_pattern_cursor_pixmap() {

	return pattern_cursor;
}

QPoint VisualSettings::get_pattern_cursor_offset() {

	return pattern_cursor_offset;
}

VisualSettings::VisualSettings() {

	singleton=this;    

	skin_box_map[SKINBOX_GLOBALVIEW_PATTERN]=new SkinBox((const char**)pattern_glob_bg_xpm,7,7,7,7,true);
	skin_box_map[SKINBOX_GLOBALVIEW_AUTOMATION]=new SkinBox((const char**)automation_glob_bg_xpm,7,7,7,7,true);
	skin_box_map[SKINBOX_GLOBALVIEW_PATTERN_SELECTED]=new SkinBox((const char**)pattern_glob_bg_sel_xpm,7,7,7,7,true);
	skin_box_map[SKINBOX_GLOBALVIEW_AUTOMATION_SELECTED]=new SkinBox((const char**)automation_glob_bg_sel_xpm,7,7,7,7,true);
	skin_box_map[SKINBOX_EDITING_PATTERN]=new SkinBox((const char**)pattern_bg_xpm,8,8,8,8);
	skin_box_map[SKINBOX_EDITING_PATTERN_SELECTED]=new SkinBox((const char**)pattern_bg_sel_xpm,8,8,8,8);
	skin_box_map[SKINBOX_EDITING_AUTOMATION]=new SkinBox((const char**)automation_bg_xpm,8,8,8,8);
	
	skin_box_map[SKINBOX_EDITING_AUTOMATION_SELECTED]=new SkinBox((const char**)automation_bg_sel_xpm,8,8,8,8);
	skin_box_map[SKINBOX_EDITING_PATTERN_SHARED]=new SkinBox((const char**)pattern_bg_shared_xpm,8,8,8,8);
	skin_box_map[SKINBOX_EDITING_AUTOMATION_SHARED]=new SkinBox((const char**)automation_bg_shared_xpm,8,8,8,8);
	
	skin_box_map[SKINBOX_EDITING_RESIZABLE_CURSOR]=new SkinBox((const char**)automation_cursor_xpm,5,5,5,5,true); 
	skin_box_map[SKINBOX_RACK_NORMAL]=new SkinBox((const char**)connection_panel_bg_xpm,26,31,20,33,true);
	skin_box_map[SKINBOX_RACK_SYSTEM]=new SkinBox((const char**)connection_panel_system_xpm,26,31,20,33,true);
	
	skin_box_map[SKINBOX_TRACK_TOP_BG]=new SkinBox((const char**)track_top_bg_xpm,6,6,6,6,true);
	
	skin_box_map[SKINBOX_THEME_SCROLLBAR_GRABBER]=new SkinBox((const char**)scrollbar_grabber_xpm,4,4,4,4,true);
	skin_box_map[SKINBOX_THEME_SCROLLBAR_H_BG]=new SkinBox((const char**)scrollbar_h_bg_xpm,6,6,6,6,true);
	skin_box_map[SKINBOX_THEME_SCROLLBAR_V_BG]=new SkinBox((const char**)scrollbar_v_bg_xpm,6,6,6,6,true);
	
	skin_box_map[SKINBOX_LINKED_BLOCK]=new SkinBox((const char**)linked_block_xpm,1,7,1,7);
	skin_box_map[SKINBOX_LINKED_BLOCK]->set_no_center_hint(true);
	
	
	color_map[COLORLIST_PATTERN_EDIT_NOTE]=QColor(0,33,0);
	color_map[COLORLIST_PATTERN_EDIT_VOL]=QColor(255,190,170);
	color_map[COLORLIST_PATTERN_EDIT_FOCUS_RECT]=QColor(255,0,0);
	color_map[COLORLIST_PATTERN_EDIT_BEAT_LINE]=QColor(200,233,255,128);
	color_map[COLORLIST_PATTERN_EDIT_SUBBEAT_LINE]=QColor(150,170,200,64);
	color_map[COLORLIST_AUTOMATION_ENVELOPE]=QColor(255,255,200,200);
	color_map[COLORLIST_GLOBAL_VIEW_BAR_LINE]=QColor(80,120,140);
	color_map[COLORLIST_GLOBAL_VIEW_BEAT_LINE]=QColor(50,60,80);
	color_map[COLORLIST_PATTERN_EDIT_BAR]=QColor(222,242,255,40);
	color_map[COLORLIST_EDITOR_SELECTION_AUTOMATION]=QColor(0x75,0x7f,0x09);
	color_map[COLORLIST_EDITOR_SELECTION_PATTERN]=QColor(0x28,0x56,0x7c);
	color_map[COLORLIST_RACK_CABLE]=QColor(0x97,0xd5,0x2b);
	color_map[COLORLIST_RACK_CABLE_DARK]=QColor(0x2b,0x8f,0x1a);
	color_map[COLORLIST_TRACK_TITLE]=QColor(200,233,255);
	color_map[COLORLIST_TRACK_TITLE_MUTE]=QColor(120,120,120);
	
	pixmap_map[PIXMAP_TRACK_OPTIONS]=QPixmap((const char**)track_options_xpm);
	pixmap_map[PIXMAP_KNOB_BASE]=QPixmap((const char**)knob_base_xpm);
	pixmap_map[PIXMAP_KNOB_HANDLE]=QPixmap((const char**)knob_handle_xpm);
	pixmap_map[PIXMAP_TRACK_SETTINGS_PATTERN]=QPixmap((const char**)track_settings_pattern_xpm);
	pixmap_map[PIXMAP_TRACK_SETTINGS_AUTOMATIONS]=QPixmap((const char**)track_settings_automations_xpm); 
	pixmap_map[PIXMAP_TRACK_SETTINGS_CONTROLS]=QPixmap((const char**)track_settings_controls_xpm);
	pixmap_map[PIXMAP_TRACK_SETTINGS_EFFECTS]=QPixmap((const char**)track_settings_effects_xpm);
	pixmap_map[PIXMAP_TRACK_SETTINGS_CONNECTIONS]=QPixmap((const char**)track_settings_connections_xpm);
	pixmap_map[PIXMAP_ENVELOPE_POINT]=QPixmap((const char**)envelope_point_xpm);
	pixmap_map[PIXMAP_RACK_JACK_HOLE]=QPixmap((const char**)jack_hole_xpm);
	pixmap_map[PIXMAP_TRACK_SLIDER_VU_BG]=QPixmap((const char**)track_variables_volume_vu_empty_xpm);
	pixmap_map[PIXMAP_TRACK_SLIDER_VU_FG]=QPixmap((const char**)track_variables_volume_vu_full_xpm);
	pixmap_map[PIXMAP_TRACK_SLIDER_VU_GRABBER]=QPixmap((const char**)track_variables_volume_vu_grabber_xpm);
	pixmap_map[PIXMAP_TRACK_SLIDER_VU_ICON]=QPixmap((const char**)track_variables_volume_xpm);
	pixmap_map[PIXMAP_TRACK_SLIDER_SWING_BG]=QPixmap((const char**)track_variables_swing_empty_xpm);
	pixmap_map[PIXMAP_TRACK_SLIDER_SWING_FG]=QPixmap((const char**)track_variables_swing_full_xpm);
	pixmap_map[PIXMAP_TRACK_SLIDER_SWING_GRABBER]=QPixmap((const char**)track_variables_swing_grabber_xpm);
	pixmap_map[PIXMAP_TRACK_SLIDER_SWING_ICON]=QPixmap((const char**)track_variables_swing_xpm);


	pixmap_map[ICON_MUTE]=QPixmap((const char**)icon_mute_xpm);
	pixmap_map[ICON_SOLO]=QPixmap((const char**)icon_solo_xpm);
	pixmap_map[ICON_FILE_OPEN]=QPixmap((const char**)icon_file_open_xpm);
	pixmap_map[ICON_FILE_SAVE]=QPixmap((const char**)icon_file_save_xpm);
	pixmap_map[ICON_FILE_SAVEAS]=QPixmap((const char**)icon_file_saveas_xpm);
	pixmap_map[ICON_FILE_FOLDER]=QPixmap((const char**)icon_file_folder_xpm);
	pixmap_map[ICON_FILE_FOLDER_NEW]=QPixmap((const char**)icon_file_folder_new_xpm);
	
	pixmap_map[ICON_TRACK_MOVE_LEFT]=QPixmap((const char**)track_move_left_xpm);
	pixmap_map[ICON_TRACK_MOVE_RIGHT]=QPixmap((const char**)track_move_right_xpm);
	pixmap_map[ICON_TRACK_DELETE]=QPixmap((const char**)track_delete_xpm);
	pixmap_map[ICON_TRACK_EDIT]=QPixmap((const char**)track_edit_xpm);
	pixmap_map[ICON_AUTOMATION_MOVE_LEFT]=QPixmap((const char**)track_move_automation_left_xpm);
	pixmap_map[ICON_AUTOMATION_MOVE_RIGHT]=QPixmap((const char**)track_move_automation_right_xpm);
	pixmap_map[ICON_COLUMN_ADD]=QPixmap((const char**)track_column_add_xpm);
	pixmap_map[ICON_COLUMN_REMOVE]=QPixmap((const char**)track_column_rem_xpm);
	pixmap_map[ICON_ZOOM_SMALL]=QPixmap((const char**)zoom_xpm);
			
	pixmap_map[ICON_GLOBAL_VIEW_SELECT_BLOCK]=QPixmap((const char**)global_edit_select_block_xpm);
	pixmap_map[ICON_GLOBAL_VIEW_ADD_BLOCK]=QPixmap((const char**)global_edit_add_block_xpm);
	pixmap_map[ICON_GLOBAL_VIEW_COPY_BLOCK]=QPixmap((const char**)global_edit_copy_block_xpm);
	pixmap_map[ICON_GLOBAL_VIEW_COPY_LINK_BLOCK]=QPixmap((const char**)global_edit_copy_block_linked_xpm);
	pixmap_map[ICON_GLOBAL_VIEW_ERASE_BLOCK]=QPixmap((const char**)global_edit_delete_block_xpm);
	
	pixmap_map[ICON_CURSOR_ADD_BLOCK]=QPixmap((const char**)cursor_add_block_xpm);
	
	pixmap_map[ICON_CONTROL_RW]=QPixmap((const char**)control_rw_xpm);
	pixmap_map[ICON_CONTROL_PLAY]=QPixmap((const char**)control_play_xpm);
	pixmap_map[ICON_CONTROL_PLAY_LOOP]=QPixmap((const char**)control_play_loop_xpm);
	pixmap_map[ICON_CONTROL_PLAY_BLOCK]=QPixmap((const char**)control_play_xpm);
	pixmap_map[ICON_CONTROL_PLAY_CURSOR]=QPixmap((const char**)control_play_cursor_xpm);
	pixmap_map[ICON_CONTROL_PLAY_BLOCK]=QPixmap((const char**)control_play_block_xpm);
	pixmap_map[ICON_CONTROL_PAUSE]=QPixmap((const char**)control_pause_xpm);
	pixmap_map[ICON_CONTROL_STOP]=QPixmap((const char**)control_stop_xpm);
	pixmap_map[ICON_CONTROL_FF]=QPixmap((const char**)control_ff_xpm);
	pixmap_map[ICON_CONTROL_RECORD]=QPixmap((const char**)control_record_xpm);
	pixmap_map[ICON_CONTROL_RECORD_AUTOMATIONS]=QPixmap((const char**)control_record_automations_xpm);
		
	pixmap_map[THEME_TOP__BPM_BOTTOM]=QPixmap((const char**)top__bpm_bottom_xpm);
	pixmap_map[THEME_TOP__BPM_DISPLAY]=QPixmap((const char**)top__bpm_display_xpm);
	pixmap_map[THEME_TOP__BPM_OCTAVE_SEPARATOR]=QPixmap((const char**)top__bpm_octave_separator_xpm);
	pixmap_map[THEME_TOP__BPM_TOP]=QPixmap((const char**)top__bpm_top_xpm);
	pixmap_map[THEME_TOP__BPM_UPDOWN]=QPixmap((const char**)top__bpm_updown_xpm);
	pixmap_map[THEME_TOP__BUTTON_FASTFORWARD_ACTIVE]=QPixmap((const char**)top__button_fastforward_active_xpm);
	pixmap_map[THEME_TOP__BUTTON_FASTFORWARD]=QPixmap((const char**)top__button_fastforward_xpm);
	pixmap_map[THEME_TOP__BUTTON_PAUSE_ACTIVE]=QPixmap((const char**)top__button_pause_active_xpm);
	pixmap_map[THEME_TOP__BUTTON_PAUSE]=QPixmap((const char**)top__button_pause_xpm);
	pixmap_map[THEME_TOP__BUTTON_PLAY_ACTIVE]=QPixmap((const char**)top__button_play_active_xpm);
	pixmap_map[THEME_TOP__BUTTON_PLAY]=QPixmap((const char**)top__button_play_xpm);
	pixmap_map[THEME_TOP__BUTTON_LOOP_ACTIVE]=QPixmap((const char**)top__button_loop_active_xpm);
	pixmap_map[THEME_TOP__BUTTON_LOOP]=QPixmap((const char**)top__button_loop_xpm);
	pixmap_map[THEME_TOP__BUTTON_RECORD_ACTIVE]=QPixmap((const char**)top__button_record_active_xpm);
	pixmap_map[THEME_TOP__BUTTON_RECORD_AUTOMATIONS_ACTIVE]=QPixmap((const char**)top__button_record_automations_active_xpm);
	pixmap_map[THEME_TOP__BUTTON_RECORD_AUTOMATIONS]=QPixmap((const char**)top__button_record_automations_xpm);
	pixmap_map[THEME_TOP__BUTTON_RECORD]=QPixmap((const char**)top__button_record_xpm);
	pixmap_map[THEME_TOP__BUTTON_REWIND_ACTIVE]=QPixmap((const char**)top__button_rewind_active_xpm);
	pixmap_map[THEME_TOP__BUTTON_REWIND]=QPixmap((const char**)top__button_rewind_xpm);
	pixmap_map[THEME_TOP__BUTTONS_BOTTOM]=QPixmap((const char**)top__buttons_bottom_xpm);
	pixmap_map[THEME_TOP__BUTTONS_BPM_SEPARATOR]=QPixmap((const char**)top__buttons_bpm_separator_xpm);
	pixmap_map[THEME_TOP__BUTTON_STOP_ACTIVE]=QPixmap((const char**)top__button_stop_active_xpm);
	pixmap_map[THEME_TOP__BUTTON_STOP]=QPixmap((const char**)top__button_stop_xpm);
	pixmap_map[THEME_TOP__BUTTONS_TOP]=QPixmap((const char**)top__buttons_top_xpm);
	pixmap_map[THEME_TOP__MARGIN_BOTTOM]=QPixmap((const char**)top__margin_bottom_xpm);
	pixmap_map[THEME_TOP__MARGIN_LEFT]=QPixmap((const char**)top__margin_left_xpm);
	pixmap_map[THEME_TOP__MARGIN_RIGHT]=QPixmap((const char**)top__margin_right_xpm);
	pixmap_map[THEME_TOP__OCTAVE_BOTTOM]=QPixmap((const char**)top__octave_bottom_xpm);
	pixmap_map[THEME_TOP__OCTAVE_DISPLAY]=QPixmap((const char**)top__octave_display_xpm);
	pixmap_map[THEME_TOP__OCTAVE_END]=QPixmap((const char**)top__octave_end_xpm);
	pixmap_map[THEME_TOP__OCTAVE_TOP]=QPixmap((const char**)top__octave_top_xpm);
	pixmap_map[THEME_TOP__OCTAVE_UPDOWN]=QPixmap((const char**)top__octave_updown_xpm);
	pixmap_map[THEME_TOP__TAB_EDIT_ACTIVE]=QPixmap((const char**)top__tab_edit_active_xpm);
	pixmap_map[THEME_TOP__TAB_EDIT]=QPixmap((const char**)top__tab_edit_xpm);
	pixmap_map[THEME_TOP__TAB_MIX_ACTIVE]=QPixmap((const char**)top__tab_mix_active_xpm);
	pixmap_map[THEME_TOP__TAB_MIX]=QPixmap((const char**)top__tab_mix_xpm);
	pixmap_map[THEME_TOP__TABS_BUTTONS_SEPARATOR]=QPixmap((const char**)top__tabs_buttons_separator_xpm);
	pixmap_map[THEME_TOP__TAB_SONG_ACTIVE]=QPixmap((const char**)top__tab_song_active_xpm);
	pixmap_map[THEME_TOP__TAB_SONG]=QPixmap((const char**)top__tab_song_xpm);
	pixmap_map[THEME_TOP__CONTROLS_END]=QPixmap((const char**)top__controls_end_xpm);

	pixmap_map[THEME_TOP__ICON_FILE_ACTIVATED]=QPixmap((const char**)top__icon_file_activated_xpm);
	pixmap_map[THEME_TOP__ICON_FILE]=QPixmap((const char**)top__icon_file_xpm);
	pixmap_map[THEME_TOP__ICON_ADD_ACTIVATED]=QPixmap((const char**)top__icon_add_activated_xpm);
	pixmap_map[THEME_TOP__ICON_ADD]=QPixmap((const char**)top__icon_add_xpm);
	pixmap_map[THEME_TOP__ICON_HELP_ACTIVATED]=QPixmap((const char**)top__icon_help_activated_xpm);
	pixmap_map[THEME_TOP__ICON_HELP]=QPixmap((const char**)top__icon_help_xpm);
	pixmap_map[THEME_TOP__ICON_REDO_ACTIVATED]=QPixmap((const char**)top__icon_redo_activated_xpm);
	pixmap_map[THEME_TOP__ICON_REDO_DISABLED]=QPixmap((const char**)top__icon_redo_disabled_xpm);
	pixmap_map[THEME_TOP__ICON_REDO]=QPixmap((const char**)top__icon_redo_xpm);
	pixmap_map[THEME_TOP__ICON_SETTINGS_ACTIVATED]=QPixmap((const char**)top__icon_settings_activated_xpm);
	pixmap_map[THEME_TOP__ICON_SETTINGS]=QPixmap((const char**)top__icon_settings_xpm);
	pixmap_map[THEME_TOP__ICON_UNDO_ACTIVATED]=QPixmap((const char**)top__icon_undo_activated_xpm);
	pixmap_map[THEME_TOP__ICON_UNDO_DISABLED]=QPixmap((const char**)top__icon_undo_disabled_xpm);
	pixmap_map[THEME_TOP__ICON_UNDO]=QPixmap((const char**)top__icon_undo_xpm);
	
	pixmap_map[THEME_LEFT__MARGIN]=QPixmap((const char**)left__margin_xpm);
	pixmap_map[THEME_RIGHT__MARGIN]=QPixmap((const char**)right__margin_xpm);
	pixmap_map[THEME_BOTTOM__MARGIN]=QPixmap((const char**)bottom__margin_xpm);
	pixmap_map[THEME_BOTTOM_LEFT__CORNER]=QPixmap((const char**)bottom_left__corner_xpm);
	pixmap_map[THEME_BOTTOM_RIGHT__CORNER]=QPixmap((const char**)bottom_right__corner_xpm);
	
	pixmap_map[THEME_GLOBAL_TOOLBAR__ADD_ACTIVE]=QPixmap((const char**)global_toolbar__add_active_xpm);
	pixmap_map[THEME_GLOBAL_TOOLBAR__ADD]=QPixmap((const char**)global_toolbar__add_xpm);
	pixmap_map[THEME_GLOBAL_TOOLBAR__COPY_ACTIVE]=QPixmap((const char**)global_toolbar__copy_active_xpm);
	pixmap_map[THEME_GLOBAL_TOOLBAR__COPY_LINK_ACTIVE]=QPixmap((const char**)global_toolbar__copy_link_active_xpm);
	pixmap_map[THEME_GLOBAL_TOOLBAR__COPY_LINK]=QPixmap((const char**)global_toolbar__copy_link_xpm);
	pixmap_map[THEME_GLOBAL_TOOLBAR__COPY]=QPixmap((const char**)global_toolbar__copy_xpm);
	pixmap_map[THEME_GLOBAL_TOOLBAR__ERASER_PUSHED]=QPixmap((const char**)global_toolbar__eraser_pushed_xpm);
	pixmap_map[THEME_GLOBAL_TOOLBAR__ERASER]=QPixmap((const char**)global_toolbar__eraser_xpm);
	pixmap_map[THEME_GLOBAL_TOOLBAR__HSPACER]=QPixmap((const char**)global_toolbar__hspacer_xpm);
	pixmap_map[THEME_GLOBAL_TOOLBAR__LEFT]=QPixmap((const char**)global_toolbar__left_xpm);
	pixmap_map[THEME_GLOBAL_TOOLBAR__RIGHT]=QPixmap((const char**)global_toolbar__right_xpm);
	pixmap_map[THEME_GLOBAL_TOOLBAR__SELECT_ACTIVE]=QPixmap((const char**)global_toolbar__select_active_xpm);
	pixmap_map[THEME_GLOBAL_TOOLBAR__SELECT_LINKED_PUSHED]=QPixmap((const char**)global_toolbar__select_linked_pushed_xpm);
	pixmap_map[THEME_GLOBAL_TOOLBAR__SELECT_LINKED]=QPixmap((const char**)global_toolbar__select_linked_xpm);
	pixmap_map[THEME_GLOBAL_TOOLBAR__SELECT]=QPixmap((const char**)global_toolbar__select_xpm);
	pixmap_map[THEME_GLOBAL_TOOLBAR__UNLINK_SELECTED_PUSHED]=QPixmap((const char**)global_toolbar__unlink_selected_pushed_xpm);
	pixmap_map[THEME_GLOBAL_TOOLBAR__UNLINK_SELECTED]=QPixmap((const char**)global_toolbar__unlink_selected_xpm);
	pixmap_map[THEME_GLOBAL_TOOLBAR__ZOOM_BG]=QPixmap((const char**)global_toolbar__zoom_bg_xpm);
	pixmap_map[THEME_GLOBAL_TOOLBAR__ZOOM_FG]=QPixmap((const char**)global_toolbar__zoom_fg_xpm);
	pixmap_map[THEME_GLOBAL_TOOLBAR__ZOOM_GRABBER]=QPixmap((const char**)global_toolbar__zoom_grabber_xpm);
	pixmap_map[THEME_GLOBAL_TOOLBAR__ZOOM_ICON]=QPixmap((const char**)global_toolbar__zoom_icon_xpm);
	pixmap_map[THEME_GLOBAL_TOOLBAR__BOTTOM_RIGHT_PIXMAP]=QPixmap((const char**)global_toolbar__bottom_right_pixmap_xpm);
	
	
	pixmap_map[THEME_EDIT_TOOLBAR__VOLUME_MASK]=QPixmap((const char**)edit_toolbar__volume_mask_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__VOLUME_MASK_ACTIVE]=QPixmap((const char**)edit_toolbar__volume_mask_active_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__VOLUME_MASK_LABEL]=QPixmap((const char**)edit_toolbar__volume_mask_label_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__VOLUME_MASK_LABEL_TOP]=QPixmap((const char**)edit_toolbar__volume_mask_label_top_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__VOLUME_MASK_LABEL_BOTTOM]=QPixmap((const char**)edit_toolbar__volume_mask_label_bottom_xpm);
	
	pixmap_map[THEME_EDIT_TOOLBAR__AUTOMATION_MODE_ENVELOPE_ACTIVE]=QPixmap((const char**)edit_toolbar__automation_mode_envelope_active_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__AUTOMATION_MODE_ENVELOPE]=QPixmap((const char**)edit_toolbar__automation_mode_envelope_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__AUTOMATION_MODE_STREAM_ACTIVE]=QPixmap((const char**)edit_toolbar__automation_mode_stream_active_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__AUTOMATION_MODE_STREAM]=QPixmap((const char**)edit_toolbar__automation_mode_stream_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__BEGIN]=QPixmap((const char**)edit_toolbar__begin_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__CURSOR_FOLLOWS_PLAYBACK_PUSHED]=QPixmap((const char**)edit_toolbar__cursor_follows_playback_pushed_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__CURSOR_FOLLOWS_PLAYBACK]=QPixmap((const char**)edit_toolbar__cursor_follows_playback_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__CURSOR_STEPPING_DROPDOWN]=QPixmap((const char**)edit_toolbar__cursor_stepping_dropdown_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__CURSOR_STEPPING_DROPDOWN_TOP]=QPixmap((const char**)edit_toolbar__cursor_stepping_dropdown_top_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__CURSOR_STEPPING_DROPDOWN_BOTTOM]=QPixmap((const char**)edit_toolbar__cursor_stepping_dropdown_bottom_xpm);	
	pixmap_map[THEME_EDIT_TOOLBAR__CURSOR_STEPPING_ICON]=QPixmap((const char**)edit_toolbar__cursor_stepping_icon_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__EDIT_MENU]=QPixmap((const char**)edit_toolbar__edit_menu_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__EDIT_MENU_TOP]=QPixmap((const char**)edit_toolbar__edit_menu_top_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__EDIT_MENU_BOTTOM]=QPixmap((const char**)edit_toolbar__edit_menu_bottom_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__END]=QPixmap((const char**)edit_toolbar__end_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__MIDI_IN_CONFIG_PUSHED]=QPixmap((const char**)edit_toolbar__midi_in_config_pushed_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__MIDI_IN_CONFIG]=QPixmap((const char**)edit_toolbar__midi_in_config_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__PLAY_CURRENT_BLOCK_PUSHED]=QPixmap((const char**)edit_toolbar__play_current_block_pushed_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__PLAY_CURRENT_BLOCK]=QPixmap((const char**)edit_toolbar__play_current_block_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__PLAY_FROM_CURSOR_PUSHED]=QPixmap((const char**)edit_toolbar__play_from_cursor_pushed_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__PLAY_FROM_CURSOR]=QPixmap((const char**)edit_toolbar__play_from_cursor_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__SNAP_DROPDOWN]=QPixmap((const char**)edit_toolbar__snap_dropdown_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__SNAP_DROPDOWN_TOP]=QPixmap((const char**)edit_toolbar__snap_dropdown_top_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__SNAP_DROPDOWN_BOTTOM]=QPixmap((const char**)edit_toolbar__snap_dropdown_bottom_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__SPACER]=QPixmap((const char**)edit_toolbar__spacer_xpm);
	pixmap_map[THEME_EDIT_TOOLBAR__SEPARATOR]=QPixmap((const char**)edit_toolbar__separator_xpm);
	
	pixmap_map[THEME_MIDDLE__SEPARATOR_BEGIN]=QPixmap((const char**)middle__separator_begin_xpm);
	pixmap_map[THEME_MIDDLE__SEPARATOR_CENTER]=QPixmap((const char**)middle__separator_center_xpm);
	pixmap_map[THEME_MIDDLE__SEPARATOR_END]=QPixmap((const char**)middle__separator_end_xpm);
	
	
	pixmap_map[THEME_RACK_TOOLBAR__ADD_PLUGIN_PUSHED]=QPixmap((const char**)rack_toolbar__add_plugin_pushed_xpm);
	pixmap_map[THEME_RACK_TOOLBAR__ADD_PLUGIN]=QPixmap((const char**)rack_toolbar__add_plugin_xpm);
	pixmap_map[THEME_RACK_TOOLBAR__BEGIN]=QPixmap((const char**)rack_toolbar__begin_xpm);
	pixmap_map[THEME_RACK_TOOLBAR__END]=QPixmap((const char**)rack_toolbar__end_xpm);
	pixmap_map[THEME_RACK_TOOLBAR__PRESET_FILE_PUSHED]=QPixmap((const char**)rack_toolbar__preset_file_pushed_xpm);
	pixmap_map[THEME_RACK_TOOLBAR__PRESET_FILE]=QPixmap((const char**)rack_toolbar__preset_file_xpm);
	pixmap_map[THEME_RACK_TOOLBAR__PRESET_ICON]=QPixmap((const char**)rack_toolbar__preset_icon_xpm);
	pixmap_map[THEME_RACK_TOOLBAR__PRESET_LABEL_BOTTOM]=QPixmap((const char**)rack_toolbar__preset_label_bottom_xpm);
	pixmap_map[THEME_RACK_TOOLBAR__PRESET_LABEL_TOP]=QPixmap((const char**)rack_toolbar__preset_label_top_xpm);
	pixmap_map[THEME_RACK_TOOLBAR__PRESET_LABEL]=QPixmap((const char**)rack_toolbar__preset_label_xpm);
	pixmap_map[THEME_RACK_TOOLBAR__SEPARATOR]=QPixmap((const char**)rack_toolbar__separator_xpm);
	pixmap_map[THEME_RACK_TOOLBAR__SHOW_BACK_ACTIVE]=QPixmap((const char**)rack_toolbar__show_back_active_xpm);
	pixmap_map[THEME_RACK_TOOLBAR__SHOW_BACK]=QPixmap((const char**)rack_toolbar__show_back_xpm);
	pixmap_map[THEME_RACK_TOOLBAR__SHOW_FRONT_ACTIVE]=QPixmap((const char**)rack_toolbar__show_front_active_xpm);
	pixmap_map[THEME_RACK_TOOLBAR__SHOW_FRONT]=QPixmap((const char**)rack_toolbar__show_front_xpm);
	pixmap_map[THEME_RACK_TOOLBAR__SPACER]=QPixmap((const char**)rack_toolbar__spacer_xpm);
	pixmap_map[THEME_RACK_TOOLBAR__TRACK_DROPDOWN_BOTTOM]=QPixmap((const char**)rack_toolbar__track_dropdown_bottom_xpm);
	pixmap_map[THEME_RACK_TOOLBAR__TRACK_DROPDOWN_TOP]=QPixmap((const char**)rack_toolbar__track_dropdown_top_xpm);
	pixmap_map[THEME_RACK_TOOLBAR__TRACK_DROPDOWN]=QPixmap((const char**)rack_toolbar__track_dropdown_xpm);
	
	pixmap_map[THEME_RACK_PANEL__JACK_IN]=QPixmap((const char**)rack_panel__jack_in_xpm);
	pixmap_map[THEME_RACK_PANEL__JACK_OUT]=QPixmap((const char**)rack_panel__jack_out_xpm);
	pixmap_map[THEME_RACK_PANEL__JACK_FORBIDDEN]=QPixmap((const char**)rack_panel__jack_forbidden_xpm);

	
	pixmap_map[THEME_RACK_PLUGIN_TOP__BEGIN]=QPixmap((const char**)rack_plugin_top__begin_xpm);
	pixmap_map[THEME_RACK_PLUGIN_TOP__END]=QPixmap((const char**)rack_plugin_top__end_xpm);
	pixmap_map[THEME_RACK_PLUGIN_TOP__INFO_PUSHED]=QPixmap((const char**)rack_plugin_top__info_pushed_xpm);
	pixmap_map[THEME_RACK_PLUGIN_TOP__INFO]=QPixmap((const char**)rack_plugin_top__info_xpm);
	pixmap_map[THEME_RACK_PLUGIN_TOP__MOVE_LEFT_PUSHED]=QPixmap((const char**)rack_plugin_top__move_left_pushed_xpm);
	pixmap_map[THEME_RACK_PLUGIN_TOP__MOVE_LEFT]=QPixmap((const char**)rack_plugin_top__move_left_xpm);
	pixmap_map[THEME_RACK_PLUGIN_TOP__MOVE_RIGHT_PUSHED]=QPixmap((const char**)rack_plugin_top__move_right_pushed_xpm);
	pixmap_map[THEME_RACK_PLUGIN_TOP__MOVE_RIGHT]=QPixmap((const char**)rack_plugin_top__move_right_xpm);
	pixmap_map[THEME_RACK_PLUGIN_TOP__PRESET_NAME]=QPixmap((const char**)rack_plugin_top__preset_name_xpm);
	pixmap_map[THEME_RACK_PLUGIN_TOP__PRESET_PUSHED]=QPixmap((const char**)rack_plugin_top__preset_pushed_xpm);
	pixmap_map[THEME_RACK_PLUGIN_TOP__PRESET]=QPixmap((const char**)rack_plugin_top__preset_xpm);
	pixmap_map[THEME_RACK_PLUGIN_TOP__REMOVE_PUSHED]=QPixmap((const char**)rack_plugin_top__remove_pushed_xpm);
	pixmap_map[THEME_RACK_PLUGIN_TOP__REMOVE]=QPixmap((const char**)rack_plugin_top__remove_xpm);
	pixmap_map[THEME_RACK_PLUGIN_TOP__SEPARATOR]=QPixmap((const char**)rack_plugin_top__separator_xpm);
	pixmap_map[THEME_RACK_PLUGIN_TOP__SKIP_ACTIVE]=QPixmap((const char**)rack_plugin_top__skip_active_xpm);
	pixmap_map[THEME_RACK_PLUGIN_TOP__SKIP]=QPixmap((const char**)rack_plugin_top__skip_xpm);
	
	
	pixmap_map[THEME_SCROLLBAR_ARROW_DOWN_PUSHED]=QPixmap((const char**)scrollbar_arrow_down_pushed_xpm);
	pixmap_map[THEME_SCROLLBAR_ARROW_DOWN]=QPixmap((const char**)scrollbar_arrow_down_xpm);
	pixmap_map[THEME_SCROLLBAR_ARROW_LEFT_PUSHED]=QPixmap((const char**)scrollbar_arrow_left_pushed_xpm);
	pixmap_map[THEME_SCROLLBAR_ARROW_LEFT]=QPixmap((const char**)scrollbar_arrow_left_xpm);
	pixmap_map[THEME_SCROLLBAR_ARROW_RIGHT_PUSHED]=QPixmap((const char**)scrollbar_arrow_right_pushed_xpm);
	pixmap_map[THEME_SCROLLBAR_ARROW_RIGHT]=QPixmap((const char**)scrollbar_arrow_right_xpm);
	pixmap_map[THEME_SCROLLBAR_ARROW_UP_PUSHED]=QPixmap((const char**)scrollbar_arrow_up_pushed_xpm);
	pixmap_map[THEME_SCROLLBAR_ARROW_UP]=QPixmap((const char**)scrollbar_arrow_up_xpm);
	
	
	
	
	pixmap_map[ICON_EDIT_VIEW_ROW_PLAYED]=QPixmap((const char**)row_icon_play_xpm);
	pixmap_map[ICON_EDIT_VIEW_ROW_LOOP_BEGIN]=QPixmap((const char**)row_icon_loop_begin_xpm);
	pixmap_map[ICON_EDIT_VIEW_ROW_LOOP_END]=QPixmap((const char**)row_icon_loop_end_xpm);
	
	
	constant_map[CONSTANT_EDIT_TOOLBAR_FONT_HEIGHT]=10;
	constant_map[CONSTANT_RACK_MINIMUM_HEIGHT]=176;
	constant_map[CONSTANT_PLUGIN_CHOOSER_ITEM_MARGIN]=8;
	
	pattern_font.set_font( QPixmap((const char**)pattern_font_xpm), "ABCDEFG#0123456789-*");
	pattern_vol_font.set_font( QPixmap((const char**)pattern_vol_font_xpm), "ABCDEFG#0123456789-*"); 

	rowcount_font.set_font( QPixmap((const char**)rowcount_font_xpm), "ABCDEFG#0123456789-*"); 
	rowcount_subbeat_font.set_font( QPixmap((const char**)rowcount_font_subbeat_xpm), "ABCDEFG#0123456789-*"); 
	global_bar_font.set_font( QPixmap((const char**)font_global_bar_xpm), "0123456789BEATR"); 
	global_beat_font.set_font( QPixmap((const char**)font_global_beat_xpm), "0123456789BEATR"); 
	editing_row_height=pattern_font.get_height()+5;
	pattern_cursor=QPixmap(pattern_cursor_xpm);
	pattern_cursor_offset=QPoint(-3,-3); 



}


VisualSettings::~VisualSettings()
{
}


}
  