//
// File: /home/red/coding/svn/reshaked2/interface/indexed_action.cpp
// Created by: User <Email>
// Created on: Sun Oct 16 22:03:32 2005
//

#include "indexed_action.h"

namespace ReShaked {

void IndexedAction::triggered_slot() {
	
	
	selected_index_signal(index);
}
	
IndexedAction::IndexedAction(int p_index,QString p_text,QPixmap p_pix,QWidget *p_parent) :
	QAction (p_pix,p_text,p_parent) {
	
	index=p_index;
	QObject::connect(this,SIGNAL(triggered()),this,SLOT(triggered_slot()));
		
}
	

IndexedAction::IndexedAction(int p_index,QString p_text,QWidget *p_parent) :
	QAction(p_text,p_parent) {
	
	index=p_index;
	QObject::connect(this,SIGNAL(triggered()),this,SLOT(triggered_slot()));
}

IndexedAction::~IndexedAction()
{
	// TODO: put destructor code here
}

}
