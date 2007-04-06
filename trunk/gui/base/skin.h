//
// C++ Interface: skin
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PIGUISKIN_H
#define PIGUISKIN_H

#include "base/defs.h"
#include "string/rstring.h"

namespace GUI {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

			
enum StyleBoxList{
	
	SB_ROOT_CONTAINER,
	SB_POPUP_BG,
				
	SB_SEPARATOR,
	SB_WINDOWTOP,
	
	SB_BUTTON_NORMAL,
	SB_BUTTON_PRESSED,
	SB_BUTTON_HOVER,
	SB_BUTTON_FOCUS,
	SB_BUTTON_CHECKED,
	SB_BUTTON_UNCHECKED,
	
	SB_CHECKBUTTON_NORMAL,
	SB_CHECKBUTTON_PRESSED,
	SB_CHECKBUTTON_HOVER,
	SB_CHECKBUTTON_FOCUS,
	SB_CHECKBUTTON_CHECKED,
	SB_CHECKBUTTON_UNCHECKED,
	
	SB_MENUBUTTON_NORMAL,
	SB_MENUBUTTON_PRESSED,
	SB_MENUBUTTON_HOVER,
	SB_MENUBUTTON_FOCUS,
	SB_MENUBUTTON_CHECKED,
	SB_MENUBUTTON_UNCHECKED,
	
	SB_OPTIONBUTTON_NORMAL,
	SB_OPTIONBUTTON_PRESSED,
	SB_OPTIONBUTTON_HOVER,
	SB_OPTIONBUTTON_FOCUS,
	
	SB_MENUBOX_NORMAL,
	SB_MENUBOX_PRESSED,
	SB_MENUBOX_HOVER,
	SB_MENUBOX_FOCUS,
	
	SB_ARROWBUTTON_NORMAL,
	SB_ARROWBUTTON_PRESSED,
	SB_ARROWBUTTON_HOVER,
	SB_ARROWBUTTON_FOCUS,
	
	SB_WINDOWBUTTON_NORMAL,
	SB_WINDOWBUTTON_PRESSED,
	SB_WINDOWBUTTON_HOVER,
	SB_WINDOWBUTTON_FOCUS,
	
	SB_LIST_NORMAL,
	SB_LIST_FOCUS,
	SB_LIST_SELECTED,
	SB_LIST_CURSOR,
	SB_LIST_EDITOR_BG,
		
	SB_SLIDER_NORMAL_H,
	SB_SLIDER_NORMAL_V,
	SB_SLIDER_FOCUS,
	SB_SLIDER_GRABBER_H,
	SB_SLIDER_GRABBER_V,
	
	SB_SCROLLBAR_NORMAL_H,
	SB_SCROLLBAR_NORMAL_V,
	SB_SCROLLBAR_FOCUS,
	SB_SCROLLBAR_GRABBER_H,
	SB_SCROLLBAR_GRABBER_V,
	
	SB_LINE_EDIT_NORMAL,
	SB_LINE_EDIT_FOCUS,

	SB_TEXT_EDIT,
	SB_TEXT_EDIT_FOCUS,
	
	SB_PROGRESSBAR_FRAME,
	SB_PROGRESSBAR_PROGRESS,
	
	SB_TABBAR_LEFT,
	SB_TABBAR_RIGHT,
	SB_TABBAR_TAB,
	SB_TABBAR_RAISED,
	SB_TABBAR_FOCUS,
		
	SB_COMBO_POPUP,
	
	SB_TABBOX,
	
	SB_MAX
};
	
enum ColorList {
		
	COLOR_WINDOWTOP_FONT,
	COLOR_WINDOWBUTTON_FONT,
	COLOR_LABEL_FONT,
	COLOR_BUTTON_FONT,
	COLOR_CHECKBUTTON_FONT,
	COLOR_MENUBUTTON_FONT,
	COLOR_OPTIONBUTTON_FONT,
	COLOR_MENUBOX_FONT,
	COLOR_LINE_EDIT_FONT,
	COLOR_LINE_EDIT_FONT_SELECTED,
	COLOR_TEXT_EDIT_FONT,
	COLOR_TEXT_EDIT_FONT_SELECTED,
	COLOR_LIST_FONT,
	COLOR_LIST_FONT_SELECTED,
	COLOR_PROGRESSBAR_FONT,
	COLOR_PROGRESSBAR_FONT_PROGRESS,
	
	COLOR_TABBAR_FONT,
	COLOR_TABBAR_FONT_RAISED,
	COLOR_ARROWBUTTON_ARROW_COLOR,
	COLOR_LINE_EDIT_SELECTION,
	COLOR_LINE_EDIT_CURSOR,
	COLOR_TEXT_EDIT_SELECTION,
	COLOR_TEXT_EDIT_CURSOR,
	COLOR_UPDOWN,
	COLOR_MAX	
};

enum ConstantList {

	C_SEPARATOR_EXTRA_MARGIN,
	C_BOX_CONTAINER_DEFAULT_SEPARATION,
	C_BUTTON_SEPARATION,
	C_BUTTON_EXTRA_MARGIN,
	C_BUTTON_DISPLACEMENT,
	C_BUTTON_HAS_CHECKBOX,
	C_BUTTON_LABEL_ALIGN_CENTER,
	C_BUTTON_CHECKBOX_SIZE,
	C_CHECKBUTTON_SEPARATION,
	C_CHECKBUTTON_EXTRA_MARGIN,
	C_CHECKBUTTON_DISPLACEMENT,
	C_CHECKBUTTON_CHECKBOX_SIZE,
	C_MENUBUTTON_SEPARATION,
	C_MENUBUTTON_EXTRA_MARGIN,
	C_MENUBUTTON_DISPLACEMENT,
	C_MENUBUTTON_CHECKBOX_SIZE,
	C_OPTIONBUTTON_SEPARATION,
	C_OPTIONBUTTON_EXTRA_MARGIN,
	C_OPTIONBUTTON_DISPLACEMENT,
	C_MENUBOX_SEPARATION,
	C_MENUBOX_EXTRA_MARGIN,
	C_MENUBOX_DISPLACEMENT,
	C_ARROWBUTTON_ARROW_SIZE,
	C_ARROWBUTTON_DISPLACEMENT,
	C_LABEL_MARGIN,
	C_LIST_SPACING,
	C_LIST_HMARGIN,
	C_LINE_EDIT_MARGIN,
	C_TEXT_EDIT_LINE_SPACING,
	C_SLIDER_GRABBER_SIZE,
	C_SCROLLBAR_GRABBER_SIZE,
	C_UPDOWN_ARROW_SIZE,
	C_UPDOWN_MARGIN,
	C_PROGRESSBAR_MARGIN,
	C_COMBOBOX_POPUP_MAX_HEIGHT,
	C_MARGIN_GROUP_SPACER_SIZE,
	C_TABBAR_CENTERED,
	C_TABBAR_TAB_SEPARATOR,
	C_TABBOX_SEPARATION,
	C_WINDOWBUTTON_DISPLACEMENT,
	C_WINDOWBOX_SPACER,
	C_MAX
};
	
enum FontList {
	
	FONT_LABEL,
	FONT_BUTTON,
	FONT_CHECKBUTTON,
	FONT_MENUBUTTON,
	FONT_OPTIONBUTTON,
	FONT_MENUBOX,
	FONT_LIST,
	FONT_LINE_EDIT,
	FONT_TEXT_EDIT,
	FONT_PROGRESSBAR,
	FONT_TABBAR,
	FONT_WINDOWTOP,
	FONT_WINDOWBUTTON,
	FONT_MAX
};

enum BitmapList {
	
	BITMAP_BUTTON_CHECKED,
	BITMAP_BUTTON_UNCHECKED,
	BITMAP_CHECKBUTTON_CHECKED,
	BITMAP_CHECKBUTTON_UNCHECKED,
	BITMAP_MENUBUTTON_CHECKED,
	BITMAP_MENUBUTTON_UNCHECKED,
	BITMAP_SLIDER_GRABBER_H,
	BITMAP_SLIDER_GRABBER_V,
	BITMAP_UPDOWN,
	BITMAP_WINDOWBUTTON_CLOSE,
	BITMAP_WINDOWBUTTON_MAXIMIZE,
	BITMAP_WINDOWBUTTON_MINIMIZE,
	BITMAP_MAX,
};


class StyleBox;
class Color;

class Skin{
	
	StyleBox *style_box_array;
	const char **style_box_name;
	int style_box_count;
	
	int *constant_array;
	const char **constant_name;
	int constant_count;
	
	BitmapID *bitmap_array;
	const char **bitmap_name;
	int bitmap_count;
	
	FontID *font_array;
	const char **font_name;
	int font_count;
	
	Color *color_array;
	const char **color_name;
	int color_count;
protected:
	void set_stylebox_name(int p_which,const char* p_name);
	void set_constant_name(int p_which,const char* p_name);
	void set_bitmap_name(int p_which,const char* p_name);
	void set_font_name(int p_which,const char* p_name);
	void set_color_name(int p_which,const char* p_name);
			
	virtual void set_default_extra() {};
public:
	
	const StyleBox& get_stylebox(unsigned int p_which);
	int get_constant(unsigned int p_which);
	BitmapID get_bitmap(unsigned int p_which);
	FontID get_font(unsigned int p_which);
	const Color& get_color(unsigned int p_which);
	
	void set_stylebox(int p_which,const StyleBox& p_style);
	void set_constant(int p_which,int p_constant);
	void set_bitmap(int p_which,BitmapID p_bitmap);
	void set_font(int p_which,FontID p_font);
	void set_color(int p_which,const Color& p_color);
	
	int find_stylebox(String p_by_name);
	int find_constant(String p_by_name);
	int find_bitmap(String p_by_name);
	int find_font(String p_by_name);
	int find_color(String p_by_name);
	
	int get_stylebox_count() { return style_box_count; }
	int get_constant_count() { return constant_count; }
	int get_bitmap_count() { return bitmap_count; }
	int get_font_count() { return font_count; }
	int get_color_count() { return color_count; }
	
	void set_default();
	
	Skin(int p_stylebox_max=SB_MAX,int p_constant_max=C_MAX,int p_bitmap_max=BITMAP_MAX,int p_font_max=FONT_MAX,int p_color_max=COLOR_MAX);
	virtual ~Skin();

};

}

#endif
