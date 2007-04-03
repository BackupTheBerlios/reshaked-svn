//
// C++ Implementation: limiter_plugin_ui
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "limiter_plugin_ui.h"

#include "sound_plugin_ui_generic.h"
#include "interface/visual_settings.h"
#include "dsp/formulas.h"

namespace ReShaked {

SoundPluginUI* LimiterPluginUI::create_this(SoundPlugin *p_plugin,QWidget *p_parent) {
	
	LimiterPlugin *limiter = dynamic_cast<LimiterPlugin*>( p_plugin );
	
	if (limiter)
		return new LimiterPluginUI(p_parent,limiter);
	else
		return NULL;
}


void LimiterPluginUI::update_vu_and_peak() {
	
	float val=limiter->get_max_val();
	
	vu->set_value( (val+60.0)/66.0 );
	QString str = (val<-60) ? "-oo" : QStrify( String::num( val,1 ) );
	
	label->set_text( str );
	
	bool peaked=limiter->has_peaked();
	
	if (peaked!=last_peak) {
		
		peak->set_bg( peaked? GET_QPIXMAP(THEME_PEAKMON__PEAK) : GET_QPIXMAP(THEME_PEAKMON__NOPEAK) );
		last_peak=peaked;
	}
	
}

void LimiterPluginUI::clear_peak() {
	
	limiter->clear_peaked();
	
}


LimiterPluginUI::LimiterPluginUI(QWidget *p_parent,LimiterPlugin *p_limiter) :SoundPluginUI(p_parent,p_limiter) {
	
	limiter=p_limiter;
	CHBox *hb=generate_default_layout(true);
	
	
	/* VU - Label */
	
	QPixmap value_pixmap(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_VALUE));
	QPixmap label_pixmap(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_LABEL));
	
	PixmapVU::Skin vu_skin(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_VU_EMPTY),GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_VU_FILL));
	
	CVBox *vb_vu = new CVBox(hb);
			
	CHBox *hb_vu = new CHBox(vb_vu);
			
	/* LABEL */
	PixmapLabel * name = new PixmapLabel(hb_vu,label_pixmap);
			
	name->set_pos(QPoint(6,label_pixmap.height()-8));
	name->get_font().setPixelSize(10);
	name->set_text( "Energy" );
	name->set_angle( -90 );
	name->set_color(QColor(0,0,22));
			
			
	/* SLIDER/VU */
	vu = new PropertyEditVU(hb_vu,vu_skin);
	vu->set_fall_time( 1 );		
	
			
	/* VALUE */
	label = new PixmapLabel( vb_vu,value_pixmap );
	label->set_color(QColor(240,240,255));
			
	vb_vu->layout()->setMargin(0);
	vb_vu->layout()->setSpacing(0);
	hb_vu->layout()->setMargin(0);
	hb_vu->layout()->setSpacing(0);
	
	CVBox *peak_vb = new CVBox(hb);
	
	new PixmapLabel(peak_vb,GET_QPIXMAP(THEME_PEAKMON__TOP));
	
	peak = new PixmapLabel(peak_vb,GET_QPIXMAP(THEME_PEAKMON__NOPEAK));
	last_peak=false;

	QObject::connect( new PixmapButton(peak_vb,PixmapButton::Skin( GET_QPIXMAP(THEME_PEAKMON__BUTTON_CLEAR),GET_QPIXMAP(THEME_PEAKMON__BUTTON_CLEAR_PUSHED) ) ), SIGNAL( mouse_pressed_signal()), this, SLOT(clear_peak()) );
	
	new PixmapLabel(peak_vb,GET_QPIXMAP(THEME_PEAKMON__BOTTOM));
	
	QTimer *updater = new QTimer(this);
	QObject::connect(updater,SIGNAL(timeout()),this,SLOT(update_vu_and_peak()));
	updater->start(40);
	
}


LimiterPluginUI::~LimiterPluginUI()
{
}


}
