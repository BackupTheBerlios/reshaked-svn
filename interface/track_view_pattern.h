#ifndef TRACK_VIEW_PATTERN_H
#define TRACK_VIEW_PATTERN_H


#include "interface/track_view.h"
#include "editing/song_edit.h"
#include "editing/pattern_edit.h"
#include  <qfont.h>


namespace ReShaked {


class TrackViewPattern : public TrackView {

	PatternEdit* pattern_edit;
	SongEdit* song_edit;

	void paintEvent(QPaintEvent *e);
	void paint_note_event( QPainter& p, int p_row, PatternEdit::EdNote & p_note );

	void paint_multiple_note_events( QPainter& p, int p_row , const PatternEdit::NoteList& p_list );
	void paint_multiple_nonvisible_events( QPainter& p, int p_row, const PatternEdit::NoteList& p_list);


	void paint_cursor(QPainter& p, int p_row);

	void keyPressEvent (QKeyEvent *e);
	void keyReleaseEvent (QKeyEvent *e);
	void mousePressEvent ( QMouseEvent * e );
public:


	TrackViewPattern(QWidget *p_parent,PatternEdit* p_edit,SongEdit *song_edit,int p_track);
};



};


#endif
