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
#include "track_top.h"
#include <Qt/qlabel.h>
#include <Qt/qscrollbar.h>

//@TODO SOFT SCROLL when ensuring cursor visible
namespace ReShaked {

void BlockListUIList::vscroll_track_list() {
	
	for (int i=0;i<block_list_ui_list.size();i++) {

		block_list_ui_list[i]->update_viewport_pos( editor->get_cursor().get_window_offset() );
	}
	printf("vscrolled\n");
	
}

static int get_pos_in_some_parent(QWidget *p_someparent, QWidget *p_widget) {
	
	QWidget *from=p_widget;
	int xmap=0;
	
	while (true) {
		
		QWidget *parent=from->parentWidget();
		xmap+=from->pos().x();
		if (parent==p_someparent) {
			
			return xmap;
		}
		if (parent==NULL) {
			
			return -1;
		}
		
		from=parent;
	}	
}


void BlockListUIList::ensure_cursor_visible() {

	ERR_FAIL_COND(hbox==NULL);

	int current_blocklist=editor->get_current_blocklist();
	ERR_FAIL_INDEX(current_blocklist,block_list_ui_list.size());

	int hbox_w=hbox->width();
	int vp_width=scrollarea->viewport()->width();
	int hbox_ofs=-hbox->pos().x();
	int blocklist_pos=get_pos_in_some_parent(hbox,block_list_ui_list[current_blocklist]);
	ERR_FAIL_COND(blocklist_pos==-1);
	int blocklist_width=block_list_ui_list[current_blocklist]->width();

	int new_hbox_ofs;
	if (blocklist_pos<hbox_ofs) {

		new_hbox_ofs=blocklist_pos;
	} else if ( (blocklist_pos+blocklist_width) > (hbox_ofs+vp_width) ) {

		new_hbox_ofs=(blocklist_pos+blocklist_width)-vp_width;
	} else {
		return; //nothing to adjust
	}

	int scroll_range=scrollarea->horizontalScrollBar()->maximum();
	int scroll_pos=new_hbox_ofs*scroll_range/(hbox_w-vp_width);

	scrollarea->horizontalScrollBar()->setValue( scroll_pos );

}
void BlockListUIList::cursor_changed_blocklist() {

	/* find new blocklist, adjust focus */

	int current_blocklist=editor->get_current_blocklist();
	ERR_FAIL_INDEX(current_blocklist,block_list_ui_list.size());

	block_list_ui_list[current_blocklist]->setFocus();

	ensure_cursor_visible();

	printf("some data: poshbox %i,%i , poswidget %i,%i - hboxw %i vpw %i -sm, %i, sp %i\n",hbox->pos().x(),hbox->pos().y(),block_list_ui_list[current_blocklist]->pos().x(),block_list_ui_list[current_blocklist]->pos().y(),hbox->width(),scrollarea->viewport()->width(),scrollarea->horizontalScrollBar()->maximum(), scrollarea->horizontalScrollBar()->value());

}
void BlockListUIList::repaint_track_list() {

	for (int i=0;i<block_list_ui_list.size();i++) {

		block_list_ui_list[i]->update();
	}
	row_display->update();
	
	printf("repainted\n");

}
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

	for (int i=0;i<editor->get_song()->get_track_count();i++) {

		QWidget *vb= new QWidget(hbox);
		hbox_layout->addWidget(vb);	
		
		QVBoxLayout *vl=new QVBoxLayout(vb);
		vb->setLayout(vl);
		
		TrackTop *top = new TrackTop(vb,editor,i);
		vl->addWidget(top);
		
		QWidget *hb = new QWidget(vb);
		QHBoxLayout *hl = new QHBoxLayout(hb);
		hb->setLayout(hl);
		vl->addWidget(hb);
		
		//hb->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
		

		SWITCH( editor->get_song()->get_track(i)->get_type_name() )

			CASE("pattern") {

				block_list_ui_list.push_back( new BlockListUI_Pattern(hb,editor,i) );
				hl->addWidget(block_list_ui_list [block_list_ui_list.size() -1] );
				
			
		//	hl->addWidget(new QPushButton("heh",hb ));
				//hbox_layout->addWidget( block_list_ui_list [block_list_ui_list.size() -1] );
			}
		END_SWITCH

		for (int j=0;j<editor->get_song()->get_track(i)->get_automation_count();j++) {

			block_list_ui_list.push_back( new BlockListUI_Automation(hb,editor,i,j) );
			hl->addWidget(block_list_ui_list [block_list_ui_list.size() -1] );

		}
		
		vl->setMargin(0);
		vl->setSpacing(0);
		hl->setMargin(0);
		hl->setSpacing(0);
		vb->show();
	}

	spacer = new QFrame(hbox);
	hbox_layout->addWidget( spacer );
	spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

	hbox_layout->setSpacing(0);
	hbox_layout->setMargin(0);

	hbox->adjustSize();


}


BlockListUIList::BlockListUIList(QWidget *p_parent,Editor *p_editor) : QWidget (p_parent)
{



	editor=p_editor;
	QHBoxLayout *l = new QHBoxLayout(this);
	row_display = new RowListDisplay(this,&editor->get_cursor());
	l->addWidget(row_display);

	scrollarea = new QScrollArea(this);
	l->addWidget(scrollarea);;
	scrollarea->setFrameStyle(QFrame::NoFrame);
	scrollarea->viewport()->setContentsMargins(0,0,0,0);
	scrollarea->setContentsMargins(0,0,0,0);


	//scrollarea->setFrameRect(QRect(0, 0, 0, 0 ));

	//scrollarea->setFrameRect(QRect(0, 0, 0, 0 ));

	hbox=NULL;
	//CHBox *temp_hbox= new CHBox(scrollarea);
	//hbox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	//emp_hbox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	spacer=NULL;

	scrollarea->setFocusPolicy(Qt::NoFocus);
	scrollarea->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

	
	l->setSpacing(0);
}


BlockListUIList::~BlockListUIList()
{
}


}

