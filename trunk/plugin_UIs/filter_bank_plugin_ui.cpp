//
// C++ Implementation: filter_bank_plugin_ui
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "filter_bank_plugin_ui.h"
#include "ui_blocks/property_editors.h"
#include "interface/visual_settings.h"
#include "ui_blocks/filter_bank_editor.h"

namespace ReShaked {

SoundPluginUI* FilterBankPluginUI::create_this(SoundPlugin *p_plugin,QWidget *p_parent) {
	
	FilterBankPlugin *filterbank=dynamic_cast<FilterBankPlugin*>(p_plugin);
	
	if (!filterbank)
		return NULL;
	
	return new FilterBankPluginUI(p_parent,filterbank);

}


FilterBankPluginUI::FilterBankPluginUI(QWidget *p_parent,FilterBankPlugin *p_filterbank) : SoundPluginUI(p_parent,p_filterbank) {
	
	CHBox *hb = generate_default_layout();
	
	CVBox *fb_vb = new CVBox(hb);
	
	new PixmapLabel( fb_vb, GET_QPIXMAP(THEME_FILTERBANK__TOP));
	
	CHBox *fb_hb = new CHBox(fb_vb);
	
	new PixmapLabel( fb_hb, GET_QPIXMAP(THEME_FILTERBANK__LEFT));
		
	CVBox *fb_ctrls = new CVBox(fb_hb);
	
	static const PixmapsList ctrl_separators[4]={THEME_FILTERBANK__CONTROLS_TOP,THEME_FILTERBANK__SEPARATOR_1,THEME_FILTERBANK__SEPARATOR_2,THEME_FILTERBANK__SEPARATOR_3};
	
	for (int i=0;i<4;i++) {
		String n_prefix=String("filter_")+String::num(i+1)+"_";
		new PixmapLabel( fb_ctrls, GET_QPIXMAP(ctrl_separators[i]) );		
		CHBox *ctrl_hb = new CHBox(fb_ctrls);
		PropertyEditor *combo = new PropertyEditCombo(ctrl_hb,GET_QPIXMAP( THEME_FILTERBANK__COMBO ) );
		combo->set_property( &p_filterbank->get_port_by_name( n_prefix+"mode" ));
		register_property_editor( combo );
		
		new PixmapLabel( ctrl_hb, GET_QPIXMAP( THEME_FILTERBANK__COMBO_STR_SEPARATOR ) );		
		
		PropertyEditor *str = new PropertyEditLabel(ctrl_hb,GET_QPIXMAP( THEME_FILTERBANK__STR_DISPLAY ) );
		str->set_property( &p_filterbank->get_port_by_name( n_prefix+"stages" ));
		register_property_editor( str );
		PropertyEditor *str_ud = new PropertyEditUpDown(ctrl_hb,PixmapUpDown::Skin( GET_QPIXMAP( THEME_FILTERBANK__STR_UPDOWN ) ) );
		str_ud->set_property( &p_filterbank->get_port_by_name( n_prefix+"stages" ));
		str_ud->add_to_group(str);
		register_property_editor( str_ud );
		
	}
	
	new PixmapLabel( fb_hb, GET_QPIXMAP(THEME_FILTERBANK__CONTROLS_DISPLAY_SEPARATOR));
	
	CVBox *fb_disp_vb = new CVBox(fb_hb);
	
	FilterBankEditor::Skin fbe_sk( GET_QPIXMAP(THEME_FILTERBANK__DISPLAY), QColor(210,220,240), 2 );
	
	FilterBankEditor *fbe = new FilterBankEditor( fb_disp_vb, 4, fbe_sk );
	
	for (int i=0;i<4;i++) {
		String n_prefix=String("filter_")+String::num(i+1)+"_";
		
		fbe->set_cutoff_property( i, &p_filterbank->get_port_by_name( n_prefix+"cutoff" ) );
		fbe->set_resonance_property( i, &p_filterbank->get_port_by_name( n_prefix+"resonance" ) );
		fbe->set_stages_property( i, &p_filterbank->get_port_by_name( n_prefix+"stages" ) );
		fbe->set_mode_property( i, &p_filterbank->get_port_by_name( n_prefix+"mode" ) );
	}
	
	fbe->set_cutoff_offset_property( &p_filterbank->get_port_by_name( "global_cutoff" ) );
	register_property_editor( fbe );
	new PixmapLabel( fb_disp_vb, GET_QPIXMAP(THEME_FILTERBANK__DISPLAY_OCTAVE_SEPARATOR));
	
	CHBox *octave_hb = new CHBox(fb_disp_vb);
	
	PixmapSlider::Skin sl_sk = PixmapSlider::Skin( GET_QPIXMAP(THEME_FILTERBANK__OCTAVE_SLIDER),GET_QPIXMAP(THEME_FILTERBANK__OCTAVE_SLIDER),GET_QPIXMAP(THEME_FILTERBANK__OCTAVE_SLIDER_GRABBER) );
	
	PropertyEditor *octave_sl = new PropertyEditSlider(octave_hb,sl_sk,PixmapSlider::TYPE_HORIZONTAL,0,0);
	octave_sl->set_property( &p_filterbank->get_port_by_name( "global_cutoff" ) );
	register_property_editor( octave_sl );
	
	new PixmapLabel( octave_hb, GET_QPIXMAP(THEME_FILTERBANK__OCTAVE_SEPARATOR));
	
	PropertyEditLabel *octave_label = new PropertyEditLabel(octave_hb,GET_QPIXMAP(THEME_FILTERBANK__OCTAVE_DISPLAY));
	octave_label->set_property( &p_filterbank->get_port_by_name( "global_cutoff" ) );
	register_property_editor( octave_label );
	octave_label->add_to_group(octave_sl);
	octave_label->set_suffix_visible( false );
	
	new PixmapLabel( fb_hb, GET_QPIXMAP(THEME_FILTERBANK__RIGHT));
	
	new PixmapLabel( fb_vb, GET_QPIXMAP(THEME_FILTERBANK__BOTTOM));
	
	
}


FilterBankPluginUI::~FilterBankPluginUI()
{
}


}
