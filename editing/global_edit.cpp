//
// C++ Implementation: global_edit
//
// Description:
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "global_edit.h"

namespace ReShaked {

int GlobalEdit::editing_octave=4;;



int GlobalEdit::get_editing_octave() {


	return editing_octave;
}
void GlobalEdit::set_editing_octave(int p_octave) {

	editing_octave=p_octave;

}


};