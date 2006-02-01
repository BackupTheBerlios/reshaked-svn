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
public:	
	
	enum Action {
		
		ACTION_OPEN,
		ACTION_SAVE,
		ACTION_NONE,
	};
	
private:
			
			
	class TreeItem : public QTreeWidgetItem {
	public:
		QString path;
		QString file;
		
		TreeItem(QTreeWidgetItem *p_parent);
		TreeItem(QTreeWidget *p_parent);

	};
	
	Action action;
	
	QTreeWidget *tree;
	
	QPushButton *open;
	QPushButton *save;	
	QPushButton *save_as;	
	QPushButton *new_subfolder;	
	QPushButton *cancel;	
	QPushButton *remove;	
	
	QString dir_path;
	QString dir;
	SoundPlugin *plugin;
	
	QString initial_file;
	QString working_path;
	QString selected_file;
	
	void parse_dir(QDir &p_dir,QTreeWidgetItem *p_parent,QTreeWidget *p_tree_base=NULL);
	
	void rebuild_tree();
	void ensure_dir_exists();
private slots:
		
	void open_slot();
	void save_as_slot();
	void save_slot();
	
	void rename_slot();
	void remove_slot();
	void make_dir();
	
	void tree_item_changed ();
	
public:
	PluginPresetBrowser(QWidget *p_parent,SoundPlugin *p_plugin);
	
	~PluginPresetBrowser();

};

}

#endif
