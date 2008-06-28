//
// C++ Interface: plugin_chooser
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDPLUGIN_CHOOSER_H
#define RESHAKEDPLUGIN_CHOOSER_H

#include "editor/editor.h"
#include "base/window.h"
#include "bundles/window_box.h"
#include "bundles/scroll_box.h"
#include "widgets/check_button.h"
#include "bundles/combo_box.h"
#include "engine/sound_plugin_list.h"

using namespace GUI;

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class PluginChooser : public Window {
	
	Editor *editor;
	ScrollBox *scroll_box;
	int current_track;
	
	VBoxContainer *scroll_vb;
	VBoxContainer *plugin_list;
	
	CheckButton *append_to_out;
	ComboBox *channels;
	
	class PluginItem : public Widget {
	
		int index;
		PluginItem **current;
		
		Size get_minimum_size_internal();
		void draw(const Point& p_global,const Size& p_size,const Rect& p_exposed);
		void mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask);
	public:		
		Signal< Method1<PluginItem*> > selected_signal;
		int get_index() { return index; }
		PluginItem( int p_index,PluginItem ** p_current );
			
	};
	
	void item_selected(PluginItem*);
	PluginItem * current;
	
public:


	void choose_plugin(int p_current_track);
	
	PluginChooser(Window *p_parent,Editor *p_editor);
	~PluginChooser();

};

}

#endif
