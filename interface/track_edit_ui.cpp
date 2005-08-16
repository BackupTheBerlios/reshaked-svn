#include "track_edit_ui.h"
#include "editing/pattern_edit.h"
#include "interface/track_view_pattern.h"

namespace ReShaked {

Track *TrackEditUI::get_track() {


	return track;

}

int TrackEditUI::get_track_ID() {

	return track_id;

}


TrackEditUI::TrackEditUI(QWidget *p_parent,SongEdit *p_song_edit,int p_track) : QHBox(p_parent) {

	TrackEdit *track_edit=p_song_edit->get_track_edit(p_track);
	track=track_edit->get_track();
	track_id=track->get_ID();

	if (dynamic_cast<PatternEdit*>(track_edit)) {

		track_view = new TrackViewPattern(this,dynamic_cast<PatternEdit*>(track_edit),p_song_edit,p_track);

	} else {
		ERR_PRINT("No match for p_track_edit!");
	}


}


}