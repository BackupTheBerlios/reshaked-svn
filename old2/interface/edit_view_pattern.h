//
// C++ Interface: blocklist_ui_pattern
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDBLOCKLIST_UI_PATTERN_H
#define RESHAKEDBLOCKLIST_UI_PATTERN_H


#include "interface/edit_view_blocklist.h"
#include "engine/song.h"
#include "engine/track_pattern.h"
#include "base/painter.h"
#include "editor/editor.h"

using namespace GUI;

namespace ReShaked {

/**
	@author red <red@killy>
*/
class EditViewPattern : public EditViewBlockList {

	Song *song;
	Editor *editor;
	Track_Pattern *track;
	
	
	void resize(const Size& p_new_size) { size=p_new_size; }
	Size size;
	int get_font_height();
	int get_font_width();
	
	int get_row_size();
	void paint_note_event( Painter& p, int p_row, Track_Pattern::NoteListElement & p_note, bool p_repeat );
	void paint_multiple_note_events( Painter& p, int p_row , const Track_Pattern::NoteList& p_list, bool p_repeat );
	void paint_multiple_nonvisible_events( Painter& p, int p_row, const Track_Pattern::NoteList& p_list);
	
	void paint_frames(Painter& p);
	void paint_row_lines(Painter &p);
	void paint_cursor(Painter& p, int p_row);
	void paint_selection(Painter&p,int p_clip_from,int p_clip_to);
	
	void draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed);	
	void mouse_motion(const Point& p_pos, const Point& p_rel, int p_button_mask);
	void mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask); ///< Overridable Method to notify the frame when it was 
	bool key(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask);
	void focus_enter();
	
	Size get_minimum_size_internal();
	
	bool can_scroll();
	void get_row_column_and_field(Point p_pos,int *row,int *column,int *field);
	void get_pos_at_pointer(Point p_pointer, int *p_blocklist,int *p_column, int *p_row);
public:
	
	EditViewPattern(Editor *p_editor, int p_track);
	~EditViewPattern();

};

}

#endif
