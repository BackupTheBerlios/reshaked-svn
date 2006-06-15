//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <Qt/qpainter.h>
#include <Qt/qevent.h>
#include "piano_keys.h"
#include "pianokeys.h"

namespace ReShaked {

Piano_Keys::Skin::Skin(QPixmap p_white_key,QPixmap p_white_key_pressed,QPixmap p_black_key,QPixmap p_black_key_pressed) {

	white_key=p_white_key;
	black_key=p_black_key;
	white_key_pressed=p_white_key_pressed;
	black_key_pressed=p_black_key_pressed;
	
}

void Piano_Keys::paintEvent(QPaintEvent *p_event) {


	QPainter painter(this);

	/* draw upper part first */

	static bool black_left[7]={0,1,1,0,1,1,1};
	static int key_offset[7]={0,2,4,5,7,9,11};

	/* Draw upper part */

	for (int i=0;i<visible_octaves*7;i++) {
		
		int key_idx=(editor->get_editing_octave()+i/7)*12+key_offset[i%7];
		if (key_idx>=MAX_KEY)
			break;
		int ofs=i*skin.white_key.width();
		painter.drawPixmap(ofs,0, keys[key_idx].pressed?skin.white_key_pressed:skin.white_key);
		if (black_left[i%7]) {
			
			key_idx--;
			if (key_idx<0)
				continue;
			ofs-=skin.black_key.width()/2;
			painter.drawPixmap(ofs,0, keys[key_idx].pressed?skin.black_key_pressed:skin.black_key);
			
		}
		
	}
	
	if (hasFocus()) {
		
		painter.setPen(QColor(255,100,100));
		painter.drawRect(0,0,width()-1,height()-1);
	}
	
}

int Piano_Keys::get_key_from_pos(const QMouseEvent & p_pos) {

	if (p_pos.pos().x()<0)
		return INVALID_KEY;

	static int key_offset[7]={0,2,4,5,7,9,11};
	static bool black_left[7]={0,1,1,0,1,1,1};
	static bool black_right[7]={1,1,0,1,1,1,0};

	int midpoint1=skin.white_key.width()/4;
	int midpoint2=skin.white_key.width()-midpoint1;

	int key=p_pos.pos().x()/skin.white_key.width();
	int key_pos=p_pos.x() % skin.white_key.width();
	int octave=key/7;
	key%=7;
	int key_final=key_offset[key];

	if (p_pos.y()<(height()/2)) {
		if (black_left[key] && key_pos<=midpoint1)
			key_final--;
		else if (black_right[key] && key_pos>=midpoint2)
			key_final++;
	}
	
	int octave_offset=editor->get_editing_octave();
	if (octave_offset<0)
		octave_offset=0;

	key_final+=(octave_offset+octave)*12;
	if (key_final<0 || key_final>=MAX_KEY)
		return INVALID_KEY;

	return key_final;

}

void Piano_Keys::mousePressEvent( QMouseEvent * e ) {

	if (e->button()!=Qt::LeftButton)
		return;

	int key=get_key_from_pos(*e);
	if (key==INVALID_KEY)
		return;

	mouse_key_drag=key;
	press_key(key);

	update();

}


void Piano_Keys::press_key(int p_key,bool p_keyboard) {

	if (p_key<0 || p_key>=MAX_KEY)
		return;

	if (!keys[p_key].pressed)
		key_pressed_signal(p_key);

	keys[p_key].pressed++;
	keys[p_key].keyboard=p_keyboard;



}
void Piano_Keys::release_key(int p_key) {

	if (p_key<0 || p_key>=MAX_KEY)
		return;

	if (keys[p_key].pressed) {

	    keys[p_key].pressed--;
	    if (!keys[p_key].pressed)
	    	key_released_signal(p_key);

	}
}


void Piano_Keys::mouseMoveEvent ( QMouseEvent * e ) {

	int key=get_key_from_pos(*e);

	if (key!=INVALID_KEY)
		note_over_signal(key);
	
	if (mouse_key_drag!=MOUSE_KEY_NO_DRAG && mouse_key_drag!=key) { //mouse key over changed
		release_key(mouse_key_drag);
		if (key!=INVALID_KEY) {
			press_key(key);
			mouse_key_drag=key;
		}

		update();
	}


}

void Piano_Keys::mouseReleaseEvent( QMouseEvent * e ) {

	if (e->button()!=Qt::LeftButton)
		return;


        if (mouse_key_drag!=MOUSE_KEY_NO_DRAG) {
		release_key(mouse_key_drag);
		mouse_key_drag=MOUSE_KEY_NO_DRAG;
		update();
	}
}

/* Keyboard SHORKUTS */


void Piano_Keys::octave_changed_slot() {
	
	for (int i=0;i<MAX_KEY;i++) {
			
		if (keys[i].pressed) { //mute all keys when switching octave, just by security
				
			key_released_signal(i);
			keys[i].pressed=0;	
			keys[i].keyboard=0;	
		}	
	}
		
	update();
	
	
}

int Piano_Keys::get_key_from_key_event( QKeyEvent * e ) {

	for (int i=KEY_C_0;i<MAX_KEYS;i++) {

		if (IS_KEYBIND(String("note_entry/")+key_name[i],e->key()))
			return ( (i-KEY_C_0) + 12*editor->get_editing_octave()) ;
	}

	return INVALID_KEY;
}

void Piano_Keys::focusOutEvent ( QFocusEvent * event ) {
	
	for (int i=0;i<MAX_KEY;i++) { //release keybiord-pressed keys
			
		if (keys[i].pressed && keys[i].keyboard) { //mute all keys when switching octave, just by security
				
			key_released_signal(i);
			keys[i].pressed=0;	
			keys[i].keyboard=0;	
		}	
	}
	update();
}


void Piano_Keys::keyPressEvent ( QKeyEvent * e ) {

	if (e->isAutoRepeat())
		return;

	if (IS_KEYBIND(String("global/raise_octave"),e->key())) {
		
		editor->set_editing_octave( editor->get_editing_octave()+1 );
		
	} else if (IS_KEYBIND(String("global/lower_octave"),e->key())) {
	
		editor->set_editing_octave( editor->get_editing_octave()-1 );
		
	} else {
		
		int key=get_key_from_key_event(e);
		if (key==INVALID_KEY)
			return;
		press_key(key,true);

		update();
	}

}

void Piano_Keys::keyReleaseEvent ( QKeyEvent * e ) {

	if (e->isAutoRepeat())
		return;
	
	int key=get_key_from_key_event(e);
	if (key==INVALID_KEY)
		return;
	release_key(key);
	update();
}


Piano_Keys::Piano_Keys(QWidget *p_parent,const Skin & p_skin,Editor *p_editor,int p_visible_octaves) :QWidget(p_parent) {

	editor=p_editor;
	visible_octaves=p_visible_octaves;
	setFixedHeight(p_skin.white_key.height());
	setFixedWidth(p_visible_octaves*p_skin.white_key.width()*7);
	setBackgroundRole(QPalette::NoRole);
	setMouseTracking(true);
	
	setFocusPolicy( Qt::ClickFocus );

	mouse_key_drag=MOUSE_KEY_NO_DRAG;
	skin=p_skin;

}


Piano_Keys::~Piano_Keys()
{
}


} //end of namespace
