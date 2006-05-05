//
// C++ Interface: lfo_params_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDLFO_PARAMS_EDITOR_H
#define RESHAKEDLFO_PARAMS_EDITOR_H

#include "dsp/lfo.h"
#include "property.h"
#include "ui_blocks/helpers.h"
#include "ui_blocks/property_editors.h"
#include "ui_blocks/pixmap_button.h"


namespace ReShaked {

/**
	@author red <red@killy>
*/
class LFO_ParamsEditor : public CHBox {
	
	
	Q_OBJECT
	
public:			
	
	struct Skin {
		
		
		PixmapSlider::Skin slider;
		QColor label;
		QPixmap display;
		
		PixmapButton::Skin button_sine;
		PixmapButton::Skin button_sawup;
		PixmapButton::Skin button_sawdown;
		PixmapButton::Skin button_square;
		
	};
	
private:
			
	Skin skin;
	
	LFO *lfo;
	
	PropertyClassFunc<LFO,unsigned long> delay;
	PropertyClassFunc<LFO,double> rate;
	PropertyClassFunc<LFO,double> depth;
	PropertyClassFunc<LFO,double> phase;
	PropertyClassFunc<LFO,double> random_depth;
	PropertyClassFunc<LFO,double> random_seed;
	
	void add_label(QString p_text,QWidget *p_parent);
	
	PropertyEditSlider * delay_slider;
	PropertyEditLabel * delay_label;
	PropertyEditSlider * rate_slider;
	PropertyEditLabel * rate_label;
	PropertyEditSlider * depth_slider;
	PropertyEditLabel * depth_label;
	PropertyEditSlider * phase_slider;
	PropertyEditSlider * random_depth_slider;
	PropertyEditSlider * random_seed_slider;
	
	PixmapButton *button_sine;
	PixmapButton *button_sawup;
	PixmapButton *button_sawdown;
	PixmapButton *button_square;
	
	PixmapButtonGroup *lfo_mode_group;
	
	static void changed_by_editor(void *p_userdata,Property*,double);
	
	
public slots:	
	
	void mode_selected(int p_mode);
	void update_values();
	
signals:	
	
	void lfo_changed_signal();
public:
	
	void set_lfo(LFO *p_lfo);
	LFO_ParamsEditor(QWidget *p_parent, const Skin& p_skin);

	~LFO_ParamsEditor();

};

}

#endif
