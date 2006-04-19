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
#include <Qt/qinputdialog.h>
#include <Qt/qfile.h>

namespace ReShaked {


void PluginPresetBrowser::rename_slot() {
	
	QString item=(selected_file=="")?working_path:selected_file;
	
	QString item_dir;
	QString item_entry;
	if (item.lastIndexOf("/")!=-1) {
		
		item_dir=item.left( item.lastIndexOf("/") );
		item_entry=item;
		item_entry.remove(0,item.lastIndexOf("/")+1);
		
	} else {
		
		item_entry=item;
		
	}
	
	QString new_name=QInputDialog::getText ( this, "Rename", "Rename:",QLineEdit::Normal,item_entry);
	
	if (new_name=="")
		return;
	
	if (new_name.lastIndexOf("/")!=-1) {
		
		new_name.remove(new_name.lastIndexOf("/")+1);
	}
	
	new_name=item_dir+"/"+new_name;
	
	if (QFile::exists(new_name)) {
		
		QMessageBox::information ( this, "Info", "File Exists!" , QMessageBox::Ok);
		return;

	}

	QDir d;
	printf("RENAME %s -> %s\n",item.toAscii().data(),new_name.toAscii().data());
	if (!d.rename(item,new_name)) {
		
		QMessageBox::information ( this, "Info", "Rename Failed" , QMessageBox::Ok);
		return;		
	}
	if (selected_file!="")
		selected_file=new_name;
	else
		working_path=new_name;
	rebuild_tree();
}

void PluginPresetBrowser::open_slot() {
	
	if (selected_file=="") 
		return;
	action=ACTION_OPEN;
	accept();
	
}
void PluginPresetBrowser::save_as_slot() {
	
	QString folder_alone=working_path;
	folder_alone.remove(0,dir_path.length());
	if (folder_alone.length())
		folder_alone.remove(0,1);
	folder_alone+="/";
	
	QString existing="";
	if (selected_file!="") {
		existing=selected_file;
		if (existing.lastIndexOf("/")!=-1) {
			
			existing.remove(0,existing.lastIndexOf("/")+1);
		}
	}	
	
	QString filename=QInputDialog::getText ( this, "New File", "Saving at: "+folder_alone+"\nEnter Filename:",QLineEdit::Normal,existing);

	if (filename.lastIndexOf("/")!=-1) {
		
		filename.remove(0,filename.lastIndexOf("/")+1);
	}
	if (filename=="")
		return;
	
	QString temptative_file=working_path+"/"+filename;
	
	
	if (QFile::exists(selected_file)) {
		
		int action=QMessageBox::warning ( this, "Warning", "File Exists, Overwrite??" , QMessageBox::Yes, QMessageBox::No);
		
		if (action==QMessageBox::No)
			return;
		
	}
	
	selected_file=temptative_file;
	action=ACTION_SAVE;
	accept();
	
}
void PluginPresetBrowser::save_slot() {
	
	if (selected_file=="") 
		return;
	
	if (initial_file!=selected_file) {
		
		QString file_alone=selected_file;
		if (file_alone.lastIndexOf("/")!=-1) {
		
			file_alone.remove(0,file_alone.lastIndexOf("/")+1);
		}
			
		int action=QMessageBox::warning ( this, "Save Current", "Overwrite: "+file_alone+" ?" , QMessageBox::Yes, QMessageBox::No);
		
		if (action==QMessageBox::No)
			return;
	}
	
	action=ACTION_SAVE;
	accept();
	
}

void PluginPresetBrowser::remove_slot() {
	
	
	QDir location(working_path);
	
	if (selected_file!="") {
		
		QString file_alone=selected_file;
		if (file_alone.lastIndexOf("/")!=-1) {
		
			file_alone.remove(0,file_alone.lastIndexOf("/")+1);
		}
		
		
		int action=QMessageBox::warning ( this, "Remove File", "Remove: "+file_alone+" ?" , QMessageBox::Yes, QMessageBox::No);
		
		if (action==QMessageBox::No)
			return;
		
		location.remove(selected_file);
		
	} else { 
		
		if (working_path==dir_path)
			return;
			
		QString folder_alone=working_path;
		if (folder_alone.lastIndexOf("/")!=-1) {
		
			folder_alone.remove(0,folder_alone.lastIndexOf("/")+1);
		}
			
		//printf("subfiles %i\n",location.entryList(QDir::Files|QDir::Dirs).size());
		
		if (location.entryList(QDir::Files|QDir::Dirs).size()>2) { // there is . and .. by default, so..
			
			QMessageBox::information ( this, "Info", "Folder '"+folder_alone+"'\n is not empty." , QMessageBox::Ok);
			return;
		}
			
		
		
		int action=QMessageBox::warning ( this, "Remove Folder", "Remove: "+folder_alone+" ?" , QMessageBox::Yes, QMessageBox::No);
		
		if (action==QMessageBox::No)
			return;
		
		location.rmdir(working_path);
		
	}
	
	selected_file="";
	working_path=dir_path;
	rebuild_tree();
}

void PluginPresetBrowser::make_dir() {
	
	QString dirname=QInputDialog::getText ( this, "New Folder", "Folder Name:");
	if (dirname=="")
		return;
	
	if (dirname.lastIndexOf("/")!=-1) {
		
		dirname.remove(0,dirname.lastIndexOf("/")+1);
	}
	
	if (dirname=="")
		return;
	
	QDir cdir(working_path);
	ERR_FAIL_COND(!cdir.mkdir(dirname));
	
	printf("Now at %s\n",working_path.toAscii().data());
	
	rebuild_tree();
}


PluginPresetBrowser::TreeItem::TreeItem(QTreeWidgetItem *p_parent) : QTreeWidgetItem(p_parent) {
	
	
}
PluginPresetBrowser::TreeItem::TreeItem(QTreeWidget *p_parent) : QTreeWidgetItem(p_parent) {
	
	
	
}


void PluginPresetBrowser::tree_item_changed (  ) {
	
	QList<QTreeWidgetItem *> sel_items = tree->selectedItems ();
	if (sel_items.empty())
		return;
			
	QTreeWidgetItem * current=*sel_items.begin();
	TreeItem *ti = dynamic_cast<TreeItem*>(current);
	ERR_FAIL_COND(!ti);
	
	printf("Item Changed\n");
	save->setEnabled(ti->file!="");
	open->setEnabled(ti->file!="");
	if (ti->file!="")
		selected_file=ti->file;
	else
		selected_file="";
	working_path=ti->path;
	
	initial_file="";
	
}


void PluginPresetBrowser::parse_dir(QDir &p_dir,QTreeWidgetItem *p_parent,QTreeWidget *p_tree_base) {
	
	printf("Enter %s\n",p_dir.path().toAscii().data());
	TreeItem *dir_item;
	
	if (p_tree_base) {
		
		dir_item  = new TreeItem(p_tree_base);
		dir_item->setText(0,"/");
	} else  {

		dir_item  = new TreeItem(p_parent);
		dir_item->setText(0,p_dir.dirName()+"/");
		
	}
	
	dir_item->path=p_dir.path();
	dir_item->setIcon(0,GET_QPIXMAP(ICON_FILE_FOLDER));
	tree->expandItem(dir_item);
	
	if (working_path==p_dir.path() && selected_file=="")
		tree->setItemSelected(dir_item,true);
	
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
		
		TreeItem *file = new TreeItem(dir_item);
		file->setText(0,*I);

		QFont f;
		f.setBold(true);
		file->setFont(0,f);
		file->path=p_dir.path();
		file->file=p_dir.path()+"/"+*I;
		if (file->file==selected_file)
			tree->setItemSelected(file,true);
	}
	
}
	


void PluginPresetBrowser::rebuild_tree() {
	
	tree->clear();
	
	QDir plugin_dir(dir_path);
	plugin_dir.setSorting(QDir::Name|QDir::IgnoreCase);
	
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

PluginPresetBrowser::Action PluginPresetBrowser::get_action() {
	
	return action;
}
QString PluginPresetBrowser::get_file() {
	
	return selected_file;
}




void PluginPresetBrowser::init(QString p_dir,QString p_current) {
	
	
	setLayout( new QHBoxLayout(this) );
	tree = new QTreeWidget(this);
	tree->setColumnCount(1);
	layout()->addWidget( tree );
	CVBox *vb = new CVBox(this);
	layout()->addWidget( vb );
	vb->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
	
	open = new QPushButton(GET_QPIXMAP(ICON_FILE_OPEN),"Open",vb);
	open->setEnabled(false);
	QObject::connect(open,SIGNAL(clicked()),this,SLOT(open_slot()));
	save = new QPushButton(GET_QPIXMAP(ICON_FILE_SAVE),"Save Current",vb);
	save->setEnabled(false);
	QObject::connect(save,SIGNAL(clicked()),this,SLOT(save_slot()));
	save_as= new QPushButton(GET_QPIXMAP(ICON_FILE_SAVEAS),"Save As",vb);
	(new QFrame(vb))->setFixedHeight(16);
	QObject::connect(save_as,SIGNAL(clicked()),this,SLOT(save_as_slot()));
	new_subfolder= new QPushButton(GET_QPIXMAP(ICON_FILE_FOLDER_NEW),"New SubFolder",vb);
	QObject::connect(new_subfolder,SIGNAL(clicked()),this,SLOT(make_dir()));
	rename = new QPushButton("Rename",vb);
	QObject::connect(rename,SIGNAL(clicked()),this,SLOT(rename_slot()));
	(new QFrame(vb))->setFixedHeight(16);
	remove = new QPushButton("Remove",vb);
	QObject::connect(remove,SIGNAL(clicked()),this,SLOT(remove_slot()));
	(new QFrame(vb))->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
	
	cancel = new QPushButton("Cancel",vb);
	QObject::connect(cancel,SIGNAL(clicked()),this,SLOT(reject()));
	

	dir=p_dir;
	ensure_dir_exists();
	
	working_path=dir_path;
	if (QFile::exists(p_current)) {
		selected_file=p_current;
		open->setEnabled(true);
		save->setEnabled(true);
		initial_file=selected_file;
	}
	
	rebuild_tree();
	tree->header()->hide();
	setMinimumSize(450,350);
	
	
	QObject::connect(tree,SIGNAL(itemSelectionChanged ()),this,SLOT(tree_item_changed()));
	
	
	
	action=ACTION_NONE;
		
}




PluginPresetBrowser::PluginPresetBrowser(QWidget *p_parent,SoundPlugin *p_plugin) : QDialog(p_parent) {
	
	init(QStrify(p_plugin->get_info()->unique_ID),QStrify(p_plugin->get_current_file()));
	
	if (p_plugin->get_info()->xpm_preview) {
		
		setWindowIcon( QPixmap(p_plugin->get_info()->xpm_preview) );

	}

	setWindowTitle(QStrify(p_plugin->get_info()->caption)+" Presets");	
}

PluginPresetBrowser::PluginPresetBrowser(QWidget *p_parent,QString p_current_file) :QDialog (p_parent){
	
	init("racks",p_current_file);
	setWindowTitle("Rack Presets");

	
}

PluginPresetBrowser::~PluginPresetBrowser()
{
}


}
