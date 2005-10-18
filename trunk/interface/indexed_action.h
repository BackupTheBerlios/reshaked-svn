//
// File: /home/red/coding/svn/reshaked2/interface/indexed_action.cpp
// Created by: User <Email>
// Created on: Sun Oct 16 22:03:32 2005
//

#ifndef INDEXED_ACTION_H
#define INDEXED_ACTION_H

#include <Qt/qaction.h>

namespace ReShaked {

class IndexedAction : public QAction {
	
	Q_OBJECT
	
	int index;
private slots:
	void triggered_slot();
signals:

	void selected_index_signal(int);
public:
	
	IndexedAction(int p_index,QString p_text,QPixmap p_pix,QWidget *p_parent);
	IndexedAction(int p_index,QString p_text,QWidget *p_parent);
	~IndexedAction();
};

}
#endif
