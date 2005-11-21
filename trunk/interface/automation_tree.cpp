//
// C++ Implementation: automation_tree
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "automation_tree.h"
#include <Qt/qhash.h>
#include <Qt/qheaderview.h>
namespace ReShaked {



String AutomationTree::QTreeAutomationItem::get_path() {
	
	return path;
}
AutomationTree::QTreeAutomationItem::QTreeAutomationItem(QTreeWidgetItem * p_parent,String p_path) :QTreeWidgetItem(p_parent) {
	
	path=p_path;
}

AutomationTree::QTreeAutomationItem::QTreeAutomationItem(QTreeWidget * p_parent,String p_path) :QTreeWidgetItem(p_parent) {
	
	path=p_path;	
}


void AutomationTree::update_automation_tree() {
	
	ERR_FAIL_COND(!track);
			
	typedef QHash<QString,QTreeWidgetItem*> PathMap;
	PathMap hashmap;
	
	tree->clear();
	
	for (int i=0;i<track->get_property_count();i++) {
		
		QString path=QStrify(track->get_property_path( i));
		
		QString dir=path.left( path.lastIndexOf("/") );
		QString name=path.right( path.length()-(path.lastIndexOf("/")+1) );
		
		//printf("Dir %s, File %s\n",dir.toAscii().data(),name.toAscii().data());
		
		/* find if we have dir! */
		
		QTreeAutomationItem * item;
		if (dir=="") { //toplevelAutomationTree( 
			
			item  = new QTreeAutomationItem(tree,track->get_property_path( i));

		} else {
			
			// have the path for this dir somewhere?
			PathMap::iterator I =hashmap.find(dir);
			if (I!=hashmap.end()) {
				item = new QTreeAutomationItem(I.value(),track->get_property_path( i));
				
			} else {
				
				/* build path */
				int from=1;
				QString dir2=dir+"/"; //so it finds the last / too
				QTreeWidgetItem*parent=NULL;
				QTreeWidgetItem*current=NULL;
				
				while (true) {
					
					if (from==dir2.length())
						break;
					int idx=dir2.indexOf("/",from);
					if (idx==-1)
						break;
					QString auxp=path.left( idx );
					from=idx+1; //so it skips the /
					printf("searching %s\n",auxp.toAscii().data());
					I=hashmap.find(auxp);
					if (I==hashmap.end()) { //we must create it!
						
						QTreeWidgetItem *node;
						if (parent) {
							node= new QTreeWidgetItem(parent);

						} else {
							node= new QTreeWidgetItem(tree);

						}
						
						QString nodename=auxp.right( auxp.length()-(auxp.lastIndexOf("/")+1) );
						node->setText(0,nodename+"/");
						
						parent=node;
						hashmap[auxp]=node;
						printf("adding %s, nodename: %s\n",auxp.toAscii().data(),nodename.toAscii().data());
						
					} else { //exists
						 
						parent=I.value();
					}
				}
				
				item = new QTreeAutomationItem(parent,track->get_property_path( i));
			}
				
				
		}
		
		item->setText( 0, QStrify( track->get_property( i)->get_caption() ) );
		item->setText( 1, QStrify( track->get_property( i)->get_text_value( track->get_property( i)->get_min() ) ) );
		item->setText( 2, QStrify( track->get_property( i)->get_text_value( track->get_property( i)->get_max() ) ) );
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
		item->setCheckState(0,track->has_property_automation(i)?Qt::Checked:Qt::Unchecked);
//		for (int j=0;j
		
	}
}
	


void AutomationTree::set_track(Track *p_track) {
	
	track=p_track;
	update_automation_tree();
	
}

void AutomationTree::item_clicked( QTreeWidgetItem * p_item, int column ) {
	
	QTreeAutomationItem *item = dynamic_cast<QTreeAutomationItem*>(p_item);
				
	if (!item)
		return;
	bool checked=(item->checkState(0)==Qt::Checked);
	int idx=track->get_idx_by_path( item->get_path() );
	if (idx==-1)
		return;
	
	bool has_auto=track->has_property_automation( idx );
	
	if (checked==has_auto)
		return; //nothing to do
	
	if (checked)
		attempt_automation_add_signal(item->get_path());	
	else
		attempt_automation_remove_signal(item->get_path());	


}

AutomationTree::AutomationTree(QWidget *p_parent) : CHBox(p_parent) {
	
	tree= new QTreeWidget(this);
	tree->setColumnCount(2);
	QStringList strlist;
	strlist << "Automatable Param" << "Min Value" << "Max Value";
	tree->setHeaderLabels(strlist);
	track=NULL;
	tree->header()->setResizeMode(0,QHeaderView::Stretch);
	
	QObject::connect(tree,SIGNAL(itemActivated(QTreeWidgetItem*,int)),this,SLOT(item_activated( QTreeWidgetItem*, int )));
	QObject::connect(tree,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(item_clicked( QTreeWidgetItem*, int )));
	QObject::connect(tree,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(item_pressed( QTreeWidgetItem*, int )));
}


AutomationTree::~AutomationTree()
{
}


}
