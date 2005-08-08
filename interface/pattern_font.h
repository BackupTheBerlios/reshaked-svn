#ifndef PATTERN_FONT_H
#define PATTERN_FONT_H

#include <qfont.h>

namespace ReShaked {

class PatternFont {

    static PatternFont *singleton;

    QFont font;

public:

    static PatternFont *get_singleton();

    QFont get_font();
    void set_font(const QFont &p_font);

    PatternFont();

};

}


#endif