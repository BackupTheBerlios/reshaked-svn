//
// C++ Implementation: track_list
//
// Description: 
//
//
// Author: Juan Linietsky <coding@reduz.com.ar>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "track_list.h"

namespace ReShaked {


void TrackList::update_track_views() {


	for(int i=0;i<track_edits.size();i++) 
		delete track_edits[i];

	track_edits.clear();
	printf("adding %i tracks\n",song_edit->get_track_count());
	for (int i=0;i<song_edit->get_track_count();i++) {

		track_edits.push_back( new TrackEditUI( shrunk_hbox ,  song_edit->get_track_edit(i) ) );
		track_edits[track_edits.size()-1]->show();

	}
	
}

TrackList::TrackList(SongEdit *p_song_edit,QWidget *p_parent) : QHBox(p_parent) {

	song_edit=p_song_edit;
	shrunk_hbox = new QHBox(this);
	shrunk_hbox->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Expanding);
	QFrame *exp_fr = new QFrame(this);
	exp_fr->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

}

}