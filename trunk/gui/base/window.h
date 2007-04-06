//
// C++ Interface: window
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PIGUIINSTANCE_H
#define PIGUIINSTANCE_H

#include "base/painter.h"
#include "base/skin.h"
#include "base/timer.h"
#include "signals/signals.h"

namespace GUI {

class Frame;
class Container;

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

class Window : public SignalTarget {
	
public:

	enum SizeMode {
		
		SIZE_NORMAL, ///<just set pos/size normally
		SIZE_CENTER, ///< Center of parent window (or desktop)
		SIZE_TOPLEVEL_CENTER ///< center of the toplevel screen, taking half of the space
	};
	
	enum Mode {
		
		MODE_NORMAL, ///< Normal window
		MODE_MODAL, ///< Modal Window (Dialog)
		MODE_POPUP, ///< Popup window (modal, but dissapears if you click somewhere else)
		
	};

private:
	
	Skin *skin;
	
	Frame *root_frame;
	Frame *last_under_cursor;
	Frame *focus_child; //has keyboard focus
	struct Drag {
		Frame *child;
		int button_mask;
		Point pos;
	} drag;
	
	Painter *painter;
	Timer *timer;

	bool visible;
	Point pos;
	Size size;

	SizeMode size_mode;
	Mode mode;

	Window *parent; //in case we have a parent;
	
	//bool queued_delete;
	

	bool size_update_needed;
	Window *childs;
	Window *next;
	
	
	Window *focus; //focused window , only the tree root can use this
	Window *root; //pointer to tree root
	
	struct ModalStack {
		
		ModalStack *next;
		Window *window;
		
		ModalStack() { next=0; window=0; }
	};
	
	ModalStack *modal_stack;
	

	void remove_from_modal_stack();
	void raise_window(Window *p_child);
	Window *find_window_at_pos(const Point& p_point);
	void initialize();
	
	void draw_widgets_over_area(const Rect &p_rect);

	void redraw_all_internal(const Rect& p_rect,Window *p_after_child=0,bool p_reset_clip_rect=true);

	void top_frame_resized(const Size p_size);
	
	void adjust_size_type();
	
	virtual void set_root_frame_internal(Container *p_root_frame) {}
	
	bool no_local_updates;
	bool no_stretch_root_frame;

public:
	
	Signal<> popup_cancel_signal;
	Signal< Method5<unsigned long,unsigned long,bool,bool,int> > key_unhandled_signal;
	Signal<> skin_changed_signal;
	
	void set_no_local_updates(bool p_disable);
	void set_no_stretch_root_frames(bool p_disable);
	
	void update(const Rect& p_rect);
	void update();
	
	
	void set_tree_size_changed();
	Painter *get_painter();
	Timer *get_timer();
	
	void set_root_frame(Container *p_root);
	Frame * get_root_frame();
	Window *get_root();

	void set_pos(const Point &p_pos);
	void set_size(const Size& p_size);
	Size get_size();
	
	Point get_global_pos();
	void redraw_all(Rect p_custom_area=Rect());
	void check_for_updates();


	void raise();
	void show();
	void hide();
	
	
	void mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask); 
	void mouse_motion(const Point& p_pos, const Point& p_rel, int p_button_mask);
	void mouse_doubleclick(const Point& p_pos,int p_modifier_mask);
	void key(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask);

	bool is_visible();
	
	Frame * get_focused_child();
	void set_focus(Frame *p_frame);
	void clear_focus();
	
	void frame_deleted_notify(Frame *p_frame);

	Skin *get_skin();
	virtual void skin_changed();
	//void queue_delete(); //queue deletion until next "check for updates"

	//void check_for_deletes();

	void frame_hide_notify(Frame *p_frame);
	
	Window(Window *p_parent,Mode p_mode=MODE_NORMAL, SizeMode p_size_mode=SIZE_NORMAL);
	Window(Painter *p_painter,Timer *p_timer,Skin *p_skin);

	virtual ~Window();
};

}

#endif
