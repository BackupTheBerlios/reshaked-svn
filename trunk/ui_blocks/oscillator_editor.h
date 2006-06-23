//
// C++ Interface: oscillator_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDOSCILLATOR_EDITOR_H
#define RESHAKEDOSCILLATOR_EDITOR_H

#include "ui_blocks/oscillator_display.h"
#include "ui_blocks/shape_editor.h"

#include <Qt/qdialog.h>
#include <Qt/qcombobox.h>
#include <Qt/qcheckbox.h>
#include <Qt/qslider.h>
#include <Qt/qpushbutton.h>


class QBoxLayout;

namespace ReShaked {

/**
	@author red <red@killy>
*/



class OscillatorEditor : public QDialog {
	
	Q_OBJECT
			
	enum WaveType {			
		
		WAVE_TYPE_SINE,
		WAVE_TYPE_SAW,
		WAVE_TYPE_TRIANGLE,
		WAVE_TYPE_SQUARE,
		WAVE_TYPE_CIRCULAR,
		WAVE_TYPE_CHIRP,
		WAVE_TYPE_CUSTOM,
		WAVE_TYPE_MAX,
	};
				
	
	enum {
		
		MAX_HARMONICS=64
	};
	
	QBoxLayout * add_section(QString p_section,bool p_add_frame=false);
			
	OscillatorDisplay *display;			
	
	QComboBox *wave_types;
	QSlider *wave_parameter;
	QSlider *wave_phase;
	ShapeEditor *shape_editor;
	
	std::vector<QSlider*> harmonic_array;
		
	
	float get_wave_value(float p_phase,int p_wave_type);
	
	Oscillator osc;
	
	Shape custom;
	
	Oscillator *edited_osc;
	Oscillator edited_osc_original;
private slots:	
	
	
	void regenerate_wave();
	void wave_changed_slot(int);
public:
	int exec(Oscillator *p_edited_osc);
	
	OscillatorEditor(QWidget *p_parent);
	~OscillatorEditor();
};

}

#endif
