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
#include "ui_blocks/helpers.h"
namespace ReShaked {


SkinBox *ConnectionRack::skin(bool p_system) {
	
	return VisualSettings::get_singleton()->get_skin_box( p_system?SKINBOX_RACK_SYSTEM:SKINBOX_RACK_NORMAL );
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
	int plugs=iheight/get_plug_size().height();
	if (plugs==0)
		plugs=1;
	return plugs;
}

int ConnectionRack::get_node_width(AudioNode *p_node) {
	
	int iwidth=skin()->get_left()*2;
	int columns=(p_node->get_input_plug_count()/get_plugs_for_height());
	if (p_node->get_input_plug_count()%get_plugs_for_height())
		columns++; //we need more!
		
	columns+=(p_node->get_output_plug_count()/get_plugs_for_height());
	if (p_node->get_output_plug_count()%get_plugs_for_height())
		columns++; //we need more!
		
	iwidth+=columns*get_plug_size().width();		
		
	return iwidth;
}


void ConnectionRack::paint_jack(QPainter&p, int p_x,int p_y, AudioPlug *p_plug,QString p_name) {
	
	QFont f;
	f.setBold(true);
	p.setFont(f);
	p.setPen(QColor(255,255,255,120));
	p.drawPixmap(p_x,p_y+jack_hole().height(),jack_hole());
	p.drawText(p_x,p_y,jack_hole().width()*2,jack_hole().height(),Qt::AlignHCenter,p_name);
	p.drawText(p_x+jack_hole().width(),p_y+jack_hole().height(),jack_hole().width(),jack_hole().height(),Qt::AlignHCenter,QString::number(p_plug->get_channels()));
	
}

void ConnectionRack::paint_node(QPainter&p,int p_offset,AudioNode *p_node) {
	
	QFont f;
	f.setBold(true);
	p.setFont(f);
	f.setPixelSize(skin()->get_top()/2);
	p.setPen(QColor(255,255,255,120));
	int node_width=get_node_width(p_node);
	
	int font_margin=VisualSettings::get_singleton()->get_rack_panel_h_margin();
	p.drawText(p_offset+font_margin,0,node_width-font_margin*2,skin()->get_top(),Qt::AlignLeft,QStrify( p_node->get_caption() ) );
	
	p_offset+=skin()->get_left();
	int y_offset=skin()->get_top();
	
	
	
	for (int i=0;i<p_node->get_input_plug_count();i++) {
		
		if (i>0) {
			if ((i%get_plugs_for_height())==0) {
			
				y_offset=skin()->get_top();
				p_offset+=get_plug_size().width();
			} else {
				
				y_offset+=get_plug_size().height();
			}
		}
		
		paint_jack(p,p_offset,y_offset,p_node->get_output_plug( i),QStrify(p_node->get_input_plug_caption( i)));
	}
	
	if (p_node->get_output_plug_count()) {
		
		p_offset+=get_plug_size().width();
	}
	for (int i=0;i<p_node->get_output_plug_count();i++) {
		
		if (i>0) {
			if ((i%get_plugs_for_height())==0) {
				
				y_offset=skin()->get_top();
				p_offset+=get_plug_size().width();
			} else {
				
				y_offset+=get_plug_size().height();
			}
		}
		
		paint_jack(p,p_offset,y_offset,p_node->get_output_plug( i),QStrify(p_node->get_output_plug_caption( i)) );
	}
	
}

void ConnectionRack::paintEvent(QPaintEvent *pe) {
	
	
	QPainter p(this);
	p.fillRect(0,0,width(),height(),QColor(0,0,0));
	
	int ofs=0;
	printf("nodes %i\n",graph->get_connection_count());
	for (int i=0;i<graph->get_node_count();i++) {
			
		AudioNode *node=get_node_at_pos( i );
		int paint_width=get_node_width( node );
		skin()->paint_into( p, ofs, 0, paint_width, height() );
		paint_node( p, ofs, node);
				
		ofs+=paint_width;
		
	}
}

ConnectionRack::ConnectionRack(QWidget *p_parent,Editor *p_editor,AudioGraph *p_graph) : QWidget(p_parent) {
	
	graph=p_graph;
	editor=p_editor;
	setBackgroundRole(QPalette::NoRole);
}


ConnectionRack::~ConnectionRack() {
}





AudioNode *ConnectionRackTracks::get_node_at_pos(int p_node) {
	
	ERR_FAIL_INDEX_V(p_node,editor->get_song()->get_track_count(),NULL);
	return editor->get_song()->get_track( p_node );
	
}

ConnectionRackTracks::ConnectionRackTracks(QWidget *p_parent,Editor *p_editor) : ConnectionRack(p_parent,p_editor,&p_editor->get_song()->get_track_graph()) {

		
	
}

}