//
// C++ Interface: add_node_dialog
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ADD_NODE_DIALOG_H
#define ADD_NODE_DIALOG_H

#include "base/window.h"
#include "base/widget.h"
#include "engine/song.h"
#include "widgets/line_edit.h"
#include "bundles/combo_box.h"
#include "widgets/button_group.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

class AddNodeDialog : public GUI::Window {

	Song *song;

	struct NodeInfoItem : public GUI::Widget {
	

		GUI::BitmapID icon;
		NodeInfoItem **current;
		const AudioNodeInfo *info;
		
		GUI::Signal< GUI::Method1<NodeInfoItem*> > selected;
		
		virtual void mouse_button(const GUI::Point& p_pos, int p_button,bool p_press,int p_modifier_mask);
		
		virtual GUI::Size get_minimum_size_internal();
	
		virtual void set_in_window();
		virtual void draw(const GUI::Point& p_global,const GUI::Size& p_size,const GUI::Rect& p_exposed);
		
		NodeInfoItem(const AudioNodeInfo *p_node_info);
	};
	
	std::vector<String> filters;
	GUI::ButtonGroup filter_group;
	void fix_name();
	
	NodeInfoItem *selected;
	void node_selected_callback(NodeInfoItem *p_item);
	void default_channels_changed(int p_to);
	
	void create_node();
	
	void filter_changed(int p_to_which);
	
	GUI::LineEdit *name;
	GUI::ComboBox *channels;
	GUI::VBoxContainer * vbox;
	int default_channels;
	bool updating_default_channels;

public:

	void show(String p_pre_type="");
	String get_current_category() ;

	AddNodeDialog(GUI::Window *p_parent, Song *p_song);
	~AddNodeDialog();

};

#endif
