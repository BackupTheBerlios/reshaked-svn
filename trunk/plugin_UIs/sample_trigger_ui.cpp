//
// C++ Implementation: sample_trigger_ui
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sample_trigger_ui.h"

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

SoundPluginUI* SampleTriggerUI::create_this(SoundPlugin *p_plugin,QWidget *p_parent) {
	
	SampleTrigger *sample_trigger = dynamic_cast<SampleTrigger*>(p_plugin);
	if (sample_trigger)
		return new SampleTriggerUI(p_parent,sample_trigger);
	else
		return NULL;
	
}


void SampleTriggerUI::load_sample_slot() {
	
	static QString sample_dir=".";
	
	QString file=QFileDialog::getOpenFileName ( this, "Open Sample",sample_dir, "Waveforms (*.wav *.au *.aif *.WAV);;All Files (*)");
			
	if (file=="")
		return;

	sample_dir=get_dir_from_path(file);
	
	AudioControl::mutex_lock();
			
	if (SampleFile::get_singleton()->load_sample( DeQStrify(file),&sample_trigger->get_sample() )) {
		
		AudioControl::mutex_unlock();
		return;
	}
	
	AudioControl::mutex_unlock();
	
	QString preset_name=get_file_from_path(file);
	
	if (preset_name.lastIndexOf(".")!=-1) {
			
		preset_name=preset_name.left(preset_name.lastIndexOf("."));
	}
	
	sample_trigger->set_current_preset_name( DeQStrify(preset_name) );
	
	viewer->set_sample_data( &sample_trigger->get_sample() );	
	
	viewer->update();
	
	preset_name_changed_signal();
	
}
void SampleTriggerUI::edit_sample_slot() {
	
			
	if (sample_trigger->get_sample().get_length()==0)
		return;
	
	SampleEditorDialog *sed = new SampleEditorDialog(this,&sample_trigger->get_sample());
	sed->exec();
	delete sed;

			
	
	
}


SampleTriggerUI::SampleTriggerUI(QWidget *p_parent,SampleTrigger *p_plugin) :SoundPluginUI(p_parent,p_plugin) {
	
	
	sample_trigger = p_plugin;

	
	CHBox *hb = generate_default_layout();
	
	/*****/
	
	CVBox *vb_sample_trigger = new CVBox(hb);
	
	CHBox *top_hb_sample_trigger = new CHBox(vb_sample_trigger);
	
	new PixmapLabel(top_hb_sample_trigger,GET_QPIXMAP(THEME_SIMPLER__TOP_LEFT ) );
	
	PixmapButton *load_button = new PixmapButton(top_hb_sample_trigger,PixmapButton::Skin( GET_QPIXMAP(THEME_SIMPLER__LOAD ),GET_QPIXMAP(THEME_SIMPLER__LOAD_PUSHED )));
	QObject::connect( load_button, SIGNAL( mouse_pressed_signal() ), this, SLOT(load_sample_slot()));
	
	PixmapButton *edit_button = new PixmapButton(top_hb_sample_trigger,PixmapButton::Skin( GET_QPIXMAP(THEME_SIMPLER__EDIT ),GET_QPIXMAP(THEME_SIMPLER__EDIT_PUSHED )));
	QObject::connect( edit_button, SIGNAL( mouse_pressed_signal() ), this, SLOT(edit_sample_slot()));
	
	new PixmapLabel(top_hb_sample_trigger,GET_QPIXMAP(THEME_SIMPLER__TOP_RIGHT ) );
	
	CHBox *mid_hb_sample_trigger = new CHBox(vb_sample_trigger);
	
	new PixmapLabel(mid_hb_sample_trigger,GET_QPIXMAP(THEME_SIMPLER__LEFT ) );
	
	CVBox *center_vb_sample_trigger = new CVBox(mid_hb_sample_trigger);
	
	CHBox *screen_hb = new CHBox(center_vb_sample_trigger);
	
	// screen
	
	new PixmapLabel(screen_hb,GET_QPIXMAP(THEME_SIMPLER__DISPLAY_LEFT ) );
	
	viewer = new SampleViewer(screen_hb);
	if (sample_trigger->get_sample().get_length())
		viewer->set_sample_data( &sample_trigger->get_sample() );
	else
		viewer->set_sample_data( NULL );

	new PixmapLabel(screen_hb,GET_QPIXMAP(THEME_SIMPLER__DISPLAY_RIGHT ) );
	
	// - screen
	
	new PixmapLabel(center_vb_sample_trigger,GET_QPIXMAP(THEME_SIMPLER__DISPLAY_BOTTOM ) );
	
	CHBox *adsr_hb = new CHBox(center_vb_sample_trigger);
	
	PropertyEditKnob * k;
	Knob::Skin knob_skin;
	knob_skin.angle_begin=30;
	knob_skin.base=GET_QPIXMAP(THEME_SIMPLER__KNOB );
	knob_skin.handle=GET_QPIXMAP(THEME_SIMPLER__KNOB_POS );
	knob_skin.handle_at_distance=10;
	
	new PixmapLabel(adsr_hb,GET_QPIXMAP(THEME_SAMPLE_TRIGGER__CONTROLS_BEGIN ) );
	
	CVBox *note_vb = new CVBox(adsr_hb);
	
	new PixmapLabel(note_vb,GET_QPIXMAP(THEME_SAMPLE_TRIGGER__NOTE_TOP ) );
	
	CHBox *note_hb = new CHBox( note_vb );
	
	PropertyEditLabel *pe = new PropertyEditLabel( note_hb,GET_QPIXMAP(THEME_SAMPLE_TRIGGER__NOTE_DISPLAY ) );
	pe->set_property( &sample_trigger->get_port_by_name( "note" ) );
	register_property_editor( pe );
	
	PropertyEditUpDown *peud = new PropertyEditUpDown( note_hb,PixmapUpDown::Skin(GET_QPIXMAP(THEME_SAMPLE_TRIGGER__NOTE_UPDOWN )) );
	peud->set_property( &sample_trigger->get_port_by_name( "note" ) );
	register_property_editor( peud );
	peud->add_to_group( pe );

	
	new PixmapLabel(note_vb,GET_QPIXMAP(THEME_SAMPLE_TRIGGER__NOTE_BOTTOM ) );
	
	if (sample_trigger->get_channels_created()==2) {
		
		k = new PropertyEditKnob(adsr_hb,knob_skin);
		k->set_property( &sample_trigger->get_port_by_name( "pan" ) );
		register_property_editor( k );
	}
	
	new PixmapLabel(adsr_hb,GET_QPIXMAP(THEME_SIMPLER__KNOB_SEPARATOR ) );
	
	k = new PropertyEditKnob(adsr_hb,knob_skin);
	k->set_property( &sample_trigger->get_port_by_name( "release" ) );
	register_property_editor( k );
	
	new PixmapLabel(mid_hb_sample_trigger,GET_QPIXMAP(THEME_SIMPLER__RIGHT ) );
	
	new PixmapLabel(vb_sample_trigger,GET_QPIXMAP(THEME_SAMPLE_TRIGGER__BOTTOM ) );
	
	create_editor_for_property( "global_gain", hb );
	
	/****/
}


SampleTriggerUI::~SampleTriggerUI() {
	
}


}
