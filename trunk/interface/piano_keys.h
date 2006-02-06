//
// C++ Interface: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PIANO_KEYS_H
#define PIANO_KEYS_H


#include <Qt/qwidget.h>
#include <Qt/qpixmap.h>
#include "editor/keyboard_input.h"
#include "editor/editor.h"
/**
@author Juan Linietsky
*/

namespace ReShaked {

class Piano_Keys : public QWidget {

	Q_OBJECT
public:
	
	struct Skin {
		
		QPixmap white_key,white_key_pressed;
		QPixmap black_key,black_key_pressed;
		
		Skin() {}
		Skin(QPixmap p_white_key,QPixmap p_white_key_pressed,QPixmap p_black_key,QPixmap p_black_key_pressed);
	};

private:	
	enum {


		MOUSE_KEY_NO_DRAG=-1,
		INVALID_KEY=-2,
		
		MAX_KEY=128
	};

	struct Key {

		int pressed; //refcount since works for keyboard too

		Key() { pressed=false; }
	};

	Skin skin;
	
	Key keys[MAX_KEY];

	void paintEvent(QPaintEvent *p_event);

	QColor white_key_col;
	QColor black_key_col;
	QColor white_key_hilite_col;
	QColor black_key_hilite_col;
	QColor white_key_shadow_col;
	QColor selected_color_col;
	QColor in_focus_col;
	QColor top_col;

	int mouse_key_drag;

	void press_key(int p_key);
	void release_key(int p_key);

	int get_key_from_key_event( QKeyEvent * e );
	int get_key_from_pos(const QMouseEvent & e);

	void mousePressEvent( QMouseEvent * e );
	void mouseMoveEvent( QMouseEvent * e );
	void mouseReleaseEvent( QMouseEvent * e );

	void keyPressEvent ( QKeyEvent * e );
	void keyReleaseEvent ( QKeyEvent * e );

	int visible_octaves;
	
	Editor *editor;
	
public slots:
			
		void octave_changed_slot();		
signals:

	void key_pressed_signal(int p_key);
	void key_released_signal(int p_key);

	void note_over_signal(int p_note);
	


public:
	
	
	Piano_Keys(QWidget *p_parent,const Skin & p_skin,Editor *p_editor,int p_visible_octaves=2);
	~Piano_Keys();

};

} //end of namespace
#endif
