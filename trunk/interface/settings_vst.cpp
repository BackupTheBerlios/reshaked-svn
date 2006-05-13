
#include "version.h"
#ifdef VST_ENABLED

#include "settings_vst.h"

#include "drivers/vst_sound_plugin_source.h"
#include <Qt/qfiledialog.h>


namespace ReShaked {

void SettingsVST::load(TreeLoader *p_loader) {
	
		
	for (int i=0;i<p_loader->get_var_count();i++) {
		
		String path=p_loader->get_string(p_loader->get_var_name(i));
		
		list->addItem(QStrify(path));
		VST_SoundPluginSource::add_path(path);
	}
}
	
	

void SettingsVST::save(TreeSaver *p_saver) {
	
	
	for (int i=0;i<list->count();i++) {
		
		p_saver->add_string("path_"+String::num(i+1),DeQStrify(list->item(i)->text()));
	}
	
}


void SettingsVST::dir_add_slot() {
	QString dir = QFileDialog::getExistingDirectory ( this, "Path to VST Plugins");
	if (dir=="")
		return;
	list->addItem(dir);
}
void SettingsVST::dir_remove_slot() {
	
	QListWidgetItem *item = list->takeItem( list->currentRow() );
	if (item)
		delete item;
	
}


SettingsVST::SettingsVST(QWidget *p_parent) : CHBox(p_parent) {

	list = new QListWidget(this);
	CVBox *buttons_vbox = new CVBox(this);
	
	QPushButton *b = new QPushButton("Add Dir",buttons_vbox);
	QObject::connect(b,SIGNAL(clicked()),this,SLOT(dir_add_slot()));
	b = new QPushButton("Remove Dir",buttons_vbox);
	QObject::connect(b,SIGNAL(clicked()),this,SLOT(dir_remove_slot()));
	
	(new QWidget(buttons_vbox))->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	buttons_vbox->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
	
	layout()->setMargin(10);
	layout()->setSpacing(10);

}	

}
#endif
