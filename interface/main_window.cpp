

#include "main_window.h"
#include <qhbox.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qaction.h>
#include <qinputdialog.h>

namespace ReShaked {

void MainWindow::add_pattern_track() {
	printf("adding track\n");
	engine.song_edit->add_pattern_track(2);
	track_list->update_track_views();
}

class ExpScrollView : public QScrollView {

	QWidget *child;
	
	/*void resizeEvent ( QResizeEvent * p_resevebt) {
		if (child==NULL)
			return;
		printf("resizeThing\n");
		
		child->resize(2,2);
		printf("viewportheight is %i\n",viewport()->height());
		
} */

	

public:

	void add_child(QWidget *p_child) { child =p_child; addChild(p_child,0,0); }

	ExpScrollView(QWidget *p_parent) : QScrollView(p_parent) {
		 child = NULL;
		 setResizePolicy(QScrollView::AutoOneFit);
		 setFrameStyle(NoFrame);		 
		// viewport()->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	}
};

void MainWindow::snap_selected_change(int p_to_new) {
	
	
	engine.song_edit->get_cursor().set_snap(p_to_new);
	track_list->repaint_track_views();	
}

void MainWindow::pattern_add_slot() {
	
	bool ok;
	QString text = QInputDialog::getText("Pattern Name?", "Enter Pattern Name:", QLineEdit::Normal,
	QString::null, &ok, this );
	if ( ok ) {
        	
		engine.song_edit->pattern_make_at_cursor(text.ascii()); //must change that immediately
	} 
	
	track_list->repaint_track_views();
	cursor_pos_changed_slot(true);
}	
void MainWindow::pattern_insert_slot() {
	
	
	
}
void MainWindow::pattern_erase_slot() {
	
	
	
}
void MainWindow::pattern_change_slot() {
	
	
	
}


void MainWindow::cursor_pos_changed_slot(bool p_force_update) {
	
	
	int beat=cursor->get_beat();
	int pattern=engine.song->get_pattern_list().get_pattern_from_global_beat(beat);
	if (pattern==cache.current_pattern && !p_force_update)
		return;
	pattern_current_line_edit->setText( QString::number(pattern) );
	cache.current_pattern=pattern;
	
}


MainWindow::MainWindow() {

	
	cache.current_pattern=-1;	

	add_default_key_binds();
	cursor = new CursorQt(this);
	
	engine.song = new Song;
	engine.song_edit = new SongEdit(cursor, &engine.undoredo_stream , engine.song );
	
	
	track_options_splitter = new QSplitter(Qt::Vertical,this);
	setCentralWidget(track_options_splitter);

	QHBox * top_hbox = new QHBox(track_options_splitter);
	row_list = new RowList(top_hbox,&engine.song_edit->get_cursor());
	track_scroll = new ExpScrollView(top_hbox);
	
	track_list = new TrackList(engine.song_edit,track_scroll->viewport());
	
	((ExpScrollView*)track_scroll)->add_child(track_list);
	track_list->show();
	track_list->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

	v_scroll = new QScrollBar(top_hbox);
	bottom_stack = new QWidgetStack(track_options_splitter);

	QObject::connect(this->tracknew_itemPatternAction,SIGNAL(activated()),this,SLOT(add_pattern_track()));
	
	
	edit_toolbar = new EditToolbar(this);
	connect(this->edit_toolbar,SIGNAL(snapSelectedSignal(int)),this,SLOT(snap_selected_change(int )));
	
	//if cursor moves, call update
	QObject::connect(cursor,SIGNAL(window_moved()),row_list,SLOT(update()));
	QObject::connect(cursor,SIGNAL(track_changed()),track_list,SLOT(repaint_track_views()));
	QObject::connect(cursor,SIGNAL(window_moved()),track_list,SLOT(repaint_track_views()));
	QObject::connect(cursor,SIGNAL(cursor_moved()),this,SLOT(cursor_pos_changed_slot()));

	QObject::connect(patternCreateAction,SIGNAL(activated()),this,SLOT(pattern_add_slot()));
	QObject::connect(patternModifyAction,SIGNAL(activated()),this,SLOT(pattern_change_slot()));
	QObject::connect(patternDeleteAction,SIGNAL(activated()),this,SLOT(pattern_erase_slot()));
	
}


}; /* end of namespace */


