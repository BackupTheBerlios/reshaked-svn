//
// C++ Implementation: range_level_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "range_level_editor.h"
#include <Qt/qevent.h>
#include <Qt/qpainter.h>
#include <Qt/qfontmetrics.h>

#include "dsp/formulas.h"
#include <math.h>
namespace ReShaked {


void RangeLevelEditor::paintEvent(QPaintEvent *e) {

	if (!scale_1 || !scale_2 || !range_1 || !range_2)
		return;

	QPainter p(this);
	p.drawPixmap(0,0,skin.bg);
	p.setPen(skin.line_color);
	p.setClipRect(QRect(skin.border_w,skin.border_w,width()-skin.border_w*2,height()-skin.border_w*2));
	
	for (int i=0;i<width();i++) {
	
		float from_x=(float)i/(float)width();
		if (from_x<range_1->get())
			continue;
		if (from_x>range_2->get())
			break;
		
		float x=from_x-range_1->get();
		
		float y;
		if (range_1->get()==range_2->get())
			y=scale_1->get();
		else
			y=scale_1->get() + x * ( scale_2->get()-scale_1->get() ) / (range_2->get()-range_1->get());
		
		int h=(int)(y*height());
	
		QColor col_from=skin.line_color;
		col_from.setAlpha(40);
		QColor col_to=skin.line_color;
		col_to.setAlpha(200);
	
		QLinearGradient gradient(i, height(), i, height()-h);
		gradient.setColorAt(0,col_from);
		gradient.setColorAt(1, col_to);
		p.setBrush(gradient);
		p.fillRect(i, height()-h,1,h,gradient);
	} 
	
	p.drawPixmap(range_1->get()*width()-skin.point_from.width()/2,height()-scale_1->get()*height()-skin.point_from.height()/2,skin.point_from);
	p.drawPixmap(range_2->get()*width()-skin.point_to.width()/2,height()-scale_2->get()*height()-skin.point_to.height()/2,skin.point_to);
	
	/* DRAG TEXT */
	if (!click.drag_point)
		return;
	char buf[20]; //sprintf is still the best :(
	bool at_left;
	bool at_top;
	if (click.drag_point==1) {
		
		sprintf(buf,"%.2f,%.2f",(float)range_1->get(),(float)scale_1->get());
		at_left=range_1->get()>0.5;
		at_top=scale_1->get()<0.5;
	} else {
		
		sprintf(buf,"%.2f,%.2f",(float)range_2->get(),(float)scale_2->get());
		at_left=range_2->get()>0.5;
		at_top=scale_2->get()<0.5;
	}
	
	QString str=buf;
	QFont font;
	font.setPixelSize(10);
	p.setFont(font);
	QFontMetrics fm(p.font());
	
	int txt_x=at_left?3:width()-(fm.width(str)+3);
	int txt_y=at_top?(fm.ascent()+3):(height()-(fm.descent()+3));
	
	p.setPen( QColor(0,0,0) );
	p.drawText(txt_x-1,txt_y-1,str);
	p.drawText(txt_x-1,txt_y+1,str);
	p.drawText(txt_x+1,txt_y-1,str);
	p.drawText(txt_x+1,txt_y+1,str);
	p.setPen( QColor(255,255,255) );
	p.drawText(txt_x,txt_y,str);
	
	
	
	
	
}

void RangeLevelEditor::mousePressEvent(QMouseEvent *e) {

	if (!scale_1 || !scale_2 || !range_1 || !range_2)
		return;

	if (e->button()!=Qt::LeftButton)
		return;

	float press_x=(float)e->x()/(float)width();
	float press_y=(float)(height()-e->y())/(float)height();
	
	float dist_1=sqrt(POW2(press_x-range_1->get())+POW2(press_y-scale_1->get()));
	float dist_2=sqrt(POW2(press_x-range_2->get())+POW2(press_y-scale_2->get()));
	
	int closer=(dist_1<dist_2)?1:2; //which is closer?
	float closer_dist=(closer==1)?dist_1:dist_2; //distance of closer
	
	
	click.drag_point=closer;
	click.scale=(closer==1)?scale_1->get():scale_2->get();
	click.range=(closer==1)?range_1->get():range_2->get();
	click.point=e->pos();
	
	

}
void RangeLevelEditor::mouseMoveEvent(QMouseEvent *e) {

	if (!scale_1 || !scale_2 || !range_1 || !range_2)
		return;

	if (!click.drag_point)
		return;
	
	
	int distance_x=e->pos().x()-click.point.x();
	int distance_y=click.point.y()-e->pos().y();
	
	float new_scale=click.scale+(float)distance_y/height();
	float new_range=click.range+(float)distance_x/width();
	
	if (click.drag_point==1) {
		if (new_range>=range_2->get())
			new_range=range_2->get();
		scale_1->set( new_scale );
		range_1->set( new_range );
	} else {
		if (new_range<=range_1->get())
			new_range=range_1->get();
		
		scale_2->set( new_scale );
		range_2->set( new_range );
	}
	
	update();

}
void RangeLevelEditor::mouseReleaseEvent(QMouseEvent *e) {

	
	if (!scale_1 || !scale_2 || !range_1 || !range_2)
		return;
	if (e->button()!=Qt::LeftButton)
		return;

	click.drag_point=0;
	update();
}

void RangeLevelEditor::set_properties(Property *p_scale_1,Property *p_scale_2,Property *p_range_1,Property *p_range_2) {
	
	range_1=p_range_1;
	scale_1=p_scale_1;
	range_2=p_range_2;
	scale_2=p_scale_2;
	
}

RangeLevelEditor::RangeLevelEditor(QWidget *p_parent,const Skin &p_skin) : QWidget(p_parent) {

	skin=p_skin;
	click.drag_point=0;
	setBackgroundRole(QPalette::NoRole);
	setFixedSize(p_skin.bg.size());

	scale_1=scale_2=range_1=range_2=NULL;

}


RangeLevelEditor::~RangeLevelEditor() {
	
}


}



