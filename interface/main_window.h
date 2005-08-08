#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "interface/uic/main_window_base.h"
#include <qscrollview.h>
#include <qsplitter.h>
#include <qscrollbar.h>
#include <qwidgetstack.h>

#include "engine/song.h"
#include "editing/song_edit.h"

#include "interface/track_list.h"
#include "interface/pattern_font.h"

namespace ReShaked {



class MainWindow : public MainWindowBase {

	Q_OBJECT

        PatternFont pattern_font;

	QSplitter *track_options_splitter;
	QScrollView *track_scroll;
	QScrollBar *v_scroll;
	QWidgetStack *bottom_stack;

	TrackList *track_list;
	struct Engine {

		UndoStream undoredo_stream;
		SongEdit *song_edit;
		Song *song;
	} engine;

	
protected slots:

	void add_pattern_track();
	
public:


	MainWindow();
};


}; /* end of namespace */

#endif