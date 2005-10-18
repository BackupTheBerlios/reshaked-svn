
#include "track_view.h"



namespace ReShaked {


int TrackView::get_track_index() {

	return track_idx;
}                    

TrackView::TrackView(QWidget *p_parent,TrackEdit *p_track_edit,int p_track_idx) : QWidget(p_parent) {

	track_idx=p_track_idx;
	setBackgroundMode(Qt::NoBackground);
	track_edit=p_track_edit;

}





};