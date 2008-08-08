//
// C++ Interface: track_editor
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TRACK_EDITOR_H
#define TRACK_EDITOR_H

#include "engine/track.h"
#include "base/widget.h"
#include "editor/editor.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class TrackEditor : public GUI::Widget {

	bool _mouse_selecting;
	Track* _track;
protected:	
	
	void pre_parse_mouse_button_event(const GUI::Point& p_pos, int p_button,bool p_press,int p_modifier_mask);

	virtual void get_column_and_row_at_pos(const GUI::Point& p_pos, int *p_column, int *p_row)=0;
	
	void mouse_selection_begin(const GUI::Point p_pos);
	void mouse_selection_update_check();
	void mouse_selection_end();
	
	int get_row_height();
		
	
public: //signals
	GUI::Signal< GUI::Method1<TrackEditor*> > track_editor_under_cursor_request_signal;
	
public:
	void set_track_editor_under_cursor(TrackEditor *p_track_editor, const GUI::Point &p_pos);
	
	TrackEditor(Track* p_track);
	
	~TrackEditor();

};

#endif
