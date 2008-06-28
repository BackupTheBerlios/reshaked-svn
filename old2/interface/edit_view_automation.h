//
// C++ Interface: blocklist_ui_automation
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDBLOCKLIST_UI_AUTOMATION_H
#define RESHAKEDBLOCKLIST_UI_AUTOMATION_H

#include "interface/edit_view_blocklist.h"
#include "editor/editor.h"
#include "engine/automation.h"

using namespace GUI;

namespace ReShaked {

/**
	@author red <red@killy>
*/
class EditViewAutomation : public EditViewBlockList {
			    
	    
	enum {
		
		CLOSEST_POINT_RADIUS=5,
		FONT_PIXEL_SIZE=12
	};
				    			    
	enum AutomationOptionsItems {
	
		AUTOMATION_OP_HIDE,
		AUTOMATION_OP_MOVE_LEFT,
		AUTOMATION_OP_MOVE_RIGHT,
		AUTOMATION_OP_SIZE_SMALL,	
		AUTOMATION_OP_SIZE_MEDIUM,	
		AUTOMATION_OP_SIZE_LARGE,	
		AUTOMATION_OP_LFO_SETTINGS
	};
			    
	
	Size size;
	void resize(const Size& p_new_size) { size=p_new_size; }
	
	Editor *editor;
	Automation *automation;
	
	void draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed);	
	void paint_value_text(Painter &p,int p_x,int p_y,float p_val);
	void paint_frames(Painter& p,int p_from_row=-1,int p_to_row=-1);
	
	void paint_envelopes(Painter &p,int p_from_row=-1, int p_to_row=-1);
	void paint_row_lines(Painter &p,int p_from_row=-1, int p_to_row=-1);
	void paint_cursor(Painter &p);
	void paint_selection(Painter&p,int p_clip_from,int p_clip_to);

	bool find_closest_point(int p_x,int p_y,int p_radius, int *r_block, int *r_point);
	bool screen_to_tick_and_val(int p_x,int p_y,Tick *p_tick, float *p_val);
	
	void compute_point_over(int p_x,int p_y);
	
	void mouse_motion(const Point& p_pos, const Point& p_rel, int p_button_mask);
	void mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask); ///< Overridable Method to notify the frame when it was 
		
	bool key(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask);
	
	struct MovingPoint {
		
		Point prev_pointer_pos;
		
		int block;
		int point;
		bool moving;
		bool moving_first;
		bool lfo_depthing;
		bool adding;
		
		Tick original_tick;
		float original_value;
		float original_lfo;
		Tick new_tick;
		float new_value;
		float new_lfo;
		
		bool snap;
		
	} moving_point;
	
	struct PointOver {
		
		int block;
		int point;
		
		
	} point_over;
	
	/* screen-screen copy helpers */
	bool paint_name_enabled;
	void fix_pre_scroll(int p_scroll);
	bool can_scroll();
	
	void cancel_motion();
	
	Size get_minimum_size_internal();


	void get_pos_at_pointer(Point p_pointer, int *p_blocklist,int *p_column, int *p_row);
	
public:
	
	void show_popup();
	
	
	EditViewAutomation(Editor *p_editor, Automation *p_automation);
	~EditViewAutomation();

};

}

#endif
