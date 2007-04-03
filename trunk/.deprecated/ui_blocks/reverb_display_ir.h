//
// C++ Interface: reverb_display_ir
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDREVERB_DISPLAY_IR_H
#define RESHAKEDREVERB_DISPLAY_IR_H

#include "property.h"
#include "dsp/reverb.h"
#include <Qt/qwidget.h>
#include <Qt/qpixmap.h>
namespace ReShaked {

/**
	@author red <red@killy>
*/

class ReverbDisplayIR : public QWidget {

	Q_OBJECT
	
public:	
	
	struct Skin {
		
		QPixmap bg;
		QColor ir_color;
		int border;
		int seconds;
		
	};
private:
	
	Skin skin;
	
	Property *predelay;   
	Property *predelay_fb;   
	Property *hpf;
	Property *room_size;   
	Property *damping;
	Property *spread;
	
	double last_predelay;   
	double last_predelay_fb;   
	double last_hpf;
	double last_room_size;   
	double last_damping;
	double last_spread;
	
	float *buffer;
	int buffer_size;
	
	bool must_precompute_buffer();
	void precompute_buffer();
	void paintEvent(QPaintEvent *pe);
	
private slots:	

	void check_for_changes();
	
public:
	
	void set_properties(Property *p_predelay, Property *p_predelay_fb,Property *p_hpf,Property *p_room_size,Property *p_damping,Property *p_spread);
	
	ReverbDisplayIR(QWidget *p_parent,const Skin &p_skin);
	~ReverbDisplayIR();

};

}

#endif
