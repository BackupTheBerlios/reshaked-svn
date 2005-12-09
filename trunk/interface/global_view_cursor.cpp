//
// C++ Implementation: global_view_cursor
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "global_view_cursor.h"
#include <Qt/qframe.h>
namespace ReShaked {

void GlobalViewCursor::add_button(GlobalView::EditMode p_mode,PixmapList p_pixmap ) {

	QPixmap px = GET_QPIXMAP(p_pixmap);
		
	mode_button[p_mode]= new QPushButton(this);
	mode_button[p_mode]->setIcon(px);
	mode_button[p_mode]->setIconSize(px.size());
	mode_button[p_mode]->setCheckable(true);
	mode_button[p_mode]->setFocusPolicy(Qt::NoFocus);
	mode_button[p_mode]->setFlat(true);
	mode_button[p_mode]->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	connect_bind_int(mode_button[p_mode],SIGNAL(clicked()),this,SLOT(mode_selected(int)),p_mode);

}

void GlobalViewCursor::mode_selected(int p_mode) {
	
	for (int i=0;i<MAX_MODES;i++) {
		
		if (i==p_mode) 
			mode_button[i]->setChecked(true);
		else
			mode_button[i]->setChecked(false);
	}
	mode=(GlobalView::EditMode)p_mode;
	edit_mode_changed_signal( mode );
}

GlobalViewCursor::GlobalViewCursor(QWidget *p_parent) : CHBox(p_parent) {
	
	add_button(GlobalView::EDIT_MODE_SELECT,ICON_GLOBAL_VIEW_SELECT_BLOCK);
	add_button(GlobalView::EDIT_MODE_ADD_BLOCK,ICON_GLOBAL_VIEW_ADD_BLOCK);
	add_button(GlobalView::EDIT_MODE_COPY_BLOCK,ICON_GLOBAL_VIEW_COPY_BLOCK);
	add_button(GlobalView::EDIT_MODE_COPY_LINK_BLOCK,ICON_GLOBAL_VIEW_COPY_LINK_BLOCK);
	delete_selected=new QPushButton(this);
	QPixmap delete_px=GET_QPIXMAP(ICON_GLOBAL_VIEW_ERASE_BLOCK);
	delete_selected->setIcon(delete_px);
	delete_selected->setIconSize(delete_px.size());
	delete_selected->setFocusPolicy(Qt::NoFocus);
	delete_selected->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	QObject::connect(delete_selected,SIGNAL(clicked()),this,SIGNAL(delete_clicked_signal()));
	
	(new QFrame(this))->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);	
	layout()->setMargin(2);
	layout()->setSpacing(0);
	mode_selected(0);
}


GlobalViewCursor::~GlobalViewCursor()
{
}


}
