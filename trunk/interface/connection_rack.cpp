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
#include <Qt/qmessagebox.h>
#include <Qt/qevent.h>

namespace ReShaked {


static unsigned int rand_state=0;
bool ConnectionRack::fast_draw=true;



bool ConnectionRack::get_plug_data_at_pos(int p_x,int p_y,PlugData* p_data) {
	
	int jack_sqr_len=(POW2(jack_hole().height())+POW2(jack_hole().width()))/2;
	
	for (int i=0;i<graph->get_node_count();i++) {
		
		for (int j=0;j<graph->get_node(i)->get_input_plug_count();j++) {
			
			QPoint p = get_input_plug_pos( i, j);
			
			int sqr_distance=POW2(p_x-p.x())+POW2(p_y-p.y());
			//printf("Compare input %i,%i against %i,%i - %i<%i\n",p_x,p_y,p.x(),p.y(),sqr_distance,jack_sqr_len);
			if (sqr_distance>jack_sqr_len)
				continue;
			
			p_data->channels=graph->get_node(i)->get_input_plug(j)->get_channels();
			p_data->graph_node=i;
			p_data->plug=j;
			p_data->type=graph->get_node(i)->get_input_plug(j)->get_type();
			return false;				
			
		}
		
	}
	
	for (int i=0;i<graph->get_node_count();i++) {
		
		for (int j=0;j<graph->get_node(i)->get_output_plug_count();j++) {
			
			QPoint p = get_output_plug_pos( i, j);
			
			int sqr_distance=POW2(p_x-p.x())+POW2(p_y-p.y());
			//printf("Compare output %i,%i against %i,%i - %i<%i\n",p_x,p_y,p.x(),p.y(),sqr_distance,jack_sqr_len);
			if (sqr_distance>jack_sqr_len)
				continue;
			
			p_data->channels=graph->get_node(i)->get_output_plug(j)->get_channels();
			p_data->graph_node=i;
			p_data->plug=j;
			p_data->type=graph->get_node(i)->get_output_plug(j)->get_type();
			return false;				
			
		}
		
	}
	
	return true; //nothing found
	
}

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

QPoint ConnectionRack::get_input_plug_pos(int p_node,int p_plug) {
	
	ERR_FAIL_INDEX_V(p_node,graph->get_node_count(),QPoint(-1,-1));
	int ofs=0;
	for (int i=0;i<graph->get_node_count();i++) {
		if (graph->get_node(p_node)==get_node_at_pos(i))
			break;
		ofs+=get_node_width( get_node_at_pos( i ) );
	}
	
	
	ofs+=skin()->get_left();
	int y_ofs=skin()->get_top();
	
	int column=p_plug/get_plugs_for_height();
	int row=p_plug%get_plugs_for_height();
	
	ofs+=column*jack_hole().width()*2;
	y_ofs+=row*jack_hole().height()*2;
	
	ofs+=jack_hole().width()/2;
	y_ofs+=jack_hole().height()*3/2;
	
	return QPoint( ofs, y_ofs );
	
}

QPoint ConnectionRack::get_output_plug_pos(int p_node,int p_plug) {
	
	ERR_FAIL_INDEX_V(p_node,graph->get_node_count(),QPoint(-1,-1));
	int ofs=0;
	
	for (int i=0;i<graph->get_node_count();i++) {
		
		if (graph->get_node(p_node)==get_node_at_pos(i))
			break;
		ofs+=get_node_width( get_node_at_pos( i ) );
	}
	
	ofs+=skin()->get_left();
	int y_ofs=skin()->get_top();
	
	int input_plugs=get_node_at_pos( p_node )->get_input_plug_count();
	int input_columns=input_plugs?((input_plugs-1)/get_plugs_for_height()+1):0;

	ofs+=jack_hole().width()*2*input_columns;
	
	int column=p_plug/get_plugs_for_height();
	int row=p_plug%get_plugs_for_height();
	
	ofs+=column*jack_hole().width()*2;
	y_ofs+=row*jack_hole().height()*2;
	
	ofs+=jack_hole().width()/2;
	y_ofs+=jack_hole().height()*3/2;
	
	return QPoint( ofs, y_ofs );
	
	
}


void ConnectionRack::paint_curve(QPainter &p,int p_src_x,int p_src_y,int p_dst_x,int p_dst_y,int p_bottom) {

	int dist=(POW2(p_src_x-p_dst_x) + POW2(p_src_y-p_dst_y));
	int steps=dist/POW2(STEPS_DIVISOR); //squared stuff :)
	if (steps<0)
		steps=3;

	
	int x_prev=p_src_x;
	int y_prev=p_src_y;
	
	QPointF *points= new QPointF[steps+1]; //i guess using stack is wiser
	points[0]=QPointF(p_src_x,p_src_y);

	for (int i=1;i<=steps;i++) {
		
		float x=(float)p_src_x+(float)(i*(p_dst_x-p_src_x))/(float)steps;
		int y_src=p_src_y+i*((p_bottom-p_src_y)*2)/steps;
		int y_dst=p_dst_y+(steps-i)*((p_bottom-p_dst_y)*2)/steps;
		float ratio=(float)i/(float)steps;
		float y=ratio*(float)y_dst+(1.0-ratio)*(float)y_src;
		points[i]=QPointF(x,y);
	}
	
	p.drawPolyline(points,steps+1);
	delete points;
	
} 

void ConnectionRack::paint_cable(QPainter &p,int p_src_x,int p_src_y,int p_dst_x,int p_dst_y) {
	
	if (p_src_x==p_dst_x)
		return;
	if (p_dst_x<p_src_x) {
		
		SWAP(p_src_x,p_dst_x);
		SWAP(p_src_y,p_dst_y);
	}
	
	int dist=(POW2(p_src_x-p_dst_x) + POW2(p_src_y-p_dst_y));
	
	int bottom=(p_src_y>p_dst_y)?p_src_y:p_dst_y;
	bottom+=BOTTOM_BASE_OFFSET+dist/POW2(BOTTOM_DIVISOR);
	
	if (bottom>height())
		bottom=height();
	
	if (!fast_draw)
		p.setRenderHint(QPainter::Antialiasing,true);
	
	QColor c;
	QPen pn;
	pn.setWidth(4);
	/* draw shadow first */	
	
	if (fast_draw)
		pn.setColor(QColor(33,33,33));
	else
		pn.setColor(QColor(0,0,0,84));
	
	p.setPen(pn);
	paint_curve(p,p_src_x,p_src_y+3,p_dst_x,p_dst_y+3,bottom+12);	
	
	
	int h=rand_r(&rand_state)%255;
	c.setHsv(h,120,240);
	
	
	pn.setWidth(2);
	pn.setColor(c);
	p.setPen(pn);
	
	
	paint_curve(p,p_src_x,p_src_y,p_dst_x,p_dst_y,bottom);	
	c.setHsv(h,220,140);
	pn.setColor(c);
	p.setPen(pn);
	paint_curve(p,p_src_x,p_src_y+2,p_dst_x,p_dst_y+2,bottom+2);	
	
	if (!fast_draw)
		p.setRenderHint(QPainter::Antialiasing,false);
	
}



void ConnectionRack::paint_jack(QPainter&p, int p_x,int p_y, AudioPlug *p_plug,QString p_name) {
	
	QFont f;
	f.setBold(true);
	f.setPixelSize(10);
	p.setFont(f);
	p.setPen(QColor(255,255,255,120));
	
	p.drawPixmap(p_x,p_y+jack_hole().height(),jack_hole());
	p.drawText(p_x,p_y,jack_hole().width()*2,jack_hole().height(),Qt::AlignHCenter,p_name);
	
	f.setPixelSize(13);
	p.setFont(f);
	
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
	int text_area=node_width-font_margin*2;
	int align_flags;
	int str_len=QFontMetrics(p.font()).boundingRect(QStrify( p_node->get_caption() )).width();
	if (str_len>=text_area)
		align_flags=Qt::AlignLeft;
	else
		align_flags=Qt::AlignHCenter;
	
	p.drawText(p_offset+font_margin,0,text_area,skin()->get_top(),align_flags,QStrify( p_node->get_caption() ) );
	
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
		
		paint_jack(p,p_offset,y_offset,p_node->get_input_plug( i),QStrify(p_node->get_input_plug_caption( i)));
	}
	
	if (p_node->get_input_plug_count()) {
		
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



void ConnectionRack::mouseMoveEvent ( QMouseEvent * e ) {
	
	
	if (connecting.enabled) {
		
		connecting.to=QPoint(e->pos().x()+offset,e->pos().y());
		update();
	}
	
}
void ConnectionRack::mousePressEvent ( QMouseEvent * e ) {
	
	if (e->button()==Qt::RightButton || (e->button()==Qt::LeftButton && e->modifiers()&Qt::ControlModifier)) {
		/* DISCONNECT! */
		/* check if we are in some plug */
		PlugData info;
		if (get_plug_data_at_pos( e->x(),e->y(),&info)) 
			return;
		
		/* make list */
		
		std::vector<AudioGraph::Connection> connections;
		connections.clear();
		
		for (int i=0;i<graph->get_connection_count();i++) {
			
			AudioGraph::Connection c=*graph->get_connection( i );
			/* Just check if this connection uses the clicked plug */
			if ( (info.type==AudioPlug::TYPE_INPUT && c.node_to==info.graph_node && c.plug_to==info.plug) || (info.type==AudioPlug::TYPE_OUTPUT && c.node_from==info.graph_node && c.plug_from==info.plug) ) {
				
				
				connections.push_back(c);
			} 

		}
		
		if (connections.empty())
			return;
		else if (connections.size()==1) {
			/* if it's only one, just erase it */
		
			AudioGraph::Connection c=connections[0];
			editor->connection_erase( graph,  c.node_from, c.plug_from, c.node_to, c.plug_to );
			return;
		}
		
		
		QList<QAction*> actions;			
		
		foreach(I, connections) {
			
			QString node_from=QStrify(graph->get_node( I->node_from )->get_caption() );
			QString node_to=QStrify(graph->get_node( I->node_to )->get_caption() );
			QString plug_from=QStrify(graph->get_node( I->node_from )->get_output_plug_caption(I->plug_from)  );
			QString plug_to=QStrify(graph->get_node( I->node_to )->get_input_plug_caption(I->plug_to)  );
			
			QString conn_text=node_from+"-"+plug_from+"   ->   "+node_to+"-"+plug_to;
			
			actions.push_back( new QAction(conn_text,this) );
		}
		
		actions.push_back( new QAction("Disconnect All",this) );
		
		QAction *which=QMenu::exec(actions,mapToGlobal(e->pos()));
		
		if (which) {
			
			int index=0;
			
			foreach(I,actions) {
				
				if (*I==which)
					break;
						
				index++;
			}
			if (index==connections.size()) { //disconnect all
				
				editor->begin_meta_undo_block("Delete ALL Connections");
				
				for (int i=0;i<connections.size();i++) {
					AudioGraph::Connection c=connections[i];
					editor->connection_erase( graph,  c.node_from, c.plug_from, c.node_to, c.plug_to );
				}
				
				editor->end_meta_undo_block();
				
			} else if (index<connections.size()) { //disconnect one
				
				AudioGraph::Connection c=connections[index];
				editor->connection_erase( graph,  c.node_from, c.plug_from, c.node_to, c.plug_to );
				
			} else {
				ERR_PRINT("WTF?");
			}
		}
		
		foreach(I,actions) {
			
			delete *I;
		}
	
		return;
	}
	
	if (e->button()!=Qt::LeftButton)
		return;

	PlugData info;
	if (get_plug_data_at_pos( e->x(),e->y(),&info)) 
		return;
	
	printf("found node %i plug %i\n",info.graph_node,info.plug);
	connecting.from=info;
	connecting.enabled=true;
	
	
}
void ConnectionRack::mouseReleaseEvent ( QMouseEvent * e ) {
	
	if (connecting.enabled) {
		
		PlugData dest_plug;
	
		if (!get_plug_data_at_pos( e->x(),e->y(),&dest_plug)) {
			
			PlugData src_plug=connecting.from;
			if (src_plug.graph_node!=dest_plug.graph_node && src_plug.type!=dest_plug.type) {
					
				if (dest_plug.type==AudioPlug::TYPE_OUTPUT) {
					
					SWAP(dest_plug,src_plug);
				}
				
				switch(editor->connection_create( graph,src_plug.graph_node, src_plug.plug, dest_plug.graph_node, dest_plug.plug)) {
					
					case AudioGraph::CONNECT_CHANNELS_DIFFER: {
						
						QMessageBox::critical(this,"Can't Connect",QString("The amount of channels differ between plugs. (")+QString::number(src_plug.channels)+","+QString::number(dest_plug.channels)+")");
						
					} break;
					case AudioGraph::CONNECT_ALREADY_EXISTS: {
						
						QMessageBox::critical(this,"Can't Connect","The connection attempted already exists!");
						
					} break;
					case AudioGraph::CONNECT_CYCLIC_LINK: {
						
						
						QMessageBox::critical(this,"Can't Connect","The connection attempted would create a Cyclic Link. Cyclic infinite flow of data. Audio data must go from input to output without any feedback.");
						
					} break;
					case AudioGraph::CONNECT_INVALID_NODE:
					case AudioGraph::CONNECT_INVALID_PLUG: {
						
						QMessageBox::critical(this,"Can't Connect","Invalid Node or Plug. This is most probably a bug, so please report it.");
						
					} break;
					
					default: {};
				}
			}
		}

	}
	
	connecting.enabled=false;
	update();
	
}



void ConnectionRack::paintEvent(QPaintEvent *pe) {
	
	
	QPainter p(this);
	p.fillRect(0,0,width(),height(),QColor(0,0,0));
	
	int ofs=0;
	//printf("nodes %i\n",graph->get_connection_count());
	for (int i=0;i<graph->get_node_count();i++) {
			
		AudioNode *node=get_node_at_pos( i );
		int paint_width=get_node_width( node );
		if (node->is_system())
			skin(true)->paint_into( p, ofs, 0, paint_width, height() );
		else
			skin()->paint_into( p, ofs, 0, paint_width, height() );
		paint_node( p, ofs, node);
				
		ofs+=paint_width;
		
	}
	
	rand_state=123; //anything goes
	
	for (int i=0;i<graph->get_connection_count();i++) {
		
		QPoint from=get_output_plug_pos(graph->get_connection(i)->node_from,graph->get_connection(i)->plug_from);
		QPoint to=get_input_plug_pos(graph->get_connection(i)->node_to,graph->get_connection(i)->plug_to);
		paint_cable(p,from.x(),from.y(),to.x(),to.y());
		
	}
	
	if (connecting.enabled) {
		
		QPoint from;
		if (connecting.from.type==AudioPlug::TYPE_INPUT)
			from=get_input_plug_pos( connecting.from.graph_node, connecting.from.plug );
		else
			from=get_output_plug_pos( connecting.from.graph_node, connecting.from.plug );
				 
		QPoint to=connecting.to;
		paint_cable(p,from.x(),from.y(),to.x(),to.y());
		
	}
}

ConnectionRack::ConnectionRack(QWidget *p_parent,Editor *p_editor,AudioGraph *p_graph) : QWidget(p_parent) {
	
	graph=p_graph;
	editor=p_editor;
	setBackgroundRole(QPalette::NoRole);
	setMouseTracking(true);
	
	connecting.enabled=false;
	offset=0;
	
}


ConnectionRack::~ConnectionRack() {
}





AudioNode *ConnectionRackTracks::get_node_at_pos(int p_node) {
	
	ERR_FAIL_INDEX_V(p_node,editor->get_song()->get_track_count()+2,NULL);
	if (p_node==0)
		return editor->get_song()->get_input_node();
	else if (p_node==(editor->get_song()->get_track_count()+1))
		return editor->get_song()->get_output_node();
	else 
		return editor->get_song()->get_track( p_node-1 );
}

ConnectionRackTracks::ConnectionRackTracks(QWidget *p_parent,Editor *p_editor) : ConnectionRack(p_parent,p_editor,&p_editor->get_song()->get_track_graph()) {

		
	
}

}