//
// C++ Interface: filter_bank_fr
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FILTER_BANK_FR_H
#define FILTER_BANK_FR_H

#include "nodes/filter_bank_node.h"
#include "gui_common/control_port_range.h"
#include "base/widget.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class FilterBankFR : public GUI::Widget {
	
	enum {
		MIN_FREQ = 20,
		AMP_STEPS = 3,
		NUMBER_H=12
				
	};
			
	struct ParamTrack {

		GUI::RangeBase *cpr;
		double last_value;
	};
		
	int filters;
	
	virtual void mouse_button(const GUI::Point& p_pos, int p_button,bool p_press,int p_modifier_mask);	
	virtual void mouse_motion(const GUI::Point& p_pos, const  GUI::Point& p_rel, int p_button_mask);
	
	virtual GUI::Size get_minimum_size_internal(); ///< Use this one when writing widgets

	virtual void draw(const GUI::Point& p_global,const GUI::Size& p_size,const GUI::Rect& p_exposed);
	
	
	struct FilterData {
		
		GUI::RangeBase *mode;
		GUI::RangeBase *stages;
		GUI::RangeBase *cutoff;
		GUI::RangeBase *resonance;
			
		float last_mode, last_stages, last_resonance, last_cutoff;
		FilterData() { mode=NULL; stages=NULL; cutoff=NULL; resonance=NULL; last_mode=0; last_stages=0; last_resonance=0; last_cutoff=0; }
		~FilterData() { if (mode) delete mode; if (stages) delete stages; if (cutoff) delete cutoff; if (resonance) delete resonance; }
	};
	
	struct Drag {
		
		int index;
		int over;
	} drag;
	
	FilterData *filter;
	float *response;
	int response_len;
	
	GUI::RangeBase *cutoff_offset;
	float last_cutoff_offset;

	
public:		
	void check_for_changes();
	
	
	void set_cutoff_range(int p_which,GUI::RangeBase *p_prop);
	void set_resonance_range(int p_which,GUI::RangeBase *p_prop);
	void set_mode_range(int p_filter,GUI::RangeBase *p_prop);
	void set_stages_range(int p_filter,GUI::RangeBase *p_prop);
	void set_cutoff_offset_range(GUI::RangeBase *p_prop);		
	
	FilterBankFR(int p_filters);
	~FilterBankFR();

};

#endif
