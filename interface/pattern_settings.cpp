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
PatternSettings::PatternSettings() {

    font.setPointSize(11);
    font.setFamily("fixed");
    font.setBold(true);
    font.setStyleHint(QFont::Courier,QFont::NoAntialias);

    singleton=this;
    pattern_margin=DEFAULT_PATTERN_MARGIN;
                                                                 
    colors[COLOR_PATTERN_BG]=QColor(3,45,20);
    colors[COLOR_PATTERN_BG_FOCUS]=QColor(33,75,50);
    colors[COLOR_PATTERN_FONT]=QColor(137,236,177);
    colors[COLOR_PATTERN_LIGHT]=QColor(29,154,79);
    colors[COLOR_PATTERN_SHADOW]=QColor(0,10,0);
    colors[COLOR_PATTERN_CURSOR]=QColor(210,255,228);
}


};