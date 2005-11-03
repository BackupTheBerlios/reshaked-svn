//
// File: global_view.h
// Created by: User <Email>
// Created on: Sun Oct 16 20:40:48 2005
//


#ifndef _GLOBALVIEW_H_
#define _GLOBALVIEW_H_

#include <Qt/qwidget.h>
#include "engine/song.h"
#include <set>
#include <list>

namespace ReShaked {
	
class GlobalView : public QWidget {
	
	Song *song;
	void paintEvent(QPaintEvent *p);
	
	enum {
		MAX_LISTS=1000
	}; //dont think there will be more than that..
	
	struct DisplayParams {
		float zoom_width;
		float zoom_height;
		float ofs_x,ofs_y;
	} display;
	
	struct MouseData {

		enum BlockPositionAction {
			POS_NOBLOCK,
			POS_OVER,
			POS_RESIZE_BEG,
			POS_RESIZE_END
		};
		
		bool deselecting; ///< Deslecting flag, gets cleared on mouse motion 
		int deselecting_block;
		int deselecting_list;
		bool shift_when_deselecting;
		bool selecting;
		int border_resize_grab_pixels;	
		BlockPositionAction action_at_selecting;
		bool resizing;		
		float click_x_from;
		float click_y_from;
		
		
	} mouse_data;
	
	struct MovingBlock {

		struct Element {
			int list;
			int block;		
			BlockList::Block *block_ptr; //litle hack for ease of code
		};
		
		
		enum Operation {
			OP_CREATE_NEW,
			OP_MOVE,
			OP_COPY,
			OP_COPY_LINK,
		};
			
		
		Operation operation;
		std::list<Element> moving_element_list;
		bool snap_to_beat;
		bool moving;
		float current_x;
		float current_y;
		
		
			
	} moving_block;

	void mouseMoveEvent ( QMouseEvent * e );
	void mousePressEvent ( QMouseEvent * e );
	void mouseReleaseEvent ( QMouseEvent * e );
	MouseData::BlockPositionAction get_block_position_action(int p_blocklist,Tick p_pos);
	//selection
	
	std::set<int> selection;
	
	//helpers
	BlockList* get_block_list(int p_idx);
	int get_block_list_count();
	float get_block_list_offset(int p_idx);
	float get_width();
	float get_block_list_width(BlockList *p_block);
	int get_block_list_at_offset(float p_offset);
	QColor get_block_list_color(BlockList *p_block);
	void add_block_to_selection(int p_list_index,int p_block);
	void remove_block_from_selection(int p_list_index,int p_block);
	void clear_selection();
	bool is_block_selected(int p_list,int p_block);
	bool get_screen_to_blocklist_and_tick( int p_x, int p_y, int *p_blocklist, Tick *p_tick );
	bool get_click_location( int p_x, int p_y, int *p_blocklist, int *p_block );
	void compute_moving_block_list();
	void commit_moving_block_list();
	bool is_block_being_moved(int p_list,int p_block);
	bool get_moving_block_pos_and_status(int p_list,int p_block,int &p_dst_list,Tick &p_dst_tick, float &p_free_x, bool &p_allowed); //if true invalid
	
	void paint_block(QPainter& p,int p_x,int p_y,int p_list,int p_block,bool p_drawover=false,bool p_notpossible=false);
	
	
public:
	GlobalView(QWidget *p_widget,Song *p_song);
	~GlobalView();

	
};

};
#endif	//_GLOBALVIEW_H_
