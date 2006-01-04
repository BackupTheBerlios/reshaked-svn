//
// C++ Implementation: automation_dialog
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "automation_dialog.h"
#include <Qt/qhash.h>
#include "ui_blocks/helpers.h"

namespace ReShaked {


bool AutomationPopup::check_action(QAction *p_action)  {


	QActionAutomation *item = dynamic_cast<QActionAutomation*>(p_action);
				
	if (!item)
		return false;
	
	bool checked=item->isChecked();
	int idx=item->get_property_idx();
	if (idx==-1)
		return true;;
	
	bool has_auto=track->has_property_visible_automation( idx );
	
	if (checked==has_auto)
		return true; //nothing to do
	
	if (checked)
		attempt_automation_add_signal(idx);	
	else
		attempt_automation_remove_signal(idx);		
	
	return true;
}

void AutomationPopup::rebuild() {
	
	typedef QHash<QString,QMenu*> PathMap;
	PathMap hashmap;
	
	clear();
		
	for (int i=0;i<track->get_property_count();i++) {
		
		QString path=QStrify(track->get_property_visual_path( i));
		
		QString dir=path.left( path.lastIndexOf("/") );
		QString name=path.right( path.length()-(path.lastIndexOf("/")+1) );
		
		//printf("Dir %s, File %s\n",dir.toAscii().data(),name.toAscii().data());
		
		/* find if we have dir! */
		
		QActionAutomation * item;
		if (dir=="") { //toplevelAutomationTree( 
			
			item  = new QActionAutomation(this,i);
			addAction(item);

		} else {
			
			// have the path for this dir somewhere?
			PathMap::iterator I =hashmap.find(dir);
			if (I!=hashmap.end()) {
				item = new QActionAutomation(I.value(),i);
				I.value()->addAction(item);
				
			} else {
				
				/* build path */
				int from=1;
				QString dir2=dir+"/"; //so it finds the last / too
				QMenu*parent=NULL;
				QMenu*current=NULL;
				
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
						
						QString nodename=auxp.right( auxp.length()-(auxp.lastIndexOf("/")+1) );
						
						QMenu* node;
						if (parent) {
							node= new QMenu(parent);
							parent->addMenu(node)->setText(nodename);

						} else {
							node= new QMenu(this);
							addMenu(node)->setText(nodename);
							

						}
						
						//QObject::connect(node,SIGNAL(triggered(QAction*)),this,SLOT(action_activated(QAction*)));						
						//node->setText(0,nodename+"/");
						
						parent=node;
						hashmap[auxp]=node;
						printf("adding %s, nodename: %s\n",auxp.toAscii().data(),nodename.toAscii().data());
						
					} else { //exists
						 
						parent=I.value();
					}
				}
				
				item = new QActionAutomation(parent,i);
				parent->addAction(item);
			}
				
				
		}
		
		if (track->get_property_automation(i)->get_block_count()) {
				
			item->setText( QStrify( track->get_property( i)->get_caption()+"(*)" ) );
		
		} else {
		
			item->setText( QStrify( track->get_property( i)->get_caption() ) );
		
		}
		item->setCheckable(true);
		
		//item->setText( 0, QStrify( track->get_property( i)->get_caption() ) );
		//item->setText( 1, QStrify( track->get_property( i)->get_text_value( track->get_property( i)->get_min() ) ) );
		//item->setText( 2, QStrify( track->get_property( i)->get_text_value( track->get_property( i)->get_max() ) ) );
		item->setChecked(track->has_property_visible_automation(i)?Qt::Checked:Qt::Unchecked);
		
		
	}
	
}

AutomationPopup::AutomationPopup(QWidget *p_parent,Track *p_track) : QMenu(p_parent)
{
	
	track=p_track;
	//QObject::connect(this,SIGNAL(triggered(QAction*)),this,SLOT(action_activated(QAction*)));
}


AutomationPopup::~AutomationPopup()
{
}


}
