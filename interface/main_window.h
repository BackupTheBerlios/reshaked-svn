#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "interface/uic/main_window_base.h"
#include <qscrollview.h>
#include <qsplitter.h>
#include <qscrollbar.h>
#include <qwidgetstack.h>

#include "engine/song.h"
#include "editing/song_edit.h"
#include "editing/keyboard_input.h"

#include "interface/track_list.h"
#include "interface/pattern_settings.h"

#include "interface/edit_toolbar.h"
#include "interface/row_list.h"
#include "interface/cursor_qt.h"

namespace ReShaked {



class MainWindow : public MainWindowBase {

	Q_OBJECT

        PatternSettings pattern_settings;

	QSplitter *track_options_splitter;
	QScrollView *track_scroll;
	QScrollBar *v_scroll;
	QWidgetStack *bottom_stack;
	RowList *row_list;
	CursorQt *cursor;

	TrackList *track_list;
	
	EditToolbar *edit_toolbar;
	
	struct Engine {

		Keyboard_Input key_input;
		UndoStream undoredo_stream;
		SongEdit *song_edit;
		Song *song;
	} engine;

	void add_default_key_binds();
	
	
protected slots:

	void add_pattern_track();
	void snap_selected_change(int p_to_new);
	
public:


	MainWindow();
};


}; /* end of namespace */

#endif