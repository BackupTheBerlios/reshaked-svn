//
// C++ Interface: audio_graph_widget
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef AUDIO_GRAPH_WIDGET_H
#define AUDIO_GRAPH_WIDGET_H


#include "engine/song.h"
#include "base/widget.h"
#include "bundles/popup_menu.h"
#include "gui_main/gui_update_notify.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class AudioGraphWidget : public GUI::Widget{

	Song *song;

	int current_layer;

	GUI::Size get_node_size(AudioNode *p_node);

	GUI::Point get_node_and_port_pos(int p_node,AudioNode::PortType p_type,AudioNode::PortFlow p_flow,int p_idx);

	enum NodeClickInfo {
		
		CLICK_SHOW_UI=-1,
		CLICK_OPTIONS=-2,
		CLICK_CLOSE=-3,
		CLICK_NODE=-4 // anywhere in the node
	};

	bool get_node_and_port_at_pos(const GUI::Point& p_pos,int& r_node,AudioNode::PortType & r_type,AudioNode::PortFlow &r_flow, int &r_port);


	void draw_node(const GUI::Rect& p_rect,AudioNode *p_node);
	virtual void draw(const GUI::Point& p_pos,const GUI::Size& p_size,const GUI::Rect& p_exposed);
	void draw_connection(const GUI::Point& p_from, const GUI::Point& p_to, const GUI::Color& p_color);

	virtual void mouse_button(const GUI::Point& p_pos, int p_button,bool p_press,int p_modifier_mask);
	virtual void mouse_motion(const GUI::Point& p_pos, const GUI::Point& p_rel, int p_button_mask);
	
	enum OperationType {
	
		NONE,
		MOVING,
		CONNECTING
	};
	
	struct Task {

		enum Type {
			NONE,
			MOVING,
			CONNECTING
		};
		
		Type type;
	 	
	 	int moving_idx;
	 	GUI::Point moving_pos;
	 	GUI::Point moving_click;
	 	
	 	AudioNode::PortType port_type;
	 	AudioNode::PortFlow port_flow;
	 	int port_idx;
	 	
	 	Task() {
	 	
	 		type=MOVING;
	 	};	
	 	
	 } task;
	 
	struct NodeMenu {
	
		AudioNode *node;
		AudioNode::PortType port_type;
		AudioNode::PortFlow port_flow;
		int port_idx;
		bool input;
		
		NodeMenu() { node=NULL; }
	} node_menu;
		 
	 GUI::PopUpMenu *node_popup;
	 
	 enum NodeMenuOption {
	 
	 	NODE_MENU_RENAME,
	 	NODE_MENU_CONTROLS,
	 	NODE_MENU_LAYERS,
	 	NODE_MENU_PRESETS,	 	
	 };
	 
	 void node_menu_callback(int p_option);
	 void set_in_window();
	 
public:

	enum {
	
		SHOW_ALL_LAYERS=-1
	};

	void redraw();

	AudioGraphWidget(GUI_UpdateNotify *p_update_notify,Song *p_song);
	~AudioGraphWidget();

};

#endif
