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
#include "widgets/scroll_bar.h"
#include "widgets/slider.h"
#include "containers/grid_container.h"
#include "containers/box_container.h"
#include "interface/global_beatbar_column.h"
#include "interface/global_view_cursor.h"

using namespace GUI;


namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class GlobalViewFrame : public VBoxContainer {

		       
			
	Editor *editor;
	GlobalView *global_view;
	HScrollBar *h_scroll;
	VScrollBar *v_scroll;
	GlobalViewCursor *cursor_op;
	
	GlobalBeatBarColumn *beat_bar_column;
	MarkerColumn *marker_column;
	LoopColumn *loop_column;
	
	
	void global_view_updated();
public: // slots
	void update_all();
	void block_list_changed_slot();
	void h_scollbar_changed_slot(double p_new_idx);
	void v_scollbar_changed_slot(double p_new_idx);
	void zoom_changed_slot(float p_to_val);
	void delete_blocks_slot(); //you just NEED IT

	Signal<> global_view_changed_blocks_signal;
public:
	
	void global_view_update();		
	
	GlobalView *get_global_view();
	GlobalViewFrame(Editor *p_editor);
	~GlobalViewFrame();

};

}

#endif
