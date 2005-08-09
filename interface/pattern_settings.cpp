#include "pattern_settings.h"

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
PatternSettings::PatternSettings() {

    font.setPointSize(20);
    font.setFamily("Fixed");
    singleton=this;
    pattern_margin=DEFAULT_PATTERN_MARGIN;


    colors[COLOR_PATTERN_BG]=QColor(3,45,20);
    colors[COLOR_PATTERN_FONT]=QColor(137,236,177);
    colors[COLOR_PATTERN_LIGHT]=QColor(29,154,79);
    colors[COLOR_PATTERN_SHADOW]=QColor(0,10,0);
    colors[COLOR_PATTERN_CURSOR]=QColor(210,255,228);
}


};