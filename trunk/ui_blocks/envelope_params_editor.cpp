//
// C++ Implementation: envelope_params_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "envelope_params_editor.h"
#include <Qt/qlabel.h>

namespace ReShaked {


void EnvelopeParamsEditor::changed_by_editor(void *p_userdata,PropertyEditor*,double) {
	
	EnvelopeParamsEditor *_this = (EnvelopeParamsEditor*)p_userdata;
	
	_this->envelope_changed_signal();
}

void EnvelopeParamsEditor::update_controls() {
	
	label_loop_begin_edit->check_if_changed();
	label_loop_end_edit->check_if_changed();
	label_susloop_begin_edit->check_if_changed();
	label_susloop_end_edit->check_if_changed();
	updown_loop_begin_edit->check_if_changed();
	updown_loop_end_edit->check_if_changed();
	updown_susloop_begin_edit->check_if_changed();
	updown_susloop_end_edit->check_if_changed();
	
	if (envelope) {
		envelope_enabled->set_pressed( envelope->is_enabled() );
		loop_enabled->set_pressed( envelope->is_loop_enabled() );
		susloop_enabled->set_pressed( envelope->is_sustain_loop_enabled() );
	}	
}

void EnvelopeParamsEditor::set_susloop(bool p_on) {
	
	if (!envelope)
		return;
	
	envelope->set_sustain_loop_enabled( p_on );
	envelope_changed_signal();
}
void EnvelopeParamsEditor::set_loop(bool p_on) {
	
	if (envelope==NULL)
		return;
	
	envelope->set_loop_enabled( p_on );
	envelope_changed_signal();
	
}

void EnvelopeParamsEditor::set_cubic(bool p_on) {
	
	if (envelope==NULL)
		return;
	
	
	envelope_changed_signal();
	
}

void EnvelopeParamsEditor::set_enabled(bool p_on) {
	
	if (envelope==NULL)
		return;
	
	envelope->set_enabled( p_on );
	envelope_changed_signal();
	
}


void EnvelopeParamsEditor::set_envelope(Envelope *p_envelope) {
	
	envelope=p_envelope;
	
	loop_begin_prop.set_instance( p_envelope );
	loop_end_prop.set_instance( p_envelope );
	susloop_begin_prop.set_instance( p_envelope );
	susloop_end_prop.set_instance( p_envelope );
	
	label_loop_begin_edit->set_property(&loop_begin_prop);
	updown_loop_begin_edit->set_property(&loop_begin_prop);
	
	label_loop_end_edit->set_property(&loop_end_prop);
	updown_loop_end_edit->set_property(&loop_end_prop);
	
	label_susloop_begin_edit->set_property(&susloop_begin_prop);
	updown_susloop_begin_edit->set_property(&susloop_begin_prop);
	
	label_susloop_end_edit->set_property(&susloop_end_prop);
	updown_susloop_end_edit->set_property(&susloop_end_prop);
	
	update_controls();
	
}

void EnvelopeParamsEditor::add_label(QString p_text,QColor p_color,QWidget *p_parent) {


	QLabel *l = new QLabel(p_text,p_parent);
	QPalette p = l->palette();
	p.setColor(QPalette::Text,p_color);
//	p.setColor(QPalette::WindowText,p_color);
	l->setPalette(p);
	QFont f;
	f.setPixelSize(13);
	f.setBold(true);
	l->setFont(f);
	
}

EnvelopeParamsEditor::EnvelopeParamsEditor(QWidget *p_parent,Skin& p_skin) : CHBox(p_parent) {
	
	loop_begin_prop.set_set_func( &Envelope::set_loop_begin );
	loop_begin_prop.set_get_func( &Envelope::get_loop_begin );
	loop_begin_prop.set_max_func( &Envelope::get_loop_end );
	loop_begin_prop.set_min(0);
	loop_begin_prop.config("loop_begin","Loop Begin",1,0);
	
	loop_end_prop.set_set_func( &Envelope::set_loop_end );
	loop_end_prop.set_get_func( &Envelope::get_loop_end );
	loop_end_prop.set_min_func( &Envelope::get_loop_begin );
	loop_end_prop.set_max_func( &Envelope::get_node_count ); //not really max but works!
	loop_end_prop.config("loop_end","Loop End",1,0);
	
	susloop_begin_prop.set_set_func( &Envelope::set_sustain_loop_begin );
	susloop_begin_prop.set_get_func( &Envelope::get_sustain_loop_begin );
	susloop_begin_prop.set_max_func( &Envelope::get_sustain_loop_end );
	susloop_begin_prop.set_min(0);
	susloop_begin_prop.config("susloop_begin","Loop Begin",1,0);
	
	susloop_end_prop.set_set_func( &Envelope::set_sustain_loop_end );
	susloop_end_prop.set_get_func( &Envelope::get_sustain_loop_end );
	susloop_end_prop.set_min_func( &Envelope::get_sustain_loop_begin );
	susloop_end_prop.set_max_func( &Envelope::get_node_count ); //not really max but works!
	susloop_end_prop.config("susloop_end","Loop End",1,0);
	
	CVBox *switches_vbox = new CVBox(this);
	
	envelope_enabled = new PixmapButton(switches_vbox,p_skin.enabled_button,PixmapButton::TYPE_TOGGLE);
	
	(new QFrame(switches_vbox))->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
	
	envelope_cubic = new PixmapButton(switches_vbox,p_skin.cubic_button,PixmapButton::TYPE_TOGGLE);
	
	CVBox *loop_vbox = new CVBox(this);
	
	loop_vbox->layout()->setSpacing(3);
	
	loop_enabled = new PixmapButton(loop_vbox,p_skin.loop_button,PixmapButton::TYPE_TOGGLE);
	
	
	CHBox *spin_hb = new CHBox(loop_vbox);
	add_label("begin: ",p_skin.label_col,spin_hb);

	label_loop_begin_edit = new PropertyEditLabel(spin_hb,p_skin.spin_label);
	updown_loop_begin_edit = new PropertyEditUpDown(spin_hb,p_skin.spin_updown);
	updown_loop_begin_edit->add_to_group( label_loop_begin_edit );
	
	
	spin_hb = new CHBox(loop_vbox);
	add_label("end: ",p_skin.label_col,spin_hb);

	label_loop_end_edit = new PropertyEditLabel(spin_hb,p_skin.spin_label);
	updown_loop_end_edit = new PropertyEditUpDown(spin_hb,p_skin.spin_updown);
	updown_loop_end_edit->add_to_group( label_loop_end_edit );
	
	
	CVBox *susloop_vbox = new CVBox(this);
	
	susloop_vbox->layout()->setSpacing(3);
	
	susloop_enabled = new PixmapButton(susloop_vbox,p_skin.susloop_button,PixmapButton::TYPE_TOGGLE);
	
	spin_hb = new CHBox(susloop_vbox);
	add_label("begin: ",p_skin.label_col,spin_hb);
	
	label_susloop_begin_edit = new PropertyEditLabel(spin_hb,p_skin.spin_label);
	updown_susloop_begin_edit = new PropertyEditUpDown(spin_hb,p_skin.spin_updown);
	updown_susloop_begin_edit->add_to_group( label_susloop_begin_edit );
	
	spin_hb = new CHBox(susloop_vbox);
	add_label("end: ",p_skin.label_col,spin_hb);
	
	label_susloop_end_edit = new PropertyEditLabel(spin_hb,p_skin.spin_label);
	updown_susloop_end_edit = new PropertyEditUpDown(spin_hb,p_skin.spin_updown);
	updown_susloop_end_edit->add_to_group( label_susloop_end_edit );
	
	layout()->setMargin(3);
	layout()->setSpacing(3);
	
	envelope=NULL;
	
	/* notification */
	
	label_loop_begin_edit->set_changed_by_editor_callback(this,&EnvelopeParamsEditor::changed_by_editor);

	updown_loop_begin_edit->set_changed_by_editor_callback(this,&EnvelopeParamsEditor::changed_by_editor);
	
	label_loop_end_edit->set_changed_by_editor_callback(this,&EnvelopeParamsEditor::changed_by_editor);
	updown_loop_end_edit->set_changed_by_editor_callback(this,&EnvelopeParamsEditor::changed_by_editor);
	
	label_susloop_begin_edit->set_changed_by_editor_callback(this,&EnvelopeParamsEditor::changed_by_editor);
	updown_susloop_begin_edit->set_changed_by_editor_callback(this,&EnvelopeParamsEditor::changed_by_editor);
	
	label_susloop_end_edit->set_changed_by_editor_callback(this,&EnvelopeParamsEditor::changed_by_editor);
	updown_susloop_end_edit->set_changed_by_editor_callback(this,&EnvelopeParamsEditor::changed_by_editor);
	
	QObject::connect(envelope_enabled,SIGNAL(mouse_toggled_signal( bool )),this,SLOT(set_enabled( bool )));
	QObject::connect(envelope_cubic,SIGNAL(mouse_toggled_signal( bool )),this,SLOT(set_cubic(bool)));
	QObject::connect(loop_enabled,SIGNAL(mouse_toggled_signal( bool )),this,SLOT( set_loop( bool )));
	QObject::connect(susloop_enabled,SIGNAL(mouse_toggled_signal( bool )),this,SLOT(set_susloop( bool )));
	

}


EnvelopeParamsEditor::~EnvelopeParamsEditor() {
}


}
