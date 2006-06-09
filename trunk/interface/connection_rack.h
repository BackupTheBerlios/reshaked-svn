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
#include "ui_blocks/pixmap_scrollbar.h"

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

					       

					       
class ConnectionRack : public QWidget {

	Q_OBJECT
	
				       
	enum {
		STEPS_DISTANCE=20,
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
	
	
	PixmapScrollBar *scrollbar;
	
	static bool fast_draw;
	
	int view_offset;
	SkinBox *skin(bool p_system=false);
	QPixmap jack_hole(AudioPlug::Type p_type=AudioPlug::TYPE_INPUT);
	int get_plugs_for_height();
	QSize get_plug_size();
	
	int get_node_width(AudioNode *p_node);

	void paint_curve(QPainter &p,int p_src_x,int p_src_y,int p_dst_x,int p_dst_y,int p_bottom);
	
	void paint_cable(QPainter &p,int p_src_x,int p_src_y,int p_dst_x,int p_dst_y);
	
	void paint_jack(QPainter&p, int p_x,int p_y, AudioPlug *p_plug,QString p_name, AudioPlug::Type p_type=AudioPlug::TYPE_INPUT);
	void paint_node(QPainter&p,int p_offset,AudioNode *p_node);
	void paintEvent(QPaintEvent *pe);
	
	QPoint get_input_plug_pos(int p_node,int p_plug);
	QPoint get_output_plug_pos(int p_node,int p_plug);
	
	
	bool get_plug_data_at_pos(int p_x,int p_y,PlugData* p_data);
	
	void mouseMoveEvent ( QMouseEvent * e );
	void mousePressEvent ( QMouseEvent * e );
	void mouseReleaseEvent ( QMouseEvent * e );
	
	
	void update_scrollbar();
	
//change depending on what we implement
	int get_total_width();
	
protected slots:	
	
	void scrollbar_changed_slot(int p_ofs);
	void set_view_offset(int p_ofs);
protected:
	
	Editor *editor;
	AudioGraph *graph;
	 
	virtual AudioNode *get_node_at_pos(int p_node);
	
public:
	
	void update_rack();
	
	void set_scrollbar(PixmapScrollBar *p_bar);
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
