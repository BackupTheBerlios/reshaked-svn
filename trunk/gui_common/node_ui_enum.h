//
// C++ Interface: node_ui_enum
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NODE_UI_ENUM_H
#define NODE_UI_ENUM_H

#include "base/widget.h"
#include "widgets/range_owner.h"

class NodeUI_Enum : public GUI::RangeOwner {

	virtual GUI::Size get_minimum_size_internal(); 
	void draw(const GUI::Point& p_pos,const GUI::Size& p_size,const GUI::Rect& p_exposed);
	
	bool pressing_left;
	bool pressing_right;
	
	void value_entered(double p_val);
	
	virtual void range_changed();
	virtual void value_changed(double p_new_val);

	void mouse_button(const GUI::Point& p_pos, int p_button,bool p_press,int p_modifier_mask);

	virtual void set_in_window();
public:
	
	NodeUI_Enum();
	~NodeUI_Enum();

};


#endif
