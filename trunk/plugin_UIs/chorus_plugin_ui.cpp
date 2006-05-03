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
	
	setLayout(new QVBoxLayout(this));
	CHBox *hb_top = new CHBox(this);
	layout()->addWidget(hb_top);
	new PixmapLabel(hb_top,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_TOP_LEFT));
	PixmapLabel *title = new PixmapLabel(hb_top,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_TOP) ,PixmapLabel::EXPAND_TILE_H);
	title->set_text( QStrify(p_chorus->get_caption())) ;
	title->setToolTip( QStrify(p_chorus->get_caption())) ;
	new PixmapLabel(hb_top,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_TOP_RIGHT));
	
	
	
	CHBox *hb = new CHBox(this);
	layout()->addWidget(hb);
	
	PixmapLabel *label_name = new PixmapLabel(hb,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_LEFT));
	label_name->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	
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
	
	CVBox *vb_port = new CVBox(hb);
		
	CHBox *hb_port = new CHBox(vb_port);
		
	/** WET SEND **/
	
	/// LABEL /
	PixmapLabel * name = new PixmapLabel(hb_port,label_pixmap);
		
	name->set_pos(QPoint(6,label_pixmap.height()-8));
	name->get_font().setPixelSize(10);
	name->set_text( "Wet" );
	name->set_angle( -90 );
	name->set_color(QColor(0,0,22));
	
	///SLIDER
	PropertyEditSlider * slider = new PropertyEditSlider(hb_port,slider_skin);
	slider->set_property(&p_chorus->get_port_by_name("wet_send"));
	
		
	register_property_editor( slider );
		
	/// VALUE 
	PropertyEditLabel * value = new PropertyEditLabel( vb_port,value_pixmap );
	value->set_property(&p_chorus->get_port_by_name("wet_send"));
	value->set_postfix_visible( false );
	value->set_color(QColor(240,240,255));
	value->add_to_group(slider); //share group
	
	register_property_editor( value );
		
	/** DRY SEND */
	
	vb_port = new CVBox(hb);
		
	hb_port = new CHBox(vb_port);
	
	/// LABEL /
	name = new PixmapLabel(hb_port,label_pixmap);
		
	name->set_pos(QPoint(6,label_pixmap.height()-8));
	name->get_font().setPixelSize(10);
	name->set_text( "Dry" );
	name->set_angle( -90 );
	name->set_color(QColor(0,0,22));
	
	///SLIDER
	slider = new PropertyEditSlider(hb_port,slider_skin);
	slider->set_property(&p_chorus->get_port_by_name("dry_send"));
			
	register_property_editor( slider );
		
	/// VALUE 
	value = new PropertyEditLabel( vb_port,value_pixmap );
	value->set_property(&p_chorus->get_port_by_name("dry_send"));
	value->set_postfix_visible( false );
	value->set_color(QColor(240,240,255));
	value->add_to_group(slider); //share group
	
	register_property_editor( value );
	
	/*
	PixmapVU::Skin vu_skin(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_VU_EMPTY),GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_VU_FILL));
	
	for (int i=0;i<p_plugin->get_port_count();i++) {
		
		if (QStrify(p_plugin->get_port(i).get_caption()).indexOf("/")>=0)
			continue;
		
		CVBox *vb_port = new CVBox(hb);
		
		CHBox *hb_port = new CHBox(vb_port);
		
		/// LABEL /
		PixmapLabel * name = new PixmapLabel(hb_port,label_pixmap);
		
		name->set_pos(QPoint(6,label_pixmap.height()-8));
		name->get_font().setPixelSize(10);
		QString name_str=QStrify(p_plugin->get_port(i).get_caption());
		if (p_plugin->get_port(i).get_postfix()!="") {
			
			name_str+=" (" +QStrify(p_plugin->get_port(i).get_postfix()).toLower() +")";
		}
		name->set_text( name_str );
		name->set_angle( -90 );
		name->set_color(QColor(0,0,22));
		
		
		/// SLIDER/VU /
		PropertyEditor *editor;
		if (p_plugin->get_port_type(i)==SoundPlugin::TYPE_WRITE) {
			PropertyEditSlider * slider = new PropertyEditSlider(hb_port,slider_skin);
			slider->set_property(&p_plugin->get_port(i));
			editor=slider;
		} else {
			PropertyEditVU * vu = new PropertyEditVU(hb_port,vu_skin);
			vu->set_property(&p_plugin->get_port(i));
			editor=vu;
		}
		
		register_property_editor( editor );
		
		/// VALUE 
		PropertyEditLabel * value = new PropertyEditLabel( vb_port,value_pixmap );
		value->set_property(&p_plugin->get_port(i));
		value->set_postfix_visible( false );
		value->set_color(QColor(240,240,255));
		value->add_to_group(editor); //share group
		
		register_property_editor( value );
		
		vb_port->layout()->setMargin(0);
		vb_port->layout()->setSpacing(0);
		hb_port->layout()->setMargin(0);
		hb_port->layout()->setSpacing(0);
		
	}
	*/
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
