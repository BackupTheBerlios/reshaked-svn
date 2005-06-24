#include "interface/uic/main_window_base.h"
#include <qscrollview.h>
#include <qsplitter.h>
#include <qscrollbar.h>
#include <qwidgetstack.h>


namespace ReShaked {



class MainWindow : public MainWindowBase {

	Q_OBJECT

	QSplitter *track_options_splitter;
	QScrollView *track_scroll;
	QScrollBar *v_scroll;
	QWidgetStack *bottom_stack;
	
public:


	MainWindow();
};


}; /* end of namespace */
