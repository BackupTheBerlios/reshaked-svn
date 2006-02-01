//
// C++ Implementation: plugin_preset_browser
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "editor/plugin_preset_manager.h"
#include "plugin_preset_browser.h"
#include "ui_blocks/visual_settings.h"
#include "ui_blocks/helpers.h"
#include <Qt/qmessagebox.h>
#include <Qt/qdir.h>
#include <Qt/qheaderview.h>
#include <Qt/qlabel.h>

namespace ReShaked {


void PluginPresetBrowser::tree_item_changed ( QTreeWidgetItem * current, QTreeWidgetItem * previous ) {
	
	//	save->setEnabled(current->data(0,ROLE_TYPE).toInt()==TYPE_FILE);
	//working_path=current->data(0,ROLE_PATH).toString();
	
	
}


void PluginPresetBrowser::parse_dir(QDir &p_dir,QTreeWidgetItem *p_parent,QTreeWidget *p_tree_base) {
	
	printf("Enter %s\n",p_dir.path().toAscii().data());
	QTreeWidgetItem *dir_item;
	
	if (p_tree_base) {
		
		dir_item  = new QTreeWidgetItem(p_tree_base);
		dir_item->setText(0,"/");
	} else  {

		dir_item  = new QTreeWidgetItem(p_parent);
		dir_item->setText(0,p_dir.dirName()+"/");
		
	}
	
	dir_item->setIcon(0,GET_QPIXMAP(ICON_FILE_OPEN));
	tree->expandItem(dir_item);
	
	QStringList subdirs=p_dir.entryList(QDir::Dirs);
	
	foreach(I,subdirs) {
		if (*I=="." || *I=="..")
			continue;
		printf("trying subdir %s\n",I->toAscii().data());
		ERR_CONTINUE(!p_dir.cd(*I));
		parse_dir(p_dir,dir_item);
		p_dir.cdUp();
	}
	
	QStringList files=p_dir.entryList(QDir::Files);
	foreach(I,files) {
		
		QTreeWidgetItem *file = new QTreeWidgetItem(dir_item);
		file->setText(0,*I);

		QFont f;
		f.setBold(true);
		file->setFont(0,f);
		

		//file->setData(0,ROLE_FILE,p_dir.path()+"/"+*I);
		
	}
	
}
	


void PluginPresetBrowser::rebuild_tree() {
	
	tree->clear();
	
	QDir plugin_dir(dir_path);
	plugin_dir.setSorting(QDir::Name);
	
	parse_dir(plugin_dir,NULL,tree);
	
}

void PluginPresetBrowser::ensure_dir_exists() {
	
	QString preset_path=QStrify(PluginPresetManager::get_singleton()->get_preset_dir());
	QDir plugin_dir;
	if (!plugin_dir.cd(preset_path)) {
		
		QMessageBox::critical ( this, "ERROR!", "Presets dir Not Found!", QMessageBox::Ok,QMessageBox::NoButton);
		reject();
		return;
	}
	
	if (!plugin_dir.cd(dir)) {
		
		if (!plugin_dir.mkdir(dir)) {
			
			printf("plugin dir couldnt be created \n");
			
			QMessageBox::critical ( this, "ERROR!", "Cant create plugin Presets Dir!", QMessageBox::Ok,QMessageBox::NoButton);
			reject();
			return;
			
		}
		
		plugin_dir.cd(dir);
	}
	
	dir_path=plugin_dir.path();
	
}

PluginPresetBrowser::PluginPresetBrowser(QWidget *p_parent,SoundPlugin *p_plugin) : QDialog(p_parent) {
	
	setLayout( new QHBoxLayout(this) );
	tree = new QTreeWidget(this);
	tree->setColumnCount(1);
	layout()->addWidget( tree );
	CVBox *vb = new CVBox(this);
	layout()->addWidget( vb );
	vb->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
	
	open = new QPushButton(GET_QPIXMAP(ICON_FILE_OPEN),"Open Preset",vb);
	save = new QPushButton(GET_QPIXMAP(ICON_FILE_SAVE),"Replace Preset",vb);
	save->setEnabled(false);
	save_as= new QPushButton(GET_QPIXMAP(ICON_FILE_SAVEAS),"New Preset",vb);
	new_subfolder= new QPushButton(GET_QPIXMAP(ICON_FILE_SAVEAS),"New SubFolder",vb);
	
	(new QFrame(vb))->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
	
	if (p_plugin->get_info()->xpm_preview) {
		
		setWindowIcon( QPixmap(p_plugin->get_info()->xpm_preview) );

	}

	
	cancel = new QPushButton("Cancel",vb);
	
	
	
	dir=QStrify(p_plugin->get_info()->unique_ID); //the dir is the unique ID
	
	
	ensure_dir_exists();
	working_path=dir_path;
	
	rebuild_tree();
	tree->header()->hide();
	setMinimumSize(450,350);
	setWindowTitle(QStrify(p_plugin->get_info()->caption)+" Presets");
	
	QObject::connect(tree,SIGNAL(currentItemChanged ( QTreeWidgetItem * current, QTreeWidgetItem * previous )),this,SLOT(tree_item_changed( QTreeWidgetItem*, QTreeWidgetItem* )));
}


PluginPresetBrowser::~PluginPresetBrowser()
{
}


}
