//
// C++ Interface: plugin_preset_browser
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDPLUGIN_PRESET_BROWSER_H
#define RESHAKEDPLUGIN_PRESET_BROWSER_H

#include "ui_blocks/helpers.h"
#include <Qt/qtreewidget.h>
#include "editor/editor.h"
#include <Qt/qpushbutton.h>
#include <Qt/qdialog.h>
#include "engine/sound_plugin.h"
#include <Qt/qdir.h>



namespace ReShaked {

/**
	@author red <red@killy>
*/
class PluginPresetBrowser : public QDialog {
	
	Q_OBJECT
	
	
	QTreeWidget *tree;
	
	QPushButton *open;
	QPushButton *save;	
	QPushButton *save_as;	
	QPushButton *new_subfolder;	
	QPushButton *cancel;	
	
	QString dir_path;
	QString dir;
	SoundPlugin *plugin;
	
	QString working_path;
	QString current_file;
	
	void parse_dir(QDir &p_dir,QTreeWidgetItem *p_parent,QTreeWidget *p_tree_base=NULL);
	
	void rebuild_tree();
	void ensure_dir_exists();
private slots:
		
	void tree_item_changed ( QTreeWidgetItem * current, QTreeWidgetItem * previous );
	
public:
	PluginPresetBrowser(QWidget *p_parent,SoundPlugin *p_plugin);
	
	~PluginPresetBrowser();

};

}

#endif
