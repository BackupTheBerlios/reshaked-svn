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
			    
	enum ItemRole {
		ROLE_ID=1,
		ROLE_INDEX=2,
	};
		
	QTreeWidget *tree;
	
	int selected_idx;
		
	virtual void accept();
public:
	int get_selected_plugin_idx();
	
	SoundPluginChooser(QWidget *p_parent, bool p_show_synths);
	~SoundPluginChooser();

};

}

#endif
