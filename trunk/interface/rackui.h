//
// C++ Interface: rackui
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDRACKUI_H
#define RESHAKEDRACKUI_H

#include "interface/connection_rack.h"
#include "interface/sound_plugin_rack.h"
#include "editor/editor.h"
#include "ui_blocks/helpers.h"
#include <Qt/qlineedit.h>
#include <Qt/qpushbutton.h>
#include <Qt/qcombobox.h>
#include <Qt/qstackedwidget.h>
#include "ui_blocks/pixmap_combo.h"
#include "ui_blocks/pixmap_button.h"


namespace ReShaked {

/**
	@author red <red@killy>
*/
class RackUI : public CVBox {
	Q_OBJECT

			    
			
	Editor *editor;
	PixmapButton *add_plugin;
	PixmapCombo *rack_choose;
	PixmapButton *rack_front;
	PixmapButton *rack_back;
	
	PixmapLabel *rack_label;
	PixmapButton *rack_file;

	
	
	int selected_rack;
	
	QStackedWidget *stack;
	
	ConnectionRack *connections;
	SoundPluginRack *plugins;
	
public slots:	
	
	void rack_front_selected();
	void rack_back_selected();
	
	void update_rack_combo_names_slot(); //probably a renaming?
	void update_rack_combo_slot();
	void update_selected_rack_slot();
	
	void rack_selected_slot(int p_selected_rack);
	
	void update_rack();
	void repaint_rack();
	
	void add_plugin_slot();
public:
	
	void select_track(int p_track);
	void select_global();
	
	
	RackUI(QWidget *p_parent,Editor *p_editor,PropertyEditUpdater *p_updater);
	~RackUI();

};

}

#endif
