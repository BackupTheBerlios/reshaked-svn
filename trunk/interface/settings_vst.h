#ifndef SETTINGS_VST_H
#define SETTINGS_VST_H

#include "version.h"

#ifdef VST_ENABLED

#include <Qt/qlistwidget.h>
#include <Qt/qpushbutton.h>
#include "tree_loader.h"
#include "tree_saver.h"
#include "ui_blocks/helpers.h"

namespace ReShaked {

class SettingsVST : public CHBox {

	Q_OBJECT
	
	QListWidget *list;
	
protected slots:

	void dir_add_slot();
	void dir_remove_slot();
	
public:
	void load(TreeLoader *p_loader);
	void save(TreeSaver *p_saver);
	
	SettingsVST(QWidget *p_parent);

};

}

#endif
#endif
