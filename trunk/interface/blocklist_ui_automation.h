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
#include "engine/editor.h"
#include "engine/automation.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class BlockListUI_Automation : public BlockListUI_Base {
			    
			    
	enum {
		
		CLOSEST_POINT_RADIUS=5,
		FONT_PIXEL_SIZE=12
	};
				    			    
				    
			    
	Editor *editor;
	Automation *automation;
	
	void paint_value_text(QPainter &p,int p_x,int p_y,float p_val);
	void paint_frames(QPainter& p,int p_from_row=-1,int p_to_row=-1);
	void paintEvent(QPaintEvent *pe);
	void paint_envelopes(QPainter &p,int p_from_row=-1, int p_to_row=-1);
	void paint_row_lines(QPainter &p);
	void paint_name(QPainter&p);
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
		bool adding;
		
		Tick original_tick;
		float original_value;
		
	} moving_point;
	
	struct PointOver {
		
		int block;
		int point;
		
		
	} point_over;
	
public:
	BlockListUI_Automation(QWidget *p_parent, Editor *p_editor, int p_track,int p_automation);
	~BlockListUI_Automation();

};

}

#endif
