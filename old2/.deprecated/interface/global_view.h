//
// File: global_view.h
// Created by: User <Email>
// Created on: Sun Oct 16 20:40:48 2005
//


#ifndef _GLOBALVIEW_H_
#define _GLOBALVIEW_H_

#include <Qt/qwidget.h>
#include "editor/editor.h"
#include <set>
#include <list>
#include "interface/visual_settings.h"

namespace ReShaked {


class GlobalView : public QWidget {

	Q_OBJECT
	
public:			
	
	enum EditMode {
		EDIT_MODE_SELECT,
		EDIT_MODE_ADD_BLOCK,
		EDIT_MODE_COPY_BLOCK,
		EDIT_MODE_COPY_LINK_BLOCK,
	};
private:
	
	enum CurrentCursor {
		CURSOR_POINT,
		CURSOR_ADD,
		CURSOR_RESIZE,
		CURSOR_ERASE,
	};
	
	CurrentCursor current_cursor;
	Song *song;
	Editor *editor;
	void paintEvent(QPaintEvent *p);

	enum {
		MAX_LISTS=1000,
		BLOCK_SPACING=3
	}; //dont think there will be more than that..

	
	struct DisplayParams {
		float zoom_width;
		float zoom_height;
		float ofs_x,ofs_y;
	} display;
	
	EditMode edit_mode;

	struct MouseData {

		enum BlockPositionAction {
			POS_NOBLOCK,
			POS_OVER,
			POS_RESIZE
		};

		bool deselecting; ///< Deslecting flag, gets cleared on mouse motion
		int deselecting_block;
		int deselecting_list;
		bool shift_when_deselecting;
		bool selecting;
		int border_resize_grab_pixels;
		BlockPositionAction action_at_selecting;
		float click_x_from;
		float click_y_from;
		int clicked_list;
		int clicked_block;

	} mouse_data;

	struct MovingBlock {

		struct Element {
			int list;
			int block; //must be known for painting purposes only.. I should remove it when separating this
			BlockList::Block *block_ptr; //litle hack for ease of code
			int new_list;
			Tick new_tick;
			bool can_move;
			float free_x;
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

	struct ResizingBlock {

		bool resizing;
		int list;
		int block; // if resizing existing
		Tick from;
		Tick len;  // where it is now
		enum Operation {
			RESIZE_CREATE_NEW,
			RESIZE,
		};

		Operation operation;

		float current_y;


	} resizing_block;
	
	void resizeEvent ( QResizeEvent * event );
	void resize_check_consistency();	
	void mouseMoveEvent( QMouseEvent * e );
	void mousePressEvent ( QMouseEvent * e );
	void mouseReleaseEvent ( QMouseEvent * e );
	MouseData::BlockPositionAction get_block_position_action(int p_blocklist,Tick p_pos);
	//selection

	std::set<int> selection; //should just be a list of pointers

	//helpers
	BlockList* get_block_list(int p_idx);
	int get_block_list_count();
	float get_block_list_offset(int p_idx);
	float get_width();
	float get_block_list_width(BlockList *p_block);
	int get_block_list_at_offset(float p_offset);
	QColor get_block_list_color(BlockList *p_block);
	SkinBox *get_block_list_skinbox(BlockList *p_block,bool p_selected=false);
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
	bool is_block_being_resized(int p_list,int p_block);
        void get_resizing_block_data(Tick* p_new_len,bool *allowed);
        void commit_resizing_block();

	void paint_name(QPainter &p,int p_blocklist,int p_ofs);
	void paint_block(QPainter& p,int p_x,int p_y,int p_list,int p_block,bool p_drawover=false,bool p_notpossible=false,Tick p_len_othersize=-1,bool p_no_contents=false,bool p_is_repeat=false);

signals:	
	
	void resized_signal();
	void drawing_signal();
public slots:
			
	void set_edit_mode(int p_edit_mode);
	void block_layout_changed_slot();
	void delete_selected_blocks();
	
	void select_linked_slot();
	void unlink_selected_slot();
	
	void block_repeat_set_signal();
	void block_repeat_unset_signal();
	
public:
	
	int get_total_pixel_width();
	int get_pixel_h_offset();
	void set_pixel_h_offset(int p_ofs);
	int get_total_pixel_height();
	int get_pixel_v_offset();
	void set_pixel_v_offset(int p_ofs);
	int get_bar_at_pixel(int p_pix);
	int get_beat_at_pixel(int p_pix);
	int get_beat_pixel(int p_beat);
	float get_beat_pixel_size();
	
	void set_zoom(float p_zoom); ///<from 0 to 1
	GlobalView(QWidget *p_widget,Editor *p_editor);
	~GlobalView();


};

};
#endif	//_GLOBALVIEW_H_
