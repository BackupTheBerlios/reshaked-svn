//
// C++ Implementation: reverb_plugin_ui
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "reverb_plugin_ui.h"

#include "ui_blocks/property_editors.h"
#include "interface/visual_settings.h"
#include "ui_blocks/reverb_display_ir.h"

namespace ReShaked {

SoundPluginUI* ReverbPluginUI::create_this(SoundPlugin *p_plugin,QWidget *p_parent) {
	
	ReverbPlugin *reverb=dynamic_cast<ReverbPlugin*>(p_plugin);
	
	if (!reverb)
		return NULL;
	
	return new ReverbPluginUI(p_parent,reverb);
}

ReverbPluginUI::ReverbPluginUI(QWidget *p_parent,ReverbPlugin *p_reverb) :SoundPluginUI(p_parent,p_reverb) {

	setLayout(new QVBoxLayout(this));
	CHBox *hb_top = new CHBox(this);
	layout()->addWidget(hb_top);
	new PixmapLabel(hb_top,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_TOP_LEFT));
	PixmapLabel *title = new PixmapLabel(hb_top,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_TOP) ,PixmapLabel::EXPAND_TILE_H);
	title->set_text( QStrify(p_reverb->get_caption())) ;
	title->setToolTip( QStrify(p_reverb->get_caption())) ;
	new PixmapLabel(hb_top,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_TOP_RIGHT));
	
	
	
	CHBox *hb = new CHBox(this);
	layout()->addWidget(hb);
	
	PixmapLabel *label_name = new PixmapLabel(hb,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_LEFT));
	label_name->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	
	QPixmap value_pixmap(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_VALUE));
	QPixmap label_pixmap(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_LABEL));
	PixmapSlider::Skin slider_skin(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_SLIDER_BASE),GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_SLIDER_LIGHT),GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_SLIDER_GRABBER));
	
	/** BEGIN **/
	
	CVBox *chorus_vb = new CVBox(hb);
	
	new PixmapLabel(chorus_vb,GET_QPIXMAP(THEME_REVERB__TOP));
	
	CHBox *params_hb = new CHBox(chorus_vb);
	
	static const char *param_names[6]={"predelay","predelay_fb","hpf","room_size","damping","spread"};
	Knob::Skin knob_skin;
	knob_skin.angle_begin=30;
	knob_skin.base=GET_QPIXMAP(THEME_REVERB__KNOB);
	knob_skin.handle=GET_QPIXMAP(THEME_CHORUS__KNOB_POS);
	knob_skin.handle_at_distance=10;
	
	for (int i=0;i<6;i++) {
		
		CVBox * pvb = new CVBox(params_hb);
		Property * p = &p_reverb->get_port_by_name(param_names[i]);
		
		ERR_CONTINUE(p==NULL);
		
		PropertyEditor *knob = new PropertyEditKnob(pvb,knob_skin);
		knob->set_property(p);		
		register_property_editor( knob );
		
		PropertyEditor *disp = new PropertyEditLabel(pvb,GET_QPIXMAP(THEME_REVERB__DISPLAY));
		disp->set_property(p);		
		register_property_editor( disp );
		disp->add_to_group(knob);
		
	}
	
	new PixmapLabel(chorus_vb,GET_QPIXMAP(THEME_REVERB__LABELS));
	
	ReverbDisplayIR::Skin ir_skin;
	ir_skin.bg=GET_QPIXMAP(THEME_REVERB__IR);
	ir_skin.border=1;
	ir_skin.ir_color=QColor(250,210,210,150);
	ir_skin.seconds=6;
	
	ReverbDisplayIR * rvbir = new ReverbDisplayIR(chorus_vb,ir_skin);
	rvbir->set_properties( &p_reverb->get_port_by_name("predelay"), &p_reverb->get_port_by_name("predelay_fb"),&p_reverb->get_port_by_name("hpf"),&p_reverb->get_port_by_name("room_size"),&p_reverb->get_port_by_name("damping"), &p_reverb->get_port_by_name("spread") );
	
				
	new PixmapLabel(chorus_vb,GET_QPIXMAP(THEME_REVERB__BOTTOM));
	
	/** DRY/SED **/
	
	CVBox *vb_port = new CVBox(hb);
		
	CHBox *hb_port = new CHBox(vb_port);
		
	/** WET SEND **/
	
	/// LABEL /
	PixmapLabel * name = new PixmapLabel(hb_port,label_pixmap);
		
	name->set_pos(QPoint(6,label_pixmap.height()-8));
	name->get_font().setPixelSize(10);
	name->set_text( "Wet" );
	name->set_angle( -90 );
	name->set_color(QColor(0,0,22));
	
	///SLIDER
	PropertyEditSlider * slider = new PropertyEditSlider(hb_port,slider_skin);
	slider->set_property(&p_reverb->get_port_by_name("wet_send"));
	
		
	register_property_editor( slider );
		
	/// VALUE 
	PropertyEditLabel * value = new PropertyEditLabel( vb_port,value_pixmap );
	value->set_property(&p_reverb->get_port_by_name("wet_send"));
	value->set_postfix_visible( false );
	value->set_color(QColor(240,240,255));
	value->add_to_group(slider); //share group
	
	register_property_editor( value );
		
	/** DRY SEND */
	
	vb_port = new CVBox(hb);
		
	hb_port = new CHBox(vb_port);
	
	/// LABEL /
	name = new PixmapLabel(hb_port,label_pixmap);
		
	name->set_pos(QPoint(6,label_pixmap.height()-8));
	name->get_font().setPixelSize(10);
	name->set_text( "Dry" );
	name->set_angle( -90 );
	name->set_color(QColor(0,0,22));
	
	///SLIDER
	slider = new PropertyEditSlider(hb_port,slider_skin);
	slider->set_property(&p_reverb->get_port_by_name("dry_send"));
			
	register_property_editor( slider );
		
	/// VALUE 
	value = new PropertyEditLabel( vb_port,value_pixmap );
	value->set_property(&p_reverb->get_port_by_name("dry_send"));
	value->set_postfix_visible( false );
	value->set_color(QColor(240,240,255));
	value->add_to_group(slider); //share group
	
	register_property_editor( value );
	/** END **/
	
	new PixmapLabel(hb,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_RIGHT));
	
	CHBox *hb_bottom = new CHBox(this);
	layout()->addWidget(hb_bottom);
	new PixmapLabel(hb_bottom,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_BOTTOM_LEFT));
	new PixmapLabel(hb_bottom,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_BOTTOM),PixmapLabel::EXPAND_TILE_H);
	new PixmapLabel(hb_bottom,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_BOTTOM_RIGHT));
	
	
	layout()->setMargin(0);
	layout()->setSpacing(0);
	
	hb->layout()->setMargin(0);
	hb->layout()->setSpacing(0);	
	
	setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	
	
	
}


ReverbPluginUI::~ReverbPluginUI()
{
}


}
