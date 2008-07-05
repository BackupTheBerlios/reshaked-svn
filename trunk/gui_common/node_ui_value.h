//
// C++ Interface: node_ui_value
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NODE_UI_VALUE_H
#define NODE_UI_VALUE_H

#include "widgets/range_owner.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class NodeUI_Value : public GUI::RangeOwner {

	virtual GUI::Size get_minimum_size_internal(); 
	void draw(const GUI::Point& p_pos,const GUI::Size& p_size,const GUI::Rect& p_exposed);

	virtual void range_changed();
	virtual void value_changed(double p_new_val);

public:
	
	NodeUI_Value();
	~NodeUI_Value();

};

#endif
