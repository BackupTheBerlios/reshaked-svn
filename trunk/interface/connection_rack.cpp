//
// C++ Implementation: connection_rack
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "connection_rack.h"
#include "ui_blocks/visual_settings.h"

namespace ReShaked {


SkinBox *ConnectionRack::skin(bool p_system) {
	
	return VisualSettings::get_singleton()->get_skin_box( p_system?SKINBOX_RACK_NORMAL:SKINBOX_RACK_SYSTEM );
}

QPixmap ConnectionRack::jack_hole() {
	
	return GET_QPIXMAP(PIXMAP_RACK_JACK_HOLE);
}

QSize ConnectionRack::get_plug_size() {
	
	return QSize(jack_hole().width()*2,jack_hole().height()*2);
}
int ConnectionRack::get_plugs_for_height() {
	
	int iheight=height();
	iheight-=skin()->get_top() + skin()->get_bottom();
	return iheight/get_plug_size().height();	
}

int ConnectionRack::get_node_width(AudioNode *p_node) {
	
	int iwidth=skin()->get_left()+skin()->get_right();
	int columns=(p_node->get_input_plug_count()/get_plugs_for_height());
	if (p_node->get_input_plug_count()%get_plugs_for_height())
		columns++; //we need more!
		
	columns+=(p_node->get_output_plug_count()/get_plugs_for_height());
	if (p_node->get_output_plug_count()%get_plugs_for_height())
		columns++; //we need more!
		
	iwidth+=columns*get_plug_size().width();		
		
	return iwidth;
}

ConnectionRack::ConnectionRack(QWidget *p_parent) : QWidget(p_parent) {
	
	
}


ConnectionRack::~ConnectionRack() {
}


}
