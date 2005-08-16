#ifndef TRACK_VIEW_H
#define TRACK_VIEW_H

#include <qwidget.h>
#include "editing/track_edit.h"

namespace ReShaked {


class TrackView : public QWidget {

	TrackEdit *track_edit;		


	int track_idx;
public:

	int get_track_index();
	TrackView(QWidget *p_parent,TrackEdit *p_track,int p_track_idx);

};

};



#endif