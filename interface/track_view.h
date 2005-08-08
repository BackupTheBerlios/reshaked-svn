#ifndef TRACK_VIEW_H
#define TRACK_VIEW_H

#include <qwidget.h>
#include "editing/track_edit.h"

namespace ReShaked {


class TrackView : public QWidget {

	TrackEdit *track_edit;		


public:

	TrackView(QWidget *p_parent,TrackEdit *p_track);

};

};



#endif