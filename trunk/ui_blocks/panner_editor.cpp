//
// C++ Implementation: panner_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "panner_editor.h"
#include <Qt/qpainter.h>
#include <Qt/qevent.h>


namespace ReShaked {


void PannerEditor::paintEvent(QPaintEvent *e) {
	
	ERR_FAIL_COND(!pos || (quad_mode && !depth));
	
	QPainter p(this);
	p.drawPixmap(0,0,skin.bg);
	p.setClipRect(QRect(skin.border_w,skin.border_w,width()-skin.border_w*2,height()-skin.border_w*2));
	
	
	if (quad_mode) {
		
		int x=(int)(pos->get()*width())-skin.pos.width()/2;
		int y=(int)(depth->get()*height())-skin.pos.height()/2;
		p.drawPixmap(x,y,skin.pos);
				
	} else {
		
		int x=(int)(pos->get()*width())-skin.pos.width()/2;
		int y=(height()-skin.pos.height())/2;
		p.drawPixmap(x,y,skin.pos);
		
	}
	
}

void PannerEditor::mousePressEvent(QMouseEvent *e) {
	
	ERR_FAIL_COND(!pos || (quad_mode && !depth));
	
	
	if (e->button()!=Qt::LeftButton)
		return;
	
	click.drag=true;
	float x=(float)e->x()/(float)width();
	float y=(float)e->y()/(float)height();
	
	pos->set(x);
	if (quad_mode)
		depth->set( y );
	
	update();

	
}

void PannerEditor::mouseMoveEvent(QMouseEvent *e) {
	
	ERR_FAIL_COND(!pos || (quad_mode && !depth));

	if (!click.drag)
		return;
	
	float x=(float)e->x()/(float)width();
	float y=(float)e->y()/(float)height();
	
	pos->set(x);
	if (quad_mode)
		depth->set( y );
	
	update();
	
}

void PannerEditor::mouseReleaseEvent(QMouseEvent *e) {
	
	if (e->button()!=Qt::LeftButton)
		return;
	
	click.drag=false;
}


void PannerEditor::set_properties(Property *p_pos,Property *p_depth) {
	
	pos=p_pos;
	depth=p_depth;
	
}

PannerEditor::PannerEditor(QWidget *p_parent,bool p_quad,const Skin &p_skin) : QWidget(p_parent) {
	
	pos=NULL;
	depth=NULL;
	quad_mode=p_quad;
	
	skin=p_skin;
	click.drag=false;
	setBackgroundRole(QPalette::NoRole);
	setFixedSize(p_skin.bg.size());
	
	
}


PannerEditor::~PannerEditor()
{
}


}
