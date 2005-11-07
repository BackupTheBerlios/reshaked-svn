//
// C++ Implementation: blocklist_ui_list
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "blocklist_ui_list.h"
#include <Qt/qpushbutton.h>
#include <Qt/qlayout.h>
#include "blocklist_ui_pattern.h"
#include "blocklist_ui_automation.h"
#include <Qt/qlabel.h>

namespace ReShaked {


void BlockListUIList::update_track_list() {

	if (hbox)
		delete hbox; //all widgets die!
		
	hbox = new QWidget(this);
	hbox_layout = new QHBoxLayout(hbox);
	hbox->setLayout(hbox_layout);
	scrollarea->setWidget(hbox);
	scrollarea->setWidgetResizable(true);
	
	hbox->show();
	
	block_list_ui_list.clear();
			
	for (int i=0;i<song->get_track_count();i++) {
		
		
		SWITCH( song->get_track(i)->get_type_name() ) 
		
			CASE("pattern") {
				
				block_list_ui_list.push_back( new BlockListUI_Pattern(hbox,song,i) );
				hbox_layout->addWidget( block_list_ui_list [block_list_ui_list.size() -1] );
			
				
			}									
		END_SWITCH
		
		for (int j=0;j<song->get_track(i)->get_automation_count();j++) {
			
			//block_list_ui_list.push_back( new BlockListUI_Automation(hbox,song,i,j) );
			
		}
	}
	
	spacer = new QFrame(hbox);
	hbox_layout->addWidget( spacer );
	spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	hbox_layout->setSpacing(0);
	
	hbox->adjustSize();
	
}


BlockListUIList::BlockListUIList(QWidget *p_parent,Song *p_song) : QWidget (p_parent)
{
	
	
	song=p_song;
	QHBoxLayout *l = new QHBoxLayout(this);
	scrollarea = new QScrollArea(this);
	l->addWidget(scrollarea);;

	hbox=NULL;
	//CHBox *temp_hbox= new CHBox(scrollarea);
	//hbox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	//emp_hbox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	spacer=NULL;
	
}


BlockListUIList::~BlockListUIList()
{
}


}

