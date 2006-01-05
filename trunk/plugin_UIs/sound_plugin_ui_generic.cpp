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
#include "pixmaps/effect_panel_generic_top.xpm"
#include "pixmaps/effect_panel_generic_left.xpm"
#include "pixmaps/effect_panel_generic_right.xpm"
#include "pixmaps/effect_panel_generic_label.xpm"
#include "pixmaps/effect_panel_generic_value.xpm"
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
	
	new PixmapLabel(hb,new QPixmap((const char**)effect_panel_generic_left_xpm), true );
	
	for (int i=0;i<p_plugin->get_port_count();i++) {
		CVBox *vb_port = new CVBox(hb);
		
		CHBox *hb_port = new CHBox(vb_port);
		PixmapLabel * name = new PixmapLabel(hb_port,new QPixmap((const char**)effect_panel_generic_label_xpm), true );
		
		
		PixmapLabel * value = new PixmapLabel(vb_port,new QPixmap((const char**)effect_panel_generic_value_xpm), true );
		
		vb_port->layout()->setMargin(0);
		vb_port->layout()->setSpacing(0);
		hb_port->layout()->setMargin(0);
		hb_port->layout()->setSpacing(0);
		
	}
	
	new PixmapLabel(hb,new QPixmap((const char**)effect_panel_generic_right_xpm), true );
	
	
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
