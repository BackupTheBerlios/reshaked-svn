//
// C++ Interface: pixmap_button
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDPIXMAP_BUTTON_H
#define RESHAKEDPIXMAP_BUTTON_H

#include <Qt/qwidget.h>
#include <Qt/qpixmap.h>

namespace ReShaked {

/**
	@author red <red@killy>
*/
class PixmapButton : public QWidget {
			    
	Q_OBJECT
public:
						    
	struct Skin {
		
		QPixmap normal;	
		QPixmap pressed;	
		QPixmap hover;	
		
		Skin( QPixmap p_normal=QPixmap(), QPixmap p_pressed=QPixmap(), QPixmap p_hover=QPixmap()) { normal=p_normal; pressed=p_pressed; hover=p_hover; };
		
	};
	
	enum Type {
		TYPE_PUSH,
		TYPE_TOGGLE,
	};
	
private:
			
	Skin skin;
	Type type;
	
	struct Status {
		
		bool pressed;
		bool hovering;
		bool press_attempt;
		bool pressing_inside;
	} status;
	
	void paintEvent(QPaintEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void enterEvent ( QEvent * event );
	void leaveEvent ( QEvent * event );
	
	
	void emit_qactions();
	
	bool ignore_actions;
protected:
	void mousePressEvent(QMouseEvent *e);
			
	virtual void mouse_press_event_override();
	virtual void mouse_toggled(bool p_new_status);
signals:	
	
	void mouse_pressed_signal();
	void mouse_toggled_signal(bool p_new_status);
public:
	
	bool is_pressed();
	void set_pressed(bool p_pressed); //for toggles
	
	void set_ignore_actions(bool p_ignore);	
	PixmapButton(QWidget *p_parent,const Skin& p_skin,Type p_type=TYPE_PUSH);
	~PixmapButton();

};


class PixmapButtonGroup : public QObject {
	
	Q_OBJECT
	
	std::vector<PixmapButton*> group;
private slots:	
	
	void button_pressed(int p_index);
signals:	
	void button_selected_signal(int p_index);
public:	
	
	void add_button(PixmapButton *p_button);
	void set_selected_index(int p_index);
	
	PixmapButtonGroup(QObject *p_parent);
	
};

}

#endif
