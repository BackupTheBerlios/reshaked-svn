//
// C++ Implementation: reverb_display_ir
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "reverb_display_ir.h"
#include "base/painter.h"
#include "gui_common/common_skin.h"
#include <math.h>

bool ReverbDisplayIR::must_precompute_buffer() {
	
	if ( !predelay   
		     || !predelay_fb   
		     || !hpf
		     || !room_size   
		     || !damping
		     || !spread )
		return false;
	
	int current_width = get_size_cache().width-get_painter()->get_stylebox_min_size( stylebox( SB_NODEUI_REVERB_IR_BG ) ).width;
	
	if (current_width!=buffer_size || 
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
	
	
	int current_width = get_size_cache().width-get_painter()->get_stylebox_min_size( stylebox( SB_NODEUI_REVERB_IR_BG ) ).width;
	
	if (current_width<1)
		return;
		   
	buffer = new float[current_width];
	buffer_size = current_width;
	
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
	//r.set_highpass( hpf->get() );
	r.set_predelay( predelay->get() );
	r.set_predelay_feedback( predelay_fb->get() );
	r.set_room_size( room_size->get() );
	r.set_extra_spread_base( 0 );
	r.set_mix_rate( buffer_size/(int)LEN_SECONDS );
	
	r.process( src, buffer, buffer_size );
	
	delete[] src;
	
	last_predelay=predelay->get();
	last_predelay_fb=predelay_fb->get();
	last_hpf=hpf->get();
	last_room_size=room_size->get();
	last_damping=damping->get();
	last_spread=spread->get();
	   	
}

GUI::Size ReverbDisplayIR::get_minimum_size_internal() {

	return GUI::Size( 0, constant( C_NODEUI_REVERB_IR_HEIGHT ) );
}

void ReverbDisplayIR::draw(const GUI::Point& p_pos,const GUI::Size& p_size,const GUI::Rect& p_exposed) {
	
	
	GUI::Painter *p=get_painter();
	
	p->draw_stylebox( stylebox( SB_NODEUI_REVERB_IR_BG ), GUI::Point(), p_size );
	
	if (must_precompute_buffer())
		precompute_buffer();
	
	
	if (!buffer)
		return;
	

	GUI::Point margin = GUI::Point( p->get_stylebox_margin( stylebox( SB_NODEUI_REVERB_IR_BG ), GUI::MARGIN_LEFT), 
		p->get_stylebox_margin( stylebox( SB_NODEUI_REVERB_IR_BG ), GUI::MARGIN_RIGHT) );
		
	GUI::Size area = p_size - p->get_stylebox_min_size( stylebox( SB_NODEUI_REVERB_IR_BG ) );

	ERR_FAIL_COND(buffer_size!=area.width);
	
	int wh=area.height;
	
	for (int i=0;i<buffer_size;i++) {
		
		
		int h=(int)(fabs(buffer[i])*wh);
		if (h>wh)
			h=wh;
			
		p->draw_fill_rect( margin + GUI::Point(i,wh-h),GUI::Size( 1, h ), color( COLOR_NODEUI_REVERB_IR_IMPULSE ) );
	}
	
}


void ReverbDisplayIR::check_for_changes() {
	
	if (must_precompute_buffer()) {
		update();
	}
	
}

void ReverbDisplayIR::set_properties(ControlPort *p_predelay, ControlPort *p_predelay_fb,ControlPort *p_hpf,ControlPort *p_room_size,ControlPort *p_damping,ControlPort *p_spread) {
	
	
	predelay=p_predelay;
	predelay_fb=p_predelay_fb;
	hpf=p_hpf;
	room_size=p_room_size;
	damping=p_damping;
	spread=p_spread;
		
	update();
}


ReverbDisplayIR::ReverbDisplayIR() {
	
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
	
}


ReverbDisplayIR::~ReverbDisplayIR() {

	if (buffer)
		delete[] buffer;
}

