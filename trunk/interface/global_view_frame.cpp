//
// C++ Implementation: global_view_frame
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "global_view_frame.h"
#include "widgets/label.h"

namespace ReShaked {


void GlobalViewFrame::h_scollbar_changed_slot(double p_new_idx) {
	
		
	global_view->set_pixel_h_offset( (int)p_new_idx );
}

void GlobalViewFrame::v_scollbar_changed_slot(double p_new_idx) {
	
	global_view->set_pixel_v_offset( (int)p_new_idx );
}

void GlobalViewFrame::block_list_changed_slot() {
	
		
	if (global_view->get_total_pixel_width()<=global_view->get_width()) {
		
		h_scroll->hide();
		
	} else  {
		h_scroll->get_range()->set_max(global_view->get_total_pixel_width());
		
		h_scroll->get_range()->set_page(global_view->get_width());
		h_scroll->get_range()->set( global_view->get_pixel_h_offset() );
		
		h_scroll->show();
		h_scroll->get_range()->set_step(global_view->get_width()/20);
	}

	int v_range=(global_view->get_total_pixel_height()*2);
	if (v_range<(global_view->get_height()*2))
		v_range=global_view->get_height()*2; //hack so there is always space to work with
	
	v_scroll->get_range()->set_max(v_range);	

	v_scroll->get_range()->set( global_view->get_pixel_v_offset() );
	v_scroll->get_range()->set_page(global_view->get_height());
	v_scroll->get_range()->set_step(global_view->get_height()/20);

	
	global_view_changed_blocks_signal.call();
}

void GlobalViewFrame::zoom_changed_slot(float  p_to_val) {
	
	global_view->set_zoom( p_to_val );
	
}

void GlobalViewFrame::update_all() {
	
	block_list_changed_slot();
	global_view->update();	
}

GlobalView *GlobalViewFrame::get_global_view() {
	
	return global_view;
}

void GlobalViewFrame::delete_blocks_slot() {
	
	global_view->delete_selected_blocks();	
}

GlobalViewFrame::GlobalViewFrame(Editor *p_editor)  {
	
	editor=p_editor;
	
	cursor_op= add(new GlobalViewCursor);
	
	
	HBoxContainer *hb = add( new HBoxContainer, 1 );
	

	
//	marker_column = hb->add( new MarkerColumn(editor) );
	//loop_column = hb->add( new LoopColumn(editor) );
	//beat_bar_column = hb->add( new GlobalBeatBarColumn(editor) );
	
	VBoxContainer *gv_vbox = hb->add( new VBoxContainer,1 );
	
	global_view = gv_vbox->add( new GlobalView( p_editor),1 );
	
//	marker_column->set_global_view( global_view );
	//loop_column->set_global_view( global_view );
	//beat_bar_column->set_global_view( global_view );


	v_scroll = hb->add( new VScrollBar );
	h_scroll = gv_vbox->add( new HScrollBar );
	
	global_view->resized_signal.connect(this,&GlobalViewFrame::block_list_changed_slot);
	
	h_scroll->get_range()->value_changed_signal.connect(this,&GlobalViewFrame::h_scollbar_changed_slot);
	v_scroll->get_range()->value_changed_signal.connect(this,&GlobalViewFrame::v_scollbar_changed_slot);
	
	cursor_op->zoom_changed_signal.connect(this,&GlobalViewFrame::zoom_changed_slot);
	
	cursor_op->edit_mode_changed_signal.connect(global_view,&GlobalView::set_edit_mode);
	
	cursor_op->delete_clicked_signal.connect(global_view,&GlobalView::delete_selected_blocks);
	
	cursor_op->select_linked_signal.connect(global_view,&GlobalView::select_linked_slot);
	
	cursor_op->unlink_selected_signal.connect(global_view,&GlobalView::unlink_selected_slot);
	
	cursor_op->repeat_set_signal.connect(global_view,&GlobalView::block_repeat_set_slot);
	
	cursor_op->repeat_unset_signal.connect(global_view,&GlobalView::block_repeat_unset_slot);
	
}


GlobalViewFrame::~GlobalViewFrame()
{
}


}
