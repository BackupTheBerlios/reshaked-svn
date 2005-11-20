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
#include "engine/track.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class AutomationTree : public CHBox {
			    
	Q_OBJECT 
					    
	class QTreeAutomationItem : public QTreeWidgetItem {
				    
		String path;
	public:
		String get_path();
		QTreeAutomationItem(QTreeWidgetItem * p_parent,String p_path); //construct with this parent
		QTreeAutomationItem(QTreeWidget * p_parent,String p_path); //construct with this parent
				    
	};				    
			    
	QTreeWidget *tree;
	Track *track;
	
public slots:	
	

	void item_clicked( QTreeWidgetItem * item, int column );
signals:
		
	void attempt_automation_add_signal(String p_path);	
	void attempt_automation_remove_signal(String p_path);	
public:
	void update_automation_tree();

	void set_track(Track *p_track);
	
	AutomationTree(QWidget *p_parent);
	~AutomationTree();

};

}

#endif
