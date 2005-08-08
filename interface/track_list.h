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
#include <qhbox.h>
#include <vector>

#include "interface/track_edit_ui.h"
#include "editing/song_edit.h"

namespace ReShaked {


class TrackList : public QHBox {
	

	SongEdit *song_edit;
	std::vector<TrackEditUI*> track_edits; /* track views */
	QHBox *shrunk_hbox;
	
public:	


	void update_track_views();	
	TrackList(SongEdit *p_song_edit,QWidget *p_parent);
};




};

#endif
