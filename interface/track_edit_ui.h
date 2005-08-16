#ifndef TRACK_EDIT_UI_H
#define TRACK_EDIT_UI_H


#include <qhbox.h>
#include <qlabel.h>
#include "editing/track_edit.h"
#include "editing/song_edit.h"
#include "interface/track_view.h"

namespace ReShaked {


class TrackEditUI : public QHBox {

	Q_OBJECT

	
	Track *track;
	TrackEdit *track_edit;
	
	int track_id;
	
	TrackView *track_view;

public:

	Track *get_track();
	int get_track_ID();
	TrackEditUI(QWidget *p_parent,SongEdit *p_song_Edit,int p_track);

};

};

#endif