#include "pattern_settings.h"
#include <qfontmetrics.h>

namespace ReShaked {



PatternSettings * PatternSettings::singleton=NULL;

PatternSettings *PatternSettings::get_singleton() {

    return singleton;
}

QFont PatternSettings::get_font() {

    return font;
}
void PatternSettings::set_font(const QFont &p_font) {

    font=p_font;
}

int PatternSettings::get_pattern_margin() {

	return pattern_margin;
}

QColor PatternSettings::get_color(Color p_color) {

	if (p_color==COLOR_MAX)
		return QColor(0,0,0);

	return colors[p_color];
}

int PatternSettings::get_row_size() {

	QFontMetrics metrics(font);
	return metrics.height()+LINE_SPACING;
}

int PatternSettings::get_editing_octave() {

	return editing_octave;
}
void PatternSettings::set_editing_octave(int p_octave) {

	editing_octave=p_octave;

}

PatternSettings::PatternSettings() {

    font.setPointSize(11);
    font.setFamily("fixed");
    font.setBold(true);
    font.setStyleHint(QFont::Courier,QFont::NoAntialias);

    editing_octave=4;

    singleton=this;
    pattern_margin=DEFAULT_PATTERN_MARGIN;

    colors[COLOR_PATTERN_BG]=QColor(63,79,44);
    colors[COLOR_PATTERN_BG_FOCUS]=QColor(81,100,59);
    colors[COLOR_PATTERN_FONT]=QColor(255,255,255);
    colors[COLOR_PATTERN_LIGHT]=QColor(41,51,27);
    colors[COLOR_PATTERN_SHADOW]=QColor(176,200,142);
    colors[COLOR_PATTERN_CURSOR]=QColor(255,0,0);
    colors[COLOR_PATTERN_VOLUME]=QColor(255,255,0);

    colors[COLOR_PATTERN_NOTEOFF]=QColor(255,140,100);
}


};