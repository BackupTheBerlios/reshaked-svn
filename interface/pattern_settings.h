#ifndef PATTERN_SETTINGS_H
#define PATTERN_SETTINGS_H

#include <qfont.h>
#include <qcolor.h>
namespace ReShaked {

class PatternSettings {
public:

	enum {
		HEADER_LINES=2,
		LINE_SPACING=2
	};

	enum Color {

                COLOR_PATTERN_BG,
		COLOR_PATTERN_FONT,
		COLOR_PATTERN_LIGHT,
		COLOR_PATTERN_SHADOW,
		COLOR_PATTERN_CURSOR,
		COLOR_MAX
	};


private:
	enum {
		DEFAULT_PATTERN_MARGIN=10
	};


	int pattern_margin;

	static PatternSettings *singleton;

	QFont font;
	QColor colors[COLOR_MAX];

	QColor pattern_bg;
	QColor pattern_font;
	QColor pattern_light;
	QColor pattern_shadow;
	QColor pattern_cursor;

public:


	static PatternSettings *get_singleton();

	int get_row_size();
	QFont get_font();
	void set_font(const QFont &p_font);
	int get_pattern_margin();
	QColor get_color(Color p_color);
	PatternSettings();

};

}


#endif