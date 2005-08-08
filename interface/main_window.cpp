

#include "main_window.h"
#include <qhbox.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qaction.h>

namespace ReShaked {

void MainWindow::add_pattern_track() {

	engine.song_edit->add_pattern_track(2);

}

class ExpScrollView : public QScrollView {

	QWidget *child;
	
	void resizeEvent ( QResizeEvent * p_resevebt) {
		if (child==NULL)
			return;

		child->resize(child->width(),viewport()->height());
	}

	

public:

	void add_child(QWidget *p_child) { child =p_child; addChild(p_child,0,0); }

	ExpScrollView(QWidget *p_parent) : QScrollView(p_parent) { child = NULL; }
};

MainWindow::MainWindow() {

	engine.song = new Song;
	engine.song_edit = new SongEdit( &engine.undoredo_stream , engine.song );
	
	
	track_options_splitter = new QSplitter(Qt::Vertical,this);
	setCentralWidget(track_options_splitter);

	QHBox * top_hbox = new QHBox(track_options_splitter);
	
	track_scroll = new ExpScrollView(top_hbox);
	track_list = new TrackList(track_scroll);
	((ExpScrollView*)track_scroll)->add_child(track_list);
	
	v_scroll = new QScrollBar(top_hbox);
	bottom_stack = new QWidgetStack(track_options_splitter);

	connect(this->tracknew_itemPatternAction,SIGNAL(activated()),this,SLOT(add_pattern_track()));
}


}; /* end of namespace */


