

#include "frame.h"


#include "base/container.h"
#include "base/window.h"
#include "base/skin.h"
namespace GUI {

struct FramePrivate {
	
	bool can_fill_vertical;	
	bool can_fill_horizontal;	
	Container *parent;	
	bool update_needed;
	bool child_needs_update;
	bool has_update_rect;
	Rect update_rect;
	Window *window;
	FocusMode focus_mode;
	bool visible;
	bool clipping;
	bool bg_on_updates;
	Skin *skin;	
	FramePrivate() { can_fill_vertical=true; can_fill_horizontal=true; parent=0; window=0; update_needed=false; child_needs_update=false; focus_mode=FOCUS_NONE; visible=true; skin=0; has_update_rect=false; clipping=false; bg_on_updates=true; }
};


void Frame::window_hid() {}

const StyleBox& Frame::stylebox(int p_which) {
	
	static StyleBox none;
	if (!_fp->skin)
		return none;

	return _fp->skin->get_stylebox( p_which );
	
}
FontID Frame::font(int p_which){
	
	if (!_fp->skin)
		return 0;
	
	return _fp->skin->get_font( p_which );
	
}
BitmapID Frame::bitmap(int p_which){
	
	if (!_fp->skin)
		return 0;
	
	return _fp->skin->get_bitmap( p_which );
	
}
int Frame::constant(int p_which){
	
	if (!_fp->skin)
		return 0;
	
	return _fp->skin->get_constant( p_which );
	
}
const Color& Frame::color(int p_which){
	
	static Color none;
	if (!_fp->skin)
		return none;
	
	return _fp->skin->get_color( p_which );
	
}

void Frame::set_clipping(bool p_clipping) {
	
	_fp->clipping=p_clipping;
}
bool Frame::is_clipping() {
	
	
	return _fp->clipping;
}

void Frame::set_in_window() {


}

void Frame::skin_changed() {
	
	
}

bool Frame::has_update_rect() {

	return _fp->has_update_rect;
}

Rect Frame::get_update_rect() {

	return _fp->update_rect;
}

void Frame::set_bg_on_updates(bool p_draw) {
	
	_fp->bg_on_updates=p_draw;
}
bool Frame::has_bg_on_updates() {
	
	return _fp->bg_on_updates;
}

void Frame::update(bool p_only_rect,const Rect& p_rect) {

	if (p_only_rect==false) {
		_fp->has_update_rect=false;
	}
	
	if (_fp->update_needed && !_fp->has_update_rect && p_only_rect)
		return; //already know that, and cant upgrade to a rect


	if (p_only_rect) {

		_fp->has_update_rect=true;
		_fp->update_rect=p_rect; //merge with previous ones?
	}
		   
	_fp->update_needed=true;	
	
	/* Create Update Tree, propagating the "Child Needs Update" */
	
	Frame *f = _fp->parent;
	
	while (f) {
		
		f->_fp->child_needs_update=true;
		f=f->_fp->parent;		
	}
}

bool Frame::child_needs_update() {
	
	return _fp->child_needs_update;
}


bool Frame::needs_update() {
	
	return _fp->update_needed;	
}
void Frame::cancel_update() {


	_fp->update_needed=false;	
	_fp->child_needs_update=false;
	_fp->has_update_rect=false;

}

FocusMode Frame::get_focus_mode() {
	
	return _fp->focus_mode;
}
void Frame::set_focus_mode(FocusMode p_focus_mode) {
	
	_fp->focus_mode=p_focus_mode;
}

void Frame::get_focus() {


	if (get_window() && _fp->focus_mode!=FOCUS_NONE) {

		
		get_window()->set_focus( this );
	}
	
}

bool Frame::has_focus() {
	
	if (!_fp->window)
		return false;
	return (_fp->window->get_focused_child()==this);
}

void Frame::set_fill_vertical(bool p_fill) {
	
	_fp->can_fill_vertical=p_fill;
}
void Frame::set_fill_horizontal(bool p_fill) {
	
	_fp->can_fill_horizontal=p_fill;
}	
bool Frame::can_fill_vertical() {
	return _fp->can_fill_vertical;
}

bool Frame::can_fill_horizontal() {
	
	return _fp->can_fill_horizontal;
	
}
	
void Frame::set_minimum_size_changed() {

	if (!_fp->window) //pointless, means it wasnt added yet
		return;
	
	if (_fp->parent)
		_fp->parent->adjust_minimum_size();
}
	
	
void Frame::set_parent(Container *p_container) {
	
	_fp->parent=p_container;
}

Container *Frame::get_parent() {

	return _fp->parent;
}
	
Frame* Frame::get_child_at_pos(const Point& p_point,const Size &p_size,Point *p_frame_local_pos) {
		
	if (p_point.x < 0)
		return 0;
	if (p_point.y < 0)
		return 0;
	
	if (p_point.x >= p_size.width)
		return 0;
	if (p_point.y >= p_size.height)
		return 0;
	
	if (p_frame_local_pos)
		*p_frame_local_pos=p_point;
	return this;
}
void Frame::mouse_enter() {}
void Frame::mouse_leave() {}

void Frame::focus_enter() {
	
}
void Frame::focus_leave() {
	
}

void Frame::set_window(Window *p_window) {
	
	_fp->window=p_window;
	if (p_window)
		_fp->skin=p_window->get_skin();
	set_in_window();
}


Window *Frame::get_window() {
	
	return _fp->window;
}

void Frame::show() {
	
	if (_fp->visible)
		return;
	
	_fp->visible=true;
//	Frame::set_minimum_size_changed();	
	if (_fp->parent)
		_fp->parent->adjust_minimum_size();
	
	update();
}
void Frame::hide() {
	
	if (!_fp->visible)
		return;

	_fp->visible=false;
	if (_fp->parent)
		_fp->parent->adjust_minimum_size();
	
	/* Set hide notify, to avoid focus to continue sending to a hdiden widget */
	if (get_window())
		get_window()->frame_hide_notify(this);
	//Frame::set_minimum_size_changed();
}
bool Frame::is_visible() {
	
	return _fp->visible;
}
	
Frame::Frame() {
	
	_fp = new FramePrivate;
}

Frame *Frame::next_in_focus( Frame *p_from ) {

	
	if (get_focus_mode()==FOCUS_ALL)
		return this;

	return 0;
}
Frame *Frame::prev_in_focus( Frame *p_from ) {


	if (get_focus_mode()==FOCUS_ALL)
		return this;

	return 0;
	
}

Size Frame::get_assigned_size() {
	
	if (_fp->parent) {
		
		return _fp->parent->get_child_size( this );
	} 
	
	return Size();
	
}

Point Frame::get_global_pos() {

	if (_fp->parent) {


		return _fp->parent->get_child_pos( this )+_fp->parent->get_global_pos();
	} 

	return Point();

}

bool Frame::is_far_parent_of(Frame *p_frame) {
	
	while (p_frame) {
		
		if (p_frame==this)
			return true;
		p_frame=p_frame->get_parent();
	}
	
	return false;
}
String Frame::get_type() {

	
	return "Frame";
}

Frame::~Frame() {
	
	deleted_signal.call();
	
	if (_fp->window)
		_fp->window->frame_deleted_notify( this );
	
	delete _fp;
}

}; //end of namespace
