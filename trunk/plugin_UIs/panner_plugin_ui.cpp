//
// C++ Implementation: panner_plugin_ui
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "panner_plugin_ui.h"

#include "interface/visual_settings.h"
#include "ui_blocks/property_editors.h"
#include "ui_blocks/panner_editor.h"
#include "plugins/panner_plugin.h"

namespace ReShaked {

SoundPluginUI* PannerPluginUI::create_this(SoundPlugin *p_plugin,QWidget *p_parent) {
	
	PannerPlugin *panner = dynamic_cast<PannerPlugin*>(p_plugin);
	if (panner)
		return new PannerPluginUI(p_parent,p_plugin);
	else
		return NULL;
}

PannerPluginUI::PannerPluginUI(QWidget *p_parent,SoundPlugin *p_plugin) :SoundPluginUI(p_parent,p_plugin) {
	
	CHBox *hb = generate_default_layout(false);

	bool quad=(p_plugin->get_channels_created()==4);
			
	new PixmapLabel(hb,GET_QPIXMAP(THEME_PANNER__LEFT));
	
	CVBox *vb = new CVBox(hb);
	
	new PixmapLabel(vb,quad?GET_QPIXMAP(THEME_PANNER__QUAD_TOP):GET_QPIXMAP(THEME_PANNER__STEREO_TOP));
	
	PannerEditor::Skin pe_skin;
	
	pe_skin.bg=quad?GET_QPIXMAP(THEME_PANNER__QUAD_PANNER):GET_QPIXMAP(THEME_PANNER__STEREO_PANNER);
	pe_skin.pos=GET_QPIXMAP(THEME_PANNER__POS);
	pe_skin.border_w=1;
	
	PannerEditor *pe = new PannerEditor(vb,quad,pe_skin);
	pe->set_properties( &p_plugin->get_port_by_name( "pan"), &p_plugin->get_port_by_name( "depth") );
		
	register_property_editor( pe );
	
	new PixmapLabel(vb,quad?GET_QPIXMAP(THEME_PANNER__QUAD_MIDDLE):GET_QPIXMAP(THEME_PANNER__STEREO_MIDDLE));
	
	PropertyEditor *mode = new PropertyEditCombo(vb, GET_QPIXMAP(THEME_PANNER__MODE));
	mode->set_property( &p_plugin->get_port_by_name( "mode") );
	register_property_editor( mode );
	
	new PixmapLabel(vb,GET_QPIXMAP(THEME_PANNER__BOTTOM));
		
	new PixmapLabel(hb,GET_QPIXMAP(THEME_PANNER__RIGHT));
	
}


PannerPluginUI::~PannerPluginUI()
{
}


}
