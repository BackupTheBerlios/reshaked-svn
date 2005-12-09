//
// C++ Interface: global_view_cursor
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDGLOBAL_VIEW_CURSOR_H
#define RESHAKEDGLOBAL_VIEW_CURSOR_H


#include <Qt/qpushbutton.h>
#include <ui_blocks/helpers.h>
#include <interface/global_view.h>

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class GlobalViewCursor : public CHBox {
				       
	Q_OBJECT
public:					       

	enum {
		MAX_MODES=4
	};
	
	GlobalView::EditMode mode;
	
	QPushButton *mode_button[MAX_MODES];
	QPushButton *delete_selected;
	void add_button(GlobalView::EditMode p_mode, PixmapList p_pixmap );
	
signals:	
	
	void edit_mode_changed_signal(GlobalView::EditMode p_mode);
	void delete_clicked_signal();
public slots:	
	void mode_selected(int p_mode);
	
public:
	
	GlobalViewCursor(QWidget *p_parent);
	~GlobalViewCursor();

};

}

#endif
