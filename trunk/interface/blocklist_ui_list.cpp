//
// C++ Implementation: blocklist_ui_list
//
// Description:
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "blocklist_ui_list.h"
#include <Qt/qpushbutton.h>
#include <Qt/qlayout.h>
#include "blocklist_ui_pattern.h"
#include "blocklist_ui_automation.h"
#include <Qt/qlabel.h>
#include <Qt/qscrollbar.h>
#include <Qt/qpainter.h>
#include "interface/blocklist_separator.h"
#include "ui_blocks/visual_settings.h"
#include "interface/indexed_action.h"
#include "editor/keyboard_input.h"

//@TODO SOFT SCROLL when ensuring cursor visible
namespace ReShaked {

class BlackWidget : public QWidget {
	
	void paintEvent(QPaintEvent *pe) {
		QPainter p(this);
		p.fillRect(0,0,width(),height(),QColor(0,0,0));
	}
public:	
	BlackWidget(QWidget *p_parent) : QWidget(p_parent) {
		
		setBackgroundRole(QPalette::NoRole);
	}
};

void BlockListUIList::vscroll_track_list() {
	
	for (int i=0;i<block_list_ui_list.size();i++) {

		block_list_ui_list[i]->update_viewport_pos( editor->get_cursor().get_window_offset() );
	}
	row_display->update();
	play_position->update();
	v_scroll->setValue( editor->get_cursor().get_window_offset() );
	
}

void BlockListUIList::property_editor_property_edited_callback(void *_this,PropertyEditor* p_editor,double p_old_val) {
	printf("callbackGRH!\n");
	BlockListUIList *instance=(BlockListUIList*)_this;
	instance->property_editor_property_edited( p_editor,p_old_val );
}

void BlockListUIList::property_editor_property_edited(PropertyEditor* p_editor,double p_old_val) {
	
	Track *t=NULL; // see if we can hint the track
	
	if (p_editor->get_property()==&editor->get_song()->get_global_properties().get_tempo()) {
		printf("is tempo!\n");
		t=&editor->get_song()->get_global_track();
		
	}
	if (!t) {
	
		for (int i=0;i<slider_vus.size();i++) { //a vu?
			
			if (slider_vus[i]==p_editor) {
				t=editor->get_song()->get_track(i);
				break;
			}
		}
	}
	
	if (!t) {
		for (int i=0;i<slider_swings.size();i++) { //a swing?
			
			if (slider_swings[i]==p_editor) {
				t=editor->get_song()->get_track(i);
				break;
			}
		}
	}

	editor->property_changed( p_editor->get_property(), p_old_val, t );
}

void BlockListUIList::update_blocklist_list(const std::list<int>& p_list) {
	
	std::list<int>::const_iterator I=p_list.begin();
	for (;I!=p_list.end();I++) {
		
		int blidx=*I;
		if (blidx<0 || blidx>=block_list_ui_list.size())
			continue;
		block_list_ui_list[blidx]->update();
	}
	
}

void BlockListUIList::repaint_names() {
	
	for (int i=0;i<track_tops.size();i++) 
		track_tops[i]->update();
		
	
}

static int get_pos_in_some_parent(QWidget *p_someparent, QWidget *p_widget) {
	
	QWidget *from=p_widget;
	int xmap=0;
	
	while (true) {
		
		QWidget *parent=from->parentWidget();
		xmap+=from->pos().x();
		if (parent==p_someparent) {
			
			return xmap;
		}
		if (parent==NULL) {
			
			return -1;
		}
		
		from=parent;
	}	
}

void BlockListUIList::update_vus() {
	
	for (int i=0;i<editor->get_song()->get_track_count();i++) {
		
		float vu_val=editor->get_song()->get_track(i)->read_highest_energy();
		if (i>=slider_vus.size())
			break;
		slider_vus[i]->update_vu(vu_val);
	}
}

void BlockListUIList::ensure_cursor_visible() {

	ERR_FAIL_COND(hbox==NULL);

	int current_blocklist=editor->get_current_blocklist();
	ERR_FAIL_INDEX(current_blocklist,block_list_ui_list.size());

	int hbox_w=hbox->width();
	int vp_width=scrollarea->viewport()->width();
	int hbox_ofs=-hbox->pos().x();
	int blocklist_pos=get_pos_in_some_parent(hbox,block_list_ui_list[current_blocklist]);
	ERR_FAIL_COND(blocklist_pos==-1);
	int blocklist_width=block_list_ui_list[current_blocklist]->width();

	int new_hbox_ofs;
	if (blocklist_pos<hbox_ofs) {

		new_hbox_ofs=blocklist_pos;
	} else if ( (blocklist_pos+blocklist_width) > (hbox_ofs+vp_width) ) {

		new_hbox_ofs=(blocklist_pos+blocklist_width)-vp_width;
	} else {
		return; //nothing to adjust
	}

	int scroll_range=scrollarea->horizontalScrollBar()->maximum();
	int scroll_pos=new_hbox_ofs*scroll_range/(hbox_w-vp_width);

	scrollarea->horizontalScrollBar()->setValue( scroll_pos );

}
void BlockListUIList::cursor_changed_blocklist() {

	/* find new blocklist, adjust focus */

	int current_blocklist=editor->get_current_blocklist();
	if (current_blocklist<0 || current_blocklist>=block_list_ui_list.size())
		return; //cursot cant be displayed.

	block_list_ui_list[current_blocklist]->setFocus();

	ensure_cursor_visible();

	printf("some data: poshbox %i,%i , poswidget %i,%i - hboxw %i vpw %i -sm, %i, sp %i\n",hbox->pos().x(),hbox->pos().y(),block_list_ui_list[current_blocklist]->pos().x(),block_list_ui_list[current_blocklist]->pos().y(),hbox->width(),scrollarea->viewport()->width(),scrollarea->horizontalScrollBar()->maximum(), scrollarea->horizontalScrollBar()->value());

}
void BlockListUIList::repaint_track_list() {

	for (int i=0;i<block_list_ui_list.size();i++) {

		block_list_ui_list[i]->update();
	}
	row_display->update();
	play_position->update();
	


}
void BlockListUIList::update_track_list() {

	//setUpdatesEnabled(false);
	
	for (int i=0;i<property_editors.size();i++) {
		property_ui_updater->remove_editor(property_editors[i]);
	}
	property_editors.clear();
	slider_vus.clear();
	slider_swings.clear();
	
	if (hbox) {
		hbox->hide();
		delete hbox; //all widgets die!
	}

	
	hbox = new QWidget(this);
	
	//hbox->hide();
	
	hbox_layout = new QHBoxLayout(hbox);
	hbox->setLayout(hbox_layout);

	scrollarea->setWidget(hbox);
	scrollarea->setWidgetResizable(true);


	block_list_ui_list.clear();
	track_tops.clear();
	
	for (int i=0;i<editor->get_song()->get_global_track().get_visible_automation_count();i++) {
		
		
		Automation *a=editor->get_song()->get_global_track().get_visible_automation(i);
		BlockListUI_Automation *au_wg=new BlockListUI_Automation(hbox,editor,a);
		block_list_ui_list.push_back( au_wg );
		hbox_layout->addWidget( au_wg );
			
		BlockList_Separator *s = new BlockList_Separator(hbox,QStrify(a->get_property()->get_caption()));
		hbox_layout->addWidget(s);
		
	}
	
	PixmapSlider::Skin amp_slider_vu_skin(GET_QPIXMAP(PIXMAP_TRACK_SLIDER_VU_BG),GET_QPIXMAP(PIXMAP_TRACK_SLIDER_VU_FG),GET_QPIXMAP(PIXMAP_TRACK_SLIDER_VU_GRABBER));
	
	PixmapSlider::Skin swing_skin(GET_QPIXMAP(PIXMAP_TRACK_SLIDER_SWING_BG),GET_QPIXMAP(PIXMAP_TRACK_SLIDER_SWING_FG),GET_QPIXMAP(PIXMAP_TRACK_SLIDER_SWING_GRABBER));
		
	for (int i=0;i<editor->get_song()->get_track_count();i++) {

		
		//hb->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
		

		SWITCH( editor->get_song()->get_track(i)->get_type_name() )

			CASE("pattern") {

				block_list_ui_list.push_back( new BlockListUI_Pattern(hbox,editor,i) );
				hbox_layout->addWidget(block_list_ui_list [block_list_ui_list.size() -1] );
				
				CVBox *cvb = new CVBox(hbox);
				hbox_layout->addWidget(cvb);
				TrackTop *top = new TrackTop(cvb,editor->get_song()->get_track(i),editor,TrackTop::TYPE_PATTERN);
				track_tops.push_back(top);
				
				Track_Pattern *pattern=dynamic_cast<Track_Pattern *>(editor->get_song()->get_track(i));
				ERR_CONTINUE(pattern==NULL);
				
				
				new PixmapLabel(cvb,GET_QPIXMAP(PIXMAP_TRACK_SLIDER_SWING_ICON));
				PropertyEditSlider *swing = new PropertyEditSlider(cvb,swing_skin);
				swing->set_property( &pattern->swing() );
				property_ui_updater->add_editor(swing);
				property_editors.push_back(swing);
				slider_swings.push_back(swing);
				swing->set_changed_by_editor_callback(this,&BlockListUIList::property_editor_property_edited_callback);
				
				new PixmapLabel(cvb,GET_QPIXMAP(PIXMAP_TRACK_SLIDER_VU_ICON));
				PropertyEditSliderVU *slider_vu = new PropertyEditSliderVU(cvb,amp_slider_vu_skin);
				slider_vu->set_property( &pattern->volume() );
				slider_vus.push_back(slider_vu);
				property_ui_updater->add_editor(slider_vu);
				property_editors.push_back(slider_vu);
				slider_vu->set_changed_by_editor_callback(this,&BlockListUIList::property_editor_property_edited_callback);
				
				
			}
		END_SWITCH

		
				
		for (int j=0;j<editor->get_song()->get_track(i)->get_visible_automation_count();j++) {

			Automation *a=editor->get_song()->get_track(i)->get_visible_automation(j);
			BlockListUI_Automation *au_wg= new BlockListUI_Automation(hbox,editor,a);
			block_list_ui_list.push_back( au_wg );
			hbox_layout->addWidget( au_wg );
			
			BlockList_Separator *s = new BlockList_Separator(hbox,QStrify(a->get_property()->get_caption()));
			hbox_layout->addWidget(s);

		}
	}

	BlackWidget *spacer = new BlackWidget(hbox);
	hbox_layout->addWidget( spacer );
	spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

	hbox_layout->setSpacing(0);
	hbox_layout->setMargin(0);


	update_h_scroll();
	//setUpdatesEnabled(true);
	hbox->show();
	hbox->adjustSize();
	
	
}

void BlockListUIList::snap_changed_slot(int p_to_idx) {
	
	if (p_to_idx<0 || p_to_idx>=MAX_DIVISORS)
		return;
	editor->set_snap(divisors[p_to_idx]);
	
	update_h_scroll();
}


void BlockListUIList::update_h_scroll() {
	
	int max_len=editor->get_cursor().ticks_to_snap( editor->get_song_max_len() );
	max_len-=editor->get_cursor().get_window_size();
	printf("max len is %i\n",max_len);
	v_scroll->setRange(0,max_len);
	v_scroll->setPageStep(editor->get_cursor().get_window_size());
}

void BlockListUIList::v_scrollbar_changed(int p_scroll) {
	
	if (scrolling)
		return;
	
	scrolling=true;	
	
	int cursor_ofs=editor->get_cursor().get_pos()-editor->get_cursor().get_window_offset();
	editor->get_cursor().set_window_offset( p_scroll );
	editor->get_cursor().set_pos( p_scroll+cursor_ofs);
	
	
	scrolling=false;
}

void BlockListUIList::play_cursor_slot() {
	
	if (editor->get_blocklist_count()==0)
		return;
	editor->get_song()->play( editor->get_cursor().get_tick_pos() );
}
void BlockListUIList::play_block_slot() {
	
	if (editor->get_blocklist_count()==0)
		return;
	
	BlockList *bl=editor->get_blocklist( editor->get_current_blocklist() );
	if (bl==NULL)
		return;
	int block=bl->get_block_idx_at_pos( editor->get_cursor().get_tick_pos() );
	if (block<0)
		return;
	Tick loop_from=bl->get_block_pos(block);
	Tick loop_to=loop_from+bl->get_block(block)->get_length();
	editor->get_song()->loop( loop_from,loop_to);
	
	
}

void BlockListUIList::show_edit_menu() {
	
	edit_menu->clear();
#define ADD_ACTION(m_text,m_index,m_accel,m_can) { QString _txt = m_text; if (m_accel!="") { _txt+=" ( "+(QKeySequence(KEYBIND(m_accel)).operator QString() )+" )"; }; QAction *_a = new IndexedAction(m_index,_txt,edit_menu); _a->setData(m_index); edit_menu->addAction(_a); QObject::connect(_a,SIGNAL(selected_index_signal(int)),this,SLOT(edit_menu_selected_item( int ))); _a->setDisabled(!(m_can)); }


	ADD_ACTION("Set Selection Begin",ACTION_SET_SELECTION_BEGIN,"editor/selection_begin",editor->get_blocklist_count()>0);
	ADD_ACTION("Set Selection End",ACTION_SET_SELECTION_END,"editor/selection_end",editor->get_blocklist_count()>0);
	ADD_ACTION("Select Column/All",ACTION_SELECT_COLUMN_ALL,"editor/select_column_block_all",editor->get_blocklist_count()>0);
	ADD_ACTION("Disable Selection",ACTION_CLEAR_SELECTION,"editor/selection_disable",editor->is_selection_active());
	
	edit_menu->addSeparator();
	
	ADD_ACTION("Copy",ACTION_COPY,"editor/selection_copy",editor->is_selection_active());
	ADD_ACTION("Cut",ACTION_CUT,"editor/selection_zap",editor->is_selection_active());
	ADD_ACTION("Paste",ACTION_PASTE,"editor/selection_paste_overwrite",editor->selection_can_paste_at_cursor());
	ADD_ACTION("Paste Insert",ACTION_PASTE_INSERT,"editor/selection_paste_insert",editor->selection_can_paste_at_cursor());
	ADD_ACTION("Paste Mix",ACTION_PASTE_MIX,"editor/selection_paste_mix",editor->selection_can_paste_at_cursor());
#undef ADD_ACTION		
	
}

void BlockListUIList::edit_menu_selected_item(int p_item) {
	
	
	printf("MEMEREMEEE!!!\n");
	switch(p_item) {
		case ACTION_SET_SELECTION_BEGIN: editor->selection_begin(); break;
		case ACTION_SET_SELECTION_END: editor->selection_end(); break;
		case ACTION_SELECT_COLUMN_ALL: editor->selection_column_all(); break;
		case ACTION_CLEAR_SELECTION: editor->disable_selection(); break;
	/***/
		case ACTION_COPY: editor->selection_copy(); break;
		case ACTION_CUT: editor->selection_zap(); break;
		case ACTION_PASTE: editor->selection_paste_overwrite(); break;
		case ACTION_PASTE_INSERT: editor->selection_paste_insert(); break;
		case ACTION_PASTE_MIX: editor->selection_paste_mix(); break;
	}
}


void BlockListUIList::fill_hb_top(QWidget* p_hb_top) {
	
	/* Play buttons */
	
	play_cursor = new QPushButton(GET_QPIXMAP(ICON_CONTROL_PLAY_CURSOR),"",p_hb_top);
	play_cursor->setIconSize(GET_QPIXMAP(ICON_CONTROL_PLAY_CURSOR).size());
	QObject::connect(play_cursor,SIGNAL(clicked()),this,SLOT(play_cursor_slot()));
	play_cursor->setFlat(true);
	play_cursor->setFocusPolicy(Qt::NoFocus);
	
	play_block = new QPushButton(GET_QPIXMAP(ICON_CONTROL_PLAY_BLOCK),"",p_hb_top);
	play_block->setIconSize(GET_QPIXMAP(ICON_CONTROL_PLAY_BLOCK).size());
	QObject::connect(play_block,SIGNAL(clicked()),this,SLOT(play_block_slot()));
	play_block->setFlat(true);
	play_block->setFocusPolicy(Qt::NoFocus);
	
	edit_menubar = new QMenuBar(p_hb_top);
	edit_menubar->setFocusPolicy(Qt::NoFocus);
	edit_menu = edit_menubar->addMenu("Edit");
	
	QObject::connect(edit_menu,SIGNAL(aboutToShow()),this,SLOT(show_edit_menu()));
	
	
	(new QFrame(p_hb_top))->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	new QLabel(" Snap: ",p_hb_top);
	

	snap = new QComboBox(p_hb_top);
	for (int i=0;i<MAX_DIVISORS;i++) {

		snap->addItem("1/"+QString::number(divisors[i]));
	}
	snap->setFocusPolicy(Qt::NoFocus);
	snap->setCurrentIndex(3);
	
	
		
	QObject::connect(snap,SIGNAL(activated(int)),this,SLOT(snap_changed_slot( int )));

	
}
void BlockListUIList::update_play_position() {
	
	play_position->check_pos_changed();
}

BlockListUIList::BlockListUIList(QWidget *p_parent,Editor *p_editor,PropertyEditUpdater *p_property_ui_updater) : QFrame (p_parent)
{

	property_ui_updater=p_property_ui_updater;
	editor=p_editor;
	
	QVBoxLayout *vl = new QVBoxLayout(this);
	setLayout(vl);
	
	CHBox *hb_top= new CHBox(this);
	vl->addWidget(hb_top);
	
	fill_hb_top( hb_top );	
	
	CHBox *hb= new CHBox(this);
	vl->addWidget(hb);
	

	
	
	row_display = new RowListDisplay(hb ,editor);
	play_position = new EditorPlayPosition(hb,editor);
	new TrackTop(hb,&editor->get_song()->get_global_track(),editor,TrackTop::TYPE_GLOBAL); // Global Track
	


	scrollarea = new QScrollArea(hb);

	scrollarea->setFrameStyle(QFrame::NoFrame);
	scrollarea->viewport()->setContentsMargins(0,0,0,0);
	scrollarea->setContentsMargins(0,0,0,0);

	v_scroll=new QScrollBar(Qt::Vertical,hb);
	QObject::connect(v_scroll,SIGNAL(valueChanged(int)),this,SLOT(v_scrollbar_changed(int)));
	
	//scrollarea->setFrameRect(QRect(0, 0, 0, 0 ));

	//scrollarea->setFrameRect(QRect(0, 0, 0, 0 ));

	
	hbox = new QWidget(scrollarea);
	hbox_layout = new QHBoxLayout(hbox);
	hbox->setLayout(hbox_layout);
	hbox_layout->addWidget(new BlackWidget(hbox));
	hbox_layout->setMargin(0);
	scrollarea->setWidget(hbox);
	scrollarea->setWidgetResizable(true);

	scrollarea->setFocusPolicy(Qt::NoFocus);
	scrollarea->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	scrollarea->setBackgroundRole(QPalette::NoRole);
	scrollarea->viewport()->setBackgroundRole(QPalette::NoRole);
	QPalette p=scrollarea->viewport()->palette();
	p.setColor(QPalette::Background,QColor(0,0,0));
	scrollarea->viewport()->setPalette(p);
	
	vl->setSpacing(0);
	vl->setMargin(0);
	hb_top->layout()->setMargin(3);
	setFrameStyle(Panel+Sunken);
	setLineWidth(1);
	scrolling=false;
	
}


BlockListUIList::~BlockListUIList()
{
}


}

