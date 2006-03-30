//
// C++ Interface: oscillator_display
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDOSCILLATOR_DISPLAY_H
#define RESHAKEDOSCILLATOR_DISPLAY_H

namespace ReShaked {

/**
	@author red <red@killy>
*/
class OscillatorDisplay : public QWidget {
	
	void paintEvent(QPaintEvent *e);
	
	Oscillator *oscil;
	
public:
	
	void set_oscillator(Oscillator *p_oscil);
	
	OscillatorDisplay(QWidget *p_parent);
	
	~OscillatorDisplay();

};

}

#endif
