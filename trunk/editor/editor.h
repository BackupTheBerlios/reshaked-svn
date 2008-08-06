//
// C++ Interface: editor
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef EDITOR_H
#define EDITOR_H

#include "engine/song.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class Editor {

	Song *song;

	static Editor *singleton;
	
	struct Cursor {
	
		Tick tick;
		int zoom_divisor; // tick divisor for zoom
		int track;
		int col;
	} cursor;
	
	Track *get_current_track() const;
	
public:

	Song *get_song() const { return song; }

	void set_cursor_tick(Tick p_tick);
	void set_cursor_track(int p_track);
	void set_cursor_col(int p_col);
	void set_cursor_zoom_divisor(int p_divisor);
	
	Tick get_cursor_tick() const;
	int get_cursor_track() const;
	int get_cursor_col() const;
	int get_cursor_zoom_divisor() const;
	
	

	static Editor *get_singleton();

	Editor(Song *p_song);
	
	~Editor();

};

#endif
