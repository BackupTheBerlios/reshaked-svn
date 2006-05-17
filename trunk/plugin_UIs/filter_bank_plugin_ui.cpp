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
	
	setLayout(new QVBoxLayout(this));
	CHBox *hb_top = new CHBox(this);
	layout()->addWidget(hb_top);
	new PixmapLabel(hb_top,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_TOP_LEFT));
	PixmapLabel *title = new PixmapLabel(hb_top,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_TOP) ,PixmapLabel::EXPAND_TILE_H);
	title->set_text( QStrify(p_filterbank->get_caption())) ;
	title->setToolTip( QStrify(p_filterbank->get_caption())) ;
	new PixmapLabel(hb_top,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_TOP_RIGHT));
	
	
	
	CHBox *hb = new CHBox(this);
	layout()->addWidget(hb);
	
	PixmapLabel *label_name = new PixmapLabel(hb,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_LEFT));
	label_name->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	
	QPixmap value_pixmap(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_VALUE));
	QPixmap label_pixmap(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_LABEL));
	PixmapSlider::Skin slider_skin(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_SLIDER_BASE),GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_SLIDER_LIGHT),GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_SLIDER_GRABBER));
	
	/** BEGIN **/
	
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
	octave_label->set_postfix_visible( false );
	
	new PixmapLabel( fb_hb, GET_QPIXMAP(THEME_FILTERBANK__RIGHT));
	
	new PixmapLabel( fb_vb, GET_QPIXMAP(THEME_FILTERBANK__BOTTOM));
	
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


FilterBankPluginUI::~FilterBankPluginUI()
{
}


}
