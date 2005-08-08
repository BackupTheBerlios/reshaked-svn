#include "pattern_font.h"

namespace ReShaked {


PatternFont * PatternFont::singleton=NULL;

PatternFont *PatternFont::get_singleton() {

    return singleton;
}

QFont PatternFont::get_font() {

    return font;
}
void PatternFont::set_font(const QFont &p_font) {

    font=p_font;
}

PatternFont::PatternFont() {

    font.setPointSize(20);
    font.setFamily("Fixed");
    singleton=this;

}

};