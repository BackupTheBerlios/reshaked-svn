
#ifndef PIGUI_CONTAINER_H
#define PIGUI_CONTAINER_H


#include "base/frame.h"
#include "base/window.h"

namespace GUI {

struct ContainerPrivate;

class ChildIterator {
friend class Container;	
	void *private_data;
	Frame *frame;
public:	
	
	Frame* operator->() ///< Overload to access the child iterator, as ->data
	{ return frame; }
	Frame* operator*() ///< Overload to access the child iterator as *
	{ return frame; }
	Frame *get() { return frame; }
	
	bool empty() ///< Child iterator is empty
	{ return frame==0; }
	bool end() ///< Iteration has reached end
	{ return frame==0; }
	ChildIterator() { frame=0; private_data=0; }
};


class Container : public Frame {

public:

	struct Element {
	
		Rect rect;
		Frame * frame;
		Element *next;		
		Element *prev;		
		
		Element() { frame = 0; next =0; prev =0; }
		virtual ~Element() {}
	};

private:

	ContainerPrivate *_cp;
	
	virtual void check_for_updates(const Point& p_pos,const Size &p_size,const StyleBox& p_bg_style,const Rect& p_bg_rect); 
	
	virtual void draw(const Point& p_pos,const Size &p_size,const Rect& p_exposed); ///< Draw the container
	
	bool draw_tree(const Point& p_global,const Size& p_size,const Rect& p_exposed);
	
	
	Point get_margin_offset();

	Frame* find_closest_focusable_to(Point p_point,Direction p_dir,int &mindist);	

	Frame* get_child_at_pos(const Point& p_point,const Size &p_size,Point *p_local_pos=0);	
	virtual void set_window(Window *p_window);
	virtual String get_type();

friend struct ContainerPrivate;	

protected:	

	virtual const StyleBox & get_stylebox();
	virtual void skin_changed();	
	virtual void set_minimum_size_changed();
	

	/* Internal API */
	Element* get_element_list();	
	Element* get_element_list_end();	
	Element* add_frame_internal( Frame * p_frame, bool p_front=false );
	void remove_frame( Frame * p_frame );

	
	/* Can be overrided */

	
	virtual Size get_minimum_size_internal()=0;
	virtual void resize_internal(const Size& p_new_size)=0;
	
	virtual Element *create_new_element();
	
	virtual void resize(const Size& p_new_size); /// OVerride WITH CARE (call this back in the override as Container::resize()

	
	/* Default Input Event Handlers */	
	
	void mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask); 
	void mouse_motion(const Point& p_pos, const Point& p_rel, int p_button_mask);
	void mouse_doubleclick(const Point& p_pos,int p_modifier_mask);

	bool key(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask);
	
public:
	
	

	Signal< Method1<const Size> > top_size_adjust_signal;
	
	void adjust_minimum_size(); ///< Not really meant to be called

	
	Point get_child_pos( Frame * p_child );
	Size get_child_size( Frame * p_child );
	
	Frame *next_in_focus( Frame *p_from=0 );
	Frame *prev_in_focus( Frame *p_from=0 );
	
	virtual void set_minimum_size(const Size & p_size );
	virtual Size get_minimum_size();
	
	void set_style( const StyleBox& p_style, bool p_draw_background=false);
	
	ChildIterator first_child();
	ChildIterator next_child(const ChildIterator& p_child);

	Container();
	~Container();
};

}

#endif