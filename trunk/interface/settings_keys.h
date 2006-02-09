//
// C++ Interface: settings_keys
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSETTINGS_KEYS_H
#define RESHAKEDSETTINGS_KEYS_H


#include "ui_blocks/helpers.h"
#include <Qt/qtreewidget.h>
#include <Qt/qpushbutton.h>

namespace ReShaked {

/**
	@author red <red@killy>
*/
class SettingsKeys : public CVBox {
	
	Q_OBJECT
	
	class KeybindItem : public QTreeWidgetItem {
	public:			
		int bind_idx;
	
		void update_bind();
		KeybindItem(QTreeWidgetItem *p_parent,int p_bind_idx);
	};
	
	
	std::vector<KeybindItem*> items;
	QTreeWidget *tree;
	QPushButton *set_bind;	
	QPushButton *clear_bind;	
	
	
	int selected_idx;
	
	
public slots:	
	
	void clear_item();
	void set_item();
	
	void itemSelectionChangedSlot();
public:
	
	void update_keybinds();
	SettingsKeys(QWidget *p_parent);
	~SettingsKeys();

};

}

#endif
