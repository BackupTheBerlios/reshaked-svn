//
// C++ Implementation: window
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "window.h"
#include "container.h"


#include <stdio.h>
namespace GUI {

void Window::set_focus(Frame *p_frame) {

	if (p_frame && p_frame->get_window()!=this)
		return;
	
	if (focus_child) {
		focus_child->focus_leave();
		focus_child->update();
	}
	focus_child=p_frame;
	if (p_frame) {
		focus_child->focus_enter();
		focus_child->update();
	}

}

void Window::draw_widgets_over_area(const Rect &p_rect) {

	if (!visible)
		return;

	Window *c=childs;

	while (c) {

		Rect crect=p_rect;
		crect.pos-=c->pos;
			
		
		c->redraw_all_internal(crect);

		c=c->next;

	}


	
	if (parent)
		parent->redraw_all_internal( Rect(parent->get_global_pos()+p_rect.pos+pos,p_rect.size), this );

}

Point Window::get_global_pos() {

	return pos+(parent?parent->pos:Point());
}

void Window::set_tree_size_changed() {
	
	
}

Timer *Window::get_timer() {
	
	return timer;
}

Painter *Window::get_painter() {
	
	return painter;
}

void Window::set_root_frame(Container *p_root_frame) {

	if (root_frame)
		return;
	root_frame=p_root_frame;
	root_frame->set_window( this );
	p_root_frame->top_size_adjust_signal.connect( this, &Window::top_frame_resized );
	set_root_frame_internal(p_root_frame);
	
	if (mode==MODE_POPUP) {
		
		p_root_frame->set_style( skin->get_stylebox( SB_POPUP_BG ), true );
	}

	set_size(size);

}

void Window::skin_changed() {
	
	
	
	if (root_frame) {
		
		if (mode==MODE_POPUP) {
		
			((Container*)root_frame)->set_style( skin->get_stylebox( SB_POPUP_BG ), true );
			
			

		}
		
		root_frame->skin_changed();
		
		adjust_size_type();
	}
	
	Window *c=childs;
	
	while (c) {
		
		c->skin_changed();
		c=c->next;
	}
		
	skin_changed_signal.call();
}


Window *Window::get_root() {
	
	return root;
}
Frame * Window::get_root_frame() {
	
	return root_frame;
}

void Window::set_pos(const Point &p_pos) {

	/* WARRRNING - moving stuff */
	pos=p_pos;
}

void Window::top_frame_resized(const Size p_size) {
	
	set_size(p_size);
	//printf("top frame resize!\n");
}

void Window::set_size(const Size& p_size) {

	
	if (!root_frame) {
		size=p_size;
		return;
	}
	
	Size new_size = root_frame->get_minimum_size();
	
	if (!no_stretch_root_frame)  {
		
		if (p_size.width > new_size.width)
			new_size.width=p_size.width;
		if (p_size.height > new_size.height)
			new_size.height=p_size.height;
	}
	
	
	if (parent && visible)
		hide();
	
	size=new_size;
	root_frame->resize( new_size );
	
	if (parent && visible)
		show();
	
}

Size Window::get_size() {
	
	return size;
}
void Window::redraw_all(Rect p_custom_area) {


	if (p_custom_area.has_no_area())
		redraw_all_internal( Rect( get_global_pos(), size ) );
	else
		redraw_all_internal( Rect( p_custom_area.pos, p_custom_area.size) );

}


void Window::redraw_all_internal(const Rect& p_rect,Window *p_after_child,bool p_reset_clip_rect) {


	/* Redrawing all should work fine */
	
	if (!root_frame)
		return;
	if (!visible)
		return;

	
	painter->set_local_rect( Rect( get_global_pos(), size ) );
	if (p_reset_clip_rect)
		painter->set_clip_rect( false );

	if (!p_after_child) {

		Rect global_rect;
		global_rect.pos=get_global_pos();
		global_rect.size=size;
		
		Rect expose_rect=global_rect.clip(p_rect);
		expose_rect.pos-=global_rect.pos;
		
		root_frame->draw_tree( global_rect.pos, size, expose_rect );
	}


	Window *c=childs;

	bool after_reached=p_after_child?false:true;
	
	while (c) {
	
		if (c->visible && after_reached) {


			c->redraw_all_internal(p_rect,0,p_reset_clip_rect);
			
		}

		if (c==p_after_child)
			after_reached=true;
			
			
		c=c->next;
	}
}


/*
void Window::check_for_deletes() {

	Window *c=childs;
	Window **cr=&childs;
	
	while (c) {

	
		if (c->queued_delete) {
			
			
			Window *ac=c;
			
			*cr=c->next;
			c=c->next;
			
			delete ac;
			continue;			
		} 
		
		c->check_for_deletes();
		
		cr=&c->next;
		c=c->next;
	}
	
}
*/
void Window::check_for_updates() {

	/* For now, if the window has a child, it wont redraw */

	if (size_update_needed) {

		if (root_frame) {

			Size rootfmin=root_frame->get_minimum_size();
			if (!no_stretch_root_frame) {
				if (size.width<rootfmin.width)
					size.width=rootfmin.width;
				if (size.height<rootfmin.height)
					size.height=rootfmin.height;
			}

			root_frame->resize( size );
		}
		
		size_update_needed=false;
	}
		
	if (visible && !no_local_updates) {
		painter->set_local_rect( Rect( get_global_pos(), size ) );
		painter->set_clip_rect( false );
		if (root_frame)
			root_frame->check_for_updates( Point() ,  size, StyleBox(), Rect( Point(), size ) );
	}	
	Window *c=childs;
	
	while (c) {

		
		c->check_for_updates();

		c=c->next;
	}
	
	
	
	
}


Window *Window::find_window_at_pos(const Point& p_point) {
	
	
	if (!visible)
		return 0;
	Window * w = childs;
	
	Window *found=0;
	while (w) {
		
		Point rpos=p_point - w->pos;
		Window *wf = w->find_window_at_pos( rpos );
		if (wf)
			found=wf;
		
		
		w=w->next;
		
	}
	
	if (found)
		return found;
	
	//childs wont have it? then I do!
	if ( p_point.x >= 0 && p_point.y >= 0 && p_point.x < size.width && p_point.y < size.height)
		return this;
	
	return 0;	
	
}


void Window::mouse_doubleclick(const Point& p_pos,int p_modifier_mask) {
	

	if (!parent && (root->focus!=this)) {
		//deliver to whoever has the focus, if root does not have it
		
		Point pos=p_pos-root->focus->get_global_pos();
		root->focus->mouse_doubleclick(pos,p_modifier_mask);
		return;
		
	}

	if (!root_frame)
		return;
	

	Point local_pos;
	Frame *child=root_frame->get_child_at_pos( p_pos, size, &local_pos );
	
	if (!child)
		return; //nothing found!
	

	
	child->mouse_doubleclick( local_pos, p_modifier_mask );
	
}
void Window::mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask) {
	
	
	if (!parent && (root->focus!=this)) {
		//deliver to whoever has the focus, if root does not have it
		
		if (!p_press) {

			
			Point pos=p_pos-root->focus->get_global_pos();
				
			root->focus->mouse_button( pos , p_button, p_press, p_modifier_mask );
			return;
			
		} else {
			
			Window *at_click=find_window_at_pos(p_pos);

			bool post_popup_hide=false;
			while (modal_stack) {


				if (modal_stack->window->mode==MODE_POPUP && at_click!=modal_stack->window) {
					
					/* good bye popup */
						
					modal_stack->window->popup_cancel_signal.call();
					modal_stack->window->hide();
					

					post_popup_hide=true;
					continue; /* Try again, if there still is a modal stack */
					
				}
				
				/* ok, found modal, and has all the right to keep the event */

				Point pos=p_pos-modal_stack->window->get_global_pos();
				root->focus=modal_stack->window; //in case there is any doubt or the user did something stupid
				modal_stack->window->mouse_button( pos, p_button, p_press, p_modifier_mask );
				
				
				return;
			}
			
			if (at_click) { //clicked somewhere meaningful
				/* focus change */
				
				/* TODO proper focus change and window raise */
				focus=at_click;
				Point pos=p_pos-root->focus->get_global_pos();
				if (post_popup_hide)
					focus->mouse_motion( pos, Point(), 0 );
				focus->mouse_button( pos, p_button, p_press, p_modifier_mask );
				return;
			}
				
		}
			
	}
	

	
	if (!root_frame)
		return;
	
	Point local_pos;
	Frame *child=0;
	
	if (drag.child) {
		
		child=drag.child;
		local_pos=p_pos-drag.pos;
	} else {
		child=root_frame->get_child_at_pos( p_pos, size, &local_pos );
	}
	
	
	if (!child)
		return; //nothing found!
	
	if (p_button<=3) {
		
		if (p_press) {

			drag.button_mask|=(1<<(p_button-1));
			if (!drag.child) {
				drag.child=child;
				drag.pos=p_pos-local_pos;
			} 
							
			/* Check if we gained focus on click */
			if (p_button==BUTTON_LEFT && child!=focus_child && child->get_focus_mode()!=FOCUS_NONE) {
				
				set_focus(child);
			}
			
		} else {
				
		
			drag.button_mask&=~(1<<(p_button-1));
			
			if (!drag.button_mask) { //not dragging anymore
				
				if (child==drag.child) { 
					
					//send event now
					child->mouse_button( local_pos, p_button, p_press, p_modifier_mask );
				
					/* try to find if child changed */
					child=root_frame->get_child_at_pos( p_pos, size, &local_pos );
					
					if (last_under_cursor && last_under_cursor!=drag.child) {
						
						last_under_cursor->mouse_enter();
					}
					
					drag.child=0;
					
					return;
				}
			
				
			}
		}

	}
	child->mouse_button( local_pos, p_button, p_press, p_modifier_mask );
	
}
void Window::mouse_motion(const Point& p_pos, const Point& p_rel, int p_button_mask) {

	
	if (!parent && (root->focus!=this)) {
		//deliver to whoever has the focus, if root does not have it
		
		Point pos=p_pos-root->focus->get_global_pos();
		
		root->focus->mouse_motion( pos , p_rel, p_button_mask );
		return;
		
	}
		
	if (!root_frame)
		return;
	
	Point local_pos;
	Frame *child=root_frame->get_child_at_pos( p_pos, size, &local_pos );
	
		
	/* Send enter/exit events */
	if (last_under_cursor!=child) {
		
		if (last_under_cursor && (!drag.child || last_under_cursor==drag.child))
			last_under_cursor->mouse_leave();
		
		if (child && (!drag.child || drag.child==child))
			child->mouse_enter();
		
		last_under_cursor=child;
	}
	
	if (drag.child) {
		
		child=drag.child;
		local_pos=p_pos-drag.pos;
	}
	
	if (!child)
		return; //nothing found!
	
	
	child->mouse_motion( local_pos, p_rel, p_button_mask );
	
}

void Window::key(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask) {

	
	if (!parent && (root->focus!=this)) {
		//deliver to whoever has the focus, if root does not have it
				
		root->focus->key( p_unicode, p_scan_code, p_press, p_repeat, p_modifier_mask );
		return;
		
	}

	
	
	if (focus_child) {

		bool eat = focus_child->key( p_unicode, p_scan_code, p_press, p_repeat, p_modifier_mask );
	
		if (eat)
			return;
		
		
		if (p_press && p_scan_code==KEY_TAB  && !(p_modifier_mask&KEY_MASK_SHIFT)  && focus_child->get_parent()) {
	
			Frame *next_f=focus_child->get_parent()->next_in_focus( focus_child );
			if (next_f) {
	
				focus_child->focus_leave();
				focus_child->update();
				focus_child=next_f;
				next_f->focus_enter();
				next_f->update();
				
			}
			
			return;
		}
		
		if (p_press && p_scan_code==KEY_TAB && p_modifier_mask&KEY_MASK_SHIFT && focus_child->get_parent()) {
	
			Frame *prev_f=focus_child->get_parent()->prev_in_focus( focus_child );
			if (prev_f) {
	
				focus_child->focus_leave();
				focus_child->update();
				focus_child=prev_f;
				prev_f->focus_enter();
				prev_f->update();
				
			}
			
			return;
		}
		
		if (p_press && p_scan_code==KEY_UP) {
			
			int mindist=9999999;
			Frame *next_f = root_frame->find_closest_focusable_to( focus_child->get_global_pos(),UP, mindist );
			if (next_f) {
				
				focus_child->focus_leave();
				focus_child->update();
				focus_child=next_f;
				next_f->focus_enter();
				next_f->update();
				
			}
		}
		
		if (p_press && p_scan_code==KEY_LEFT) {
			
			int mindist=99999999;
			Frame *next_f = root_frame->find_closest_focusable_to( focus_child->get_global_pos(),LEFT, mindist );
			if (next_f) {
				
				focus_child->focus_leave();
				focus_child->update();
				focus_child=next_f;
				next_f->focus_enter();
				next_f->update();
				
			}
			
		}
		
		if (p_press && p_scan_code==KEY_RIGHT) {
			
			int mindist=9999999;
			Frame *next_f = root_frame->find_closest_focusable_to( focus_child->get_global_pos()+Size(focus_child->get_assigned_size().width,0),RIGHT, mindist );
			if (next_f) {
				
				focus_child->focus_leave();
				focus_child->update();
				focus_child=next_f;
				next_f->focus_enter();
				next_f->update();
				
			}
		}
		
		if (p_press && p_scan_code==KEY_DOWN) {
			
			int mindist=9999999;
			Frame *next_f = root_frame->find_closest_focusable_to( focus_child->get_global_pos()+Size(0,focus_child->get_assigned_size().height),DOWN, mindist );
			if (next_f) {
				
				focus_child->focus_leave();
				focus_child->update();
				focus_child=next_f;
				next_f->focus_enter();
				next_f->update();
				
			}			
		}
		
	} else {
		
		
		if (p_press && p_scan_code==KEY_TAB  && !(p_modifier_mask&KEY_MASK_SHIFT)) {
			
			Frame *next_f=root_frame->next_in_focus(0);
			if (next_f) {
				
				focus_child=next_f;
				next_f->focus_enter();
				next_f->update();
				
			}
			
			return;
		}
		
		if (p_press && p_scan_code==KEY_TAB && p_modifier_mask&KEY_MASK_SHIFT) {
			
			Frame *prev_f=root_frame->prev_in_focus(0);
			if (prev_f) {
				
				focus_child=prev_f;
				prev_f->focus_enter();
				prev_f->update();
				
			}
			
			return;
		}
		
	}
	
	if (p_press && p_scan_code==KEY_ESCAPE && parent && mode==MODE_POPUP && root->modal_stack && root->modal_stack->window==this) {
		
		popup_cancel_signal.call();
		hide();
					
	}
	/** No one handled the keypress, emiting it! */
	
	key_unhandled_signal.call(p_unicode,p_scan_code,p_press,p_repeat,p_modifier_mask);
	
}

void Window::clear_focus() {
	
	
}
void Window::frame_hide_notify(Frame *p_frame) {
	
	/* determine if focused frame is here */
	
	if (!focus_child)
		return;
	
	Frame *from=focus_child;
	bool found=false;
	
	while(from) {
		
		if (from==p_frame) {
			found=true;
			break;
		}
		from=from->get_parent();
	}
	
	if (!found)
		return; //nothing to do
	
	from->focus_leave();
	focus_child=root_frame->next_in_focus(); //see if something else can be focused
	
}

void Window::frame_deleted_notify(Frame *p_frame) {
	
	/* May seem like overload, but if a frame is deleted and has an window, I need to make sure
	it isnt used from here for anything else */
	
	if (last_under_cursor==p_frame)
		last_under_cursor=0;
	if (root_frame==p_frame)
		root_frame=0;
	if (drag.child==p_frame)
		drag.child=0;
	if (focus_child==p_frame)
		focus_child=0;
		
	
}

void Window::update(const Rect& p_rect) {

	
	draw_widgets_over_area( p_rect );
	Rect global=p_rect;
	global.pos+=get_global_pos();
	
	painter->update_screen_rect( global );

	

}

void Window::update() {
	
	painter->update_screen();
}

Frame * Window::get_focused_child() {
	
	return focus_child;
}

/*
void Window::queue_delete() {


	queued_delete=true;
}
*/

void Window::initialize() {


	parent=0;
	
	painter=0;
	root_frame=0;
	focus_child=0;
	last_under_cursor=0;
	drag.child=0;
	drag.button_mask=0;
	mode=MODE_NORMAL;
	size_mode=SIZE_NORMAL;
	visible=true;
	//queued_delete=false;
	next=0;
	childs=0;
	root=this;
	focus=this;
	modal_stack=0;
	skin=0;
	size_update_needed=true;
	no_local_updates=false;
	no_stretch_root_frame=false;
}

void Window::set_no_local_updates(bool p_disable) {
	
	no_local_updates=p_disable;
}

void Window::set_no_stretch_root_frames(bool p_disable) {

	no_stretch_root_frame=p_disable;
}

void Window::adjust_size_type() {

	switch (size_mode) {

		case SIZE_CENTER: {
			
			set_size( root_frame->get_minimum_size() );
			pos=(painter->get_display_size()-size)/2;
			
		} break;
		case SIZE_TOPLEVEL_CENTER: {
			
			set_size( painter->get_display_size()*3/4 );
	
			pos=(painter->get_display_size()-size)/2;
			
		} break;
		case SIZE_NORMAL: {
			
			/* Adjust so it doesnt show out of window */
			
			Point glob_pos=get_global_pos();
			Point root_size=painter->get_display_size(); //more reliable than window
			
			if ( glob_pos.y<0 )				
				pos.y-=glob_pos.y;
			if ( glob_pos.x<0 )				
				pos.x-=glob_pos.x;
			
			if ( (glob_pos.y+size.y)>root_size.y)
				pos.y-=(glob_pos.y+size.y)-root_size.y;
			if ( (glob_pos.x+size.x)>root_size.x)
				pos.x-=(glob_pos.x+size.x)-root_size.x;
		} break;
		
	}

}

void Window::show() {

	if (visible)
		return;
	
	if (mode==MODE_MODAL || mode==MODE_POPUP) {

		
		if (root && root->focus && root->focus->last_under_cursor) {
			
			root->focus->last_under_cursor->mouse_leave();	
			root->focus->last_under_cursor=0;
		}
		
		if (root->focus)
			root->focus->drag.child=0; //disable drag on whoever, just in case we clicked
		root->focus=this;
		ModalStack * ms = new ModalStack;
		ms->window=this;
		ms->next=root->modal_stack;
		root->modal_stack=ms;
		raise();
	}

	/* Adjust size */
	if (parent && root_frame) {
		
		adjust_size_type();
	}
	
	visible=true;
	drag.child=0;
	
	if (!parent)
		return;
	
	if (root_frame && !focus_child) {
		Frame *prev_f=root_frame->prev_in_focus(0);
		if (prev_f) {
			
			focus_child=prev_f;
			prev_f->focus_enter();
			prev_f->update();
			
		}	
	}
	redraw_all();
	painter->update_screen_rect( Rect(get_global_pos(), size) );
	
}

void Window::raise_window(Window *p_child) {


	
	bool found=false;

	Window *w=childs;
	Window **wr=&childs;


	if (w && !w->next) {//pointless, have only one child

		return;
	}
	
	while (w) {



		if (w==p_child) {

			if (!w->next) {
				

				return; //also pointless, already at end
			}

//			printf("RAISE found!\n");
			found=true;

			*wr=w->next;
			w=w->next;
			continue;
		}

		if (!w->next && found) {

//			printf("RAISE at end!\n");
			p_child->next=0;
			w->next=p_child;

		}

		wr=&w->next;
		w=w->next;

	}

	
	if (found) {
		//repaint!

	}
	
}

void Window::raise() {

	if (parent)
		parent->raise_window(this);

}

void Window::remove_from_modal_stack() {

	if (mode!=MODE_MODAL && mode!=MODE_POPUP)
		return; //otherwise it will never be in the modal stack
	

	ModalStack * m = root->modal_stack;
	ModalStack ** mr = &root->modal_stack;


	while (m) {


		if (m->window==this) {


			ModalStack *msa=m;
			*mr=m->next;
			m=m->next;
			continue;
				
		}

		mr=&m->next;
		m=m->next;
			
	}

	if (root->focus==this) {
		if (root->modal_stack)
			root->focus=root->modal_stack->window;
		else if (parent)
			root->focus=parent;

	}
		
}

void Window::hide() {

	remove_from_modal_stack();
	visible=false;
	if (!parent)
		return;
	Rect global_update_rect=Rect( get_global_pos(),size );
	painter->set_clip_rect(true, global_update_rect );
	root->redraw_all_internal(global_update_rect,0,false);
	painter->update_screen_rect( global_update_rect );
	painter->set_clip_rect(false);
	if (focus_child)
		focus_child->window_hid();

	//parent->update(Rect(pos,size));
}

Skin *Window::get_skin() {
	
	return skin;
}

Window::Window(Window *p_parent,Mode p_mode, SizeMode p_size_mode) {

	
	initialize();
	mode=p_mode;
	size_mode=p_size_mode;
	root=p_parent->root;
	focus=0;
	
	if (p_mode==MODE_MODAL || p_mode==MODE_POPUP) {

		visible=false;

	}

	
	parent=p_parent;
	
	next=p_parent->childs;
	p_parent->childs=this;

	painter=p_parent->painter;
	timer=p_parent->timer;
	skin=p_parent->skin;

}



bool Window::is_visible() {
	
	return visible;
}
Window::Window(Painter *p_painter,Timer *p_timer,Skin *p_skin) {


	
	initialize();

	painter=p_painter;
	skin=p_skin;
	timer=p_timer;

}

Window::~Window() {

	remove_from_modal_stack();

	if (root->focus==this) {
		
		root->focus=parent;
	}
	
//	if (parent)
		//parent->child_window.child=0; //parent lost a child
	
	if (root_frame)
		delete root_frame;
	
	root_frame=0;
}



}
