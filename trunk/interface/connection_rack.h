//
// C++ Interface: connection_rack
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDCONNECTION_RACK_H
#define RESHAKEDCONNECTION_RACK_H
#include "engine/audio_graph.h"
#include "ui_blocks/skin_box.h"

#include <Qt/qwidget.h>

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

class ConnectionRack : public QWidget {

	int offset;
	SkinBox *skin(bool p_system=false);
	QPixmap jack_hole();
	int get_plugs_for_height();
	QSize get_plug_size();
	
	int get_node_width(AudioNode *p_node);

	
public:
	ConnectionRack(QWidget *p_parent);
	~ConnectionRack();

};

}

#endif
