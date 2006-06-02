//
// C++ Implementation: filter_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "filter_editor.h"

#include <Qt/qevent.h>
#include <Qt/qpainter.h>
#include <Qt/qfontmetrics.h>

#include "dsp/formulas.h"
#include "dsp/filter.h"
#include <math.h>

namespace ReShaked {

#define FREQ_2_LOG( m_val ) ( log( (m_val) ) / log (2) )

#define LOG_2_FREQ( m_val ) ( pow(2, (m_val)) )


static float float_2_freq(float p_float,float p_min,float p_max) {
	
	float logfbeg=FREQ_2_LOG(p_min);
	float logfend=FREQ_2_LOG(p_max);
	float logf=logfbeg+(logfend-logfbeg)*p_float;
	return LOG_2_FREQ(logf);
	
}


static int freq_2_pixel(float p_freq,float p_min,float p_max,float p_width) {
	
	float w=p_width;
	float logfbeg=FREQ_2_LOG(p_min);
	float logfend=FREQ_2_LOG(p_max);
	float logf=FREQ_2_LOG(p_freq);

	return (int) (w * ( (logf-logfbeg)/(logfend-logfbeg)));
}

void FilterEditor::paintEvent(QPaintEvent *e) {

	ERR_FAIL_COND( !mode || !cutoff || !resonance);

	QPainter p(this);
	p.drawPixmap(0,0,skin.bg);
	p.setPen(skin.line_color);
	p.setClipRect(QRect(skin.border_w,skin.border_w,width()-skin.border_w*2,height()-skin.border_w*2));
	
	
	
	Filter f;
	
	switch ((int)mode->get()) {
		
		case 0: return; break;
		case 1: f.set_mode( Filter::LOWPASS ); break;		
		case 2: f.set_mode( Filter::BANDPASS ); break;		
		case 3: f.set_mode( Filter::HIGHPASS ); break;		
		case 4: f.set_mode( Filter::NOTCH ); break;		
	}
	
	f.set_cutoff( cutoff->get() );
	f.set_resonance( resonance->get() );
	

	Filter::Coeffs fc;
	f.prepare_coefficients(&fc);

	
	for (int i=0;i<width();i++) {
		
		
		float freq= float_2_freq(  (float)i/(float)width()  ,MIN_FREQ,cutoff->get_max() );
		
		float response=sqrt( f.get_response( freq, &fc ) );
		response/=(float)AMP_STEPS;
		
		int h=(int)(response*height());
	
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
	/* TEXT */
	
	char buf[20]; //sprintf is still the best :(
	bool left;
	
	left=(freq_2_pixel(cutoff->get(),MIN_FREQ,cutoff->get_max(),width())>(width()/2));
	
	if (cutoff->get() < 1000.0 ) {
		
		sprintf(buf,"%ihz",(int)cutoff->get());
	} else {
		
		sprintf(buf,"%.1fkhz",cutoff->get()/1000.0);
	}
		
	QString str=buf;
	QFont font;
	font.setPixelSize(12);
	p.setFont(font);
	QFontMetrics fm(p.font());
	
	int txt_x=left?3:width()-(fm.width(str)+3);
	int txt_y=fm.ascent()+3;
	
	p.setPen( QColor(0,0,0) );
	p.drawText(txt_x-1,txt_y-1,str);
	p.drawText(txt_x-1,txt_y+1,str);
	p.drawText(txt_x+1,txt_y-1,str);
	p.drawText(txt_x+1,txt_y+1,str);
	p.setPen( QColor(255,255,255) );
	p.drawText(txt_x,txt_y,str);
	

	
	
	
}

void FilterEditor::changed(int) {
	
	update();
}


void FilterEditor::mousePressEvent(QMouseEvent *e) {

	ERR_FAIL_COND( !mode || !cutoff || !resonance);

	if (e->button()!=Qt::LeftButton)
		return;

	float press_x=(float)e->x()/(float)width();
	float press_y=(float)(height()-e->y())/(float)height();

	
	float freq=float_2_freq(press_x,MIN_FREQ,cutoff->get_max());
	float reso=press_y*resonance->get_max();
	
	set( PROPERTY_CUTOFF , freq );
	set( PROPERTY_RESONANCE , reso );
	
	click.drag=true;
	
	update();

}
void FilterEditor::mouseMoveEvent(QMouseEvent *e) {

	ERR_FAIL_COND( !mode || !cutoff || !resonance);

	
	if (!click.drag)
		return;
	
	float press_x=(float)e->x()/(float)width();
	float press_y=(float)(height()-e->y())/(float)height();

	float freq=float_2_freq(press_x,MIN_FREQ,cutoff->get_max());
	float reso=press_y*resonance->get_max();
	
	set( PROPERTY_CUTOFF , freq );
	set( PROPERTY_RESONANCE , reso );

	update();

}
void FilterEditor::mouseReleaseEvent(QMouseEvent *e) {

	
	ERR_FAIL_COND( !mode || !cutoff || !resonance);
	
	if (e->button()!=Qt::LeftButton)
		return;

	click.drag=false;
	update();
}

void FilterEditor::set_properties(Property *p_mode,Property *p_cutoff,Property *p_resonance) {
	
	mode=p_mode;
	cutoff=p_cutoff;
	resonance=p_resonance;
	set_property( PROPERTY_CUTOFF , p_cutoff );
	set_property( PROPERTY_RESONANCE , p_resonance );
	
}

FilterEditor::FilterEditor(QWidget *p_parent,const Skin &p_skin) : QWidget(p_parent), MultiPropertyEditor(2) {

	skin=p_skin;
	click.drag=false;
	setBackgroundRole(QPalette::NoRole);
	setFixedSize(p_skin.bg.size());

	mode=cutoff=resonance=NULL;

}


FilterEditor::~FilterEditor() {
	
}


}

