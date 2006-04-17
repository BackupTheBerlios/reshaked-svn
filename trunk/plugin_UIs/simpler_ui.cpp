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

#include "pixmaps/effect_panel_generic_top_left.xpm"
#include "pixmaps/effect_panel_generic_top_right.xpm"
#include "pixmaps/effect_panel_generic_top.xpm"
#include "pixmaps/effect_panel_generic_left.xpm"
#include "pixmaps/effect_panel_generic_right.xpm"
#include "pixmaps/effect_panel_generic_bottom_left.xpm"
#include "pixmaps/effect_panel_generic_bottom_right.xpm"
#include "pixmaps/effect_panel_generic_bottom.xpm"

#include "plugin_UIs/pixmaps/simpler__bottom.xpm"
#include "plugin_UIs/pixmaps/simpler__display_bottom.xpm"
#include "plugin_UIs/pixmaps/simpler__display_left.xpm"
#include "plugin_UIs/pixmaps/simpler__display_right.xpm"
#include "plugin_UIs/pixmaps/simpler__edit_pushed.xpm"
#include "plugin_UIs/pixmaps/simpler__edit.xpm"
#include "plugin_UIs/pixmaps/simpler__knob_pos.xpm"
#include "plugin_UIs/pixmaps/simpler__knob.xpm"
#include "plugin_UIs/pixmaps/simpler__knob_separator.xpm"
#include "plugin_UIs/pixmaps/simpler__left.xpm"
#include "plugin_UIs/pixmaps/simpler__load_pushed.xpm"
#include "plugin_UIs/pixmaps/simpler__load.xpm"
#include "plugin_UIs/pixmaps/simpler__right.xpm"
#include "plugin_UIs/pixmaps/simpler__top_left.xpm"
#include "plugin_UIs/pixmaps/simpler__top_right.xpm"
#include "dsp/sample_file.h"
#include "ui_blocks/sample_editor.h"
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
	
	setLayout(new QVBoxLayout(this));
	CHBox *hb_top = new CHBox(this);
	layout()->addWidget(hb_top);
	new PixmapLabel(hb_top,QPixmap((const char**)effect_panel_generic_top_left_xpm));
	PixmapLabel *title = new PixmapLabel(hb_top,QPixmap((const char**)effect_panel_generic_top_xpm) ,PixmapLabel::EXPAND_TILE_H);
	title->set_text( QStrify(p_plugin->get_caption())) ;
	title->setToolTip( QStrify(p_plugin->get_caption())) ;
	new PixmapLabel(hb_top,QPixmap((const char**)effect_panel_generic_top_right_xpm));
	
	
	CHBox *hb = new CHBox(this);
	layout()->addWidget(hb);
	
	PixmapLabel *label_name = new PixmapLabel(hb,QPixmap((const char**)effect_panel_generic_left_xpm));
	label_name->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	
	/*****/
	
	CVBox *vb_simpler = new CVBox(hb);
	
	CHBox *top_hb_simpler = new CHBox(vb_simpler);
	
	new PixmapLabel(top_hb_simpler,QPixmap( (const char**)simpler__top_left_xpm ) );
	
	PixmapButton *load_button = new PixmapButton(top_hb_simpler,PixmapButton::Skin( QPixmap( (const char**)simpler__load_xpm ),QPixmap( (const char**)simpler__load_pushed_xpm )));
	QObject::connect( load_button, SIGNAL( mouse_pressed_signal() ), this, SLOT(load_sample_slot()));
	
	PixmapButton *edit_button = new PixmapButton(top_hb_simpler,PixmapButton::Skin( QPixmap( (const char**)simpler__edit_xpm ),QPixmap( (const char**)simpler__edit_pushed_xpm )));
	QObject::connect( edit_button, SIGNAL( mouse_pressed_signal() ), this, SLOT(edit_sample_slot()));
	
	new PixmapLabel(top_hb_simpler,QPixmap( (const char**)simpler__top_right_xpm ) );
	
	CHBox *mid_hb_simpler = new CHBox(vb_simpler);
	
	new PixmapLabel(mid_hb_simpler,QPixmap( (const char**)simpler__left_xpm ) );
	
	CVBox *center_vb_simpler = new CVBox(mid_hb_simpler);
	
	CHBox *screen_hb = new CHBox(center_vb_simpler);
	
	// screen
	
	new PixmapLabel(screen_hb,QPixmap( (const char**)simpler__display_left_xpm ) );
	
	viewer = new SampleViewer(screen_hb);
	if (simpler->get_sample().get_length())
		viewer->set_sample_data( &simpler->get_sample() );
	else
		viewer->set_sample_data( NULL );

	new PixmapLabel(screen_hb,QPixmap( (const char**)simpler__display_right_xpm ) );
	
	// - screen
	
	new PixmapLabel(center_vb_simpler,QPixmap( (const char**)simpler__display_bottom_xpm ) );
	
	CHBox *adsr_hb = new CHBox(center_vb_simpler);
	
	PropertyEditKnob * k;
	Knob::Skin knob_skin;
	knob_skin.angle_begin=30;
	knob_skin.base=QPixmap( (const char**)simpler__knob_xpm );
	knob_skin.handle=QPixmap( (const char**)simpler__knob_pos_xpm );
	knob_skin.handle_at_distance=10;
	
	k = new PropertyEditKnob(adsr_hb,knob_skin);
	k->set_property( &simpler->get_port_by_name( "adsr_attack" ) );
	register_property_editor( k );
	
	new PixmapLabel(adsr_hb,QPixmap( (const char**)simpler__knob_separator_xpm ) );
	
	k = new PropertyEditKnob(adsr_hb,knob_skin);
	k->set_property( &simpler->get_port_by_name( "adsr_decay" ) );
	register_property_editor( k );
	
	new PixmapLabel(adsr_hb,QPixmap( (const char**)simpler__knob_separator_xpm ) );
	
	k = new PropertyEditKnob(adsr_hb,knob_skin);
	k->set_property( &simpler->get_port_by_name( "adsr_sustain" ) );
	register_property_editor( k );
	
	new PixmapLabel(adsr_hb,QPixmap( (const char**)simpler__knob_separator_xpm ) );
	
	k = new PropertyEditKnob(adsr_hb,knob_skin);
	k->set_property( &simpler->get_port_by_name( "adsr_release" ) );
	register_property_editor( k );
	
	new PixmapLabel(mid_hb_simpler,QPixmap( (const char**)simpler__right_xpm ) );
	
	new PixmapLabel(vb_simpler,QPixmap( (const char**)simpler__bottom_xpm ) );
	
	/* ***/
	new PixmapLabel(hb,QPixmap((const char**)effect_panel_generic_right_xpm));
	
	CHBox *hb_bottom = new CHBox(this);
	layout()->addWidget(hb_bottom);
	new PixmapLabel(hb_bottom,QPixmap((const char**)effect_panel_generic_bottom_left_xpm));
	new PixmapLabel(hb_bottom,QPixmap((const char**)effect_panel_generic_bottom_xpm),PixmapLabel::EXPAND_TILE_H);
	new PixmapLabel(hb_bottom,QPixmap((const char**)effect_panel_generic_bottom_right_xpm));
	
	
	layout()->setMargin(0);
	layout()->setSpacing(0);
	
	hb->layout()->setMargin(0);
	hb->layout()->setSpacing(0);	
	
	setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


SimplerUI::~SimplerUI()
{
}


}
