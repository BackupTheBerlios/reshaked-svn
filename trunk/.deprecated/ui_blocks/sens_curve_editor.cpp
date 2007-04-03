//
// C++ Implementation: sens_curve_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sens_curve_editor.h"


#include <Qt/qevent.h>
#include <Qt/qpainter.h>

#include "dsp/formulas.h"


namespace ReShaked {


void SensCurveEditor::paintEvent(QPaintEvent *e) {

	if (!get_property())
		return;

	QPainter p(this);
	p.drawPixmap(0,0,skin.bg);
	p.setPen(skin.line_color);
	p.setClipRect(QRect(skin.border_w,skin.border_w,width()-skin.border_w*2,height()-skin.border_w*2));

	double val=get_property()->get();

	for (int i=0;i<width();i++) {
	
	
		int h=(int)( vel_sens_function( (float)i/(float)width() , val ) * height() );
	
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
}

void SensCurveEditor::mousePressEvent(QMouseEvent *e) {

	if (!get_property())
		return;

	if (e->button()!=Qt::LeftButton)
		return;

	click.drag=true;
	click.point=e->pos();
	click.val=get();

}
void SensCurveEditor::mouseMoveEvent(QMouseEvent *e) {

	if (!get_property())
		return;

	if (!click.drag)
		return;

	int distance=click.point.x()-e->pos().x();
	float set_val_to=click.val+(float)distance/100.0;

	set(set_val_to);
	update();

}
void SensCurveEditor::mouseReleaseEvent(QMouseEvent *e) {

	if (!get_property())
		return;
	if (e->button()!=Qt::LeftButton)
		return;

	click.drag=false;
}

void SensCurveEditor::changed() {

	update();
}

SensCurveEditor::SensCurveEditor(QWidget *p_parent,const Skin &p_skin) : QWidget(p_parent) {

	skin=p_skin;
	click.drag=false;
	setBackgroundRole(QPalette::NoRole);
	setFixedSize(p_skin.bg.size());


}


SensCurveEditor::~SensCurveEditor()
{
}


}
