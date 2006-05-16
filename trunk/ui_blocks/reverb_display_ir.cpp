//
// C++ Implementation: reverb_display_ir
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "reverb_display_ir.h"
#include <Qt/qtimer.h>
#include <Qt/qpainter.h>
namespace ReShaked {

bool ReverbDisplayIR::must_precompute_buffer() {
	
	if ( !predelay   
		     || !predelay_fb   
		     || !hpf
		     || !room_size   
		     || !damping
		     || !spread )
		return false;
	
	if (width()!=buffer_size || 
		   last_predelay!=predelay->get() ||
		   last_predelay_fb!=predelay_fb->get() ||
		   last_hpf!=hpf->get() ||
		   last_room_size!=room_size->get() ||
		   last_damping!=damping->get() ||
		   last_spread!=spread->get() )
	   
		return true;
				
		
	return false;	
	
}


void ReverbDisplayIR::precompute_buffer() {
	
	if (buffer)
		delete[] buffer;
	
	buffer=NULL;
	buffer_size=-1;
	
	if ( !predelay   
		     || !predelay_fb   
		     || !hpf
		     || !room_size   
		     || !damping
		     || !spread )
		return;
	
	
	
	if (width()<1)
		return;
		   
	buffer = new float[width()];
	buffer_size = width();
	
	float *src = new float[buffer_size];
	//clear them
	for (int i=0;i<buffer_size;i++) {
		
		buffer[i]=0;
		src[i]=0;
	}
	src[0]=1.0; //impulse
	
	Reverb r;
	r.set_damp( damping->get() );
	r.set_dry(0);
	r.set_wet(0.6);
	r.set_extra_spread( 0 );
	r.set_highpass( hpf->get() );
	r.set_predelay( predelay->get() );
	r.set_predelay_feedback( predelay_fb->get() );
	r.set_room_size( room_size->get() );
	r.set_extra_spread_base( 0 );
	r.set_mix_rate( buffer_size/skin.seconds );
	
	r.process( src, buffer, buffer_size );
	
	delete[] src;
	
	last_predelay=predelay->get();
	last_predelay_fb=predelay_fb->get();
	last_hpf=hpf->get();
	last_room_size=room_size->get();
	last_damping=damping->get();
	last_spread=spread->get();
	   	
}

void ReverbDisplayIR::paintEvent(QPaintEvent *pe) {
	
	
	QPainter p(this);
	p.drawPixmap(0,0,skin.bg);
	
	if (must_precompute_buffer())
		precompute_buffer();
	
	
	if (!buffer)
		return;
	
	ERR_FAIL_COND(buffer_size!=width());
	p.setPen(skin.ir_color);

	int wh=height()-skin.border*2;
	
	for (int i=0;i<buffer_size;i++) {
		
		
		float h=fabs(buffer[i]);
		h*=wh;
		
		
		
		p.drawLine(i,height()-skin.border*2,i,height()-skin.border*2-(int)h);
	}
	
}


void ReverbDisplayIR::check_for_changes() {
	
	if (must_precompute_buffer()) {
		update();
	}
	
}

void ReverbDisplayIR::set_properties(Property *p_predelay, Property *p_predelay_fb,Property *p_hpf,Property *p_room_size,Property *p_damping,Property *p_spread) {
	
	
	predelay=p_predelay;
	predelay_fb=p_predelay_fb;
	hpf=p_hpf;
	room_size=p_room_size;
	damping=p_damping;
	spread=p_spread;
		
	update();
}


ReverbDisplayIR::ReverbDisplayIR(QWidget *p_parent,const Skin &p_skin) : QWidget(p_parent) {
	
	skin=p_skin;
	setFixedSize(p_skin.bg.size());
	buffer=NULL;	
	buffer_size=-1;
	
	predelay=NULL;   
	predelay_fb=NULL;   
	hpf=NULL;
	room_size=NULL;   
	damping=NULL;
	spread=NULL;
	
	last_predelay=-1;   
	last_predelay_fb=-1;   
	last_hpf=-1;
	last_room_size=-1;   
	last_damping=-1;
	last_spread=-1;
	
	QTimer *updater = new QTimer(this);
	QObject::connect(updater,SIGNAL(timeout()),this,SLOT(check_for_changes()));
	updater->start(50);
	
}


ReverbDisplayIR::~ReverbDisplayIR()
{
}


}
