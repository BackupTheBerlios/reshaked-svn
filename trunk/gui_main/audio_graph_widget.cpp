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
	
	int port_h = MIN( plug_size.height, p->get_font_height( font( FONT_GRAPH_NODE_PORT ) ) );
	port_h+=constant( C_GRAPH_NODE_VSPACING );
	int plug_w = plug_size.width;
		
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
	
	GUI::Point text_ofs=GUI::Point( ofs.x+constant( C_GRAPH_NODE_HSPACING )+button_size.x, ofs.y+(title_height-title_font_height)/2);
	
	printf("text should be at %i,%i\n",text_ofs.x,text_ofs.y);
	printf("text font should be %i\n",font( FONT_GRAPH_NODE_NAME ));
	printf("text color should be %i,%i,%i\n",color( COLOR_GRAPH_NODE_NAME_FONT ).r,color( COLOR_GRAPH_NODE_NAME_FONT ).g,color( COLOR_GRAPH_NODE_NAME_FONT ).b);
	printf("text name should be %s\n",p_node->get_name().ascii().get_data());
	p->draw_text( font( FONT_GRAPH_NODE_NAME ), text_ofs, p_node->get_name(), color( COLOR_GRAPH_NODE_NAME_FONT ) );
	

}

void AudioGraphWidget::draw(const GUI::Point& p_pos,const GUI::Size& p_size,const GUI::Rect& p_exposed) {
	
	int layer_idx = (current_layer<0 || current_layer>=MAX_LAYER_COLORS) ? MAX_LAYER_COLORS : current_layer;
	
	
	get_painter()->draw_stylebox( stylebox( SB_LAYER_STYLE_BG_0+layer_idx), GUI::Point(), p_size );
	
	printf("total nodes: %i\n",song->get_audio_graph()->get_node_count());
	
	for (int i=0;i<song->get_audio_graph()->get_node_count();i++) {
	
		
		AudioNode *an = song->get_audio_graph()->get_node(i);
		
		if (!(current_layer==SHOW_ALL_LAYERS || an->get_layer()==0 || (an->get_layer() & (1<<layer_idx))))
			continue;
			
		GUI::Rect node_rect( GUI::Point(an->get_x(), an->get_y() ), get_node_size(an) );
		
		if (!p_exposed.intersects_with( node_rect ))
			continue; // don't bother
	
		draw_node( node_rect, an );
	}
	
}

void AudioGraphWidget::redraw() {

	update();
}

AudioGraphWidget::AudioGraphWidget(GUI_UpdateNotify *p_update_notify,Song *p_song) {

	p_update_notify->audio_graph_changed_signal.connect( this, &AudioGraphWidget::redraw );
	song=p_song;
	current_layer=0;
	
}


AudioGraphWidget::~AudioGraphWidget()
{
}


