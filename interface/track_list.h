//
// C++ Interface: track_list
//
// Description: 
//
//
// Author: Juan Linietsky <coding@reduz.com.ar>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef TRACK_LIST_H
#define TRACK_LIST_H

#include <qvbox.h>
#include <vector>

#include "interface/track_edit_ui.h"

namespace ReShaked {


class TrackList : public QHBox {
	


	std::vector<TrackEditUI*> track_edits; /* track views */

	
public:	

	void update_track_views();	
	TrackList(QWidget *p_parent);	
};




};

#endif
