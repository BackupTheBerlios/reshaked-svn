//
// C++ Implementation: audio_graph_widget
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "audio_graph_widget.h"
#include "base/painter.h"
#include "gui_common/common_skin.h"
#include "editor/edit_commands.h"


GUI::Point AudioGraphWidget::get_node_and_port_pos(int p_node,AudioNode::PortType p_type,AudioNode::PortFlow p_flow,int p_idx) {

	GUI::Painter *p=get_painter();

	GUI::Point result;
	
	AudioNode *an = song->get_audio_graph()->get_node(p_node);

	GUI::Rect node_rect( GUI::Point( an->get_x(), an->get_y() ), get_node_size( an ) );
	
	if (task.type==Task::MOVING && task.moving_idx==p_node) {	
	
		node_rect.pos+=(task.moving_pos - task.moving_click);
	}
			
	int y=node_rect.pos.y;
								
	int layer_port = (current_layer<0 || current_layer>=MAX_LAYER_COLORS) ? MAX_LAYER_COLORS : current_layer;
	
	y+=p->get_stylebox_margin(stylebox( SB_LAYER_STYLE_NODE_0 + layer_port ), GUI::MARGIN_TOP );
	
	int button_h=p->get_bitmap_size( bitmap( BITMAP_GRAPH_NODE_OPTIONS ) ).height;
	
	int title_h= MAX( button_h, p->get_font_height( font( FONT_GRAPH_NODE_NAME ) ))+constant( C_GRAPH_NODE_VSPACING );
	
	y+=title_h;
		
	
	GUI::Size plug_size = p->get_bitmap_size( bitmap( BITMAP_GRAPH_JACK ) );
	
	int port_h = MAX( plug_size.height, p->get_font_height( font( FONT_GRAPH_NODE_PORT ) )+p->get_stylebox_min_size( stylebox( SB_GRAPH_AUDIO_PORT ) ).height );
	port_h+=constant( C_GRAPH_NODE_VSPACING );
				
		
	static const AudioNode::PortType port_types[3]={
		AudioNode::PORT_AUDIO,
		AudioNode::PORT_EVENT,
		AudioNode::PORT_CONTROL
	};
	
	static const AudioNode::PortFlow port_flows[2]={
		AudioNode::PORT_IN,
		AudioNode::PORT_OUT
	};
	
	int left_margin=p->get_stylebox_margin(stylebox( SB_LAYER_STYLE_NODE_0 + layer_port ), GUI::MARGIN_LEFT);
	int right_margin=p->get_stylebox_margin(stylebox( SB_LAYER_STYLE_NODE_0 + layer_port ), GUI::MARGIN_RIGHT);
	
	bool found=false;

	for (int i=0;i<3;i++) {
	
		for (int j=0;j<2;j++) {
	
			for (int k=0;k<an->get_port_count( port_types[i], port_flows[j] ); k++) {
			
				if ( port_types[i]==AudioNode::PORT_CONTROL && !an->get_control_port( port_flows[j], k)->is_visible())
					continue;
					
				if (p_type==port_types[i] && p_flow==port_flows[j] && k==p_idx) {
				
					switch( p_flow ) {
					
						case AudioNode::PORT_IN: {
						
							result=GUI::Point( node_rect.pos.x+left_margin, y+(port_h-plug_size.height)/2);
						} break;
						case AudioNode::PORT_OUT: {
						
							result=GUI::Point( node_rect.pos.x+node_rect.size.width-right_margin-plug_size.width, y+(port_h-plug_size.height)/2);
						} break;
					}
					found=true;
					break;
				}
			
			
				
				y+=port_h;
			}
			
			if (found)
				break;
		}	
		if (found)
			break;
	}
			
	return result;
}


bool AudioGraphWidget::get_node_and_port_at_pos(const GUI::Point& p_pos,int& r_node,AudioNode::PortType & r_type,AudioNode::PortFlow &r_flow, int &r_port) {

	GUI::Painter *p=get_painter();

	for (int i=song->get_audio_graph()->get_node_count()-1;i>=0;i--) {
	
		AudioNode *an = song->get_audio_graph()->get_node(i);
	
		if (!(current_layer==AudioNode::LAYER_ALWAYS_VISIBLE || an->get_layer()==AudioNode::LAYER_ALWAYS_VISIBLE || (an->get_layer() & (1<<current_layer))))
			continue;
	
		GUI::Rect node_rect( GUI::Point( an->get_x(), an->get_y() ), get_node_size( an ) );
				
		
		if (!node_rect.contains(p_pos))
			continue;
			
		r_node=i;
					
		int h=p_pos.y-node_rect.pos.y;
									
		int layer_port = (current_layer<0 || current_layer>=MAX_LAYER_COLORS) ? MAX_LAYER_COLORS : current_layer;
		
		h-=p->get_stylebox_margin(stylebox( SB_LAYER_STYLE_NODE_0 + layer_port ), GUI::MARGIN_TOP );
		
		GUI::Size button_size=p->get_bitmap_size( bitmap( BITMAP_GRAPH_NODE_OPTIONS ) );
		
		int title_h= MAX( button_size.height, p->get_font_height( font( FONT_GRAPH_NODE_NAME ) ))+constant( C_GRAPH_NODE_VSPACING );
		
		if (h<title_h) {
		
			int left_margin=p->get_stylebox_margin(stylebox( SB_LAYER_STYLE_NODE_0 + layer_port ), GUI::MARGIN_LEFT);
			int right_margin=p->get_stylebox_margin(stylebox( SB_LAYER_STYLE_NODE_0 + layer_port ), GUI::MARGIN_RIGHT);
			
			int w=node_rect.size.width-left_margin-right_margin;
			int x=p_pos.x-node_rect.pos.x - left_margin;
			
			if (x<button_size.width) {
			
				r_port=CLICK_SHOW_UI;
			} else if (x>w-button_size.width) {
			
				r_port=CLICK_CLOSE;			
			} else if (x>w-button_size.width*2) {
			
				r_port=CLICK_OPTIONS;			
			} else {
				r_port=CLICK_NODE; // -1 equals titlebar
			}
			return true;
		}			
			
		h-=title_h;
		
		
		GUI::Size plug_size = p->get_bitmap_size( bitmap( BITMAP_GRAPH_JACK ) );
		
		int port_h = MAX( plug_size.height, p->get_font_height( font( FONT_GRAPH_NODE_PORT ) )+p->get_stylebox_min_size( stylebox( SB_GRAPH_AUDIO_PORT ) ).height );
		port_h+=constant( C_GRAPH_NODE_VSPACING );
					
			
		static const AudioNode::PortType port_types[3]={
			AudioNode::PORT_AUDIO,
			AudioNode::PORT_EVENT,
			AudioNode::PORT_CONTROL
		};
		
		static const AudioNode::PortFlow port_flows[2]={
			AudioNode::PORT_IN,
			AudioNode::PORT_OUT
		};
		
	
		for (int i=0;i<3;i++) {
		
			for (int j=0;j<2;j++) {
		
				for (int k=0;k<an->get_port_count( port_types[i], port_flows[j] ); k++) {
				
					if ( port_types[i]==AudioNode::PORT_CONTROL && !an->get_control_port( port_flows[j], k)->is_visible())
						continue;
						
						
					if (h<port_h) {
					
						r_type=port_types[i];
						r_flow=port_flows[j];
						r_port=k;
						return true;
					}
					
					h-=port_h;
				}
			}	
		}
					
		r_port=CLICK_NODE;
		return true;
			
	}
	
	return false;
}


GUI::Size AudioGraphWidget::get_node_size(AudioNode *p_node) {

	GUI::Painter *p = get_painter();

	int layer_idx = (current_layer<0 || current_layer>=MAX_LAYER_COLORS) ? MAX_LAYER_COLORS : current_layer;

	GUI::Size size= p->get_stylebox_min_size( stylebox( SB_LAYER_STYLE_NODE_0 + layer_idx ) );
	
	/* buttons */
	
	GUI::Size buttons_size= p->get_bitmap_size( bitmap( BITMAP_GRAPH_NODE_OPTIONS ) );
	buttons_size.width*=3;
	int min_w=buttons_size.width;
		
	size.height+= constant( C_GRAPH_NODE_VSPACING );
	
	/* name */
	
	size.height+= MAX( buttons_size.height, p->get_font_height( font( FONT_GRAPH_NODE_NAME ) ));
	
	min_w+= p->get_font_string_width( font( FONT_GRAPH_NODE_NAME ), p_node->get_name() );
	min_w+= constant( C_GRAPH_NODE_HSPACING )*2;
			
	GUI::Size plug_size = p->get_bitmap_size( bitmap( BITMAP_GRAPH_JACK ) );
	
	int port_h = MAX( plug_size.height, p->get_font_height( font( FONT_GRAPH_NODE_PORT ) )+p->get_stylebox_min_size( stylebox( SB_GRAPH_AUDIO_PORT ) ).height );
	port_h+=constant( C_GRAPH_NODE_VSPACING );
	int plug_w = plug_size.width+p->get_stylebox_min_size( stylebox( SB_GRAPH_AUDIO_PORT ) ).width+constant( C_GRAPH_NODE_HSPACING );
		
	static const AudioNode::PortType port_types[3]={
		AudioNode::PORT_AUDIO,
		AudioNode::PORT_EVENT,
		AudioNode::PORT_CONTROL
	};
	
	static const AudioNode::PortFlow port_flows[2]={
		AudioNode::PORT_IN,
		AudioNode::PORT_OUT
	};
	
	
	for (int i=0;i<3;i++) {
	
		for (int j=0;j<2;j++) {
	
			for (int k=0;k<p_node->get_port_count( port_types[i], port_flows[j] ); k++) {
			
				if ( port_types[i]==AudioNode::PORT_CONTROL && !p_node->get_control_port( port_flows[j], k)->is_visible())
					continue;
					
				String name = p_node->get_port_name( port_types[i], port_flows[j], k );
				
				
				int w = plug_w + p->get_font_string_width( font( FONT_GRAPH_NODE_PORT ), name );
				
				if (w>min_w)
					min_w=w;
					
				size.height+=port_h;
			}
		}	
	}
	
	size.width+=min_w;
	
	return size;
}


void AudioGraphWidget::draw_connection(const GUI::Point& p_from, const GUI::Point& p_to, const GUI::Color& p_color) {

	get_painter()->draw_line(p_from,p_to,p_color,5);

}


void AudioGraphWidget::draw_node(const GUI::Rect& p_rect,AudioNode *p_node) {

	GUI::Painter *p = get_painter();

	int layer_idx = (current_layer<0 || current_layer>=MAX_LAYER_COLORS) ? MAX_LAYER_COLORS : current_layer;

	p->draw_stylebox( stylebox( SB_LAYER_STYLE_NODE_0 + layer_idx ), p_rect.pos, p_rect.size );
	
	GUI::Size sb_minsize= p->get_stylebox_min_size( stylebox( SB_LAYER_STYLE_NODE_0 + layer_idx ) );
	
	GUI::Point ofs=p_rect.pos;
	ofs+=GUI::Point( p->get_stylebox_margin( stylebox( SB_LAYER_STYLE_NODE_0 + layer_idx ), GUI::MARGIN_LEFT), p->get_stylebox_margin( stylebox( SB_LAYER_STYLE_NODE_0 + layer_idx ), GUI::MARGIN_TOP ) );
	
	GUI::Size button_size=p->get_bitmap_size( bitmap( BITMAP_GRAPH_NODE_EDIT ) );
	
	int title_font_height=p->get_font_height( font( FONT_GRAPH_NODE_NAME ) );
	int title_height= MAX( button_size.height, title_font_height );
	
	GUI::Point button_ofs=GUI::Point( ofs.x, ofs.y+(title_height-button_size.height)/2);
	
	int button_right_x=p_rect.pos.x+p_rect.size.width-p->get_stylebox_margin( stylebox( SB_LAYER_STYLE_NODE_0 + layer_idx ), GUI::MARGIN_RIGHT );
	
	p->draw_bitmap( bitmap( BITMAP_GRAPH_NODE_EDIT ), button_ofs );
	p->draw_bitmap( bitmap( BITMAP_GRAPH_NODE_CLOSE ), GUI::Point(button_right_x-button_size.x,button_ofs.y) );
	p->draw_bitmap( bitmap( BITMAP_GRAPH_NODE_OPTIONS ), GUI::Point(button_right_x-button_size.x*2,button_ofs.y) );
	
	int text_area=button_right_x-button_size.x*2 - constant( C_GRAPH_NODE_HSPACING );
	
	GUI::Point text_ofs=GUI::Point( ofs.x+constant( C_GRAPH_NODE_HSPACING )+button_size.x, ofs.y+(title_height-title_font_height)/2 + p->get_font_ascent( font( FONT_GRAPH_NODE_NAME ) ) );
	
	text_ofs.x+= (text_area - text_ofs.x - p->get_font_string_width( font( FONT_GRAPH_NODE_NAME ), p_node->get_name() ))/2;
	
	p->draw_text( font( FONT_GRAPH_NODE_NAME ), text_ofs, p_node->get_name(), color( COLOR_GRAPH_NODE_NAME_FONT ) );
	
	ofs.y+=title_height;
	ofs.y+=constant( C_GRAPH_NODE_HSPACING );
	

	static const AudioNode::PortType port_types[3]={
		AudioNode::PORT_AUDIO,
		AudioNode::PORT_EVENT,
		AudioNode::PORT_CONTROL
	};
	
	static const AudioNode::PortFlow port_flows[2]={
		AudioNode::PORT_IN,
		AudioNode::PORT_OUT
	};
	
	GUI::Size plug_size = p->get_bitmap_size( bitmap( BITMAP_GRAPH_JACK ) );
	
	int port_h = MAX( plug_size.height, p->get_font_height( font( FONT_GRAPH_NODE_PORT ) )+p->get_stylebox_min_size( stylebox( SB_GRAPH_AUDIO_PORT ) ).height );
	
	GUI::Rect port_rect( GUI::Point(), GUI::Size( p_rect.size.width - sb_minsize.width - plug_size.width -constant( C_GRAPH_NODE_HSPACING ), port_h ));	
	
	GUI::Point text_rect_ofs= GUI::Point( p->get_stylebox_margin( stylebox( SB_GRAPH_AUDIO_PORT ), GUI::MARGIN_LEFT), p->get_stylebox_margin( stylebox( SB_GRAPH_AUDIO_PORT ), GUI::MARGIN_TOP)+p->get_font_ascent( font( FONT_GRAPH_NODE_PORT) ) );
	
	for (int i=0;i<3;i++) {
	
		for (int j=0;j<2;j++) {
	
			for (int k=0;k<p_node->get_port_count( port_types[i], port_flows[j] ); k++) {
			
				if ( port_types[i]==AudioNode::PORT_CONTROL && !p_node->get_control_port( port_flows[j], k)->is_visible())
					continue;
					
				
				String name = p_node->get_port_name( port_types[i], port_flows[j], k );	
								
				GUI::Point plug_ofs=ofs+GUI::Point(0,(port_h-plug_size.height)/2);
				GUI::Rect this_port_rect=port_rect;
				this_port_rect.pos+=ofs;
				
				if (port_flows[j]==AudioNode::PORT_IN) {
				
					this_port_rect.pos.x+=plug_size.width+constant( C_GRAPH_NODE_HSPACING );
					
				} else {
				
					plug_ofs.x+=this_port_rect.size.width+constant( C_GRAPH_NODE_HSPACING );				
				}
									
				switch( port_types[i] ) {
				
					case AudioNode::PORT_AUDIO: {
					
						p->draw_stylebox( stylebox( SB_GRAPH_AUDIO_PORT ), this_port_rect.pos, this_port_rect.size );
						p->draw_text( font( FONT_GRAPH_NODE_PORT), this_port_rect.pos+text_rect_ofs, name, color( COLOR_GRAPH_NODE_AUDIO_PORT_FONT ) );
					} break;
					case AudioNode::PORT_EVENT: {
					
						p->draw_stylebox( stylebox( SB_GRAPH_EVENT_PORT ), this_port_rect.pos, this_port_rect.size );
						p->draw_text( font( FONT_GRAPH_NODE_PORT), this_port_rect.pos+text_rect_ofs, name, color( COLOR_GRAPH_NODE_EVENT_PORT_FONT ) );
					} break;
					case AudioNode::PORT_CONTROL: {
					
						p->draw_stylebox( stylebox( SB_GRAPH_CONTROL_PORT ), this_port_rect.pos, this_port_rect.size );
						p->draw_text( font( FONT_GRAPH_NODE_PORT), this_port_rect.pos+text_rect_ofs, name, color( COLOR_GRAPH_NODE_CONTROL_PORT_FONT ) );
					} break;
				}
				
				p->draw_bitmap( bitmap( BITMAP_GRAPH_JACK ), plug_ofs );
				
				ofs.y+=constant( C_GRAPH_NODE_VSPACING )+port_h;
			}
		}	
	}
}

void AudioGraphWidget::draw(const GUI::Point& p_pos,const GUI::Size& p_size,const GUI::Rect& p_exposed) {
	
	int layer_idx = (current_layer<0 || current_layer>=MAX_LAYER_COLORS) ? MAX_LAYER_COLORS : current_layer;
	
	/* draw bg */
	get_painter()->draw_stylebox( stylebox( SB_LAYER_STYLE_BG_0+layer_idx), GUI::Point(), p_size );
	
	
	/* draw nodes */
	
	
	for (int i=0;i<song->get_audio_graph()->get_node_count();i++) {
	
		
		AudioNode *an = song->get_audio_graph()->get_node(i);
		
		if (!(current_layer==AudioNode::LAYER_ALWAYS_VISIBLE || an->get_layer()==AudioNode::LAYER_ALWAYS_VISIBLE || (an->get_layer() & (1<<current_layer))))
			continue;
			
		GUI::Rect node_rect( GUI::Point(an->get_x(), an->get_y() ), get_node_size(an) );
		
		if (task.type==Task::MOVING && task.moving_idx==i) {
		
			node_rect.pos+=(task.moving_pos - task.moving_click);
		}
		if (!p_exposed.intersects_with( node_rect ))
			continue; // don't bother
	
		draw_node( node_rect, an );
	}
	
	/* draw connections */

	for(int i=0;i<song->get_audio_graph()->get_connection_count();i++) {
	
		const AudioConnection &c=*song->get_audio_graph()->get_connection(i);
		
		// make sure both nodes are in this layer
		
		AudioNode *node_from=song->get_audio_graph()->get_node( c.from_node );
		ERR_CONTINUE(!node_from);
		if (!(current_layer==AudioNode::LAYER_ALWAYS_VISIBLE || node_from->get_layer()==AudioNode::LAYER_ALWAYS_VISIBLE || (node_from->get_layer() & (1<<current_layer))))
			continue;
		
		AudioNode *node_to=song->get_audio_graph()->get_node( c.to_node );
		ERR_CONTINUE(!node_to);
		if (!(current_layer==AudioNode::LAYER_ALWAYS_VISIBLE || node_to->get_layer()==AudioNode::LAYER_ALWAYS_VISIBLE || (node_to->get_layer() & (1<<current_layer))))
			continue;
		
		// compute points and color
		
		GUI::Point from = get_node_and_port_pos(c.from_node,c.type,AudioNode::PORT_OUT,c.from_port);
		GUI::Point to = get_node_and_port_pos(c.to_node,c.type,AudioNode::PORT_IN,c.to_port);
		
		
		
		GUI::Color col;
		
		switch(c.type) {
		
			case AudioNode::PORT_AUDIO: col=color( COLOR_GRAPH_AUDIO_CABLE ); break;
			case AudioNode::PORT_EVENT: col=color( COLOR_GRAPH_EVENT_CABLE ); break;
			case AudioNode::PORT_CONTROL: col=color( COLOR_GRAPH_CONTROL_CABLE ); break;
		}
		
		// plot connection
		
		draw_connection(from,to,col);
	}
		
	if (task.type==Task::CONNECTING) {
		
		GUI::Point from = task.moving_click; //get_node_and_port_pos( task.moving_idx, task.port_type, task.in, task.port_idx );
		GUI::Color c=color(COLOR_GRAPH_CONNECTING_CABLE);
		int node_idx,port_idx;
		AudioNode::PortType port_type;
		AudioNode::PortFlow port_flow;
		
		if (get_node_and_port_at_pos(task.moving_pos,node_idx,port_type,port_flow,port_idx)) {
		
			//printf("connecting from type: %i flow %i port %i\n",port_type,port_flow, port_idx);
		
			if (node_idx!=-1 && port_idx>=0 && node_idx!=task.moving_idx && port_type == task.port_type && port_flow!=task.port_flow) {
			
				switch(port_type) {
				
					case AudioNode::PORT_AUDIO: c=color( COLOR_GRAPH_AUDIO_CABLE ); break;
					case AudioNode::PORT_EVENT: c=color( COLOR_GRAPH_EVENT_CABLE ); break;
					case AudioNode::PORT_CONTROL: c=color( COLOR_GRAPH_CONTROL_CABLE ); break;
				}
			}
		}

		draw_connection(from,task.moving_pos,c);
		
	}		
		
	
}


/** MOUSE **/



void AudioGraphWidget::mouse_button(const GUI::Point& p_pos, int p_button,bool p_press,int p_modifier_mask) {


	if (p_press) {

		task.type=Task::NONE;
		
		int node_idx,port_idx;
		AudioNode::PortFlow port_flow;
		AudioNode::PortType port_type;
		
		if (!get_node_and_port_at_pos(p_pos,node_idx,port_type,port_flow,port_idx))
			return;
			
		if (port_idx<0) {
			// clicked on node
			
			if (p_button==GUI::BUTTON_LEFT) {
			
				switch (port_idx) {
				
					case CLICK_SHOW_UI: {
					
						AudioNode *an=song->get_audio_graph()->get_node(node_idx);
						node_menu.node_idx=node_idx;
						
						/* determine popup pos */
						{
												
							GUI::Rect node_rect( GUI::Point( an->get_x(), an->get_y() ), get_node_size( an ) );
							int layer_port = (current_layer<0 || current_layer>=MAX_LAYER_COLORS) ? MAX_LAYER_COLORS : current_layer;
	
							int top_margin=get_painter()->get_stylebox_margin(stylebox( SB_LAYER_STYLE_NODE_0 + layer_port ), GUI::MARGIN_TOP);
							int right_margin=get_painter()->get_stylebox_margin(stylebox( SB_LAYER_STYLE_NODE_0 + layer_port ), GUI::MARGIN_RIGHT);
							
							GUI::Size button_size=get_painter()->get_bitmap_size( bitmap( BITMAP_GRAPH_NODE_OPTIONS ) );
							
							GUI::Point popup_pos=node_rect.pos;
							popup_pos.y+=top_margin+button_size.height;
							popup_pos.x+=right_margin;											
							node_ui_window->edit_node( popup_pos + get_global_pos(), an );
						}										
						
					} break;
					case CLICK_OPTIONS: {
					
						node_popup->clear();
						
						AudioNode *an=song->get_audio_graph()->get_node(node_idx);
						node_menu.node_idx=node_idx;
						
						node_popup->add_item( bitmap( BITMAP_GRAPH_NODE_RENAME ), "Rename",NODE_MENU_RENAME);
						
						
						if (an->get_port_count( AudioNode::PORT_CONTROL,AudioNode::PORT_IN) || 
							an->get_port_count( AudioNode::PORT_CONTROL,AudioNode::PORT_OUT) ) {
						
							node_popup->add_item( bitmap( BITMAP_GRAPH_NODE_CONTROL_EDIT ), "Controls",NODE_MENU_CONTROLS);						
							//node_popup->add_item("Show/Hide Control Ports",NODE_MENU_EDIT_PORTS );
						}
						
						node_popup->add_item( bitmap( BITMAP_GRAPH_NODE_LAYERS ), "Layers",NODE_MENU_LAYERS);
						node_popup->add_item( bitmap( BITMAP_GRAPH_NODE_PRESET ), "Presets",NODE_MENU_PRESETS);
						
						/* determine popup pos */
						{
												
							GUI::Rect node_rect( GUI::Point( an->get_x(), an->get_y() ), get_node_size( an ) );
							int layer_port = (current_layer<0 || current_layer>=MAX_LAYER_COLORS) ? MAX_LAYER_COLORS : current_layer;
	
							int top_margin=get_painter()->get_stylebox_margin(stylebox( SB_LAYER_STYLE_NODE_0 + layer_port ), GUI::MARGIN_TOP);
							int right_margin=get_painter()->get_stylebox_margin(stylebox( SB_LAYER_STYLE_NODE_0 + layer_port ), GUI::MARGIN_RIGHT);
							
							GUI::Size button_size=get_painter()->get_bitmap_size( bitmap( BITMAP_GRAPH_NODE_OPTIONS ) );
							
							GUI::Point popup_pos=node_rect.pos;
							popup_pos.y+=top_margin+button_size.height;
							popup_pos.x+=node_rect.size.width-right_margin-button_size.width*2;														
							node_popup->popup( popup_pos + get_global_pos() );
						}				
					
					} break;
					case CLICK_CLOSE: {
					
						EditCommands::get_singleton()->audio_graph_remove_node( song->get_audio_graph(), node_idx );
					} break;
					case CLICK_NODE: {
					
						task.type=Task::MOVING;
						task.moving_click=p_pos;
						task.moving_pos=p_pos;
						EditCommands::get_singleton()->audio_graph_swap_nodes(song->get_audio_graph(),node_idx,song->get_audio_graph()->get_node_count()-1);
						task.moving_idx=song->get_audio_graph()->get_node_count()-1;
					
					} break;
				
				}
					
			}
		
		} else {
			// clicked on port
		
			if (p_button==GUI::BUTTON_LEFT) {
				// move
				task.type=Task::CONNECTING;
				task.moving_click=p_pos;
				task.moving_pos=p_pos;
				EditCommands::get_singleton()->audio_graph_swap_nodes(song->get_audio_graph(),node_idx,song->get_audio_graph()->get_node_count()-1);
				task.moving_idx=song->get_audio_graph()->get_node_count()-1;
				task.port_type=port_type;
				task.port_flow=port_flow;
				printf("node i foudn has flow %i\n",port_flow);
				task.port_idx=port_idx;
				
							
				
			}
			if (p_button==GUI::BUTTON_RIGHT) {
				// move
				node_popup->clear();
				
				AudioNode *an=song->get_audio_graph()->get_node(node_idx);
				node_menu.node_idx=node_idx;
				node_menu.port_type=port_type;
				node_menu.port_flow=port_flow;
				node_menu.port_idx=port_idx;
				
				if (port_type==AudioNode::PORT_CONTROL) {
				
					node_popup->add_item("Automate",NODE_MENU_AUTOMATE_PORT );
					node_popup->add_separator();
					// if no connections node_popup->add_item("Hide",PORT_MENU_CONTROL_HIDE);
				
				}
				
				int idx=0;
				
				for (int i=0;i<song->get_audio_graph()->get_connection_count();i++) {
				
					const AudioConnection &c=*song->get_audio_graph()->get_connection(i);
										
					String other_end;
					
					if (c.type!=port_type)
						continue;
					
					if (port_flow==AudioNode::PORT_IN) {
					
						if (c.to_node!=node_idx)
							continue;
						if (c.to_port!=port_idx)
							continue;
							
						AudioNode *another=song->get_audio_graph()->get_node(c.from_node);
						
						ERR_FAIL_COND(!another);
						other_end=another->get_name()+"::"+another->get_port_name( port_type, AudioNode::PORT_OUT, c.from_port);
					} else {
					
						if (c.from_node!=node_idx)
							continue;
						if (c.from_port!=port_idx)
							continue;
							
						AudioNode *another=song->get_audio_graph()->get_node(c.to_node);
		
						ERR_FAIL_COND(!another);
						other_end=another->get_name()+"::"+			other_end=another->get_port_name( port_type, AudioNode::PORT_OUT, c.to_port);
					
					}
						
					
					node_popup->add_item("Disconnect: "+other_end,NODE_MENU_DISCONNECT_PORTS+idx );
					
					idx++;
						
				}
				
				if (idx>1) {
					node_popup->add_separator();
					node_popup->add_item("Disconnect All",NODE_MENU_DISCONNECT_PORT_ALL );

				}
				
//				node_popup->add_item( "Disconnect All",PORT_MENU_DISCONNECT);
				node_popup->popup( p_pos + get_global_pos() );				
			}
		
		}
	} else {
	
	
		if (p_button==GUI::BUTTON_LEFT && task.type==Task::MOVING) {
		
			ERR_FAIL_INDEX(task.moving_idx,song->get_audio_graph()->get_node_count());
			GUI::Point node_pos = GUI::Point( song->get_audio_graph()->get_node(task.moving_idx)->get_x() , song->get_audio_graph()->get_node(task.moving_idx)->get_y() );
			GUI::Point new_pos = node_pos + (task.moving_pos - task.moving_click);
			EditCommands::get_singleton()->audio_graph_move_node( song->get_audio_graph(), task.moving_idx, new_pos.x, new_pos.y );
		}
		
		if (p_button==GUI::BUTTON_LEFT && task.type==Task::CONNECTING) {
		
			printf("attempt connect!\n");
			ERR_FAIL_INDEX(task.moving_idx,song->get_audio_graph()->get_node_count());
		
			int node_idx;
			AudioNode::PortFlow port_flow;
			AudioNode::PortType port_type;
			int port_idx;
			
			if (get_node_and_port_at_pos(task.moving_pos,node_idx,port_type,port_flow,port_idx)) {
			
				printf("attempt to connect over node %i, port %i, flow %i\n",node_idx, port_idx, port_flow);
				if (node_idx!=-1 && node_idx!=task.moving_idx && port_type == task.port_type && port_flow!=task.port_flow) {
				
					printf("actual connect!\n");
					AudioConnection ac;
					ac.type=task.port_type;
					ac.from_port=task.port_idx;
					ac.from_node=task.moving_idx;
					ac.to_node=node_idx;
					ac.to_port=port_idx;
										
					printf("swap? flow is %i\n",task.port_flow);
					if (task.port_flow==AudioNode::PORT_IN) {
					
						SWAP( ac.from_node, ac.to_node );
						SWAP( ac.from_port, ac.to_port );
						task.port_flow=AudioNode::PORT_OUT;
					}
					
					printf("connect type %i\n",ac.type);
					printf("From: %s, %s port %i\n",song->get_audio_graph()->get_node(ac.from_node)->get_name().ascii().get_data(),(task.port_flow==AudioNode::PORT_IN?"IN":"OUT"),ac.from_port);
					printf("To: %s, port %i\n",song->get_audio_graph()->get_node(ac.to_node)->get_name().ascii().get_data(),ac.to_port);
					
					EditCommands::get_singleton()->audio_graph_connect(song->get_audio_graph(),ac);
				}	
			}
		}		
		task.type=Task::NONE;
		update();
	
	}
}
void AudioGraphWidget::mouse_motion(const GUI::Point& p_pos, const GUI::Point& p_rel, int p_button_mask) {


	if (p_button_mask&GUI::BUTTON_MASK_LEFT) {
	
		if (task.type==Task::MOVING) {
			task.moving_pos=p_pos;
			update();
		}
		
		if (task.type==Task::CONNECTING) {
		
			task.moving_pos=p_pos;
			update();
		}		
	}


}



void AudioGraphWidget::redraw() {

	update();
}


void AudioGraphWidget::node_menu_callback(int p_option) {

	switch(p_option) {
		case NODE_MENU_CONTROLS: {
		
			control_port_editor->edit( song->get_audio_graph()->get_node( node_menu.node_idx ) );
		} break;
		case NODE_MENU_LAYERS: {
		
			node_layer_editor->edit( song->get_audio_graph()->get_node( node_menu.node_idx ) );
		} break;
		case NODE_MENU_AUTOMATE_PORT: {
		
			
		} break;
		case NODE_MENU_DISCONNECT_PORT_ALL: {
		
			EditCommands::get_singleton()->audio_graph_disconnect_port( song->get_audio_graph(), node_menu.node_idx, node_menu.port_type, node_menu.port_flow, node_menu.port_idx );
		} break;
		default: {
			if (p_option>=NODE_MENU_DISCONNECT_PORTS) {
		
				int idx_disconnect=p_option-NODE_MENU_DISCONNECT_PORTS;
				
				int idx=0;
				
				for (int i=0;i<song->get_audio_graph()->get_connection_count();i++) {
				
					const AudioConnection &c=*song->get_audio_graph()->get_connection(i);
										
					String other_end;
					
					if (c.type!=node_menu.port_type)
						continue;
					
					if (node_menu.port_flow==AudioNode::PORT_IN) {
					
						if (c.to_node!=node_menu.node_idx)
							continue;
						if (c.to_port!=node_menu.port_idx)
							continue;							
						if (idx==idx_disconnect) {							
							EditCommands::get_singleton()->audio_graph_disconnect( song->get_audio_graph(), c );
							return;
						}
						
					} else {
					
						if (c.from_node!=node_menu.node_idx)
							continue;
						if (c.from_port!=node_menu.port_idx)
							continue;
							
						if (idx==idx_disconnect) {							
							EditCommands::get_singleton()->audio_graph_disconnect( song->get_audio_graph(), c );
							return;
						}
					
					}
					
					idx++;
						
				}
				
			}
		} break;
	}


}

void AudioGraphWidget::set_current_layer(int p_layer) {

	current_layer=p_layer;
	update();
}

void AudioGraphWidget::node_edit_preset_callback( AudioNode *p_node ) {

	preset_browser->edit_preset( p_node );
}

void AudioGraphWidget::set_in_window() {

	node_popup = new GUI::PopUpMenu( get_window() );
	node_popup->selected_id_signal.connect(this, &AudioGraphWidget::node_menu_callback );
	
	control_port_editor = new ControlPortVisibilityEditor( song->get_audio_graph(), get_window() 
	);	
	node_layer_editor = new NodeLayerEditor( song->get_audio_graph(), get_window() );
	
	node_ui_window = new NodeUI_Window( get_window() );
	node_ui_window->edited_signal.connect( this, &AudioGraphWidget::node_edit_preset_callback );
	
	preset_browser = new PresetBrowser( get_window() );
	
}


AudioGraphWidget::AudioGraphWidget(GUI_UpdateNotify *p_update_notify,Song *p_song) {

	
	p_update_notify->audio_graph_changed_signal.connect( this, &AudioGraphWidget::redraw );
	song=p_song;
	current_layer=0;
	
}


AudioGraphWidget::~AudioGraphWidget()
{
}


