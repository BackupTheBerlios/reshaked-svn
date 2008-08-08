//
// C++ Interface: track_editor_pattern
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TRACK_EDITOR_PATTERN_H
#define TRACK_EDITOR_PATTERN_H


#include "engine/pattern_track.h"
#include "gui_main/track_editor.h"

namespace GUI {
class Painter; 
};
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class TrackEditorPattern : public TrackEditor {

	Song *song;
	
	PatternTrack *track;
	
	struct NoteListElement {
	
		PatternTrack::Position pos;
		PatternTrack::Note note;
	};
	
	typedef std::list<NoteListElement> NoteList;
	
	void resize(const GUI::Size& p_new_size) { size=p_new_size; }
	GUI::Size size;
	int get_font_height();
	int get_font_width();
	
	int get_row_size();
	void paint_note_event( GUI::Painter& p, int p_row, NoteListElement & p_note, bool p_repeat );
	void paint_multiple_note_events( GUI::Painter& p, int p_row , const NoteList& p_list, bool p_repeat );
	void paint_multiple_nonvisible_events( GUI::Painter& p, int p_row, const NoteList& p_list);
	
	void paint_frames(GUI::Painter& p);
	void paint_row_lines(GUI::Painter &p);
	void paint_cursor(GUI::Painter& p, int p_row);
	void paint_selection(GUI::Painter&p,int p_clip_from,int p_clip_to);
	
	void draw(const GUI::Point& p_pos,const GUI::Size& p_size,const GUI::Rect& p_exposed);	
	void mouse_motion(const GUI::Point& p_pos, const GUI::Point& p_rel, int p_button_mask);
	void mouse_button(const GUI::Point& p_pos, int p_button,bool p_press,int p_modifier_mask); ///< Overridable Method to notify the frame when it was 
	bool key(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask);
	void focus_enter();
	
	GUI::Size get_minimum_size_internal();
	
	bool can_scroll();
	void get_row_column_and_field(GUI::Point p_pos,int *row,int *column,int *field);
	void get_column_and_row_at_pos(const GUI::Point& p_pos, int *p_column, int *p_row);

public:
	
	TrackEditorPattern(PatternTrack *p_track);
	~TrackEditorPattern();

};


#endif
