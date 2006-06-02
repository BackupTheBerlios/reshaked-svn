//
// C++ Implementation: simpler_ui
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "simpler_ui.h"

#include <Qt/qpushbutton.h>
#include <Qt/qlayout.h>
#include <Qt/qfiledialog.h>
#include "ui_blocks/helpers.h"
#include "ui_blocks/pixmap_label.h"
#include "ui_blocks/pixmap_slider.h"
#include "ui_blocks/property_editors.h"
#include "ui_blocks/pixmap_button.h"

#include "dsp/sample_file.h"
#include "ui_blocks/sample_editor.h"
#include "interface/visual_settings.h"
#include "engine/audio_control.h"

namespace ReShaked {

SoundPluginUI* SimplerUI::create_this(SoundPlugin *p_plugin,QWidget *p_parent) {
	
	if (dynamic_cast<Simpler*>(p_plugin))
		return new SimplerUI(p_parent,p_plugin);
	else
		return NULL;
	
}


void SimplerUI::load_sample_slot() {
	
	
	QString file=QFileDialog::getOpenFileName ( this, "Open Sample",".", "Waveforms (*.wav *.au *.aif *.WAV);;All Files (*)");
			
	if (file=="")
		return;

	
	AudioControl::mutex_lock();
			
	if (SampleFile::get_singleton()->load_sample( DeQStrify(file),&simpler->get_sample() )) {
		
		AudioControl::mutex_unlock();
		return;
	}
	
	AudioControl::mutex_unlock();
	
	viewer->set_sample_data( &simpler->get_sample() );	
	
	viewer->update();
	
}
void SimplerUI::edit_sample_slot() {
	
			
	if (simpler->get_sample().get_length()==0)
		return;
	
	SampleEditorDialog *sed = new SampleEditorDialog(this,&simpler->get_sample());
	sed->exec();
	delete sed;

			
	
	
}


SimplerUI::SimplerUI(QWidget *p_parent,SoundPlugin *p_plugin) :SoundPluginUI(p_parent,p_plugin) {
	
	
	simpler = dynamic_cast<Simpler*>(p_plugin);
	ERR_FAIL_COND(!simpler);
	
	CHBox *hb = generate_default_layout();
	
	/*****/
	
	CVBox *vb_simpler = new CVBox(hb);
	
	CHBox *top_hb_simpler = new CHBox(vb_simpler);
	
	new PixmapLabel(top_hb_simpler,GET_QPIXMAP(THEME_SIMPLER__TOP_LEFT ) );
	
	PixmapButton *load_button = new PixmapButton(top_hb_simpler,PixmapButton::Skin( GET_QPIXMAP(THEME_SIMPLER__LOAD ),GET_QPIXMAP(THEME_SIMPLER__LOAD_PUSHED )));
	QObject::connect( load_button, SIGNAL( mouse_pressed_signal() ), this, SLOT(load_sample_slot()));
	
	PixmapButton *edit_button = new PixmapButton(top_hb_simpler,PixmapButton::Skin( GET_QPIXMAP(THEME_SIMPLER__EDIT ),GET_QPIXMAP(THEME_SIMPLER__EDIT_PUSHED )));
	QObject::connect( edit_button, SIGNAL( mouse_pressed_signal() ), this, SLOT(edit_sample_slot()));
	
	new PixmapLabel(top_hb_simpler,GET_QPIXMAP(THEME_SIMPLER__TOP_RIGHT ) );
	
	CHBox *mid_hb_simpler = new CHBox(vb_simpler);
	
	new PixmapLabel(mid_hb_simpler,GET_QPIXMAP(THEME_SIMPLER__LEFT ) );
	
	CVBox *center_vb_simpler = new CVBox(mid_hb_simpler);
	
	CHBox *screen_hb = new CHBox(center_vb_simpler);
	
	// screen
	
	new PixmapLabel(screen_hb,GET_QPIXMAP(THEME_SIMPLER__DISPLAY_LEFT ) );
	
	viewer = new SampleViewer(screen_hb);
	if (simpler->get_sample().get_length())
		viewer->set_sample_data( &simpler->get_sample() );
	else
		viewer->set_sample_data( NULL );

	new PixmapLabel(screen_hb,GET_QPIXMAP(THEME_SIMPLER__DISPLAY_RIGHT ) );
	
	// - screen
	
	new PixmapLabel(center_vb_simpler,GET_QPIXMAP(THEME_SIMPLER__DISPLAY_BOTTOM ) );
	
	CHBox *adsr_hb = new CHBox(center_vb_simpler);
	
	PropertyEditKnob * k;
	Knob::Skin knob_skin;
	knob_skin.angle_begin=30;
	knob_skin.base=GET_QPIXMAP(THEME_SIMPLER__KNOB );
	knob_skin.handle=GET_QPIXMAP(THEME_SIMPLER__KNOB_POS );
	knob_skin.handle_at_distance=10;
	
	k = new PropertyEditKnob(adsr_hb,knob_skin);
	k->set_property( &simpler->get_port_by_name( "adsr_attack" ) );
	register_property_editor( k );
	
	new PixmapLabel(adsr_hb,GET_QPIXMAP(THEME_SIMPLER__KNOB_SEPARATOR ) );
	
	k = new PropertyEditKnob(adsr_hb,knob_skin);
	k->set_property( &simpler->get_port_by_name( "adsr_decay" ) );
	register_property_editor( k );
	
	new PixmapLabel(adsr_hb,GET_QPIXMAP(THEME_SIMPLER__KNOB_SEPARATOR ) );
	
	k = new PropertyEditKnob(adsr_hb,knob_skin);
	k->set_property( &simpler->get_port_by_name( "adsr_sustain" ) );
	register_property_editor( k );
	
	new PixmapLabel(adsr_hb,GET_QPIXMAP(THEME_SIMPLER__KNOB_SEPARATOR ) );
	
	k = new PropertyEditKnob(adsr_hb,knob_skin);
	k->set_property( &simpler->get_port_by_name( "adsr_release" ) );
	register_property_editor( k );
	
	new PixmapLabel(mid_hb_simpler,GET_QPIXMAP(THEME_SIMPLER__RIGHT ) );
	
	new PixmapLabel(vb_simpler,GET_QPIXMAP(THEME_SIMPLER__BOTTOM ) );
	
	create_editor_for_property( "global_gain", hb );
	/* ***/
}


SimplerUI::~SimplerUI()
{
}


}
