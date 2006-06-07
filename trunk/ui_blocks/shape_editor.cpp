//
// C++ Implementation: shape_editor
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "shape_editor.h"
#include <Qt/qpainter.h>
#include <Qt/qpen.h>
#include <Qt/qevent.h>
#include <math.h>
namespace ReShaked {

QPoint ShapeEditor::get_screen_pos(float p_pos,float p_val) {
	
	float v=p_val;
		
	v-=shape->get_min();
	v/=shape->get_max()-shape->get_min();
		
	int y = lrint(v*(float)height());
		
	if (shape->has_display_inverted())
		y=height()-y;
		
	int x = lrint( p_pos * (float)width() );
	
	return QPoint(x,y);
	
}

void ShapeEditor::get_shape_pos(QPoint p_pos,float *r_val, float *r_pos) {
	
	
	*r_pos=(float)p_pos.x()/(float)width();
	
	float sv=p_pos.y();
	sv/=(float)height();
	
	if (shape->has_display_inverted())
		sv=1.0-sv;
	
	sv*=shape->get_max()-shape->get_min();
	sv=shape->get_min()+sv;
	
	*r_val=sv;
}

void ShapeEditor::paintEvent(QPaintEvent *e) {
	
	QPainter p(this);
	
	if (skin.bg.isNull())
		p.fillRect(0,0,width(),height(),skin.bg_col);
	else
		p.drawPixmap(0,0,skin.bg);
	
	if (!shape)
		return;
	
	int w=width();
	
	int h=height();
	
	p.setPen( skin.zero_col );
	
	float none,zero;
	
	QPoint zerop=get_screen_pos( 0, skin.zero_pos );
	
	p.drawLine( 0, zerop.y(), width(), zerop.y() );
	
	p.setPen( skin.line_col );
	
	int prev_y;
	for (int i=0;i<w;i++) {
		
		float c=(float)i/(float)w;
		
		float v=shape->get_value( c );
		
		v-=shape->get_min();
		v/=shape->get_max()-shape->get_min();
		
		int y = lrint(v*(float)h);
		
		if (shape->has_display_inverted())
			y=h-y;
		
		
		if (i==0)
			prev_y=y;
		p.drawLine(i,prev_y,i,y);
		
		prev_y=y;

	}
		
	QPen pen( skin.point_col );
	pen.setWidth(3);
	p.setPen( pen );
	
	for (int i=0;i<shape->get_stream_size();i++) {
		
		QPoint pos=get_screen_pos( shape->get_index_pos( i ), shape->get_index_value( i ) );
		
		p.drawPoint(pos);
		
	}
	
}


int ShapeEditor::get_closest_point( QPoint p_pos ) {
	
	int move_idx=-1;
		
	float closest=999999;
	for (int i=0;i<shape->get_stream_size();i++) {
		
			
		QPoint pos=get_screen_pos( shape->get_index_pos( i ), shape->get_index_value( i ) );
		
		float dist=sqrtf( powf(pos.x()-p_pos.x(),2.0) + powf(pos.y()-p_pos.y(),2.0) );
			
		if (dist<closest && dist<GRAB_NEAR_TO) {
				
			closest=dist;
			move_idx=i;
		}
	}
	
	return move_idx;
}

void ShapeEditor::mousePressEvent(QMouseEvent *e) {
	
	if (!shape)
		return;
	
	if ( (e->button()==Qt::LeftButton && e->modifiers()&Qt::ControlModifier) || e->button()==Qt::RightButton ) {
		
		int move_idx=get_closest_point( e->pos() );
		if (move_idx>0) { //0 cant be erased
		
			shape->erase_index( move_idx );
			
		}
	} else if (e->button()==Qt::LeftButton) {
		
		int move_idx=get_closest_point( e->pos() );
		
		if (move_idx==-1) {
			
			float val,pos;
			get_shape_pos( e->pos(), &val,&pos);
			
			moving.move_idx=shape->insert( pos, val );
			
		} else {
			
			moving.move_idx=move_idx;
			
		}
	
		update();
		
		shape_changed_signal();
	}
	
}

void ShapeEditor::mouseReleaseEvent(QMouseEvent *e) {
	
	if (!shape)
		return;
	
	moving.move_idx=-1;
	update();
	
}

void ShapeEditor::mouseMoveEvent(QMouseEvent *e) {
	
	if (!shape)
		return;
	
	
	if (moving.move_idx<0)
		return;
	
	float val,pos;
	get_shape_pos( e->pos(), &val,&pos);
	
	if (moving.move_idx==0)
		pos=0; //the first one cant be moved
	
	float prev_xlimit = (moving.move_idx>0)?(shape->get_index_pos( moving.move_idx-1 )+0.0000001):0;
	float next_xlimit = (moving.move_idx<(shape->get_stream_size()-1))?(shape->get_index_pos( moving.move_idx+1 )-0.0000001):1.0;
	
	if (prev_xlimit>next_xlimit)
		return; //do nothing
	
	if (pos<prev_xlimit)
		pos=prev_xlimit;
	if (pos>next_xlimit)
		pos=next_xlimit;
	
	if (val<shape->get_min())
		val=shape->get_min();
	if (val>shape->get_max())
		val=shape->get_max();
	
	shape->erase_index( moving.move_idx );
	moving.move_idx=shape->insert( pos, val );
	
	update();
	shape_changed_signal();
}

void ShapeEditor::set_shape(Shape *p_shape) {
	
	shape=p_shape;
	update();
}

ShapeEditor::ShapeEditor(QWidget *p_parent,const Skin& p_skin) : QWidget(p_parent) {
	
	skin=p_skin;
	if (!skin.bg.isNull())
		setFixedSize(skin.bg.size());
	shape=NULL;
	moving.move_idx=-1;
}


ShapeEditor::~ShapeEditor()
{
}


}
