//
// C++ Interface: node_ui_label
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NODE_UI_LABEL_H
#define NODE_UI_LABEL_H

#include "base/widget.h"


/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class NodeUI_Label : public GUI::Widget {
public:
	
	enum Orientation {
		HORIZONTAL,
		VERTICAL
	};
		

	virtual GUI::Size get_minimum_size_internal(); 
	void draw(const GUI::Point& p_pos,const GUI::Size& p_size,const GUI::Rect& p_exposed);
	String name;
	Orientation orientation;

public:
	
	void set_orientation(Orientation p_orientation);
	void set_text(String p_text);

	NodeUI_Label(Orientation p_orientation=HORIZONTAL);
	~NodeUI_Label();

};

#endif
