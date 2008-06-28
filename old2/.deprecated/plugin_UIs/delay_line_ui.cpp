//
// C++ Implementation: delay_line_ui
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//

//
#include "delay_line_ui.h"
#include "ui_blocks/property_editors.h"
#include "ui_blocks/panner_editor.h"
#include "interface/visual_settings.h"

namespace ReShaked {

SoundPluginUI* DelayLinePluginUI::create_this(SoundPlugin *p_plugin,QWidget *p_parent) {
	
	DelayLinePlugin *delayline = dynamic_cast<DelayLinePlugin*>( p_plugin );
	
	if (delayline)
		return new DelayLinePluginUI(p_parent,delayline);
	else
		return NULL;
}

void DelayLinePluginUI::create_label_and_button(QWidget *p_parent,String p_prop,QPixmap p_label,bool p_first) {
	
	CVBox *vb = new CVBox(p_parent);
	
	new PixmapLabel(vb,p_label);
	
	PropertyEditor *peb; //button
	
	if (p_first) {
		
		CHBox *hb = new CHBox(vb);
		peb = new PropertyEditButton(hb,PixmapButton::Skin( GET_QPIXMAP(THEME_DELAYLINE__BUTTON),GET_QPIXMAP(THEME_DELAYLINE__BUTTON_ACTIVE)));
		new PixmapLabel(hb,GET_QPIXMAP(THEME_DELAYLINE__MAIN_NOKNOB));
	} else {
		
		peb = new PropertyEditButton(vb,PixmapButton::Skin( GET_QPIXMAP(THEME_DELAYLINE__BUTTON),GET_QPIXMAP(THEME_DELAYLINE__BUTTON_ACTIVE)));
		
	}
	
	peb->set_property( &plugin->get_port_by_name( p_prop ) );
	register_property_editor( peb );
}
void DelayLinePluginUI::create_knob_and_display(QWidget *p_parent,String p_prop, bool p_above) {
	
	CVBox *vb = new CVBox(p_parent);
	
	Knob::Skin sk;
	
	sk.angle_begin=30;
	sk.base=p_above?GET_QPIXMAP( THEME_DELAYLINE__KNOB_UPPER ):GET_QPIXMAP( THEME_DELAYLINE__KNOB_LOWER );
	sk.handle=GET_QPIXMAP( THEME_DELAYLINE__KNOB_POS );
	sk.handle_at_distance=8;
	
	PropertyEditor *knob = new PropertyEditKnob( vb,sk );
	PropertyEditLabel *label = new PropertyEditLabel( vb, p_above?GET_QPIXMAP( THEME_DELAYLINE__DISPLAY_UPPER ):GET_QPIXMAP( THEME_DELAYLINE__DISPLAY_LOWER ) );
	
	knob->set_property( &plugin->get_port_by_name( p_prop ) );
	label->set_property( &plugin->get_port_by_name( p_prop ) );
	label->add_to_group( knob );
	label->set_suffix_visible( false );
	register_property_editor( knob );
	register_property_editor( label );
	
}

void DelayLinePluginUI::create_pan(QWidget *p_parent,String p_basename) {
	
	if (plugin->get_channels_created()==1) {
		
		new PixmapLabel(p_parent,GET_QPIXMAP( THEME_DELAYLINE__NOPAN ) );
				
	} else if (plugin->get_channels_created()==2) {
		
		create_knob_and_display( p_parent, p_basename+"_pan" );
	} else if (plugin->get_channels_created()==4) {
		
		CVBox *vb = new CVBox(p_parent);
		
		new PixmapLabel(vb,GET_QPIXMAP( THEME_DELAYLINE__QUADPAN_TOP ) );
		
		PannerEditor *pe;
		PannerEditor::Skin sk;
		sk.bg=GET_QPIXMAP( THEME_DELAYLINE__QUADPAN );
		sk.border_w=2;
		sk.pos=GET_QPIXMAP( THEME_DELAYLINE__QUADPAN_POS );
		
		pe = new PannerEditor(vb, true, sk );
		
		pe->set_properties( &plugin->get_port_by_name( p_basename+"_pan" ),&plugin->get_port_by_name( p_basename+"_pan_depth" ) );
		register_property_editor( pe );
		new PixmapLabel(vb,GET_QPIXMAP( THEME_DELAYLINE__QUADPAN_BOTTOM ) );
		
	}
	
}

DelayLinePluginUI::DelayLinePluginUI(QWidget *p_parent,DelayLinePlugin *p_plugin) :SoundPluginUI(p_parent,p_plugin) {
	
	plugin=p_plugin;
	
	CHBox *hb = generate_default_layout(false);
	
	CVBox *vb = new CVBox(hb);
	
	CHBox *upper_hb = new CHBox(vb);
	
	
	/* 
	LocalProperty main_active;
	LocalProperty main_level;
	LocalProperty main_pan;
	LocalProperty main_pan_depth;
	
	LocalProperty tap_1_active;
	LocalProperty tap_1_delay;
	LocalProperty tap_1_level;
	LocalProperty tap_1_pan;
	LocalProperty tap_1_pan_depth;
	
	LocalProperty tap_2_active;
	LocalProperty tap_2_delay;
	LocalProperty tap_2_level;
	LocalProperty tap_2_pan;
	LocalProperty tap_2_pan_depth;
	
	LocalProperty feedback_active;
	LocalProperty feedback_delay;
	LocalProperty feedback_level;
	LocalProperty feedback_lowpass;
	*/	
	create_label_and_button( upper_hb, "main_active", GET_QPIXMAP( THEME_DELAYLINE__LABEL_MAIN ), true );
	
	create_label_and_button( upper_hb, "tap_1_active", GET_QPIXMAP( THEME_DELAYLINE__LABEL_TAP1 ) );
	
	create_knob_and_display( upper_hb, "tap_1_delay", true );
	
	create_label_and_button( upper_hb, "tap_2_active", GET_QPIXMAP( THEME_DELAYLINE__LABEL_TAP2 ) );
	
	create_knob_and_display( upper_hb, "tap_2_delay", true );
	
	create_label_and_button( upper_hb, "feedback_active", GET_QPIXMAP( THEME_DELAYLINE__LABEL_FDBCK ) );
	
	create_knob_and_display( upper_hb, "feedback_delay", true );
	
	if (p_plugin->is_bpm_based())
		new PixmapLabel(vb,GET_QPIXMAP(THEME_DELAYLINE__MIDDLE_BPM));
	else
		new PixmapLabel(vb,GET_QPIXMAP(THEME_DELAYLINE__MIDDLE_MSEC));
	
	CHBox *lower_hb = new CHBox(vb);
	
	new PixmapLabel(lower_hb,GET_QPIXMAP( THEME_DELAYLINE__BOTTOM_EXTERNAL_SEPARATOR ) );
	
	create_knob_and_display( lower_hb, "main_level" );
	new PixmapLabel(lower_hb,GET_QPIXMAP( THEME_DELAYLINE__BOTTOM_INTERNAL_SEPARATOR ) );
	create_pan(lower_hb,"main");

	
	new PixmapLabel(lower_hb,GET_QPIXMAP( THEME_DELAYLINE__BOTTOM_EXTERNAL_SEPARATOR ) );
	
	create_knob_and_display( lower_hb, "tap_1_level" );
	new PixmapLabel(lower_hb,GET_QPIXMAP( THEME_DELAYLINE__BOTTOM_INTERNAL_SEPARATOR ) );
	create_pan(lower_hb,"tap_1");
	
	new PixmapLabel(lower_hb,GET_QPIXMAP( THEME_DELAYLINE__BOTTOM_EXTERNAL_SEPARATOR ) );
	
	create_knob_and_display( lower_hb, "tap_2_level" );
	new PixmapLabel(lower_hb,GET_QPIXMAP( THEME_DELAYLINE__BOTTOM_INTERNAL_SEPARATOR ) );
	create_pan(lower_hb,"tap_2");
	
	new PixmapLabel(lower_hb,GET_QPIXMAP( THEME_DELAYLINE__BOTTOM_EXTERNAL_SEPARATOR ) );
	
	create_knob_and_display( lower_hb, "feedback_level" );
	new PixmapLabel(lower_hb,GET_QPIXMAP( THEME_DELAYLINE__BOTTOM_INTERNAL_SEPARATOR ) );
	create_knob_and_display( lower_hb, "feedback_lowpass" );
	
	new PixmapLabel(vb,GET_QPIXMAP(THEME_DELAYLINE__BOTTOM));
	
}


DelayLinePluginUI::~DelayLinePluginUI()
{
}


}
