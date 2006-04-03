//
// C++ Implementation: lfo_params_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "lfo_params_editor.h"
#include <Qt/qlabel.h>


namespace ReShaked {

void LFO_ParamsEditor::changed_by_editor(void *p_userdata,PropertyEditor*,double) {
	
	LFO_ParamsEditor* _this= (LFO_ParamsEditor*)p_userdata;
	
	_this->lfo_changed_signal();
	
}

void LFO_ParamsEditor::mode_selected(int p_mode) {
	
	switch (p_mode) {
		
		case 0: lfo->set_mode(LFO::MODE_SINE); break;
		case 1: lfo->set_mode(LFO::MODE_SQUARE); break;
		case 2: lfo->set_mode(LFO::MODE_SAW_UP); break;
		case 3: lfo->set_mode(LFO::MODE_SAW_DOWN); break;
	}
	
	lfo_changed_signal();
	
}
void LFO_ParamsEditor::update_values() {

	delay_slider->check_if_changed();
	delay_label->check_if_changed();
	
	rate_slider->check_if_changed();
	rate_label->check_if_changed();
	depth_slider->check_if_changed();
	depth_label->check_if_changed();
	phase_slider->check_if_changed();
	random_depth_slider->check_if_changed();
	random_seed_slider->check_if_changed();
	
	switch (lfo->get_mode()) {
		
		case LFO::MODE_SINE: lfo_mode_group->set_selected_index( 0 ); break;
		case LFO::MODE_SQUARE: lfo_mode_group->set_selected_index( 1 ); break;
		case LFO::MODE_SAW_UP: lfo_mode_group->set_selected_index( 2 ); break;
		case LFO::MODE_SAW_DOWN: lfo_mode_group->set_selected_index( 3 ); break;
	}
	
}

void LFO_ParamsEditor::set_lfo(LFO *p_lfo) {
	
	lfo=p_lfo;
	delay.set_instance(p_lfo);
	rate.set_instance(p_lfo);
	depth.set_instance(p_lfo);
	phase.set_instance(p_lfo);
	random_depth.set_instance(p_lfo);
	random_seed.set_instance(p_lfo);
	
	delay_slider->set_property( &delay );
	delay_label->set_property( &delay );
	rate_slider->set_property( &rate );
	rate_label->set_property( &rate );
	depth_slider->set_property( &depth );
	depth_label->set_property( &depth );
	phase_slider->set_property( &phase );
	random_depth_slider->set_property( &random_depth );
	random_seed_slider->set_property( &random_seed );
	
	
	update_values();
	
	
	
	
}

void LFO_ParamsEditor::add_label(QString p_text,QWidget *p_parent) {


	QLabel *l = new QLabel(p_text,p_parent);
	QPalette p = l->palette();
	p.setColor(QPalette::Text,skin.label);
//	p.setColor(QPalette::WindowText,skin.label);
	l->setPalette(p);
	QFont f;
	f.setPixelSize(13);
	f.setBold(true);
	l->setFont(f);
	
}


LFO_ParamsEditor::LFO_ParamsEditor(QWidget *p_parent, const Skin& p_skin) : CHBox(p_parent) {
	
	skin=p_skin;
	lfo=NULL;
	
	delay.set_set_func(&LFO::set_delay);
	delay.set_get_func(&LFO::get_delay);
	delay.set_min( 0 );
	delay.set_max( 4000 );
	delay.config( "delay", "Delay", 1.0, 0.0, "ms");
	
	rate.set_set_func(&LFO::set_rate);
	rate.set_get_func(&LFO::get_rate);
	rate.set_min( 0.01 );
	rate.set_max( 16.0 );
	rate.config( "rate", "Rate", 0.01, 1.0, "hz");
	
	depth.set_set_func(&LFO::set_depth);
	depth.set_get_func(&LFO::get_depth);
	depth.set_min( 0.0 );
	depth.set_max_func( &LFO::get_max_depth );
	depth.config( "depth", "Depth", 0.01,0.0,"");
	
	phase.set_set_func(&LFO::set_phase);
	phase.set_get_func(&LFO::get_phase);
	phase.set_min( 0.0 );
	phase.set_max( 1.0 );
	phase.config( "phase", "Phase", 0.01,0.0,"");
	
	random_depth.set_set_func(&LFO::set_random_depth);
	random_depth.set_get_func(&LFO::get_random_depth);
	random_depth.set_min( 0.0 );
	random_depth.set_max( 1.0 );
	random_depth.config( "random_depth", "Random Depth", 0.01,0.0,"");
	
	random_seed.set_set_func(&LFO::set_random_seed);
	random_seed.set_get_func(&LFO::get_random_seed);
	random_seed.set_min( 0.0 );
	random_seed.set_max( 1.0 );
	random_seed.config( "random_seed", "Random Seed", 0.01,0.0,"");
	
	
	lfo_mode_group = new PixmapButtonGroup(this);
	QObject::connect(lfo_mode_group,SIGNAL(button_selected_signal( int )),this,SLOT(mode_selected( int )));
	
	CVBox *vbox = new CVBox(this);
	layout()->setAlignment(vbox,Qt::AlignTop);
	add_label( "Mode",vbox);
	vbox->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	
	CHBox *hb = new CHBox(vbox);
	
	button_sine = new PixmapButton(hb,skin.button_sine,PixmapButton::TYPE_TOGGLE);
	lfo_mode_group->add_button( button_sine );
	
	button_square = new PixmapButton(hb,skin.button_square,PixmapButton::TYPE_TOGGLE);
	lfo_mode_group->add_button( button_square );
	
	hb = new CHBox(vbox);
	
	button_sawup = new PixmapButton(hb,skin.button_sawup,PixmapButton::TYPE_TOGGLE);
	lfo_mode_group->add_button( button_sawup );
	
	button_sawdown = new PixmapButton(hb,skin.button_sawdown,PixmapButton::TYPE_TOGGLE);
	lfo_mode_group->add_button( button_sawdown );
	
	vbox = new CVBox(this);
	vbox->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	hb = new CHBox(vbox);
	
	add_label("Delay",hb);
	delay_label = new PropertyEditLabel(hb,skin.display);
	
	delay_slider = new PropertyEditSlider(vbox,skin.slider,PixmapSlider::TYPE_HORIZONTAL);
	delay_label->add_to_group( delay_slider );
	
	hb = new CHBox(vbox);
	
	add_label("Rate",hb);
	rate_label = new PropertyEditLabel(hb,skin.display);
	rate_slider = new PropertyEditSlider(vbox,skin.slider,PixmapSlider::TYPE_HORIZONTAL);
	rate_label->add_to_group( rate_slider );
	
	hb = new CHBox(vbox);
	add_label("Depth",hb);
	depth_label = new PropertyEditLabel(hb,skin.display);
	depth_slider = new PropertyEditSlider(vbox,skin.slider,PixmapSlider::TYPE_HORIZONTAL);
	depth_label->add_to_group( depth_slider );
	
	vbox = new CVBox(this);
	vbox->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	
	add_label("Phase",vbox);
	phase_slider = new PropertyEditSlider(vbox,skin.slider,PixmapSlider::TYPE_HORIZONTAL);
	
	add_label("Random",vbox);
	random_depth_slider = new PropertyEditSlider(vbox,skin.slider,PixmapSlider::TYPE_HORIZONTAL);
	
	add_label("Rand.Seed",vbox);
	random_seed_slider = new PropertyEditSlider(vbox,skin.slider,PixmapSlider::TYPE_HORIZONTAL);
	
	layout()->setMargin(3);
	layout()->setSpacing(3);
	
	/* updates */
	
	delay_slider->set_changed_by_editor_callback( this, &LFO_ParamsEditor::changed_by_editor );
	delay_label->set_changed_by_editor_callback( this, &LFO_ParamsEditor::changed_by_editor );
	rate_slider->set_changed_by_editor_callback( this, &LFO_ParamsEditor::changed_by_editor );
	rate_label->set_changed_by_editor_callback( this, &LFO_ParamsEditor::changed_by_editor );
	depth_slider->set_changed_by_editor_callback( this, &LFO_ParamsEditor::changed_by_editor );
	depth_label->set_changed_by_editor_callback( this, &LFO_ParamsEditor::changed_by_editor );
	phase_slider->set_changed_by_editor_callback( this, &LFO_ParamsEditor::changed_by_editor );
	random_depth_slider->set_changed_by_editor_callback( this, &LFO_ParamsEditor::changed_by_editor );
	random_seed_slider->set_changed_by_editor_callback( this, &LFO_ParamsEditor::changed_by_editor );
	
	
}


LFO_ParamsEditor::~LFO_ParamsEditor()
{
}


}
