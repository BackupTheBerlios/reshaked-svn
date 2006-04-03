//
// C++ Implementation: route_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "route_editor.h"
#include <Qt/qevent.h>
#include <Qt/qpainter.h>
#include "error_macros.h"

namespace ReShaked {

void RouteEditor::paintEvent(QPaintEvent *e) {
	
	QPainter p(this);
	p.drawPixmap(0,0,skin.bg);
	
	
	p.setPen(skin.line_bg);
	
	for(int i=1;i<nodes;i++) { /* regular lines.. */
		
		
		int to_y=skin.node_size.height()*i+skin.node_size.height()/2;
		int to_x=skin.node_size.width()*i;
		
		
		for (int j=0;j<i;j++) {
			
			int ofs=get_ofs(i,j);
			ERR_CONTINUE(ofs==-1); 
			
			int from_y=skin.node_size.height()*(j+1);
			int from_x=skin.node_size.width()*j+skin.node_size.width()/2;
			
			if (ofs==cursor_over)
				p.drawPixmap(from_x-skin.cursor_over.width()/2,to_y-skin.cursor_over.height()/2,skin.cursor_over);
			
			p.drawLine(from_x,from_y,from_x,to_y);
			p.drawLine(from_x,to_y,to_x,to_y);
			
		}
	}
	
	p.setPen(skin.line_connected);
	
	for(int i=1;i<nodes;i++) {
		
		int to_y=skin.node_size.height()*i+skin.node_size.height()/2;
		int to_x=skin.node_size.width()*i;
		
		for (int j=0;j<i;j++) {
			
			int ofs=get_ofs(i,j);
			ERR_CONTINUE(ofs==-1); 
			
			int from_y=skin.node_size.height()*(j+1);
			int from_x=skin.node_size.width()*j+skin.node_size.width()/2;
			
			if (!are_connected( i,j) )
				continue;
			
			p.drawLine(from_x,from_y,from_x,to_y);
			p.drawLine(from_x,to_y,to_x,to_y);
			
			
		}
	}
	
}

int RouteEditor::get_click_pos(QPoint p_point) {
	
	
	int x= p_point.x()/skin.node_size.width();	
	int y= p_point.y()/skin.node_size.height();	
	int aux_over=-1;
	if (x<y) {
		aux_over=get_ofs(x,y);
		if (aux_over>=connections.size())
			aux_over=-1;
	}

	return aux_over;
	
	
}

void RouteEditor::mousePressEvent(QMouseEvent *e){
	
	cursor_over=get_click_pos( e->pos() );
	if (cursor_over>=0 && cursor_over<connections.size()) {
		
		connections[cursor_over]=!connections[cursor_over];
		update();
		matrix_changed_signal();		
	}
}
void RouteEditor::mouseMoveEvent(QMouseEvent *e){
	
	
	cursor_over=get_click_pos( e->pos() );
	
	update();
}

void RouteEditor::leaveEvent ( QEvent * ){
	
	cursor_over=-1;
	update();
	
}	


int RouteEditor::get_ofs(int p_node,int p_to_node) {
	
	if (p_node==p_to_node)
		return -1;
	if (p_node<0 || p_to_node<0)
		return -1;
	if (p_node>=nodes || p_to_node>=nodes)
		return -1;
	
	int maj=(p_to_node>p_node)?p_to_node:p_node;
	int min=(p_to_node<p_node)?p_to_node:p_node;
	
	return ((maj-1)*maj/2)+min;
	
}

void RouteEditor::connect_nodes(int p_node,int p_to_node) {
	
	int ofs=get_ofs(p_node,p_to_node);
	ERR_FAIL_COND(ofs==-1);
	ERR_FAIL_INDEX(ofs,connections.size());
	
	connections[ofs]=true;
	
	update();
}
void RouteEditor::disconnect_nodes(int p_node,int p_from_node) {
	
	int ofs=get_ofs(p_node,p_from_node);
	ERR_FAIL_COND(ofs==-1);
	ERR_FAIL_INDEX(ofs,connections.size());
	
	connections[ofs]=false;
	
	update();
	
}
bool RouteEditor::are_connected(int p_node,int p_with_node) {
	
	int ofs=get_ofs(p_node,p_with_node);
	ERR_FAIL_COND_V(ofs==-1,false);
	
	ERR_FAIL_INDEX_V(ofs,connections.size(),false);
	return connections[ofs];
}

RouteEditor::RouteEditor(QWidget *p_parent,int p_nodes,const Skin& p_skin) : QWidget(p_parent) {
	
	setMinimumSize(p_skin.bg.size());
	
	skin=p_skin;
	nodes=p_nodes;
	connections.resize( (nodes-1)*(nodes) / 2 );
	for (int i=0;i<connections.size();i++)
		connections[i]=false;
	cursor_over=-1;
	setMouseTracking(true);
	
}


RouteEditor::~RouteEditor()
{

}


}
