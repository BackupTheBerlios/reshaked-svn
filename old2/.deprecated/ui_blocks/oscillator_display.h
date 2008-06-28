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
#include <Qt/qwidget.h>
#include "dsp/oscillator.h"
#include <Qt/qpixmap.h>

namespace ReShaked {

/**
	@author red <red@killy>
*/
class OscillatorDisplay : public QWidget {
	
public:	
	
	struct Skin {
		
		QPixmap bg;
		QColor bg_color;
		QColor wave_color;
		int margin;
		
		Skin(QColor p_bg=QColor(0,0,0), QColor p_wave=QColor(120,150,190)) { bg_color=p_bg; wave_color=p_wave; margin=5; }
		Skin(QPixmap p_bg, QColor p_wave=QColor(120,150,190)) { bg=p_bg; wave_color=p_wave; margin=5; }
	};
private:
	
	void paintEvent(QPaintEvent *e);
	
	Oscillator *oscil;
	
	Skin skin;
	
	int submap;
	
	void wheelEvent ( QWheelEvent * event );
public:
	
	void set_oscillator(Oscillator *p_oscil);
	
	OscillatorDisplay(QWidget *p_parent, const Skin& p_skin=Skin());
	
	~OscillatorDisplay();

};

}

#endif
