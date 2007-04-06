
#include "widget.h"
#include "base/window.h"

#include <stdio.h>
namespace GUI {

void __print_error(const char *p_file,int p_line,const char* p_error) {
	
	printf("ERROR: at %s:%i -- %s\n",p_file,p_line,p_error);

	
}


struct WidgetPrivate {
	
	Size minimum_size;
	
};

Frame* Widget::find_closest_focusable_to(Point p_point,Direction p_dir,int &mindist) {
	
	return 0;
}

void Widget::check_for_updates(const Point& p_pos,const Size &p_size,const StyleBox& p_bg_style,const Rect& p_bg_rect) {
	
	
}


void Widget::set_minimum_size(const Size & p_size ) {
	
	_wp->minimum_size=p_size;
}
Size Widget::get_minimum_size() {
	
	Size internal=get_minimum_size_internal();
	if (internal.width < _wp->minimum_size.width )
		internal.width=_wp->minimum_size.width;
				
	if (internal.height < _wp->minimum_size.height )
		internal.height=_wp->minimum_size.height;
		
	return internal;
}

Size Widget::get_minimum_size_internal() {
	
	return Size();
}


Timer *Widget::get_timer() {
	
	if (!get_window())
		return 0;
	return get_window()->get_timer();
	
}


Painter *Widget::get_painter() {
	if (!get_window())
		return 0;
	return get_window()->get_painter();
}

void Widget::resize(const Size& p_new_size) {
	
	
}

void Widget::draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed) {
	
	
}

void Widget::mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask) {
	
	
}
void Widget::mouse_motion(const Point& p_pos, const Point& p_rel, int p_button_mask) {
	
	
}

void Widget::mouse_doubleclick(const Point& p_pos,int p_modifier_mask) {
	
	
}

bool Widget::key(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask) {
	
	return false;
}

bool Widget::draw_tree(const Point& p_global,const Size& p_size,const Rect& p_exposed) {

	Rect draw_rect=p_exposed;


	if (needs_update() && has_update_rect()) {
		
		if (!p_exposed.intersects_with( get_update_rect() ))
			return true; //dont even bother drawing, nothing should be seen
		
		draw_rect=get_update_rect().clip(draw_rect);

	}

	
	draw(p_global, p_size, draw_rect);
	
	if (needs_update()) {

		
		if (has_update_rect() && !p_exposed.contains(get_update_rect()))
			return true;

		if (!has_update_rect() && !p_exposed.contains( Rect( Point(), p_size ) ) ) {

			return true;

		}
		
		cancel_update();
		
	} else if (child_needs_update()) { //just in case...
		
		cancel_update();
	}
	
		


	return false;
	
	
}

Widget::Widget() {
	
	_wp = new WidgetPrivate;
}
Widget::~Widget() {
	
	delete _wp;
}



} //end of namespace