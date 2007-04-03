//
// C++ Interface: envelope_params_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDENVELOPE_PARAMS_EDITOR_H
#define RESHAKEDENVELOPE_PARAMS_EDITOR_H


#include "ui_blocks/helpers.h"
#include "ui_blocks/property_editors.h"
#include "ui_blocks/pixmap_button.h"
#include "dsp/envelope.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class EnvelopeParamsEditor : public CHBox {
	
	Q_OBJECT
	
public:			
	
	struct Skin {
		
		PixmapButton::Skin enabled_button;
		PixmapButton::Skin cubic_button;
		PixmapButton::Skin loop_button;
		PixmapButton::Skin susloop_button;
		
		QPixmap spin_label;
		PixmapUpDown::Skin spin_updown;
		QColor label_col;
	};
private:	
	
	Envelope *envelope;
	
	PropertyClassFunc<Envelope,int> loop_begin_prop;
	PropertyClassFunc<Envelope,int> loop_end_prop;
	PropertyClassFunc<Envelope,int> susloop_begin_prop;
	PropertyClassFunc<Envelope,int> susloop_end_prop;
	
	PropertyEditLabel *label_loop_begin_edit;	
	PropertyEditUpDown *updown_loop_begin_edit;	
	
	PropertyEditLabel *label_loop_end_edit;	
	PropertyEditUpDown *updown_loop_end_edit;	
	
	PropertyEditLabel *label_susloop_begin_edit;	
	PropertyEditUpDown *updown_susloop_begin_edit;	
	
	PropertyEditLabel *label_susloop_end_edit;	
	PropertyEditUpDown *updown_susloop_end_edit;	
			
	PixmapButton *envelope_enabled;
	PixmapButton *envelope_cubic;
	
	PixmapButton *loop_enabled;
	PixmapButton *susloop_enabled;
	
	void add_label(QString p_text,QColor p_color,QWidget *p_parent);
	
	static void changed_by_editor(void *p_userdata,Property*,double);
	
protected slots:	
	
	void update_controls();
	
	void set_susloop(bool p_on);
	void set_loop(bool p_on);
	void set_enabled(bool p_on);
	void set_cubic(bool p_on);
signals:	
	
	void envelope_changed_signal();
public:
	
	void set_envelope(Envelope *p_envelope);
	
	EnvelopeParamsEditor(QWidget *p_parent,Skin& p_skin);
	~EnvelopeParamsEditor();

};

}

#endif
