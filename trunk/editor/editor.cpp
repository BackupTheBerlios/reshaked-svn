//
// C++ Implementation: editor
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "editor.h"

Editor *Editor::singleton=NULL;

Editor *Editor::get_singleton() {

	return singleton;
}

Track *Editor::get_current_track() const {

	if (cursor.track<0 || cursor.track >=song->get_track_count() )
		return NULL;
		
	return song->get_track(cursor.track);
}
void Editor::set_cursor_tick(Tick p_tick) {

	if (p_tick<0)
		return;
		
	cursor.tick=p_tick;
}
void Editor::set_cursor_track(int p_track) {

	if (p_track<0)
		p_track=0;
		
	if (p_track>=song->get_track_count())
		p_track=song->get_track_count()-1;
}
void Editor::set_cursor_col(int p_col) {

	if (p_col<0)
		p_col=0;
		
	Track *t = get_current_track();
	
	if (!t)
		return;
		
	if (p_col>=t->get_visible_columns())
		p_col=t->get_visible_columns()-1;
	
	cursor.col=p_col;
}
void Editor::set_cursor_zoom_divisor(int p_divisor) {

	if (p_divisor<0)
		p_divisor=0;
		
	if (p_divisor>=MAX_DIVISORS)
		p_divisor=MAX_DIVISORS-1;
		
	cursor.zoom_divisor=p_divisor;
}

Tick Editor::get_cursor_tick() const {

	return cursor.tick;
}
int Editor::get_cursor_track() const {

	return cursor.track;
}
int Editor::get_cursor_col() const {

	return cursor.col;
}
int Editor::get_cursor_zoom_divisor() const {

	return cursor.zoom_divisor;
}


Editor::Editor(Song *p_song) {
	
	singleton=this;
	song=p_song;
	
	cursor.tick=0;
	cursor.track=-1;
	cursor.zoom_divisor=3;
	cursor.col=0;
	
	
}


Editor::~Editor()
{
}


