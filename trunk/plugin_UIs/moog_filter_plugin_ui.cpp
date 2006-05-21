//
// C++ Implementation: moog_filter_plugin_ui
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "moog_filter_plugin_ui.h"

#include "ui_blocks/pixmap_label.h"
#include "interface/visual_settings.h"

namespace ReShaked {

SoundPluginUI* MoogFilterPluginUI::create_this(SoundPlugin *p_plugin,QWidget *p_parent) {
	
	MoogFilterPlugin *moog = dynamic_cast<MoogFilterPlugin*>(p_plugin);
	
	if (moog)
		return new MoogFilterPluginUI(p_parent,moog);
	else
		return NULL;
}

MoogFilterPluginUI::MoogFilterPluginUI(QWidget *p_parent,MoogFilterPlugin *p_plugin) :SoundPluginUI(p_parent,p_plugin) {
	
	CHBox *hb = generate_default_layout(true);
	
	new PixmapLabel(hb,GET_QPIXMAP(THEME_MOOG__MODE));
}


MoogFilterPluginUI::~MoogFilterPluginUI()
{
}


}
