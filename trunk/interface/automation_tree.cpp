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
#include <Qt/qpushbutton.h>
#include <Qt/qlayout.h>

namespace ReShaked {



int AutomationTree::QTreeAutomationItem::get_property_idx() {
	
	return property_idx;
}
AutomationTree::QTreeAutomationItem::QTreeAutomationItem(QTreeWidgetItem * p_parent,int p_property_idx) :QTreeWidgetItem(p_parent) {
	
	property_idx=p_property_idx;
}

AutomationTree::QTreeAutomationItem::QTreeAutomationItem(QTreeWidget * p_parent,int p_property_idx) :QTreeWidgetItem(p_parent) {
	
	property_idx=p_property_idx;
}


void AutomationTree::update_automation_tree() {
	
	ERR_FAIL_COND(!track);
			
	typedef QHash<QString,QTreeWidgetItem*> PathMap;
	PathMap hashmap;
	
	tree->clear();
	item_array.clear();
	
	for (int i=0;i<track->get_property_count();i++) {
		
		QString path=QStrify(track->get_property_visual_path( i));
		
		QString dir=path.left( path.lastIndexOf("/") );
		QString name=path.right( path.length()-(path.lastIndexOf("/")+1) );
		
		//printf("Dir %s, File %s\n",dir.toAscii().data(),name.toAscii().data());
		
		/* find if we have dir! */
		
		QTreeAutomationItem * item;
		if (dir=="") { //toplevelAutomationTree( 
			
			item  = new QTreeAutomationItem(tree,i);

		} else {
			
			// have the path for this dir somewhere?
			PathMap::iterator I =hashmap.find(dir);
			if (I!=hashmap.end()) {
				item = new QTreeAutomationItem(I.value(),i);
				
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
						
						tree->expandItem(node);
						QString nodename=auxp.right( auxp.length()-(auxp.lastIndexOf("/")+1) );
						node->setText(0,nodename+"/");
						
						parent=node;
						hashmap[auxp]=node;
						printf("adding %s, nodename: %s\n",auxp.toAscii().data(),nodename.toAscii().data());
						
					} else { //exists
						 
						parent=I.value();
					}
				}
				
				item = new QTreeAutomationItem(parent,i);
			}
				
				
		}
		
		QString captionstr=QStrify( track->get_property( i)->get_caption());

		if (captionstr.lastIndexOf("/",-1)>=0) {
			
			captionstr=captionstr.right( captionstr.length()-(captionstr.lastIndexOf("/")+1));
		}
		
		if (track->get_property_automation(i)->get_block_count()) {
				
			item->setText( 0, captionstr+"(*)"  );
		
		} else {
		
			item->setText( 0, captionstr );
		
		}
		
		//item->setText( 0, QStrify( track->get_property( i)->get_caption() ) );
		item->setText( 1, QStrify( track->get_property( i)->get_text_value( track->get_property( i)->get_min() ) ) );
		item->setText( 2, QStrify( track->get_property( i)->get_text_value( track->get_property( i)->get_max() ) ) );
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
		item->setCheckState(0,track->has_property_visible_automation(i)?Qt::Checked:Qt::Unchecked);
		tree->expandItem(item);
		item_array.push_back(item);
//		for (int j=0;j
		
	}
}
	

void AutomationTree::update_item_status() {
	
	if (!track)
		return;
	
	
	for (int i=0;i<item_array.size();i++) {
		
		int idx=item_array[i]->get_property_idx();
		if (idx<0)
			continue;
		
		if (track->get_property_automation(i)->get_block_count()) {
				
			item_array[i]->setText( 0, QStrify( track->get_property( idx)->get_caption()+"(*)" ) );
		
		} else {
		
			item_array[i]->setText( 0, QStrify( track->get_property( idx)->get_caption() ) );
		
		}
		
		item_array[i]->setCheckState(0,track->has_property_visible_automation(idx)?Qt::Checked:Qt::Unchecked);
	}
	
}


void AutomationTree::set_track(Track *p_track) {
	
	track=p_track;
	update_automation_tree();
	
	
}

void AutomationTree::item_clicked( QTreeWidgetItem * p_item, int column ) {
	
	QTreeAutomationItem *item = dynamic_cast<QTreeAutomationItem*>(p_item);
	printf("callbacko\n");				
	if (!item)
		return;
	bool checked=(item->checkState(0)==Qt::Checked);
	int idx=item->get_property_idx();
	if (idx==-1)
		return;
	
	bool has_auto=track->has_property_visible_automation( idx );
	
	if (checked==has_auto)
		return; //nothing to do
	
	printf("checked %i\n",idx);
	if (checked)

		attempt_automation_add_signal(idx);	
	else
		attempt_automation_remove_signal(idx);	


	
}

AutomationTree::AutomationTree(QWidget *p_parent) : CHBox(p_parent) {
	
	tree= new QTreeWidget(this);
	tree->setColumnCount(2);
	QStringList strlist;
	strlist << "Automatable Param" << "Min Value" << "Max Value";
	tree->setHeaderLabels(strlist);
	track=NULL;
	tree->header()->setResizeMode(0,QHeaderView::Stretch);
	
//	QObject::connect(tree,SIGNAL(itemActivated(QTreeWidgetItem*,int)),this,SLOT(item_activated( QTreeWidgetItem*, int )));
	QObject::connect(tree,SIGNAL(itemChanged(QTreeWidgetItem* , int)),this,SLOT(item_clicked( QTreeWidgetItem*, int )));
}


AutomationTree::~AutomationTree()
{
}


void AutomationTreeeDialog::attempt_automation_add_slot(int p_index) {
	
	printf("add %i\n",p_index);
	editor->show_automation( p_index, track );
}
void AutomationTreeeDialog::attempt_automation_remove_slot(int p_index) {
	
	printf("rem %i\n",p_index);
	editor->hide_automation( p_index, track );
	
}

AutomationTreeeDialog::AutomationTreeeDialog(QWidget *p_parent,Track *p_track,Editor *p_editor) : QDialog(p_parent) {
	
	editor=p_editor;
	track=p_track;	
	setLayout( new QVBoxLayout(this) );
	
	AutomationTree *at = new AutomationTree(this);
	layout()->addWidget(at);
	
	at->set_track( track );
	
	QObject::connect(at,SIGNAL(attempt_automation_add_signal( int )),this,SLOT(attempt_automation_add_slot( int )));
	QObject::connect(at,SIGNAL(attempt_automation_remove_signal( int )),this,SLOT(attempt_automation_remove_slot( int )));
	
	CHBox *hb = new CHBox(this);
	layout()->addWidget(hb);
	
	(new QFrame(hb))->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	
	QPushButton *ab = new QPushButton("Accept",hb); //accept button
	QObject::connect(ab,SIGNAL(clicked()),this,SLOT(accept()));
	
	(new QFrame(hb))->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	
	setWindowTitle("Automations of '"+QStrify(p_track->get_name())+"'");
	setMinimumSize(650,400);
	
	
}

}
