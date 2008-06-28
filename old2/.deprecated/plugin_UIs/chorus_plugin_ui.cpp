//
// C++ Implementation: chorus_plugin_ui
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "chorus_plugin_ui.h"

#include "interface/sound_plugin_ui.h"

#include "ui_blocks/property_editors.h"
#include "interface/visual_settings.h"


namespace ReShaked {

SoundPluginUI* ChorusPluginUI::create_this(SoundPlugin *p_plugin,QWidget *p_parent) {
	
	ChorusPlugin *chorus=dynamic_cast<ChorusPlugin*>(p_plugin);
	
	if (!chorus)
		return NULL;
	
	return new ChorusPluginUI(p_parent,chorus);
}

void ChorusPluginUI::voice_selected_slot(int p_which) {
	

	String prefix=String("voice_")+String::num(p_which+1)+"_";
	
	delay->set_property( &chorus->get_port_by_name( prefix+"delay" ) );
	rate->set_property( &chorus->get_port_by_name( prefix+"rate" ) );
	depth->set_property( &chorus->get_port_by_name( prefix+"depth" ) );
	level->set_property( &chorus->get_port_by_name( prefix+"level" ) );
	width->set_property( &chorus->get_port_by_name( prefix+"width") );
	cutoff->set_property( &chorus->get_port_by_name( prefix+"cutoff" ) );
	pan->set_property( &chorus->get_port_by_name( prefix+"pan" ) );
	
	selected_voice->which=p_which;
	
}

ChorusPluginUI::ChorusPluginUI(QWidget *p_parent,ChorusPlugin *p_chorus) :SoundPluginUI(p_parent,p_chorus) {
	
	chorus=p_chorus;
	
	CHBox *hb = generate_default_layout();
	
	
	QPixmap value_pixmap(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_VALUE));
	QPixmap label_pixmap(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_LABEL));
	PixmapSlider::Skin slider_skin(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_SLIDER_BASE),GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_SLIDER_LIGHT),GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_SLIDER_GRABBER));
	
	
	CVBox *chorus_vb = new CVBox(hb);
	
	new PixmapLabel(chorus_vb,GET_QPIXMAP(THEME_CHORUS__TOP));
	
	CHBox *chorus_top_hb = new CHBox(chorus_vb);
	
	CVBox *chorus_labels_vb= new CVBox(chorus_top_hb);
	
	new PixmapLabel(chorus_labels_vb,GET_QPIXMAP(THEME_CHORUS__LABELS));
	
	CHBox *voice_hb = new CHBox(chorus_labels_vb);
	
	current_voice = new PixmapButtonGroup(this);
	
	PixmapButton *b = new PixmapButton(voice_hb,PixmapButton::Skin( GET_QPIXMAP( THEME_CHORUS__VOICE_1 ),GET_QPIXMAP( THEME_CHORUS__VOICE_1_ACTIVE )), PixmapButton::TYPE_TOGGLE );
	current_voice->add_button( b );
			
	b = new PixmapButton(voice_hb,PixmapButton::Skin( GET_QPIXMAP( THEME_CHORUS__VOICE_2 ),GET_QPIXMAP( THEME_CHORUS__VOICE_2_ACTIVE )), PixmapButton::TYPE_TOGGLE );
	
	current_voice->add_button( b );
	
	b = new PixmapButton(voice_hb,PixmapButton::Skin( GET_QPIXMAP( THEME_CHORUS__VOICE_3 ),GET_QPIXMAP( THEME_CHORUS__VOICE_3_ACTIVE )), PixmapButton::TYPE_TOGGLE );
	
	current_voice->add_button( b );
	
	b = new PixmapButton(voice_hb,PixmapButton::Skin( GET_QPIXMAP( THEME_CHORUS__VOICE_4 ),GET_QPIXMAP( THEME_CHORUS__VOICE_4_ACTIVE )), PixmapButton::TYPE_TOGGLE );
	
	current_voice->add_button( b );
	
	new PixmapLabel(chorus_top_hb,GET_QPIXMAP(THEME_CHORUS__SEPARATOR));
	
	CVBox *top_knobs_vb = new CVBox(chorus_top_hb);
	
	CHBox *top_knobs_hb = new CHBox(top_knobs_vb);
		
	Knob::Skin up_knob_skin;
	up_knob_skin.angle_begin=30;
	up_knob_skin.base=GET_QPIXMAP(THEME_CHORUS__TOP_KNOB);
	up_knob_skin.handle=GET_QPIXMAP(THEME_CHORUS__KNOB_POS);
	up_knob_skin.handle_at_distance=10;
	
	cutoff = new PropertyEditKnob(top_knobs_hb,up_knob_skin);
	register_property_editor( cutoff );
	
	pan = new PropertyEditKnob(top_knobs_hb,up_knob_skin);
	
	new PixmapLabel(top_knobs_vb,GET_QPIXMAP(THEME_CHORUS__TOP_KNOBS_BOTTOM));
	
	new PixmapLabel(chorus_vb,GET_QPIXMAP(THEME_CHORUS__MIDDLE));
	
	CHBox *bottom_hb = new CHBox(chorus_vb);
	
	Knob::Skin knob_skin;
	knob_skin.angle_begin=30;
	knob_skin.base=GET_QPIXMAP(THEME_CHORUS__BOTTOM_KNOB);
	knob_skin.handle=GET_QPIXMAP(THEME_CHORUS__KNOB_POS);
	knob_skin.handle_at_distance=10;
	
	delay = new PropertyEditKnob(bottom_hb,knob_skin);
	register_property_editor( delay );
	
	rate = new PropertyEditKnob(bottom_hb,knob_skin);
	register_property_editor( rate );
	
	depth = new PropertyEditKnob(bottom_hb,knob_skin);
	register_property_editor( depth );
	level = new PropertyEditKnob(bottom_hb,knob_skin);
	register_property_editor( level );
	
	width = new PropertyEditKnob(bottom_hb,knob_skin);
	register_property_editor( width );
	
	new PixmapLabel(chorus_vb,GET_QPIXMAP(THEME_CHORUS__BOTTOM));
	
	create_editor_for_property( "wet_send",hb );
	create_editor_for_property( "dry_send",hb );
	
	/* set default props */
	
	selected_voice=NULL;
	
	selected_voice = dynamic_cast<SelectedVoice*>( p_chorus->get_metadata() );
	if (selected_voice==NULL) {
		
		selected_voice= new SelectedVoice;
		p_chorus->set_metadata( selected_voice );
	}
	
	current_voice->set_selected_index( selected_voice->which );
	voice_selected_slot(selected_voice->which);

	
	QObject::connect(current_voice,SIGNAL(button_selected_signal( int )),this,SLOT(voice_selected_slot( int )));
	
}


ChorusPluginUI::~ChorusPluginUI()
{
}


}
