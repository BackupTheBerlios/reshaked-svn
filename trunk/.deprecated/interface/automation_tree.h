//
// C++ Interface: automation_tree
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDAUTOMATION_TREE_H
#define RESHAKEDAUTOMATION_TREE_H


#include "ui_blocks/helpers.h"
#include <Qt/qtreewidget.h>
#include <Qt/qdialog.h>
#include "engine/track.h"
#include <vector>
#include "editor/editor.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class AutomationTree : public CHBox {
			    
	Q_OBJECT 
					    
	class QTreeAutomationItem : public QTreeWidgetItem {
				    
		int property_idx;
	public:
		int get_property_idx();
		QTreeAutomationItem(QTreeWidgetItem * p_parent,int p_property_idx); //construct with this parent
		QTreeAutomationItem(QTreeWidget * p_parent,int p_property_idx); //construct with this parent
				    
	};				    
			    
	QTreeWidget *tree;
	Track *track;
	
	std::vector<QTreeAutomationItem*> item_array;
public slots:	
	

	void item_clicked( QTreeWidgetItem * item, int column );
	void update_item_status();
signals:
		
	void attempt_automation_add_signal(int p_index);	
	void attempt_automation_remove_signal(int p_index);	
public:
	void update_automation_tree();

	void set_track(Track *p_track);
	
	AutomationTree(QWidget *p_parent);
	~AutomationTree();

};



class AutomationTreeeDialog : public QDialog {
		
	Q_OBJECT
	
	Editor *editor;
	Track *track;
	
public slots:	
		
		
	void attempt_automation_add_slot(int p_index);	
	void attempt_automation_remove_slot(int p_index);	
		
public:
		
		
	AutomationTreeeDialog(QWidget *p_parent,Track *p_track,Editor *p_editor);
		
};


}

#endif
