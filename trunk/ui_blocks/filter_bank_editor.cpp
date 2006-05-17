//
// C++ Implementation: filter_bank_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "filter_bank_editor.h"
#include <math.h>
#include <Qt/qpainter.h>
#include <Qt/qfontmetrics.h>
#include <Qt/qtimer.h>
#include "dsp/formulas.h"
#include "dsp/filter.h"
#include <Qt/qevent.h>
#include "ui_blocks/helpers.h"


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

void FilterBankEditor::paintEvent(QPaintEvent *e) {
	
	if (width()<1)
		return;
	
	QPainter p(this);
	
	p.drawPixmap(0,0,skin.bg);
	
	p.setPen(QColor(255,255,255));
	/* Frequency response graph */
	float *response = new float[width()];
	int response_len=width();
	
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
	
	QColor col_from=skin.line_color;
	col_from.setAlpha(40);
	QColor col_to=skin.line_color;
	col_to.setAlpha(200);
		
	for (int i=0;i<response_len;i++) {
		
		int h=(int)( sqrt(response[i])*height()/linear_roof );
		//p.drawPoint(i,height()-h);
		QLinearGradient gradient(i, height(), i, height()-h);
		gradient.setColorAt(0,col_from);
		gradient.setColorAt(1, col_to);
		p.setBrush(gradient);
		p.fillRect(i, height()-h,1,h,gradient); 
			
	}
			
	
	for (int i=0;i<filters;i++) {
		
		//just check
		ERR_CONTINUE(filter[i].mode==NULL || filter[i].stages==NULL || filter[i].cutoff==NULL || filter[i].resonance==NULL || cutoff_offset==NULL);
		
		if (filter[i].mode->get()==0)
			continue;
		
		int x=freq_2_pixel( filter[i].cutoff->get()*powf( 2 , cutoff_offset->get() ) ,MIN_FREQ,filter[i].cutoff->get_max(),response_len);
			
		bool selected=(drag.index==i || (drag.over==i && drag.index==-1));
		
		if (selected)
			p.setPen(QColor(255,255,255,230));
		else
			p.setPen(QColor(255,255,255,160));
		
		p.drawLine( x,0,x,height());
		float range=(filter[i].resonance->get()/ (filter[i].resonance->get_max()-filter[i].resonance->get_min()))*(float)height();
		p.drawLine( x-4,height()-(int)range,x+4,height()-(int)range);

		
		QFont f;
		f.setPixelSize(NUMBER_H);
		f.setBold(true);
		
		QFontMetrics fm(f);
		
		QString s=QString::number(i+1);
		int wd=fm.width(s);
		
		x-=wd/2;
		int y=height()-fm.descent()-2;
		p.setPen(QColor(0,0,0));
		p.drawText(x-1,y-1,s);
		p.drawText(x-1,y+1,s);
		p.drawText(x+1,y-1,s);
		p.drawText(x+1,y+1,s);
		
		p.setPen(QColor(255,255,255));
		
		p.drawText(x,y,s);
		
		
	
	}
	
	QFont f;
	f.setPixelSize(NUMBER_H);
	QFontMetrics fm(f);
	
	
	/* show freq and dB */
	
	QString max_db=QString("Peak: ") + (response_roof==0?QString("-oo"):QStrify( String::num(energy_to_db(response_roof),1) )) + "dB";
	p.drawText(5,3+fm.ascent(),max_db);
	
	if (drag.over>=0) {
		
		float cutoff=filter[drag.over].cutoff->get()*powf( 2 , cutoff_offset->get());
		
		QString text;
		
		if (cutoff>=1000.0)
			
			text=QStrify( String::num( cutoff/1000,1 ) ) + "khz";
		else 
			text=QString::number( (int)cutoff ) + "hz";
		
		p.drawText(width()-fm.width(text)-3,3+fm.ascent(),text);
	}
	

	
	delete[] response;
	
	for (int i=0;i<param_tracking.size();i++) 
		param_tracking[i].last_value=param_tracking[i].property->get();

	need_update=false;		    
}

void FilterBankEditor::mousePressEvent(QMouseEvent *e) {
	
	if (e->button()==Qt::RightButton && drag.index==-1) {
		
		if (drag.over>=0) {
		
			external_edit_signal( filter[drag.over].cutoff );
		}
	}
	
	if (e->button()!=Qt::LeftButton)
		return;
	
	if (drag.over>=0)
		drag.index=drag.over;
	
}

void FilterBankEditor::mouseMoveEvent(QMouseEvent *e) {
	
	if (drag.index==-1) {
		
		/* no drag, find over */
		
		int closest=-1;
		int closest_dist=0;
		
		for (int i=0;i<filters;i++) {
			
		//just check
			ERR_CONTINUE(filter[i].mode==NULL || filter[i].stages==NULL || filter[i].cutoff==NULL || filter[i].resonance==NULL || cutoff_offset==NULL);
		
			if (filter[i].mode->get()==0)
				continue;
			
			int x=freq_2_pixel( filter[i].cutoff->get()*powf( 2 , cutoff_offset->get() ) ,MIN_FREQ,filter[i].cutoff->get_max(),width());
			
			int distance=fabs(e->pos().x()-x);
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
	
		float freq= float_2_freq(  (float)e->pos().x()/(float)width() ,MIN_FREQ,filter[drag.index].cutoff->get_max() );
				
		freq/=powf( 2 , cutoff_offset->get() );
		filter[drag.index].cutoff->set(freq);
		
		float reso=(float)(height()-e->pos().y())/height();
		if (reso<0)
			reso=0;
		if (reso>1)
			reso=1;
		
		reso*=filter[drag.index].resonance->get_max();
		filter[drag.index].resonance->set(reso);
		update();
	
	}
	
}

void FilterBankEditor::mouseReleaseEvent(QMouseEvent *e) {
	
	drag.index=-1;
	update();
	
}

void FilterBankEditor::set_cutoff_property(int p_which,Property *p_prop) {
	
	ERR_FAIL_INDEX(p_which,filters);
	
	set_property(p_which,p_prop);
	filter[p_which].cutoff=p_prop;
	update();
}

void FilterBankEditor::set_resonance_property(int p_which,Property *p_prop) {
	
	ERR_FAIL_INDEX(p_which,filters);
	set_property(p_which+filters,p_prop);
	filter[p_which].resonance=p_prop;
	update();
}

void FilterBankEditor::set_mode_property(int p_which,Property *p_prop) {
	
	ERR_FAIL_INDEX(p_which,filters);
	filter[p_which].mode=p_prop;
	update();
	add_param_tracking( p_prop );	
}
void FilterBankEditor::set_stages_property(int p_which,Property *p_prop) {
	
	ERR_FAIL_INDEX(p_which,filters);
	filter[p_which].stages=p_prop;
	update();
	add_param_tracking( p_prop );
	
}

void FilterBankEditor::set_cutoff_offset_property(Property *p_prop) {
	
	cutoff_offset=p_prop;
	add_param_tracking( p_prop );
}


void FilterBankEditor::add_param_tracking(Property *p_prop) {
	
	ParamTrack pt;
	pt.property=p_prop;
	pt.last_value=p_prop->get();
	
	param_tracking.push_back(pt);
	update();
	
}


void FilterBankEditor::check_param_tracking() {
	
	if (need_update) {
		update();
		return;
	}
	
	for (int i=0;i<param_tracking.size();i++) {
		
		if (param_tracking[i].property->get()!=param_tracking[i].last_value) {
			update();
			return; //only one update
		}
	}
}

void FilterBankEditor::changed(int) {
	
	need_update=true; //done to avoid refreshes too much.. as this is not a very easy to draw widget
}

FilterBankEditor::FilterBankEditor(QWidget *p_parent,int p_filters,const Skin& p_skin) : QWidget(p_parent) , MultiPropertyEditor(p_filters*2) {
	
	skin=p_skin;
	setFixedSize(skin.bg.size());
	filters=p_filters;
	filter	= new FilterData[p_filters];
	QTimer *updater = new QTimer(this);
	QObject::connect(updater,SIGNAL(timeout()),this,SLOT(check_param_tracking()));
	cutoff_offset=NULL;	
	updater->start(100);
	drag.index=-1;
	drag.over=-1;
	setMouseTracking(true);
	need_update=true;
	
}


FilterBankEditor::~FilterBankEditor(){ 
	
	delete[] filter;
}


}
