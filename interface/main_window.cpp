

#include "main_window.h"
#include <qhbox.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qaction.h>

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
		// viewport()->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	}
};




MainWindow::MainWindow() {

	add_default_key_binds();
	
	engine.song = new Song;
	engine.song_edit = new SongEdit( &engine.undoredo_stream , engine.song );
	
	
	track_options_splitter = new QSplitter(Qt::Vertical,this);
	setCentralWidget(track_options_splitter);

	QHBox * top_hbox = new QHBox(track_options_splitter);
	
	track_scroll = new ExpScrollView(top_hbox);
	track_list = new TrackList(engine.song_edit,track_scroll->viewport());
	((ExpScrollView*)track_scroll)->add_child(track_list);
	track_list->show();
	track_list->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	



	v_scroll = new QScrollBar(top_hbox);
	bottom_stack = new QWidgetStack(track_options_splitter);

	connect(this->tracknew_itemPatternAction,SIGNAL(activated()),this,SLOT(add_pattern_track()));
}


}; /* end of namespace */


