//
// C++ Interface: edit_view_beatbar
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDEDIT_VIEW_BEATBAR_H
#define RESHAKEDEDIT_VIEW_BEATBAR_H

#include "base/widget.h"
#include "editor/editor.h"

using namespace GUI;

	
namespace ReShaked {
	
class EditViewBeatBar : public Widget {

	enum {
		CHAR_WIDTH=8
	};

	int get_row_height();
	void paint_marker(int p_marker_idx,int p_row,bool p_paint_arrow=true,int p_check_next=-1);
	virtual void draw(const Point& p_global,const Size& p_size,const Rect& p_exposed);
	virtual Size get_minimum_size_internal();
	
	Cursor *cursor;
	Song *song;
public:

	EditViewBeatBar(Editor *p_editor);
	~EditViewBeatBar();
};


}

#endif
