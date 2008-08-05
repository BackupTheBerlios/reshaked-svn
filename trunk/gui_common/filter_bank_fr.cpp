//
// C++ Implementation: filter_bank_fr
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "filter_bank_fr.h"
#include "gui_common/common_skin.h"
#include "dsp/formulas.h"

#define FREQ_2_LOG( m_val ) ( ::logf( (m_val) ) / ::logf (2) )

#define LOG_2_FREQ( m_val ) ( ::powf(2, (m_val)) )


static float float_2_freq(float p_float,float p_min,float p_max) {
	
	float lbeg=FREQ_2_LOG(p_min);
	float lend=FREQ_2_LOG(p_max);
	float l=lbeg+(lend-lbeg)*p_float;
	return LOG_2_FREQ(l);
	
}


static int freq_2_pixel(float p_freq,float p_min,float p_max,float p_width) {
	
	float w=p_width;
	float logfbeg=FREQ_2_LOG(p_min);
	float logfend=FREQ_2_LOG(p_max);
	float logf=FREQ_2_LOG(p_freq);

	return (int) (w * ( (logf-logfbeg)/(logfend-logfbeg)));
}


GUI::Size FilterBankFR::get_minimum_size_internal() {

	return GUI::Size( constant( C_NODEUI_FILTERBANK_FR_MIN_WIDTH) , 0 );
}

void FilterBankFR::draw(const GUI::Point& p_global,const GUI::Size& p_size,const GUI::Rect& p_exposed) {
		
	if (p_size.width<1)
		return;
	
	
	GUI::Painter *p=get_painter();
	
	p->draw_stylebox( stylebox( SB_NODEUI_FILTER_BANK_FR_BG ), GUI::Point(), p_size );
	
	GUI::Size areasize = p_size - p->get_stylebox_min_size( stylebox( SB_NODEUI_FILTER_BANK_FR_BG ) );
	GUI::Point margin = GUI::Point( p->get_stylebox_margin( stylebox( SB_NODEUI_FILTER_BANK_FR_BG ), GUI::MARGIN_LEFT), 
		p->get_stylebox_margin( stylebox( SB_NODEUI_FILTER_BANK_FR_BG ), GUI::MARGIN_RIGHT) );
		
	if (response && response_len!=areasize.width) {
	
		delete[] response;
		response=NULL;
	}
	
	if (!response) {
	
		response_len=areasize.width;
		response = new float[response_len];
	}
		
	/* Frequency response graph */
	float *response = new float[areasize.width];
	int response_len=areasize.width;
	
	for (int i=0;i<response_len;i++)
		response[i]=0;
	
	
	float response_roof=0;
	
	for (int i=0;i<filters;i++) {
		
		//just check
		ERR_CONTINUE(filter[i].mode==NULL || filter[i].stages==NULL || filter[i].cutoff==NULL || filter[i].resonance==NULL || cutoff_offset==NULL);
			
		
		Filter f;
	
		bool disabled=false;
		
		switch ((int)filter[i].mode->get()) {
		
			case 0: disabled=true; break;
			case 1: f.set_mode( Filter::LOWPASS ); break;		
			case 2: f.set_mode( Filter::BANDPASS ); break;		
			case 3: f.set_mode( Filter::HIGHPASS ); break;		
			case 4: f.set_mode( Filter::NOTCH ); break;		
		}
	
		if (disabled)
			continue;
		
		f.set_cutoff( filter[i].cutoff->get()*powf( 2 , cutoff_offset->get() ) );

		f.set_resonance( filter[i].resonance->get() );
		f.set_stages( (int)filter[i].stages->get()+1 );
	

		Filter::Coeffs fc;
		f.prepare_coefficients(&fc);

	
		for (int j=0;j<response_len;j++) {
		
		
			float freq= float_2_freq(  (float)j/(float)response_len  ,MIN_FREQ,filter[i].cutoff->get_max() );
		
			float freqresponse=f.get_response( freq, &fc );
			freqresponse=powf(freqresponse,filter[i].stages->get()+1.0); //apply stages
			
			response[j]+=freqresponse;
			if (response[j]>response_roof)
				response_roof=response[j];
			
		}
	}
	
	
	float linear_roof=sqrt(response_roof);
	
	if (linear_roof<AMP_STEPS)
		linear_roof=AMP_STEPS;
		
	for (int i=0;i<response_len;i++) {
		
		int h=(int)( sqrt(response[i])*areasize.height/linear_roof );
		//p.drawPoint(i,areasize.height-h);
		p->draw_fill_rect( margin+GUI::Point( i, areasize.height-h ), GUI::Size(1, h), color( COLOR_NODEUI_FILTER_BANK_FREQ ) );
			
	}
			
	GUI::FontID f = font( FONT_NODEUI_FILTER_BANK_FR_FONT );

	for (int i=0;i<filters;i++) {
		
		//just check
		ERR_CONTINUE(filter[i].mode==NULL || filter[i].stages==NULL || filter[i].cutoff==NULL || filter[i].resonance==NULL || cutoff_offset==NULL);
		
		if (filter[i].mode->get()==0)
			continue;
		
		int x=freq_2_pixel( filter[i].cutoff->get()*powf( 2 , cutoff_offset->get() ) ,MIN_FREQ,filter[i].cutoff->get_max(),response_len);
			
		bool selected=(drag.index==i || (drag.over==i && drag.index==-1));
		
		GUI::Color linecol = color( selected ? COLOR_NODEUI_FILTER_BANK_SELECTED_CUTOFF : COLOR_NODEUI_FILTER_BANK_CUTOFF );
		p->draw_fill_rect( margin+GUI::Point( x, 0 ), GUI::Size( 1, areasize.height ), linecol );
		
		float range=(filter[i].resonance->get()/ (filter[i].resonance->get_max()-filter[i].resonance->get_min()))*(float)areasize.height;
		p->draw_fill_rect( GUI::Point(x-4,areasize.height-(int)range),GUI::Size(9,1),linecol);

				
		String s=String::num(i+1);
		int wd=p->get_font_string_width( font( FONT_NODEUI_FILTER_BANK_FR_FONT ), s );
		
		x-=wd/2;
		int y=areasize.height-p->get_font_descent(font( FONT_NODEUI_FILTER_BANK_FR_FONT ))-2;
		
		
		GUI::Color shadowcol = color( COLOR_NODEUI_FILTER_BANK_FR_FONT_SHADOW );
		p->draw_text( f, margin+GUI::Point(x-1,y-1), s, shadowcol );
		p->draw_text( f, margin+GUI::Point(x-1,y+1), s, shadowcol );
		p->draw_text( f, margin+GUI::Point(x+1,y-1), s, shadowcol );
		p->draw_text( f, margin+GUI::Point(x+1,y+1), s, shadowcol );
		
		p->draw_text( f, margin+GUI::Point(x,y), s, color( COLOR_NODEUI_FILTER_BANK_FR_FONT ) );
	
	}

	
	/* show freq and dB */
	
	String max_db=String("Pk: ") + (response_roof==0?String("-oo"):(String::num(energy_to_db(response_roof),1) + "dB"));
	GUI::Point db_pos = margin + GUI::Point( 0, p->get_font_ascent(f));
	p->draw_text(f, db_pos+GUI::Point(1,1), max_db, color( COLOR_NODEUI_FILTER_BANK_FR_FONT_SHADOW ) );
	p->draw_text(f, db_pos+GUI::Point(-1,1), max_db, color( COLOR_NODEUI_FILTER_BANK_FR_FONT_SHADOW ) );
	p->draw_text(f, db_pos+GUI::Point(1,-1), max_db, color( COLOR_NODEUI_FILTER_BANK_FR_FONT_SHADOW ) );
	p->draw_text(f, db_pos+GUI::Point(-1,-1), max_db, color( COLOR_NODEUI_FILTER_BANK_FR_FONT_SHADOW ) );
	
	p->draw_text(f, db_pos, max_db, color( COLOR_NODEUI_FILTER_BANK_FR_FONT ) );
	
	if (drag.over>=0) {
		
		float cutoff=filter[drag.over].cutoff->get()*powf( 2 , cutoff_offset->get());
		
		String text;
		
		if (cutoff>=1000.0)
			
			text=String::num( cutoff/1000,1 ) + "khz";
		else 
			text=String::num( (int)cutoff ) + "hz";
		
		GUI::Point pos = margin + GUI::Point( areasize.width-p->get_font_string_width(f,text), p->get_font_ascent(f));
		p->draw_text(f, pos+GUI::Point(1,1) , text, color( COLOR_NODEUI_FILTER_BANK_FR_FONT_SHADOW ) );
		p->draw_text(f, pos+GUI::Point(1,-1) , text, color( COLOR_NODEUI_FILTER_BANK_FR_FONT_SHADOW ) );
		p->draw_text(f, pos+GUI::Point(-1,1) , text, color( COLOR_NODEUI_FILTER_BANK_FR_FONT_SHADOW ) );
		p->draw_text(f, pos+GUI::Point(-1,-1) , text, color( COLOR_NODEUI_FILTER_BANK_FR_FONT_SHADOW ) );

		p->draw_text(f, pos , text, color( COLOR_NODEUI_FILTER_BANK_FR_FONT ) );

	}
	
	
}

void FilterBankFR::mouse_button(const GUI::Point& p_pos, int p_button,bool p_press,int p_modifier_mask) {
	
	
	
	if (p_button!=GUI::BUTTON_LEFT)
		return;
	
	if (p_press) {
		if (drag.over>=0)
			drag.index=drag.over;
	} else {
	
		drag.index=-1;
		update();
	
	}
	
}

void FilterBankFR::mouse_motion(const GUI::Point& p_pos, const  GUI::Point& p_rel, int p_button_mask) {
	
	if (drag.index==-1) {
		
		/* no drag, find over */
		
		int closest=-1;
		int closest_dist=0;
		
		for (int i=0;i<filters;i++) {
			
		//just check
			ERR_CONTINUE(filter[i].mode==NULL || filter[i].stages==NULL || filter[i].cutoff==NULL || filter[i].resonance==NULL || cutoff_offset==NULL);
		
			if (filter[i].mode->get()==0)
				continue;
			
			int x=freq_2_pixel( filter[i].cutoff->get()*powf( 2 , cutoff_offset->get() ) ,MIN_FREQ,filter[i].cutoff->get_max(),get_size_cache().width);
			
			int distance=fabsf(p_pos.x-x);
			if (distance>10)
				continue;
			
			if (closest==-1 || closest_dist>distance)
				closest=i;
			
		}
		
		if (closest!=drag.over) {
			
			drag.over=closest;
			update();
		}
		
	} else {
	
		float freq= float_2_freq(  (float)p_pos.x/(float)get_size_cache().width ,MIN_FREQ,filter[drag.index].cutoff->get_max() );
				
		freq/=powf( 2 , cutoff_offset->get() );
		//filter[drag.index].cutoff->set(freq);
		filter[drag.index].cutoff->set(freq);
		float reso=(float)(get_size_cache().height-p_pos.y)/get_size_cache().height;
		if (reso<0)
			reso=0;
		if (reso>1)
			reso=1;
		
		reso*=filter[drag.index].resonance->get_max();
		
		filter[drag.index].resonance->set(reso);

		update();
	
	}
	
}

void FilterBankFR::set_cutoff_range(int p_which,GUI::RangeBase *p_prop) {
	
	ERR_FAIL_INDEX(p_which,filters);
	
	if (filter[p_which].cutoff)
		delete filter[p_which].cutoff;
	filter[p_which].cutoff=p_prop;
	update();
}

void FilterBankFR::set_resonance_range(int p_which,GUI::RangeBase *p_prop) {
	
	ERR_FAIL_INDEX(p_which,filters);
	
	if (filter[p_which].resonance)
		delete filter[p_which].resonance;
	filter[p_which].resonance=p_prop;
	update();
}

void FilterBankFR::set_mode_range(int p_which,GUI::RangeBase *p_prop) {
	
	ERR_FAIL_INDEX(p_which,filters);
	
	if (filter[p_which].mode)
		delete filter[p_which].mode;
	filter[p_which].mode=p_prop;
	update();
}

void FilterBankFR::set_stages_range(int p_which,GUI::RangeBase *p_prop) {
	
	ERR_FAIL_INDEX(p_which,filters);
	
	if (filter[p_which].stages)
		delete filter[p_which].stages;
	filter[p_which].stages=p_prop;
	update();
}


void FilterBankFR::set_cutoff_offset_range(GUI::RangeBase *p_prop) {
	
	cutoff_offset=p_prop;
}


void FilterBankFR::check_for_changes() {

	for (int i=0;i<filters;i++) {
	
		if (
			filter[i].mode->get() != filter[i].last_mode || 
			filter[i].stages->get() != filter[i].last_stages || 
			filter[i].cutoff->get() != filter[i].last_cutoff || 
			filter[i].resonance->get() != filter[i].last_resonance 
			) {
			
			update();	
			filter[i].last_mode=filter[i].mode->get();
			filter[i].last_stages=filter[i].stages->get();
			filter[i].last_cutoff=filter[i].cutoff->get();
			filter[i].last_resonance=filter[i].resonance->get();
			
		}
	}
	if (last_cutoff_offset!=cutoff_offset->get() ) {
	
		last_cutoff_offset=cutoff_offset->get();
		update();
	}
}

FilterBankFR::FilterBankFR(int p_filters) {
	
	
	filters=p_filters;
	filter	= new FilterData[p_filters];
	cutoff_offset=NULL;	
	drag.index=-1;
	drag.over=-1;
	last_cutoff_offset=-1;
	response_len=0;
	response=NULL;
	
}


FilterBankFR::~FilterBankFR(){ 
	
	if (response)
		delete[] response;
		
	delete[] filter;
	
	if (cutoff_offset)
		delete cutoff_offset;
}

