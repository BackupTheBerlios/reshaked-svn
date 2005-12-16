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

#include "editor/editor.h"

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

	void paintEvent(QPaintEvent *pe);
	
//change depending on what we implement
	
protected:
	
	Editor *editor;
	AudioGraph *graph;
	 
	virtual AudioNode *get_node_at_pos(int p_node)=0;
	
public:
	ConnectionRack(QWidget *p_parent,Editor *p_editor,AudioGraph *p_graph);
	~ConnectionRack();

};


class ConnectionRackTracks : public ConnectionRack {

	
	virtual AudioNode *get_node_at_pos(int p_node);
public:
	
	ConnectionRackTracks(QWidget *p_parent,Editor *p_editor);
	
};


}
#endif
