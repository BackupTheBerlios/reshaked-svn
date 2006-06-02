//
// C++ Implementation: oscillator_display
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "oscillator_display.h"

#include <Qt/qpainter.h>
#include <Qt/qevent.h>

namespace ReShaked {


void OscillatorDisplay::wheelEvent ( QWheelEvent * event ) {
	
	if (event->delta()>0)
		submap++;
	else if (event->delta()<0)
		submap--;
	
	if (submap<0)
		submap=0;
	
	update();

}


void OscillatorDisplay::paintEvent(QPaintEvent *e) {
	
	QPainter p(this);
	if (skin.bg.isNull())
		p.fillRect(0,0,width(),height(),skin.bg_color);
	else
		p.drawPixmap(0,0,skin.bg);
	
	if (!oscil)
		return;
	
	int w=width();

	int prev_y;
	p.setPen(skin.wave_color);
	
	int h=height()-skin.margin*2;
	
	int submap_size=(1<<oscil->get_osc_bits(submap));
	
	printf("submap %i, size %i\n",submap,submap_size);
	for (int i=0;i<w;i++) {
		
		float oscv=oscil->get_osc(submap)[i*submap_size/w];
		oscv+=1.0;
		oscv/=2.0;
		oscv*=(float)h;
		int y=skin.margin+(int)oscv;
		
		if (i==0)
			prev_y=y;
		int prev_x=(i==0)?0:(i-1);
		
		p.drawLine(prev_x,prev_y,i,y);
		prev_y=y;
	}
	
}


void OscillatorDisplay::set_oscillator(Oscillator *p_oscil) {
	
	oscil=p_oscil;
	update();
}


OscillatorDisplay::OscillatorDisplay(QWidget *p_parent, const Skin& p_skin) : QWidget(p_parent)
{
	if (!p_skin.bg.isNull())
		setFixedSize(p_skin.bg.size());
	
	skin=p_skin;
	oscil=NULL;
	submap=0;
	
}


OscillatorDisplay::~OscillatorDisplay()
{
}


}
