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

#include "pixmaps/effect_panel_generic_top.xpm"
#include "pixmaps/effect_panel_generic_left.xpm"
#include "pixmaps/effect_panel_generic_right.xpm"
#include "pixmaps/effect_panel_generic_label.xpm"
#include "pixmaps/effect_panel_generic_value.xpm"
#include "pixmaps/effect_panel_generic_slider_base.xpm"
#include "pixmaps/effect_panel_generic_slider_light.xpm"
#include "pixmaps/effect_panel_generic_slider_grabber.xpm"
#include "pixmaps/effect_panel_generic_vu_empty.xpm"
#include "pixmaps/effect_panel_generic_vu_fill.xpm"
namespace ReShaked {

SoundPluginUI* SoundPluginUI_Generic::create_this(SoundPlugin *p_plugin,QWidget *p_parent) {
	
	
	return new SoundPluginUI_Generic(p_parent,p_plugin);
}

SoundPluginUI_Generic::SoundPluginUI_Generic(QWidget *p_parent,SoundPlugin *p_plugin) :SoundPluginUI(p_parent,p_plugin) {
	
	setLayout(new QVBoxLayout(this));
	PixmapLabel *title = new PixmapLabel(this,new SkinBox((const char**)effect_panel_generic_top_xpm,15,40,15,40), true);
	layout()->addWidget(title);
	
	
	CHBox *hb = new CHBox(this);
	layout()->addWidget(hb);
	
	PixmapLabel *label_name = new PixmapLabel(hb,QPixmap((const char**)effect_panel_generic_left_xpm));
	label_name->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	
	QPixmap value_pixmap((const char**)effect_panel_generic_value_xpm);
	QPixmap label_pixmap((const char**)effect_panel_generic_label_xpm);
	PixmapSlider::Skin slider_skin(effect_panel_generic_slider_base_xpm,effect_panel_generic_slider_light_xpm,effect_panel_generic_slider_grabber_xpm);
	
	PixmapVU::Skin vu_skin(effect_panel_generic_vu_empty_xpm,effect_panel_generic_vu_fill_xpm);
	
	for (int i=0;i<p_plugin->get_port_count();i++) {
		CVBox *vb_port = new CVBox(hb);
		
		CHBox *hb_port = new CHBox(vb_port);
		
		/* LABEL */
		PixmapLabel * name = new PixmapLabel(hb_port,label_pixmap);
		
		name->set_pos(QPoint(3,label_pixmap.height()-8));
		QFont name_font;
		name_font.setPixelSize(10);
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
		
		set_periodic_updates( editor );
		
		/* VALUE */
		PropertyEditLabel * value = new PropertyEditLabel( vb_port,value_pixmap );
		value->set_property(&p_plugin->get_port(i));
		value->set_postfix_visible( false );
		value->set_color(QColor(240,230,255));
		value->add_to_group(editor); //share group
		
		set_periodic_updates( value );
		
		vb_port->layout()->setMargin(0);
		vb_port->layout()->setSpacing(0);
		hb_port->layout()->setMargin(0);
		hb_port->layout()->setSpacing(0);
		
	}
	
	new PixmapLabel(hb,QPixmap((const char**)effect_panel_generic_right_xpm));
	
	
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
