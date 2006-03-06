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

#include "interface/blocklist_ui_base.h"
#include "editor/editor.h"
#include "engine/automation.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class BlockListUI_Automation : public BlockListUI_Base {
			    
	Q_OBJECT
			    
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
			    
	Editor *editor;
	Automation *automation;
	
	void paint_value_text(QPainter &p,int p_x,int p_y,float p_val);
	void paint_frames(QPainter& p,int p_from_row=-1,int p_to_row=-1);
	void paintEvent(QPaintEvent *pe);
	void paint_envelopes(QPainter &p,int p_from_row=-1, int p_to_row=-1);
	void paint_row_lines(QPainter &p,int p_from_row=-1, int p_to_row=-1);
	void paint_name(QPainter&p);
	void paint_cursor(QPainter &p);
	void paint_selection(QPainter&p,int p_clip_from,int p_clip_to);

	bool find_closest_point(int p_x,int p_y,int p_radius, int *r_block, int *r_point);
	bool screen_to_tick_and_val(int p_x,int p_y,Tick *p_tick, float *p_val);
	
	void compute_point_over(int p_x,int p_y);
	
	
	
	void mouseMoveEvent ( QMouseEvent * e );
	void mousePressEvent ( QMouseEvent * e );
	void mouseReleaseEvent ( QMouseEvent * e );
	
	struct MovingPoint {
		
		int block;
		int point;
		bool moving;
		bool lfo_depthing;
		bool adding;
		
		Tick original_tick;
		float original_value;
		float original_lfo;
		Tick new_tick;
		float new_value;
		float new_lfo;
		
		
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
	
	/* helpers */
	int get_row_size();
	
	bool event( QEvent * ev );
	void get_pos_at_pointer(QPoint p_pointer, int *p_blocklist,int *p_column, int *p_row);
	
public:
	
	void show_popup();
	
	
	BlockListUI_Automation(QWidget *p_parent, Editor *p_editor, Automation *p_automation);
	~BlockListUI_Automation();

};

}

#endif
