//
// C++ Implementation: sound_plugin_ui_generic
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sound_plugin_ui_generic.h"
#include <Qt/qpushbutton.h>
#include <Qt/qlayout.h>
#include "ui_blocks/helpers.h"
#include "ui_blocks/pixmap_label.h"
#include "ui_blocks/pixmap_slider.h"
#include "ui_blocks/property_editors.h"
#include "interface/visual_settings.h"


namespace ReShaked {

SoundPluginUI* SoundPluginUI_Generic::create_this(SoundPlugin *p_plugin,QWidget *p_parent) {
	
	
	return new SoundPluginUI_Generic(p_parent,p_plugin);
}

SoundPluginUI_Generic::SoundPluginUI_Generic(QWidget *p_parent,SoundPlugin *p_plugin) :SoundPluginUI(p_parent,p_plugin) {
	
	setLayout(new QVBoxLayout(this));
	CHBox *hb_top = new CHBox(this);
	layout()->addWidget(hb_top);
	new PixmapLabel(hb_top,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_TOP_LEFT));
	PixmapLabel *title = new PixmapLabel(hb_top,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_TOP) ,PixmapLabel::EXPAND_TILE_H);
	title->set_text( QStrify(p_plugin->get_caption())) ;
	title->setToolTip( QStrify(p_plugin->get_caption())) ;
	new PixmapLabel(hb_top,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_TOP_RIGHT));
	
	
	
	CHBox *hb = new CHBox(this);
	layout()->addWidget(hb);
	
	PixmapLabel *label_name = new PixmapLabel(hb,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_LEFT));
	label_name->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	
	QPixmap value_pixmap(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_VALUE));
	QPixmap label_pixmap(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_LABEL));
	PixmapSlider::Skin slider_skin(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_SLIDER_BASE),GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_SLIDER_LIGHT),GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_SLIDER_GRABBER));
	
	PixmapVU::Skin vu_skin(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_VU_EMPTY),GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_VU_FILL));
	
	for (int i=0;i<p_plugin->get_port_count();i++) {
		
		if (QStrify(p_plugin->get_port(i).get_caption()).indexOf("/")>=0)
			continue;
		
		CVBox *vb_port = new CVBox(hb);
		
		CHBox *hb_port = new CHBox(vb_port);
		
		/* LABEL */
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
		
		
		/* SLIDER/VU */
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
		
		/* VALUE */
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


SoundPluginUI_Generic::~SoundPluginUI_Generic()
{
}


}
