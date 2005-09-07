//
// C++ Interface: global_edit
//
// Description:
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef GLOBAL_EDIT_H
#define GLOBAL_EDIT_H

namespace ReShaked {

class GlobalEdit {

	static int editing_octave;

public:

	static int get_editing_octave();
	static void set_editing_octave(int p_octave);

};

};


#endif