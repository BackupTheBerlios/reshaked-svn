

#ifndef MISC_FUNCS_H
#define MISC_FUNCS_H

#include "rstring.h"
#include <vector>
namespace ReShaked {

String get_note_str(int p_note);

std::vector<String> split_str(String::CharType p_key,String p_string);

int get_decimal_count(double p_number);

}


#endif


