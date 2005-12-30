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
#include <Qt/qmenu.h>

#include "editor/editor.h"

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

class ConnectionRack : public QWidget {

				       
	enum {
		STEPS_DIVISOR=14,
		BOTTOM_DIVISOR=80,
		BOTTOM_BASE_OFFSET=20	
		
		
	};
				       
	
	struct PlugData {
		
		int graph_node;
		int plug;
		AudioPlug::Type type;
		int channels;
	};
	
	
	struct Connecting {
		
		bool enabled;
		PlugData from;
		QPoint to;
		
	} connecting;
	
	
	static bool fast_draw;
	
	int offset;
	SkinBox *skin(bool p_system=false);
	QPixmap jack_hole();
	int get_plugs_for_height();
	QSize get_plug_size();
	
	int get_node_width(AudioNode *p_node);

	void paint_curve(QPainter &p,int p_src_x,int p_src_y,int p_dst_x,int p_dst_y,int p_bottom);
	
	void paint_cable(QPainter &p,int p_src_x,int p_src_y,int p_dst_x,int p_dst_y);
	
	void paint_jack(QPainter&p, int p_x,int p_y, AudioPlug *p_plug,QString p_name);
	void paint_node(QPainter&p,int p_offset,AudioNode *p_node);
	void paintEvent(QPaintEvent *pe);
	
	QPoint ConnectionRack::get_input_plug_pos(int p_node,int p_plug);
	QPoint ConnectionRack::get_output_plug_pos(int p_node,int p_plug);
	
	
	bool get_plug_data_at_pos(int p_x,int p_y,PlugData* p_data);
	
	void mouseMoveEvent ( QMouseEvent * e );
	void mousePressEvent ( QMouseEvent * e );
	void mouseReleaseEvent ( QMouseEvent * e );
	
	
//change depending on what we implement
	
protected:
	
	Editor *editor;
	AudioGraph *graph;
	 
	virtual AudioNode *get_node_at_pos(int p_node);
	
public:
	
	void set_audio_graph(AudioGraph *p_graph);
	ConnectionRack(QWidget *p_parent,Editor *p_editor);
	~ConnectionRack();

};


class ConnectionRackTracks : public ConnectionRack {

	
	virtual AudioNode *get_node_at_pos(int p_node);
public:
	
	ConnectionRackTracks(QWidget *p_parent,Editor *p_editor);
	
};


}
#endif
