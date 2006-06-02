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

	CHBox *hb = generate_default_layout();
	
	 
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
	
	create_editor_for_property( "wet_send",hb );
	create_editor_for_property( "dry_send",hb );
	
	
}


ReverbPluginUI::~ReverbPluginUI()
{
}


}
