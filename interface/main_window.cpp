#include "main_window.h"
#include <qhbox.h>
#include <qpushbutton.h>
#include <qlabel.h>

namespace ReShaked {



MainWindow::MainWindow() {

	track_options_splitter = new QSplitter(Qt::Vertical,this);
	setCentralWidget(track_options_splitter);

	QHBox * top_hbox = new QHBox(track_options_splitter);
	
	track_scroll = new QScrollView(top_hbox);
	v_scroll = new QScrollBar(top_hbox);
	bottom_stack = new QWidgetStack(track_options_splitter);
}


}; /* end of namespace */
