//
// C++ Interface: sound_plugin_chooser
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSOUND_PLUGIN_CHOOSER_H
#define RESHAKEDSOUND_PLUGIN_CHOOSER_H


#include <Qt/qtreewidget.h>
#include <Qt/qdialog.h>

namespace ReShaked {

/**
	@author red <red@killy>
*/
class SoundPluginChooser : public QDialog {

	Q_OBJECT
			    
	QTreeWidget *tree;
		
public:
	SoundPluginChooser(QWidget *p_parent);
	~SoundPluginChooser();

};

}

#endif
