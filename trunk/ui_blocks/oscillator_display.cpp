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

namespace ReShaked {


void OscillatorDisplay::paintEvent(QPaintEvent *e) {
	
	QPainter p(this);
	p.fillRect(0,0,width(),height(),skin.bg_color);
	
	if (!oscil)
		return;
	
	int w=width();

	int prev_y;
	p.setPen(skin.wave_color);
	for (int i=0;i<w;i++) {
		
		float oscv=oscil->get_osc()[i*Oscillator::BASE_SIZE/w];
		oscv+=1.0;
		oscv/=2.0;
		oscv*=(float)height();
		int y=(int)oscv;
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
	skin=p_skin;
	oscil=NULL;
	
}


OscillatorDisplay::~OscillatorDisplay()
{
}


}
