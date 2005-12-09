//
// C++ Interface: editor_global
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDEDITOR_GLOBAL_H
#define RESHAKEDEDITOR_GLOBAL_H

namespace ReShaked {

/**
				

	@author red <red@killy>
*/
				    
/* Whenever I have some time.....
class EditorGlobal{
			    
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
			    
public:
	
	void click_select(float p_beat,int p_blocklist);
	void click_add(float p_beat,int p_blocklist);
	
	void click_add(float p_x
	void cancel_selecting();
    EditorGlobal();

    ~EditorGlobal();

};
*/
}

#endif
