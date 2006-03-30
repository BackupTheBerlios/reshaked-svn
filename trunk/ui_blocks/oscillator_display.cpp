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

namespace ReShaked {


void OscillatorDisplay::paintEvent(QPaintEvent *e) {
	
	
	
}


void OscillatorDisplay::set_oscillator(Oscillator *p_oscil) {
	
	oscil=p_oscil;
	update();
}


OscillatorDisplay::OscillatorDisplay(QWidget *p_parent) : QWidget(p_parent)
{
	
	oscil=NULL;
	
}


OscillatorDisplay::~OscillatorDisplay()
{
}


}
