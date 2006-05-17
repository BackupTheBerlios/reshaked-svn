//
// C++ Interface: filter_bank_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDFILTER_BANK_EDITOR_H
#define RESHAKEDFILTER_BANK_EDITOR_H

#include <Qt/qwidget.h>
#include <Qt/qpixmap.h>
#include "property.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class FilterBankEditor : public QWidget , public MultiPropertyEditor {
	
	Q_OBJECT
public:			
	struct Skin {
			
		QPixmap	bg;
		QColor line_color;
		int border_w;
		
		Skin(QPixmap p_bg=QPixmap(),QColor p_line_color=QColor(0,0,0),int p_border_w=0) { bg=p_bg;   line_color=p_line_color; border_w=p_border_w; }
	};	
	
private:	
	enum {
		MIN_FREQ = 20,
		AMP_STEPS = 3,
		NUMBER_H=12
				
	};
			
	struct ParamTrack {

		Property *property;
		double last_value;
	};
	
	
	std::vector<ParamTrack> param_tracking; //track parameters for changes
	
	int filters;
	
	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
		
	Skin skin;
	
	struct FilterData {
		
		Property *mode;
		Property *stages;
		Property *cutoff;
		Property *resonance;
			
		FilterData() { mode=NULL; stages=NULL; }
	};
	
	struct Drag {
		
		int index;
		int over;
	} drag;
	
	bool need_update;
	Property *cutoff_offset;
	
	FilterData *filter;
	
	void changed(int);
	
	void add_param_tracking(Property *p_prop);
	
private slots:	
	
	void check_param_tracking();
signals:	

	void external_edit_signal(Property *p_property);
	
public:
	
	void set_cutoff_property(int p_which,Property *p_prop);
	void set_resonance_property(int p_which,Property *p_prop);
	void set_mode_property(int p_filter,Property *p_prop);
	void set_stages_property(int p_filter,Property *p_prop);
	void set_cutoff_offset_property(Property *p_prop);
	
	
	
	FilterBankEditor(QWidget *p_parent,int p_filters,const Skin& p_skin);
	~FilterBankEditor();

};

}

#endif
