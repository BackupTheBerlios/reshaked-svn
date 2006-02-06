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


#include "interface/blocklist_ui_base.h"
#include "engine/song.h"
#include "engine/track_pattern.h"
#include "editor/editor.h"
namespace ReShaked {

/**
	@author red <red@killy>
*/
class BlockListUI_Pattern : public BlockListUI_Base {

	Song *song;
	Editor *editor;
	Track_Pattern *track;
	
	int get_row_size();
	void paint_note_event( QPainter& p, int p_row, Track_Pattern::NoteListElement & p_note );
	void paint_multiple_note_events( QPainter& p, int p_row , const Track_Pattern::NoteList& p_list );
	void paint_multiple_nonvisible_events( QPainter& p, int p_row, const Track_Pattern::NoteList& p_list);
	
	void paint_frames(QPainter& p);
	void paint_row_lines(QPainter &p);
	void paintEvent(QPaintEvent *e);
	void paint_cursor(QPainter& p, int p_row);
	void focusInEvent ( QFocusEvent * event );
	void mousePressEvent ( QMouseEvent * e );
	void mouseMoveEvent ( QMouseEvent * e );
	void mouseReleaseEvent ( QMouseEvent * e );
	void paint_selection(QPainter&p,int p_clip_from,int p_clip_to);
	
	bool event( QEvent * ev );
	
	bool can_scroll();
	void get_row_column_and_field(QPoint p_pos,int *row,int *column,int *field);
	void get_pos_at_pointer(QPoint p_pointer, int *p_blocklist,int *p_column, int *p_row);
public:
	
	BlockListUI_Pattern(QWidget *p_parent,Editor *p_editor, int p_track);
	~BlockListUI_Pattern();

};

}

#endif
