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
#include "ui_blocks/helpers.h"
#include <interface/global_view.h>
#include "ui_blocks/pixmap_button.h"
#include "ui_blocks/pixmap_label.h"
#include "ui_blocks/pixmap_slider.h"

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
	
	PixmapButton *mode_button[MAX_MODES];
	PixmapButton *delete_selected;
	PixmapButton *select_linked;
	PixmapButton *unlink_selected;
	PixmapSlider *zoom;
	void add_button(GlobalView::EditMode p_mode, PixmapsList p_pixmap_fg, PixmapsList p_pixmap_bg,QString p_hint);
	
	
signals:	
	
	void repeat_set_signal();
	void repeat_unset_signal();
	void edit_mode_changed_signal(int);
	void delete_clicked_signal();
	void select_linked_signal();
	void unlink_selected_signal();
	void zoom_changed(float p_to_val);	
public slots:	
	void mode_selected(int p_mode);

	
public:
	
	GlobalViewCursor(QWidget *p_parent);
	~GlobalViewCursor();

};

}

#endif
