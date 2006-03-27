//
// C++ Interface: envelope_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDENVELOPE_EDITOR_H
#define RESHAKEDENVELOPE_EDITOR_H

#include "engine/audio_control.h"
#include <Qt/qwidget.h>
#include <Qt/qpixmap.h>
#include "dsp/envelope.h"
#include "dsp/lfo.h"
#include <vector>
	

namespace ReShaked {

/**
 *
 * Juan Linietsky
 **/
class EnvelopeEditor : public QWidget {

	Q_OBJECT

	enum {

		MIN_DRAG_DISTANCE=8 // min drag distance
	};
public:
	
	struct Skin {
		
		QPixmap bg;
		QColor line_color;
		QColor point_color;
		QColor zero_color;
		int border_size;
		
		Skin(const QPixmap& p_bg=QPixmap(), QColor p_line=QColor(200,200,255), QColor p_point=QColor(40,100,200), QColor p_zero=QColor(150,150,150), int p_border=0 ) { bg=p_bg; line_color=p_line; point_color=p_point; zero_color=p_zero; border_size=p_border; }
	};
	
private:	
	
	Skin skin;
	
	float zoom;
	float min_zoom;

	void paintEvent( QPaintEvent * p_event );

	Envelope *envelope;
	LFO *lfo; //draw LFO also!
	struct Grab {

		bool moving_point;
		int node;
		float x,y;
	} grab;

	bool adjust_zoom_to_window();
	void paint_envelope_points(QPainter &painter);
	bool event ( QEvent * e );
	void move_mouse_to_selected_node();

	void draw_envelope_pos(int p_x,QPainter &painter);
	

signals:

	void envelopeChanged();

public:

	
	void set_lfo(LFO *p_lfo);
	void set_envelope(Envelope *p_envelope);
	EnvelopeEditor(QWidget *p_parent,const Skin &p_skin=Skin(),float p_min_zoom=0.2);

	~EnvelopeEditor();



};
}

#endif
