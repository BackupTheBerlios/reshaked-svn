//
// C++ Interface: global_view_frame
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDGLOBAL_VIEW_FRAME_H
#define RESHAKEDGLOBAL_VIEW_FRAME_H

#include "interface/global_view.h"
#include <Qt/qscrollbar.h>
#include <Qt/qslider.h>
#include "interface/global_beatbar_column.h"
#include "interface/global_view_cursor.h"
#include <Qt/qframe.h>
#include "ui_blocks/pixmap_scrollbar.h"


namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class GlobalViewFrame : public QFrame {

	Q_OBJECT
				       
			
	Editor *editor;
	GlobalView *global_view;
	PixmapScrollBar *h_scroll;
	PixmapScrollBar *v_scroll;
	GlobalViewCursor *cursor_op;
	
	GlobalBeatBarColumn *beat_bar_column;
	MarkerColumn *marker_column;
	LoopColumn *loop_column;
	
public slots:
	void update();
	void block_list_changed_slot();
	void h_scollbar_changed_slot(int p_new_idx);
	void v_scollbar_changed_slot(int p_new_idx);
	void zoom_changed_slot(float p_to_val);
	void delete_blocks_slot(); //you just NEED IT
signals:
	void global_view_changed_blocks_signal();		
public:
	GlobalView *get_global_view();
	GlobalViewFrame(QWidget *p_parent,Editor *p_editor);
	~GlobalViewFrame();

};

}

#endif
