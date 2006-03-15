//
// C++ Implementation: chionic_window
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "chionic_window.h"
#include "ui_blocks/helpers.h"
#include "plugin_UIs/chionic_pixmaps/chionic_bg.xpm"
#include <Qt/qpainter.h>
#include <Qt/qpushbutton.h>
#include <Qt/qlabel.h>
#include "ui_blocks/pixmap_label.h"


#include "plugin_UIs/chionic_pixmaps/frame__bottom_left.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__bottom_right.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__bottom.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__left.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__right.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__section_begin.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__section_end.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__section_envlfo_active.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__section_envlfo.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__section_global_active.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__section_global.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__section_params_active.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__section_params.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__section_regions_active.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__section_regions.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__section_source_active.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__section_source.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__top_left.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__top_right.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__top_separator.xpm"

#include "plugin_UIs/chionic_pixmaps/control_frame__bottom_left.xpm"
#include "plugin_UIs/chionic_pixmaps/control_frame__bottom_right.xpm"
#include "plugin_UIs/chionic_pixmaps/control_frame__bottom.xpm"
#include "plugin_UIs/chionic_pixmaps/control_frame__left.xpm"
#include "plugin_UIs/chionic_pixmaps/control_frame__right.xpm"
#include "plugin_UIs/chionic_pixmaps/control_frame__top_left.xpm"
#include "plugin_UIs/chionic_pixmaps/control_frame__top_right.xpm"
#include "plugin_UIs/chionic_pixmaps/control_frame__top.xpm"

#include "plugin_UIs/chionic_pixmaps/controls__display.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__slider.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__slider_grabber.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__key_scaling_bg.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__range_begin.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__range_end.xpm"

#include "plugin_UIs/chionic_pixmaps/controls__stereo_panner.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__stereo_panner_pos.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__quad_panner.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__quad_panner_pos.xpm"

#include "plugin_UIs/chionic_pixmaps/controls__filter_display.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__filter_bandpass_active.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__filter_bandpass.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__filter_hipass_active.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__filter_hipass.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__filter_lowpass_active.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__filter_lowpass.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__filter_notch_active.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__filter_notch.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__filter_off_active.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__filter_off.xpm"

#include "plugin_UIs/chionic_pixmaps/controls__spinbox_label.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__spinbox_spin.xpm"

namespace ReShaked {


void ChionicWindow::main_page_select(int p_page) {
	
	
	switch (p_page) {
		
		case SECTION_SOURCE: {
			
			
		} break;
		case SECTION_GLOBAL: {
			
			
		} break;
		case SECTION_REGIONS: {
			
			
		} break;
		case SECTION_PARAMS: {
			
			main_stack->setCurrentWidget(params.main_hbox);
		} break;
		case SECTION_ENVLFO: {
			
			
		} break;
		
	}
}

void ChionicWindow::filter_mode_select(int p_mode) {
	
	ERR_FAIL_INDEX(layers.selected,ChionicParams::MAX_LAYERS);
	
	ChionicParams::Layer::Parameters &p=chionic->get_params()->layer[layers.selected].params;
	printf("mode for %i\n",p_mode);
	p.filter.type.set(p_mode);
	params.filter_editor->update();
	
}
void ChionicWindow::init_params_page() {
	
	ChionicParams::Layer::Parameters &p=chionic->get_params()->layer[0].params;
	
	
	params.main_vbox = new CVBox(main_stack);
	main_stack->addWidget( params.main_vbox );
	params.main_vbox->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	
	
	params.main_hbox = new CHBox(params.main_vbox);
	
	CVBox *left_vbox = new CVBox(params.main_hbox);
	left_vbox->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	add_main_label("Volume",left_vbox);
	
	begin_control_frame(left_vbox);	
			
	params.main_hbox->layout()->setAlignment(left_vbox,Qt::AlignTop);
	CVBox *controls_vb = new CVBox( control_frame_current() );
	
	
	params.vol_send=add_slider_edit_control( controls_vb, &p.volume.send,&params.vol_send_disp );
	
	
	add_control_label("Vel/Exp Range",controls_vb);
	params.vol_range = new RangeLevelEditor(controls_vb,settings.range_level_skin);
	params.vol_range->set_properties(&p.volume.velocity_range_begin_level,&p.volume.velocity_range_end_level,&p.volume.velocity_range_begin,&p.volume.velocity_range_end);
			
	add_control_label( QStrify(p.volume.pitch_scale.get_caption()), controls_vb );
	params.vol_pitch_scale=new KeyScalingEditor(controls_vb,settings.key_scaling_skin);
	params.vol_pitch_scale->set_property(&p.volume.pitch_scale);
			
	params.vol_random=add_slider_edit_control( controls_vb, &p.volume.random,&params.vol_random_disp );
	
	end_control_frame();
	
	add_main_label("Pan",left_vbox);

	begin_control_frame(left_vbox);	
	controls_vb = new CVBox( control_frame_current() );
	
	if (chionic->get_channels_created()==4) {
		
		CHBox *panner_hb = new CHBox(controls_vb);
		add_control_label("Pos ",panner_hb);
		params.pan_panner = new PannerEditor( panner_hb, true, settings.panner_skin );
		params.pan_panner->set_properties(&p.pan.pos,&p.pan.depth);
		
	} else {
		
		add_control_label("Position",controls_vb);
		params.pan_panner = new PannerEditor( controls_vb,false,settings.panner_skin );
		params.pan_panner->set_properties(&p.pan.pos,&p.pan.depth);
		
	}
			
	
	
	params.pan_random=add_slider_edit_control( controls_vb, &p.pan.random,&params.pan_random_disp );
	
	end_control_frame();
	
	/* separate ^^ */
	(new QFrame(params.main_hbox))->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	CVBox *right_vbox = new CVBox(params.main_hbox);
	right_vbox->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	params.main_hbox->layout()->setAlignment(right_vbox,Qt::AlignTop);
	
	add_main_label("Filter",right_vbox);
	
	begin_control_frame(right_vbox);	
			
	controls_vb = new CVBox( control_frame_current() );
	
	CHBox *filter_mode_hb = new CHBox(controls_vb);
	
	params.filter_mode_group = new PixmapButtonGroup(this);
	QObject::connect( params.filter_mode_group, SIGNAL(button_selected_signal( int )), this, SLOT(filter_mode_select( int )));
	
	params.filter_mode_off = new PixmapButton( filter_mode_hb, PixmapButton::Skin( QPixmap( (const char**)controls__filter_off_xpm), QPixmap( (const char**)controls__filter_off_active_xpm) ), PixmapButton::TYPE_TOGGLE );
	params.filter_mode_group->add_button( params.filter_mode_off );
		
	params.filter_mode_lp = new PixmapButton( filter_mode_hb, PixmapButton::Skin( QPixmap( (const char**)controls__filter_lowpass_xpm), QPixmap( (const char**)controls__filter_lowpass_active_xpm) ), PixmapButton::TYPE_TOGGLE );
	params.filter_mode_group->add_button( params.filter_mode_lp );
	
	params.filter_mode_bp = new PixmapButton( filter_mode_hb, PixmapButton::Skin( QPixmap( (const char**)controls__filter_bandpass_xpm), QPixmap( (const char**)controls__filter_bandpass_active_xpm) ), PixmapButton::TYPE_TOGGLE );
	params.filter_mode_group->add_button( params.filter_mode_bp );
	
	params.filter_mode_hp = new PixmapButton( filter_mode_hb, PixmapButton::Skin( QPixmap( (const char**)controls__filter_hipass_xpm), QPixmap( (const char**)controls__filter_hipass_active_xpm) ), PixmapButton::TYPE_TOGGLE );
	params.filter_mode_group->add_button( params.filter_mode_hp );
	
	params.filter_mode_br = new PixmapButton( filter_mode_hb, PixmapButton::Skin( QPixmap( (const char**)controls__filter_notch_xpm), QPixmap( (const char**)controls__filter_notch_active_xpm) ), PixmapButton::TYPE_TOGGLE );
	params.filter_mode_group->add_button( params.filter_mode_br );
	
	params.filter_editor = new FilterEditor(controls_vb, settings.filter_editor_skin );
	params.filter_editor->set_properties( &p.filter.type, &p.filter.cutoff, &p.filter.resonance );
	
	params.filter_velsens_cut=add_slider_edit_control( controls_vb, &p.filter.cutoff_velsens,&params.filter_velsens_cut_disp );
	
	params.filter_velsens_reso=add_slider_edit_control( controls_vb, &p.filter.resonance_velsens,&params.filter_velsens_reso_disp );
	
	params.filter_freqtrack=add_slider_edit_control( controls_vb, &p.filter.freq_tracking,NULL );
	
	params.filter_random=add_slider_edit_control( controls_vb, &p.filter.random,NULL );
	
	end_control_frame();
	
	add_main_label("Time",right_vbox);
	
	begin_control_frame(right_vbox);	
			
	controls_vb = new CVBox( control_frame_current() );
	
	params.time_delay=add_slider_edit_control( controls_vb, &p.time.delay,&params.time_delay_disp );
	params.time_random=add_slider_edit_control( controls_vb, &p.time.random,&params.time_random_disp );
	
	end_control_frame();
	
	add_main_label("Tune",params.main_vbox);
	begin_control_frame(params.main_vbox);	
	
	CHBox *controls_hb = new CHBox( control_frame_current() );
	params.tune_fine=add_slider_edit_control( controls_hb, &p.pitch.tune_fine,&params.tune_fine_disp );
	/* separate ^^ */
	(new QFrame(controls_hb))->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	params.tune_coarse=add_spin_edit_control(controls_hb,&p.pitch.tune_coarse,&params.tune_coarse_updown);
	
	end_control_frame();
	
	
}

PropertyEditor* ChionicWindow::add_slider_edit_control(QWidget *p_base,Property *p_prop,PropertyEditor** p_with_editbox) {
	
	CVBox *vb = new CVBox(p_base);
	
	vb->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	if (p_with_editbox) {
		
		CHBox *hb = new CHBox(vb);
		add_control_label( QStrify( p_prop->get_caption()), hb );
		*p_with_editbox = new PropertyEditLabel(hb,settings.controls_display);
		(*p_with_editbox)->set_property(p_prop);
		
	} else {
		
		add_control_label( QStrify( p_prop->get_caption()), vb );
		
	}
	
	PropertyEditor *spin = new PropertyEditSlider(vb,settings.controls_slider_skin,PixmapSlider::TYPE_HORIZONTAL,1,1);
	if (p_with_editbox)
		(*p_with_editbox)->add_to_group(spin);
	
	spin->set_property(p_prop);
	return spin;
}

PropertyEditor* ChionicWindow::add_spin_edit_control(QWidget *p_base,Property *p_prop,PropertyEditor** p_updown) {
	
	CVBox *vb = new CVBox(p_base);
	vb->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	
	add_control_label( QStrify( p_prop->get_caption()), vb );
	
	CHBox *hb = new CHBox(vb);
	
	PropertyEditor *label = new PropertyEditLabel(hb,settings.spin_bg);
	label->set_property( p_prop );
	
	
	*p_updown= new PropertyEditUpDown(hb,settings.updown_skin);
	(*p_updown)->add_to_group(label);
	
	(*p_updown)->set_property( p_prop );
	
	
	return label;
	
}

void ChionicWindow::begin_control_frame(QWidget *p_parent) {
	
	settings.current_frame_vb = new CVBox(p_parent);
	settings.current_frame_vb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	CHBox *aux_hb = new CHBox( settings.current_frame_vb );
	
	new PixmapLabel(aux_hb,QPixmap((const char**)control_frame__top_left_xpm));
	new PixmapLabel(aux_hb,QPixmap((const char**)control_frame__top_xpm),PixmapLabel::EXPAND_TILE_H);
	new PixmapLabel(aux_hb,QPixmap((const char**)control_frame__top_right_xpm));
	
	settings.current_frame_hb = new CHBox(settings.current_frame_vb);
	new PixmapLabel(settings.current_frame_hb,QPixmap((const char**)control_frame__left_xpm),PixmapLabel::EXPAND_TILE_V);
	
	
}
void ChionicWindow::end_control_frame() {
	
	new PixmapLabel(settings.current_frame_hb,QPixmap((const char**)control_frame__right_xpm),PixmapLabel::EXPAND_TILE_V);
	
	CHBox *aux_hb = new CHBox( settings.current_frame_vb );
	
	new PixmapLabel(aux_hb,QPixmap((const char**)control_frame__bottom_left_xpm));
	new PixmapLabel(aux_hb,QPixmap((const char**)control_frame__bottom_xpm),PixmapLabel::EXPAND_TILE_H);
	new PixmapLabel(aux_hb,QPixmap((const char**)control_frame__bottom_right_xpm));
}

QWidget* ChionicWindow::control_frame_current() {
	
	return settings.current_frame_hb;
}

void ChionicWindow::add_main_label(QString p_text,QWidget *p_parent) {
	
	QLabel *l = new QLabel(p_text,p_parent);
	QPalette p = l->palette();
	p.setColor(QPalette::Text,settings.main_label_color);
	p.setColor(QPalette::WindowText,settings.main_label_color);
	l->setPalette(p);
	QFont f;
	f.setPixelSize(13);
	f.setBold(true);
	l->setFont(f);
	
}

void ChionicWindow::add_control_label(QString p_text,QWidget *p_parent) {
	
	QLabel *l = new QLabel(p_text,p_parent);
	QPalette p = l->palette();
	p.setColor(QPalette::Text,settings.control_label_color);
	p.setColor(QPalette::WindowText,settings.control_label_color);
	l->setPalette(p);
	QFont f;
	f.setPixelSize(13);
	f.setBold(true);
	l->setFont(f);
	
}

ChionicWindow::ChionicWindow(QWidget *p_parent,Chionic *p_chionic) : QDialog(p_parent) {
	
	chionic=p_chionic;
	layers.selected=0;
	
	settings.bg_color = QColor(0x59,0x5d,0x72);
	settings.main_label_color = QColor(0xD0,0xD0,0xD0);
	settings.control_label_color = QColor(0xFF,0xFF,0xFF);
	settings.controls_display = QPixmap( (const char **) controls__display_xpm );
	settings.controls_slider_skin = PropertyEditSlider::Skin( QPixmap( (const char **) controls__slider_xpm ), QPixmap( (const char **) controls__slider_xpm ), QPixmap( (const char **) controls__slider_grabber_xpm ) );
	
	if (p_chionic->get_channels_created()==2) { //panner
		settings.panner_skin=PannerEditor::Skin( QPixmap( (const char **)controls__stereo_panner_xpm), QPixmap( (const char **)controls__quad_panner_pos_xpm), 1 );
	} else {
							
		settings.panner_skin=PannerEditor::Skin( QPixmap( (const char **)controls__quad_panner_xpm), QPixmap( (const char **)controls__quad_panner_pos_xpm), 1 );
			
	}
	settings.key_scaling_skin=KeyScalingEditor::Skin(QPixmap( (const char**)controls__key_scaling_bg_xpm),QColor(235,245,255,200),1);
	settings.range_level_skin=RangeLevelEditor::Skin( QPixmap( (const char**)controls__key_scaling_bg_xpm), QPixmap( (const char**) controls__range_begin_xpm), QPixmap( (const char**) controls__range_end_xpm),QColor(235,245,255,200),1);
	settings.filter_editor_skin = FilterEditor::Skin( QPixmap( (const char**) controls__filter_display_xpm ), QPixmap( (const char**) controls__filter_display_xpm ),QColor(235,245,255,200),1);
	settings.updown_skin=PixmapUpDown::Skin( QPixmap ( (const char**)controls__spinbox_spin_xpm ),QPixmap ( (const char**)controls__spinbox_spin_xpm ),QPixmap ( (const char**)controls__spinbox_spin_xpm ) );
	settings.spin_bg=QPixmap ( (const char**)controls__spinbox_label_xpm );
	
	QPalette p=palette();
	p.setColor(QPalette::Background,settings.bg_color);
	setPalette(p);
	
	setLayout(new QVBoxLayout(this));
	
	/* TOP BOX */
	
	CHBox *hb_top = new CHBox(this);
	layout()->addWidget(hb_top);
	new PixmapLabel(hb_top,frame__top_left_xpm);
	new PixmapLabel(hb_top,frame__section_begin_xpm);
	//section buttons
	button_group = new PixmapButtonGroup(this);
	button_group->add_button( new PixmapButton(hb_top,PixmapButton::Skin(QPixmap((const char**)frame__section_source_xpm),QPixmap((const char**)frame__section_source_active_xpm)),PixmapButton::TYPE_TOGGLE ) ) ;
	
	button_group->add_button( new PixmapButton(hb_top,PixmapButton::Skin(QPixmap((const char**)frame__section_global_xpm),QPixmap((const char**)frame__section_global_active_xpm)),PixmapButton::TYPE_TOGGLE ) );
	
	new PixmapLabel(hb_top,frame__top_separator_xpm,PixmapLabel::EXPAND_STRETCH_H);
	
	button_group->add_button( new PixmapButton(hb_top,PixmapButton::Skin(QPixmap((const char**)frame__section_regions_xpm),QPixmap((const char**)frame__section_regions_active_xpm)),PixmapButton::TYPE_TOGGLE ) );
	
	button_group->add_button( new PixmapButton(hb_top,PixmapButton::Skin(QPixmap((const char**)frame__section_params_xpm),QPixmap((const char**)frame__section_params_active_xpm)),PixmapButton::TYPE_TOGGLE ) );
	
	button_group->add_button( new PixmapButton(hb_top,PixmapButton::Skin(QPixmap((const char**)frame__section_envlfo_xpm),QPixmap((const char**)frame__section_envlfo_active_xpm)),PixmapButton::TYPE_TOGGLE ) );
	
	new PixmapLabel(hb_top,frame__section_end_xpm);
	new PixmapLabel(hb_top,frame__top_right_xpm);
	
	/* MIDDLE BOX */
	CHBox *hb_middle = new CHBox(this);
	layout()->addWidget(hb_middle);
	
	new PixmapLabel(hb_middle,frame__left_xpm,PixmapLabel::EXPAND_STRETCH_V);
	
	main_vbox = new CVBox(hb_middle);
	main_vbox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	/* layer stuff here */
	
	main_stack = new QStackedWidget(main_vbox);
	
	
	new PixmapLabel(hb_middle,frame__right_xpm,PixmapLabel::EXPAND_STRETCH_V);
	
	/* BOTTOM BOX */
	CHBox *hb_bottom = new CHBox(this);
	layout()->addWidget(hb_bottom);
	
	new PixmapLabel(hb_bottom,frame__bottom_left_xpm);
	new PixmapLabel(hb_bottom,frame__bottom_xpm,PixmapLabel::EXPAND_STRETCH_H);
	new PixmapLabel(hb_bottom,frame__bottom_right_xpm);
	
	/* INIT PAGES */
	
	init_params_page();
	
	hide();
	
	layout()->setMargin(0);
	layout()->setSpacing(0);
	setWindowTitle("Chionic 2");
}


ChionicWindow::~ChionicWindow()
{
}


}
