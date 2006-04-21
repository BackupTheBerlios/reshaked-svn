//
// C++ Implementation: pixmap_combo
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pixmap_combo.h"
#include <Qt/qmenu.h>
#include <Qt/qevent.h>
#include "typedefs.h"

namespace ReShaked {


void PixmapCombo::wheelEvent ( QWheelEvent * e )  {
	
	int prev=selected;
	if (e->delta()>0) 
		select_item( selected -1 );
	else if (e->delta()<0)
		select_item( selected +1 );
		
	if (prev!=selected) {
		item_selected(selected);
		item_selected_signal(selected);
	}
}


void PixmapCombo::item_selected(int p_to_item) {
	
	
}

void PixmapCombo::click_override() {
	
	if (string_list.empty())
		return;
	
	QList<QAction*> action_list;
	
	QAction *at=NULL;
	for (int i=0;i<string_list.size();i++) {
		QAction *qa = new QAction(string_list[i],this);
		qa->setCheckable(true);
		if (i==selected) {
			qa->setChecked(true);
			at=qa;
		}
		action_list.push_back(qa);
		qa->setData(i);
	}
	
	
	QAction *res = QMenu::exec(action_list,mapToGlobal( QPoint(0,height())));
	if (res) {
		
		int idx_selected=res->data().toInt();
		if (idx_selected>=0 && idx_selected<string_list.size()) {
			
			selected=idx_selected;
			update_internal();
			item_selected(selected);
			item_selected_signal(selected);
		}
		
	}
	
	foreach(I,action_list) {
		
		delete *I;
	}

		
}

		
		
void PixmapCombo::update_internal() {
			
	if (selected<0 || selected>=string_list.size())
		selected=0;
	if (string_list.size())
		set_text(string_list[selected]);
	else
		set_text("");
	
	
}
	
void PixmapCombo::clear() {

	string_list.clear();
	update_internal();
}
void PixmapCombo::add_item(QString p_item) {
	
	string_list.push_back(p_item);
	if (string_list.size()==1) {
		selected=0; //if first item inserted, select it
		update_internal();
	}
	
}

void PixmapCombo::set_item_text(int p_item,QString p_text) {
	
	ERR_FAIL_INDEX(p_item,string_list.size());
	string_list[p_item]=p_text;
	update_internal();
}
void PixmapCombo::select_item(int p_item){
	if (p_item==selected)
		return;
	if (p_item<0 || p_item>=string_list.size())
		return;
	selected=p_item;
	update_internal();
	
}
int PixmapCombo::get_selected() {
	
	
	return selected;	
}

int PixmapCombo::get_item_count() {
	
	return string_list.size();
}
PixmapCombo::PixmapCombo(QWidget *p_parent,QPixmap p_bg) :PixmapLabel(p_parent,p_bg) {
	
	selected=0;
	
}


PixmapCombo::~PixmapCombo()
{
}


}
