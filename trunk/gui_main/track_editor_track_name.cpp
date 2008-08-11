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
	
		bmp=bitmap(BITMAP_ICON_TRACK_VISIBLE);
	} else {
	
		bmp=bitmap(BITMAP_ICON_TRACK_COLLAPSED);
	}
	
	get_painter()->draw_bitmap( bmp, ofs );
	
	ofs.y+=get_painter()->get_bitmap_size( bmp ).height;
	
	
	get_painter()->draw_bitmap( bitmap(BITMAP_ICON_TRACK_OPTIONS), ofs );
	
	ofs.y+=get_painter()->get_bitmap_size( bitmap(BITMAP_ICON_TRACK_OPTIONS) ).height*2;
	
	ofs.x+=get_painter()->get_font_descent( font( FONT_TRACK_EDITOR_TRACK_NAME ) );
	
	GUI::Color text_col=GUI::Color(255);
	
	if (dynamic_cast<PatternTrack*>( track_editor->get_track() ))
		text_col=color( COLOR_PATTERN_FONT );
	
	get_painter()->draw_text( font( FONT_TRACK_EDITOR_TRACK_NAME ), ofs, track_editor->get_track()->get_name(), GUI::DOWN, text_col );	
	
}


TrackEditorTrackName::TrackEditorTrackName(TrackEditor *p_track_editor) {


	track_editor=p_track_editor;
	if (p_track_editor->get_track()->is_collapsed())
		p_track_editor->hide();
}


TrackEditorTrackName::~TrackEditorTrackName()
{
}


