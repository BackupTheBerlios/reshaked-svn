//
// C++ Implementation: track_editor_track_name
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "track_editor_track_name.h"
#include "gui_common/common_skin.h"
#include "engine/pattern_track.h"
#include "editor/edit_commands.h"
#include "editor/key_bindings.h"


GUI::Size TrackEditorTrackName::get_minimum_size_internal() {

	return GUI::Size( MIN( get_painter()->get_bitmap_size( bitmap(BITMAP_ICON_TRACK_VISIBLE) ).width, get_painter()->get_font_height( font( FONT_TRACK_EDITOR_TRACK_NAME ) ) ) )+get_painter()->get_stylebox_min_size( stylebox( SB_TRACK_EDITOR_TRACK_NAME ) );
}

void TrackEditorTrackName::draw(const GUI::Point& p_pos,const GUI::Size& p_size,const GUI::Rect& p_exposed) {

	get_painter()->draw_stylebox( stylebox( SB_TRACK_EDITOR_TRACK_NAME ), GUI::Point(), p_size );

	GUI::Point ofs(
		get_painter()->get_stylebox_margin( stylebox( SB_TRACK_EDITOR_TRACK_NAME ), GUI::MARGIN_LEFT ), 
		get_painter()->get_stylebox_margin( stylebox( SB_TRACK_EDITOR_TRACK_NAME ), GUI::MARGIN_TOP ) );
		
	
	GUI::BitmapID bmp;
	
	if (track_editor->get_track()->is_collapsed()) {
	
		bmp=bitmap(over==0?BITMAP_ICON_TRACK_COLLAPSED_HL:BITMAP_ICON_TRACK_COLLAPSED);
	} else {
	
		bmp=bitmap(over==0?BITMAP_ICON_TRACK_VISIBLE_HL:BITMAP_ICON_TRACK_VISIBLE);
	}
	
	get_painter()->draw_bitmap( bmp, ofs );
	
	ofs.y+=get_painter()->get_bitmap_size( bmp ).height;
	
	
	get_painter()->draw_bitmap( bitmap( over==1?BITMAP_ICON_TRACK_OPTIONS_HL:BITMAP_ICON_TRACK_OPTIONS), ofs );
	
	ofs.y+=get_painter()->get_bitmap_size( bitmap(BITMAP_ICON_TRACK_OPTIONS) ).height*2;
	
	ofs.x+=get_painter()->get_font_descent( font( FONT_TRACK_EDITOR_TRACK_NAME ) );
	
	GUI::Color text_col=GUI::Color(255);
	
	if (dynamic_cast<PatternTrack*>( track_editor->get_track() ))
		text_col=color( COLOR_PATTERN_FONT );
	
	get_painter()->draw_text( font( FONT_TRACK_EDITOR_TRACK_NAME ), ofs, track_editor->get_track()->get_name(), GUI::DOWN, text_col );	
	
}

void TrackEditorTrackName::mouse_button(const GUI::Point& p_pos, int p_button,bool p_press,int p_modifier_mask) {

	if (!p_press)
		return;
	int y=p_pos.y;
	
	y-=get_painter()->get_stylebox_margin( stylebox( SB_TRACK_EDITOR_TRACK_NAME ), GUI::MARGIN_TOP );
		
	// test collapse
	if (y<get_painter()->get_bitmap_size( bitmap(BITMAP_ICON_TRACK_COLLAPSED) ).height ) {
	
		EditCommands::get_singleton()->track_collapse( track_editor->get_track(), !track_editor->get_track()->is_collapsed() );
		return;
	}
	
	y-=get_painter()->get_bitmap_size( bitmap(BITMAP_ICON_TRACK_COLLAPSED) ).height;
	// test options
	
	if (y<get_painter()->get_bitmap_size( bitmap(BITMAP_ICON_TRACK_COLLAPSED) ).height ) {
	
		GUI::Point popup_pos;
		popup_pos.y=get_painter()->get_stylebox_margin( stylebox( SB_TRACK_EDITOR_TRACK_NAME ), GUI::MARGIN_TOP )+get_painter()->get_bitmap_size( bitmap(BITMAP_ICON_TRACK_COLLAPSED) ).height*2;
		popup_pos.x+=get_painter()->get_stylebox_margin( stylebox( SB_TRACK_EDITOR_TRACK_NAME ), GUI::MARGIN_LEFT);
		popup_pos+=get_global_pos();
		
		menu->popup( popup_pos );
	}
	
	
}

void TrackEditorTrackName::mouse_motion(const GUI::Point& p_pos, const GUI::Point& p_rel, int p_button_mask) {

	int y=p_pos.y;
	
	y-=get_painter()->get_stylebox_margin( stylebox( SB_TRACK_EDITOR_TRACK_NAME ), GUI::MARGIN_TOP );
		
	// test collapse
	if (y<get_painter()->get_bitmap_size( bitmap(BITMAP_ICON_TRACK_COLLAPSED) ).height ) {
	
		over=0;
		update();
		return;
	}
	
	y-=get_painter()->get_bitmap_size( bitmap(BITMAP_ICON_TRACK_COLLAPSED) ).height;
	// test options
	if (y<get_painter()->get_bitmap_size( bitmap(BITMAP_ICON_TRACK_COLLAPSED) ).height ) {

		over=1;
		update();
		return;
	}
	if (over!=-1)
		update();
	over=-1;
}
void TrackEditorTrackName::mouse_leave() {

	over=-1;
	update();
}
	
	
void TrackEditorTrackName::menu_callback(int p_option) {

	switch(p_option) {
		case TRACK_MUTE: {
		
		
		} break;
		case TRACK_SOLO: {
		
		
		} break;
		case PATTERN_ADD_COLUMN: {
			if (dynamic_cast<PatternTrack*>(track_editor->get_track())) {
		
				PatternTrack* pt = dynamic_cast<PatternTrack*>(track_editor->get_track());
				EditCommands::get_singleton()->pattern_set_visible_columns( pt, pt->get_visible_columns() +1 );
			}
		} break;
		case PATTERN_REMOVE_COLUMN: {
		
			if (dynamic_cast<PatternTrack*>(track_editor->get_track())) {
		
				PatternTrack* pt = dynamic_cast<PatternTrack*>(track_editor->get_track());
				EditCommands::get_singleton()->pattern_set_visible_columns( pt, pt->get_visible_columns() -1 );
			}
		
		} break;
		case TRACK_MOVE_LEFT: {
		
			int idx = Editor::get_singleton()->get_song()->find_track_pos( track_editor->get_track() );
			
			EditCommands::get_singleton()->song_swap_tracks( Editor::get_singleton()->get_song(), idx, idx -1 );
			
		} break;
		case TRACK_MOVE_RIGHT: {
		
			int idx = Editor::get_singleton()->get_song()->find_track_pos( track_editor->get_track() );
			
			EditCommands::get_singleton()->song_swap_tracks( Editor::get_singleton()->get_song(), idx, idx +1 );
		
		} break;
		case TRACK_RENAME: {
		
			name_input->show("Rename Track:         ",track_editor->get_track()->get_name() );		
		} break;
		case TRACK_REMOVE: {
		
			EditCommands::get_singleton()->audio_graph_remove_node( Editor::get_singleton()->get_song()->get_audio_graph(), 
			Editor::get_singleton()->get_song()->get_audio_graph()->get_node_index( track_editor->get_track()) );
		
		} break;
	}
}
	
void TrackEditorTrackName::about_to_show_menu() {

	menu->clear();

	if (dynamic_cast<PatternTrack*>(track_editor->get_track())) {
		// pattern
		PatternTrack* pt = dynamic_cast<PatternTrack*>(track_editor->get_track());
		menu->add_item( bitmap( pt->is_mute() ? BITMAP_ICON_MUTE: BITMAP_ICON_UNMUTE) , pt->is_mute()?"Unmute":"Mute",TRACK_MUTE,NULL,KEYBIND("editor/track_mute"),false );
		menu->add_item( bitmap( BITMAP_ICON_SOLO ),"Solo",TRACK_SOLO,NULL,KEYBIND("editor/track_solo"),false );
		menu->add_separator();
		
		if (pt->get_visible_columns()<PatternTrack::MAX_VISIBLE_COLUMNS)
			menu->add_item( bitmap( BITMAP_ICON_COLUMN_ADD ),"Add Pattern Column",PATTERN_ADD_COLUMN,NULL,KEYBIND("editor/add_column"),false );
			
		if (pt->get_visible_columns()>1)
			menu->add_item( bitmap( BITMAP_ICON_COLUMN_REMOVE ),"Remove Pattern Column",PATTERN_REMOVE_COLUMN,NULL,KEYBIND("editor/remove_column"),false );
		menu->add_separator();

	}
	
	if (Editor::get_singleton()->get_song()->find_track_pos( track_editor->get_track() ) > 0 )
		menu->add_item( bitmap( BITMAP_ICON_MOVE_TRACK_LEFT),"Move Track Left",TRACK_MOVE_LEFT );
	if (Editor::get_singleton()->get_song()->find_track_pos( track_editor->get_track() ) < Editor::get_singleton()->get_song()->get_track_count()-1 )
		menu->add_item( bitmap( BITMAP_ICON_MOVE_TRACK_RIGHT),"Move Track Right",TRACK_MOVE_RIGHT );
		
	if (Editor::get_singleton()->get_song()->get_track_count()>1)
		menu->add_separator();
	menu->add_item( bitmap( BITMAP_ICON_RENAME_TRACK),"Rename",TRACK_RENAME );
	menu->add_item( bitmap( BITMAP_ICON_REMOVE_TRACK),"Remove",TRACK_REMOVE );
	
}
	
void TrackEditorTrackName::name_input_callback(String p_string) {
	
	EditCommands::get_singleton()->audio_node_set_name( track_editor->get_track(), p_string );	
}
	
void TrackEditorTrackName::set_in_window() {

	menu = new GUI::PopUpMenu(get_window());
	menu->pre_show_signal.connect( this, &TrackEditorTrackName::about_to_show_menu );
	menu->selected_id_signal.connect( this, &TrackEditorTrackName::menu_callback );
	name_input = new GUI::StringInputDialog( get_window() );
	name_input->entered_string_signal.connect( this, &TrackEditorTrackName::name_input_callback );
		
}
	
TrackEditorTrackName::TrackEditorTrackName(TrackEditor *p_track_editor) {

	
	menu=NULL;
	track_editor=p_track_editor;
	if (p_track_editor->get_track()->is_collapsed())
		p_track_editor->hide();
	over=-1;
}


TrackEditorTrackName::~TrackEditorTrackName()
{
}


