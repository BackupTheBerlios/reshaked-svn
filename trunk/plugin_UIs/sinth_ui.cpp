//
// C++ Implementation: synth_ui
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sinth_ui.h"
#include "interface/visual_settings.h"
#include "ui_blocks/panner_editor.h"
#include "ui_blocks/helpers.h"


namespace ReShaked {

SoundPluginUI* SinthUI::create_this(SoundPlugin *p_plugin,QWidget *p_parent) {
	
	Sinth *sinth = dynamic_cast<Sinth*>( p_plugin );
	if (sinth)
		return new SinthUI(p_parent,sinth);
	else
		return NULL;
		
}

void SinthUI::oscedit_make_sine_slot(int p_which) {
	
	Oscillator *o=sinth->get_osc(p_which);
	ERR_FAIL_COND(!o);
	
	for (int i=0;i<Oscillator::BASE_SIZE;i++) {
		
		float ph=(float)i/(float)Oscillator::BASE_SIZE;
		float v=sin( ph*M_PI*2.0 );
		o->set_frame(i,v);
	}
	
	o->update_submaps();
	
	oscview_1->update();
	oscview_2->update();
	
}
void SinthUI::oscedit_make_saw_slot(int p_which) {
	
	Oscillator *o=sinth->get_osc(p_which);
	ERR_FAIL_COND(!o);
	
	for (int i=0;i<Oscillator::BASE_SIZE;i++) {
		
		float ph=(float)i/(float)Oscillator::BASE_SIZE;
		float v=2.0*(1.0-ph)-1.0;
		o->set_frame(i,v);
	}
	
	o->update_submaps();
	
	oscview_1->update();
	oscview_2->update();
	
}
void SinthUI::oscedit_make_square_slot(int p_which) {
	
	Oscillator *o=sinth->get_osc(p_which);
	ERR_FAIL_COND(!o);
	
	for (int i=0;i<Oscillator::BASE_SIZE;i++) {
		
		float ph=(float)i/(float)Oscillator::BASE_SIZE;
		float v=(ph<0.5)?-1.0:1.0;
		o->set_frame(i,v);
	}
	
	o->update_submaps();
	
	oscview_1->update();
	oscview_2->update();
	
}
void SinthUI::oscedit_edit_slot(int p_which) {
	
	ERR_FAIL_INDEX(p_which,2);
	
	oscedit_container->edit_osc[p_which]->exec( sinth->get_osc( p_which ) );
	oscview_1->update();
	oscview_2->update();
}

void SinthUI::filter_mode_selected(int p_mode) {
	
	Property *p=&sinth->get_port_by_name("filter_type");
	ERR_FAIL_COND( !p );
	p->set( p_mode );
	filter_editor->update();
}

SinthUI::SinthUI(QWidget *p_parent,Sinth *p_sinth) :SoundPluginUI(p_parent,p_sinth) {
	
	
	oscedit_container=NULL;
	
	oscedit_container = dynamic_cast<OscEditContain*>( p_sinth->get_metadata() );
	
	if (oscedit_container==NULL) {
		
		oscedit_container = new OscEditContain;
		oscedit_container->edit_osc[0] = new OscillatorEditor(topLevelOf(this));
		oscedit_container->edit_osc[1] = new OscillatorEditor(topLevelOf(this));
		
		p_sinth->set_metadata( oscedit_container );
	}

	
	sinth=p_sinth;
	
	CHBox *hb = generate_default_layout(false);
	
	new PixmapLabel(hb, GET_QPIXMAP( THEME_SINTH__BEGIN ) );
	
	
	
	{ /* OSC EDIT */
		
		CVBox *osc_edit_vb = new CVBox( hb );
		
		new PixmapLabel(osc_edit_vb, GET_QPIXMAP( THEME_SINTH__OSCEDIT_TOP ) );
		CHBox *action_hb = new CHBox(osc_edit_vb);
		
		PixmapButton *b;
		
		b = new PixmapButton(action_hb,PixmapButton::Skin( GET_QPIXMAP( THEME_SINTH__OSCEDIT_WAVE_SINE),GET_QPIXMAP( THEME_SINTH__OSCEDIT_WAVE_SINE_PUSHED) ) );
		connect_bind_int(b,SIGNAL(mouse_pressed_signal()),this,SLOT(oscedit_make_sine_slot( int )),0);
	
		b = new PixmapButton(action_hb,PixmapButton::Skin( GET_QPIXMAP( THEME_SINTH__OSCEDIT_WAVE_SAW),GET_QPIXMAP( THEME_SINTH__OSCEDIT_WAVE_SAW_PUSHED) ) );
		connect_bind_int(b,SIGNAL(mouse_pressed_signal()),this,SLOT(oscedit_make_saw_slot( int )),0);
	
		b = new PixmapButton(action_hb,PixmapButton::Skin( GET_QPIXMAP( THEME_SINTH__OSCEDIT_WAVE_SQUARE),GET_QPIXMAP( THEME_SINTH__OSCEDIT_WAVE_SQUARE_PUSHED) ) );
		connect_bind_int(b,SIGNAL(mouse_pressed_signal()),this,SLOT(oscedit_make_square_slot( int )),0);
		
		b = new PixmapButton(action_hb,PixmapButton::Skin( GET_QPIXMAP( THEME_SINTH__OSCEDIT_WAVE_EDIT),GET_QPIXMAP( THEME_SINTH__OSCEDIT_WAVE_EDIT_PUSHED) ) );
		connect_bind_int(b,SIGNAL(mouse_pressed_signal()),this,SLOT(oscedit_edit_slot( int )),0);
		
		OscillatorDisplay::Skin sk;
		
		sk.bg=GET_QPIXMAP( THEME_SINTH__OSCEDIT_DISPLAY);
		sk.wave_color=QColor(220,220,250);
		
		oscview_1 = new OscillatorDisplay(osc_edit_vb,sk);
		oscview_1->set_oscillator( sinth->get_osc(0) );
		new PixmapLabel(osc_edit_vb, GET_QPIXMAP( THEME_SINTH__OSCEDIT_MIDDLE ) );
		
		action_hb = new CHBox(osc_edit_vb);
		
		b = new PixmapButton(action_hb,PixmapButton::Skin( GET_QPIXMAP( THEME_SINTH__OSCEDIT_WAVE_SINE),GET_QPIXMAP( THEME_SINTH__OSCEDIT_WAVE_SINE_PUSHED) ) );
		connect_bind_int(b,SIGNAL(mouse_pressed_signal()),this,SLOT(oscedit_make_sine_slot( int )),1);
	
		b = new PixmapButton(action_hb,PixmapButton::Skin( GET_QPIXMAP( THEME_SINTH__OSCEDIT_WAVE_SAW),GET_QPIXMAP( THEME_SINTH__OSCEDIT_WAVE_SAW_PUSHED) ) );
		connect_bind_int(b,SIGNAL(mouse_pressed_signal()),this,SLOT(oscedit_make_saw_slot( int )),1);
	
		b = new PixmapButton(action_hb,PixmapButton::Skin( GET_QPIXMAP( THEME_SINTH__OSCEDIT_WAVE_SQUARE),GET_QPIXMAP( THEME_SINTH__OSCEDIT_WAVE_SQUARE_PUSHED) ) );
		connect_bind_int(b,SIGNAL(mouse_pressed_signal()),this,SLOT(oscedit_make_square_slot( int )),1);
		
		b = new PixmapButton(action_hb,PixmapButton::Skin( GET_QPIXMAP( THEME_SINTH__OSCEDIT_WAVE_EDIT),GET_QPIXMAP( THEME_SINTH__OSCEDIT_WAVE_EDIT_PUSHED) ) );
		connect_bind_int(b,SIGNAL(mouse_pressed_signal()),this,SLOT(oscedit_edit_slot( int )),1);
		
		oscview_2 = new OscillatorDisplay(osc_edit_vb,sk);
		oscview_2->set_oscillator( sinth->get_osc(1) );
		
		new PixmapLabel(osc_edit_vb, GET_QPIXMAP( THEME_SINTH__OSCEDIT_BOTTOM ) );
		
	}
		
	
	new PixmapLabel(hb, GET_QPIXMAP( THEME_SINTH__OSCEDIT_OSCENV_SEPARATOR ) );
	
	{ /* ENVELOPES */
		
		CVBox *osc_env_vb = new CVBox( hb );
		
		new PixmapLabel(osc_env_vb, GET_QPIXMAP( THEME_SINTH__OSCENV_OSC1_LABEL ) );
		
		CHBox *osc1_env_hb = new CHBox(osc_env_vb);
		
		static const char * env_stage_name[4]={"env_attack","env_decay","env_sustain","env_release"};
		
		for (int i=0;i<4;i++) {
			
			String name=String("osc_1_")+env_stage_name[i];
			
			Knob::Skin sk;
			
			sk.angle_begin=30;
			sk.handle_at_distance=9;
			sk.base=GET_QPIXMAP( THEME_SINTH__OSCENV_OSC1_KNOB );
			sk.handle=GET_QPIXMAP( THEME_SINTH__KNOB_POS );
			
			if (i>0)
				new PixmapLabel(osc1_env_hb,GET_QPIXMAP( THEME_SINTH__OSCENV_OSC1_KNOB_SEPARATOR ) );
			
			printf("name is %s\n",name.ascii().get_data());
			Property *p = &sinth->get_port_by_name( name );
			ERR_CONTINUE(p==NULL);
			PropertyEditor *pe = new PropertyEditKnob(osc1_env_hb, sk );
			pe->set_property( p );
			register_property_editor( pe );
			
		}
		
		new PixmapLabel(osc_env_vb, GET_QPIXMAP( THEME_SINTH__OSCENV_MIDDLE ) );

		CHBox *osc2_env_hb = new CHBox(osc_env_vb);
		
		for (int i=0;i<4;i++) {
			
			String name=String("osc_2_")+env_stage_name[i];
			
			Knob::Skin sk;
			
			sk.angle_begin=30;
			sk.handle_at_distance=9;
			sk.base=GET_QPIXMAP( THEME_SINTH__OSCENV_OSC2_KNOB );
			sk.handle=GET_QPIXMAP( THEME_SINTH__KNOB_POS );
			
			if (i>0)
				new PixmapLabel(osc2_env_hb,GET_QPIXMAP( THEME_SINTH__OSCENV_OSC2_KNOB_SEPARATOR ) );
			
			Property *p = &sinth->get_port_by_name( name );
			ERR_CONTINUE(p==NULL);
			PropertyEditor *pe = new PropertyEditKnob(osc2_env_hb, sk );
			pe->set_property( p );
			register_property_editor( pe );
			
		}
		
		new PixmapLabel(osc_env_vb, GET_QPIXMAP( THEME_SINTH__OSCENV_BOTTOM ) );
		
		
	}
	
	new PixmapLabel(hb, GET_QPIXMAP( THEME_SINTH__OSCENV_OSCTUNE_SEPARATOR ) );
	
	{ /* TUNE */
		
		CVBox *osc_tune_vb = new CVBox( hb );
		
		new PixmapLabel(osc_tune_vb, GET_QPIXMAP( THEME_SINTH__OSCTUNE_OSC1_COARSE_LABEL ) );
		
		CHBox *spin_hb;
		
		spin_hb = new CHBox(osc_tune_vb);
		
		PropertyEditLabel *label = new PropertyEditLabel(spin_hb, GET_QPIXMAP( THEME_SINTH__OSCTUNE_DISPLAY ) );
		label->set_property( &sinth->get_port_by_name( "osc_1_tune_coarse" ) );
		label->set_suffix_visible( false );		
		PropertyEditor *updown = new PropertyEditUpDown(spin_hb,PixmapUpDown::Skin( GET_QPIXMAP( THEME_SINTH__OSCTUNE_UPDOWN ) ) );
		updown->set_property( label->get_property() );
		updown->add_to_group( label);
	
		register_property_editor( label );
		register_property_editor( updown );
		
		new PixmapLabel(osc_tune_vb, GET_QPIXMAP( THEME_SINTH__OSCTUNE_OSC1_FINE_LABEL ) );
		
		spin_hb = new CHBox(osc_tune_vb);
		
		label = new PropertyEditLabel(spin_hb, GET_QPIXMAP( THEME_SINTH__OSCTUNE_DISPLAY ) );
		label->set_property( &sinth->get_port_by_name( "osc_1_tune_fine" ) );
		label->set_suffix_visible( false );
		
		updown = new PropertyEditUpDown(spin_hb,PixmapUpDown::Skin( GET_QPIXMAP( THEME_SINTH__OSCTUNE_UPDOWN ) ) );
		updown->set_property( label->get_property() );
		updown->add_to_group( label);
		
		register_property_editor( label );
		register_property_editor( updown );
		
		new PixmapLabel(osc_tune_vb, GET_QPIXMAP( THEME_SINTH__OSCTUNE_OSC2_COARSE_LABEL ) );
		
		spin_hb = new CHBox(osc_tune_vb);
		
		label = new PropertyEditLabel(spin_hb, GET_QPIXMAP( THEME_SINTH__OSCTUNE_DISPLAY ) );
		label->set_property( &sinth->get_port_by_name( "osc_2_tune_coarse" ) );
		label->set_suffix_visible( false );
		
		updown = new PropertyEditUpDown(spin_hb,PixmapUpDown::Skin( GET_QPIXMAP( THEME_SINTH__OSCTUNE_UPDOWN ) ) );
		updown->set_property( label->get_property() );
		updown->add_to_group( label);
	
		register_property_editor( label );
		register_property_editor( updown );
		
		new PixmapLabel(osc_tune_vb, GET_QPIXMAP( THEME_SINTH__OSCTUNE_OSC2_FINE_LABEL ) );
		
		spin_hb = new CHBox(osc_tune_vb);
		
		label = new PropertyEditLabel(spin_hb, GET_QPIXMAP( THEME_SINTH__OSCTUNE_DISPLAY ) );
		label->set_property( &sinth->get_port_by_name( "osc_2_tune_fine" ) );
		label->set_suffix_visible( false );
		
		updown = new PropertyEditUpDown(spin_hb,PixmapUpDown::Skin( GET_QPIXMAP( THEME_SINTH__OSCTUNE_UPDOWN ) ) );
		updown->set_property( label->get_property() );
		updown->add_to_group( label);
		
		register_property_editor( label );
		register_property_editor( updown );
		
		new PixmapLabel(osc_tune_vb, GET_QPIXMAP( THEME_SINTH__OSCTUNE_BOTTOM ) );
		
	}
	
	new PixmapLabel(hb, GET_QPIXMAP( THEME_SINTH__OSCTUNE_ROUTING_SEPARATOR ) );
		
	{ /* ROUTING */
		
		CVBox *routing_vb = new CVBox( hb );
		
		new PixmapLabel(routing_vb, GET_QPIXMAP( THEME_SINTH__ROUTING_TOP ) );
		
		CHBox *routing_hb = new CHBox(routing_vb);
		
		PropertyEditor *combo = new PropertyEditCombo(routing_hb,GET_QPIXMAP( THEME_SINTH__ROUTING_COMBO ));
		combo->set_property( &sinth->get_port_by_name( "modulation_mode" ) );
		
		register_property_editor( combo );
		
		new PixmapLabel(routing_hb, GET_QPIXMAP( THEME_SINTH__ROUTING_COMBO_RIGHT ) );
		
		new PixmapLabel(routing_vb, GET_QPIXMAP( THEME_SINTH__ROUTING_MIDDLE ) );
		
		routing_hb = new CHBox(routing_vb);
		
		Knob::Skin sk;
		
		sk.base=GET_QPIXMAP( THEME_SINTH__ROUTING_STRENGTH );
		sk.angle_begin=30;
		sk.handle_at_distance=9;
		sk.handle=GET_QPIXMAP( THEME_SINTH__KNOB_POS );
		
		new PixmapLabel(routing_hb, GET_QPIXMAP( THEME_SINTH__ROUTING_STRENGTH_LEFT ) );
		
		PropertyEditor *knob = new PropertyEditKnob(routing_hb,sk);
		knob->set_property( &sinth->get_port_by_name( "modulation_amount" ) );
		
		register_property_editor( knob );
		
		new PixmapLabel(routing_vb, GET_QPIXMAP( THEME_SINTH__ROUTING_BOTTOM ) );
		
	}
		
				
	new PixmapLabel(hb, GET_QPIXMAP( THEME_SINTH__ROUTING_FILTER_SEPARATOR ) );
	
	{ /* FILTER */
		
		
		CVBox *filter_vb = new CVBox(hb);
		
		new PixmapLabel(filter_vb, GET_QPIXMAP( THEME_SINTH__FILTER_TOP ) );
		
		CHBox *filter_mode_hb = new CHBox( filter_vb );
		
		PixmapButtonGroup *filter_mode = new PixmapButtonGroup(this);
		
		filter_mode->add_button( new PixmapButton( filter_mode_hb, PixmapButton::Skin( GET_QPIXMAP( THEME_SINTH__FILTER_MODE_OFF ), GET_QPIXMAP( THEME_SINTH__FILTER_MODE_OFF_ACTIVE ) ), PixmapButton::TYPE_TOGGLE ) );
		
		filter_mode->add_button( new PixmapButton( filter_mode_hb, PixmapButton::Skin( GET_QPIXMAP( THEME_SINTH__FILTER_MODE_LPF ), GET_QPIXMAP( THEME_SINTH__FILTER_MODE_LPF_ACTIVE ) ), PixmapButton::TYPE_TOGGLE ) );
		
		filter_mode->add_button( new PixmapButton( filter_mode_hb, PixmapButton::Skin( GET_QPIXMAP( THEME_SINTH__FILTER_MODE_BPF ), GET_QPIXMAP( THEME_SINTH__FILTER_MODE_BPF_ACTIVE ) ), PixmapButton::TYPE_TOGGLE ) );
		
		filter_mode->add_button( new PixmapButton( filter_mode_hb, PixmapButton::Skin( GET_QPIXMAP( THEME_SINTH__FILTER_MODE_HPF ), GET_QPIXMAP( THEME_SINTH__FILTER_MODE_HPF_ACTIVE ) ), PixmapButton::TYPE_TOGGLE ) );
		
		Property *filter_mode_prop = &sinth->get_port_by_name( "filter_type");
		if (filter_mode_prop)
			filter_mode->set_selected_index( (int)filter_mode_prop->get() );
		
		QObject::connect(filter_mode,SIGNAL(button_selected_signal(int)),this,SLOT(filter_mode_selected(int)));
		
		FilterEditor::Skin filter_sk;
		
		filter_sk.bg=GET_QPIXMAP( THEME_SINTH__FILTER_DISPLAY );
		filter_sk.line_color=QColor(220,220,250);
		filter_sk.border_w=1;
		
		filter_editor = new FilterEditor(filter_vb,filter_sk);
		
		filter_editor->set_properties( &sinth->get_port_by_name( "filter_type"), &sinth->get_port_by_name( "filter_cutoff"), &sinth->get_port_by_name( "filter_resonance") );
		
		register_property_editor( filter_editor );
		
		new PixmapLabel(filter_vb, GET_QPIXMAP( THEME_SINTH__FILTER_ENVELOPE_LABEL ) );
		
		CHBox *filter_env_hb = new CHBox (filter_vb);
		
		CVBox *filter_env_stg_vb = new CVBox(filter_env_hb);
		
		PropertyEditor *attack_enabled = new PropertyEditButton(filter_env_stg_vb, PixmapButton::Skin( GET_QPIXMAP( THEME_SINTH__FILTER_ENVELOPE_ATTACK ),  GET_QPIXMAP( THEME_SINTH__FILTER_ENVELOPE_ATTACK_ACTIVE ) ) );
		attack_enabled->set_property( &sinth->get_port_by_name( "filter_attack_enabled") );
		register_property_editor( attack_enabled );
		
		PannerEditor::Skin pe_sk;
		pe_sk.bg=GET_QPIXMAP( THEME_SINTH__FILTER_ATTACK_DISPLAY );
		pe_sk.pos=GET_QPIXMAP( THEME_SINTH__ENV_POS );
		pe_sk.border_w=1;
		
		PannerEditor *pne = new PannerEditor(filter_env_stg_vb,true,pe_sk);
		pne->set_properties( &sinth->get_port_by_name( "filter_attack_time"), &sinth->get_port_by_name( "filter_attack_level") );
		register_property_editor( pne );
		
		filter_env_stg_vb = new CVBox(filter_env_hb);
		
		PropertyEditor *release_enabled = new PropertyEditButton(filter_env_stg_vb, PixmapButton::Skin( GET_QPIXMAP( THEME_SINTH__FILTER_ENVELOPE_RELEASE ),  GET_QPIXMAP( THEME_SINTH__FILTER_ENVELOPE_RELEASE_ACTIVE ) ) );
		release_enabled->set_property( &sinth->get_port_by_name( "filter_release_enabled") );
		register_property_editor( release_enabled );
		
		pe_sk.bg=GET_QPIXMAP( THEME_SINTH__FILTER_RELEASE_DISPLAY );
		pe_sk.pos=GET_QPIXMAP( THEME_SINTH__ENV_POS );
		pe_sk.border_w=1;
		
		pne = new PannerEditor(filter_env_stg_vb,true,pe_sk);
		pne->set_properties( &sinth->get_port_by_name( "filter_release_time"), &sinth->get_port_by_name( "filter_release_level") );
		register_property_editor( pne );
		
		
		new PixmapLabel(hb, GET_QPIXMAP( THEME_SINTH__FILTER_SEPARATOR) );
		
		CVBox *filter_lfo_vb = new CVBox(hb);
		
		
		new PixmapLabel(filter_lfo_vb, GET_QPIXMAP( THEME_SINTH__FILTER_LFO_SPEED_LABEL ) );
		
		Knob::Skin skin_speed;
			
		skin_speed.angle_begin=30;
		skin_speed.handle_at_distance=7;
		skin_speed.base=GET_QPIXMAP( THEME_SINTH__FILTER_LFO_SPEED_KNOB );
		skin_speed.handle=GET_QPIXMAP( THEME_SINTH__KNOB_POS );
		
		PropertyEditor *pe = new PropertyEditKnob(filter_lfo_vb,skin_speed);
		pe->set_property( &sinth->get_port_by_name( "filter_lfo_rate") );
		register_property_editor(pe);
		
		new PixmapLabel(filter_lfo_vb, GET_QPIXMAP( THEME_SINTH__FILTER_LFO_DEPTH_LABEL ) );
		
		Knob::Skin skin_depth;
			
		skin_depth.angle_begin=30;
		skin_depth.handle_at_distance=7;
		skin_depth.base=GET_QPIXMAP( THEME_SINTH__FILTER_LFO_DEPTH_KNOB );
		skin_depth.handle=GET_QPIXMAP( THEME_SINTH__KNOB_POS );
		
		pe = new PropertyEditKnob(filter_lfo_vb,skin_depth);
		pe->set_property( &sinth->get_port_by_name("filter_lfo_depth") );
		register_property_editor(pe);
		
		new PixmapLabel(filter_lfo_vb, GET_QPIXMAP( THEME_SINTH__FILTER_LFO_BOTTOM ) );
		
		new PixmapLabel(hb, GET_QPIXMAP( THEME_SINTH__END) );
		
	}
	
	//add gain
	create_editor_for_property( "global_gain", hb );	
	
	
	
}


SinthUI::~SinthUI()
{
}


}
