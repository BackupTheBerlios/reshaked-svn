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
#include <Qt/qinputdialog.h>
#include "interface/blocklist_separator.h"
#include "interface/visual_settings.h"
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
	v_scroll->set_value( editor->get_cursor().get_window_offset() );
	
}

void BlockListUIList::property_editor_property_edited_callback(void *_this,PropertyEditor* p_editor,double p_old_val) {

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

void BlockListUIList::blocklist_ui_under_cursor_request_signal(BlockListUI_Base*p_ui) {
	
	for (int i=0;i<block_list_ui_list.size();i++) {
		
		QRect global_rect(block_list_ui_list[i]->mapToGlobal(QPoint(0,0)),block_list_ui_list[i]->size());
		
		if (QCursor::pos().x()<global_rect.x() && QCursor::pos().x()>(global_rect.x()+global_rect.width()))
			continue;
		
		QPoint contained=QCursor::pos()-global_rect.topLeft();
		p_ui->set_blocklist_ui_under_cursor( block_list_ui_list[i], contained );
		break;
		
	}
	
}

void BlockListUIList::update_vus() {
	/*	
	for (int i=0;i<editor->get_song()->get_track_count();i++) {
		
		float vu_val=editor->get_song()->get_track(i)->read_highest_energy();
		if (i>=slider_vus.size())
			break;
		slider_vus[i]->update_vu(vu_val);
	}
	*/
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
		
		CVBox *auto_vb = new CVBox(hbox);
		hbox_layout->addWidget( auto_vb );
		BlackWidget * top = new BlackWidget(auto_vb);
		top->setFixedHeight( GET_QPIXMAP( PIXMAP_TRACK_OPTIONS ).height() );
				
		CHBox *auto_hb = new CHBox(auto_vb);
				
		Automation *a=editor->get_song()->get_global_track().get_visible_automation(i);
		BlockListUI_Automation *au_wg=new BlockListUI_Automation(auto_hb,editor,a);
		block_list_ui_list.push_back( au_wg );
		
			
		BlockList_Separator *s = new BlockList_Separator(auto_hb,QStrify(a->get_property()->get_caption()));
		
	}
	
	PixmapSlider::Skin amp_slider_vu_skin(GET_QPIXMAP(PIXMAP_TRACK_SLIDER_VU_BG),GET_QPIXMAP(PIXMAP_TRACK_SLIDER_VU_FG),GET_QPIXMAP(PIXMAP_TRACK_SLIDER_VU_GRABBER));
	
	PixmapSlider::Skin swing_skin(GET_QPIXMAP(PIXMAP_TRACK_SLIDER_SWING_BG),GET_QPIXMAP(PIXMAP_TRACK_SLIDER_SWING_FG),GET_QPIXMAP(PIXMAP_TRACK_SLIDER_SWING_GRABBER));
		
	for (int i=0;i<editor->get_song()->get_track_count();i++) {

		
		//hb->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
		
		CVBox *track_vb = new CVBox(hbox);
		hbox_layout->addWidget( track_vb );

		TrackTop *top = new TrackTop(track_vb,editor->get_song()->get_track(i),editor,TrackTop::TYPE_PATTERN);
		track_tops.push_back(top);
				
		CHBox *track_hb = new CHBox(track_vb);
		BlockListUI_Base *bui;
		SWITCH( editor->get_song()->get_track(i)->get_type_name() )

			CASE("pattern") {

				bui=new BlockListUI_Pattern(track_hb,editor,i);
				block_list_ui_list.push_back( bui );
				
				Track_Pattern *pattern=dynamic_cast<Track_Pattern *>(editor->get_song()->get_track(i));
				ERR_CONTINUE(pattern==NULL);
				

				/*
				new PixmapLabel(cvb,GET_QPIXMAP(PIXMAP_TRACK_SLIDER_VU_ICON));
				PropertyEditSliderVU *slider_vu = new PropertyEditSliderVU(cvb,amp_slider_vu_skin);
				slider_vu->set_property( &pattern->volume() );
				slider_vus.push_back(slider_vu);
				property_ui_updater->add_editor(slider_vu);
				property_editors.push_back(slider_vu);
				slider_vu->set_changed_by_editor_callback(this,&BlockListUIList::property_editor_property_edited_callback);
				*/
				
			}
		END_SWITCH

		QObject::connect( bui, SIGNAL(blocklist_ui_under_cursor_request_signal( BlockListUI_Base* )), this, SLOT(blocklist_ui_under_cursor_request_signal( BlockListUI_Base* ) ));
		
				
		for (int j=0;j<editor->get_song()->get_track(i)->get_visible_automation_count();j++) {

			Automation *a=editor->get_song()->get_track(i)->get_visible_automation(j);
			BlockListUI_Automation *au_wg= new BlockListUI_Automation(track_hb,editor,a);
			block_list_ui_list.push_back( au_wg );
			
			new BlockList_Separator(track_hb,QStrify(a->get_property()->get_caption()));
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
	v_scroll->set_max(max_len);
	v_scroll->set_pagesize(editor->get_cursor().get_window_size());
}

void BlockListUIList::v_scrollbar_changed(int p_scroll) {
	
	if (scrolling)
		return;
	
	scrolling=true;	
	
	//int cursor_ofs=editor->get_cursor().get_pos()-editor->get_cursor().get_window_offset();
	editor->get_cursor().set_window_offset( p_scroll );
	//editor->get_cursor().set_pos( p_scroll+cursor_ofs);
	
	
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
	ADD_ACTION("Select Column/Block",ACTION_SELECT_COLUMN_ALL,"editor/select_column_block",editor->get_blocklist_count()>0);
	ADD_ACTION("Disable Selection",ACTION_CLEAR_SELECTION,"editor/selection_disable",editor->is_selection_active());
	
	edit_menu->addSeparator();
	
	ADD_ACTION("Copy",ACTION_COPY,"editor/selection_copy",editor->is_selection_active());
	ADD_ACTION("Cut",ACTION_CUT,"editor/selection_zap",editor->is_selection_active());
	ADD_ACTION("Paste",ACTION_PASTE,"editor/selection_paste_overwrite",editor->selection_can_paste_at_cursor());
	ADD_ACTION("Paste Insert",ACTION_PASTE_INSERT,"editor/selection_paste_insert",editor->selection_can_paste_at_cursor());
	ADD_ACTION("Paste Mix",ACTION_PASTE_MIX,"editor/selection_paste_mix",editor->selection_can_paste_at_cursor());
	
	edit_menu->addSeparator();
	
	ADD_ACTION("Edit Marker",ACTION_EDIT_MARKER,"editor/edit_marker",editor->get_blocklist_count()>0);
	
	edit_menu->addSeparator();
	
	ADD_ACTION("Set Loop Begin",ACTION_SET_LOOP_BEGIN,"editor/set_loop_begin",editor->get_blocklist_count()>0);
	ADD_ACTION("Set Loop End",ACTION_SET_LOOP_END,"editor/set_loop_end",editor->get_blocklist_count()>0);
	
	ADD_ACTION("Set Loop from Selection",ACTION_SELECTION_TO_LOOP,"editor/selection_to_loop",editor->is_selection_active());
	
	edit_menu->addSeparator();
	
	ADD_ACTION("Create Block from Selection",ACTION_SELECTION_TO_BLOCK,"editor/selection_create_blocks",editor->is_selection_active());

	edit_menu->addSeparator();
	
	bool sel_active=editor->is_selection_active();
	ADD_ACTION(sel_active?"Selection Transpose Up Semitone":"Cursor Transpose Up Semitone",ACTION_SELECTION_RAISE_SEMITONE,"editor/transpose_up",editor->is_selection_active());
	
	ADD_ACTION(sel_active?"Selection Transpose Down Semitone":"Cursor Transpose Down Semitone",ACTION_SELECTION_LOWER_SEMITONE,"editor/transpose_down",editor->is_selection_active());
	
	ADD_ACTION("Selection Scale Volumes",ACTION_SELECTION_SCALE_VOLUMES,"editor/selection_scale_volumes",editor->is_selection_active());
	
	ADD_ACTION("Selection Apply Volume Mask",ACTION_SELECTION_SET_VOLUMES_TO_MASK,"editor/selection_apply_volume_mask",editor->is_selection_active());
	
#undef ADD_ACTION		

	edit_menu->popup(edit_button->mapToGlobal(QPoint(0,edit_button->height())));
	
}

void BlockListUIList::midi_in_toggle(bool p_enabled) {
	
	editor->set_pattern_midi_input_enabled(p_enabled);
}

void BlockListUIList::poly_input_toggle(bool p_enabled) {
	
	editor->set_polyphonic_midi_input_enabled(p_enabled);
}

void BlockListUIList::cursor_step_changed(int p_step) {
	
	editor->cursor_set_step(p_step);
}
void BlockListUIList::edit_menu_selected_item(int p_item) {
	
	
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
		
		case ACTION_EDIT_MARKER: {
			
		}break;
		case ACTION_SET_LOOP_BEGIN: {
			
			editor->set_loop_begin_at_cursor();
		} break;
		case ACTION_SET_LOOP_END: {
			
			editor->set_loop_end_at_cursor();
		
		}break;
		case ACTION_SELECTION_TO_LOOP: {
			
			editor->selection_to_loop();			
		} break;
		/**/
		case ACTION_SELECTION_TO_BLOCK: editor->selection_create_block(); break;
		/**/
		case ACTION_SELECTION_RAISE_SEMITONE: editor->selection_cursor_transpose_up(); break;
		case ACTION_SELECTION_LOWER_SEMITONE: editor->selection_cursor_transpose_down();break;
		case ACTION_SELECTION_SCALE_VOLUMES: scale_volume_slot(); break;
		case ACTION_SELECTION_SET_VOLUMES_TO_MASK: editor->selection_set_volumes_to_mask(); break;
		
	}
}

void BlockListUIList::scale_volume_slot() {
	
	static int last_scale=100;
	
	bool ok;
	
	int scale_factor= QInputDialog::getInteger ( topLevelOf(this), "Scale Volumes", "Scale %", last_scale, 1, 500,  1,&ok );
	
	if (!ok)
		return;
	
	editor->selection_volume_scale(scale_factor);
	last_scale=scale_factor;
	
}
void BlockListUIList::edit_marker_slot() {
	
	int beat=editor->get_cursor().get_tick_pos()/TICKS_PER_BEAT;
	int marker_idx=editor->get_song()->get_marker_list().get_exact_index( beat );
			
	QString current_text=QStrify( (marker_idx>=0)?editor->get_song()->get_marker_list().get_index_value( marker_idx ):"" );
	bool ok;
	QString text=QInputDialog::getText ( this,"Insert Marker", "Marker Text:", QLineEdit::Normal, current_text,&ok);
	if (!ok)
		return;
	if (text=="" && marker_idx>=0)
		editor->marker_remove(marker_idx);		
	else
		editor->marker_set(beat,DeQStrify(text));
			
	row_display->update();
	
}


void BlockListUIList::volume_mask_togled(bool p_active) {
	
	editor->set_volume_mask_active( p_active );
}


void BlockListUIList::fill_hb_top(QWidget* p_hb_top) {
	
	/* Play buttons */
	
	//PixmapButton *play_cursor;
	//PixmapButton *play_block;
	
	new PixmapLabel(p_hb_top,GET_QPIXMAP(THEME_EDIT_TOOLBAR__BEGIN));
	
	play_cursor = new PixmapButton(p_hb_top,PixmapButton::Skin(GET_QPIXMAP(THEME_EDIT_TOOLBAR__PLAY_FROM_CURSOR),GET_QPIXMAP(THEME_EDIT_TOOLBAR__PLAY_FROM_CURSOR_PUSHED)));
	QObject::connect(play_cursor,SIGNAL(mouse_pressed_signal()),this,SLOT(play_cursor_slot()));
	
	play_block = new PixmapButton(p_hb_top,PixmapButton::Skin(GET_QPIXMAP(THEME_EDIT_TOOLBAR__PLAY_CURRENT_BLOCK),GET_QPIXMAP(THEME_EDIT_TOOLBAR__PLAY_CURRENT_BLOCK_PUSHED)));
	QObject::connect(play_block,SIGNAL(mouse_pressed_signal()),this,SLOT(play_block_slot()));
	
	new PixmapLabel(p_hb_top,GET_QPIXMAP(THEME_EDIT_TOOLBAR__SEPARATOR));
	
	CVBox *edit_vb = new CVBox(p_hb_top);
	edit_vb->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

	new PixmapLabel(edit_vb,GET_QPIXMAP(THEME_EDIT_TOOLBAR__EDIT_MENU_TOP));
	edit_button = new PixmapLabel(edit_vb,GET_QPIXMAP(THEME_EDIT_TOOLBAR__EDIT_MENU));
	edit_button->get_font().setPixelSize(GET_CONSTANT(CONSTANT_EDIT_TOOLBAR_FONT_HEIGHT));
	new PixmapLabel(edit_vb,GET_QPIXMAP(THEME_EDIT_TOOLBAR__EDIT_MENU_BOTTOM));
	edit_button->set_text("Edit");
	QObject::connect(edit_button,SIGNAL(clicked_signal()),this,SLOT(show_edit_menu()));
	edit_menu = new QMenu(this);
	
	new PixmapLabel(p_hb_top,GET_QPIXMAP(THEME_EDIT_TOOLBAR__SEPARATOR));
	
	edit_mask= new PixmapButton(p_hb_top,PixmapButton::Skin(GET_QPIXMAP(THEME_EDIT_TOOLBAR__VOLUME_MASK),GET_QPIXMAP(THEME_EDIT_TOOLBAR__VOLUME_MASK_ACTIVE)),PixmapButton::TYPE_TOGGLE);
	edit_mask->set_pressed(true);
	CVBox *mask_vb = new CVBox(p_hb_top);
	mask_vb->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	QObject::connect(edit_mask,SIGNAL(mouse_toggled_signal( bool )),this,SLOT(volume_mask_togled(bool)));

	new PixmapLabel(mask_vb,GET_QPIXMAP(THEME_EDIT_TOOLBAR__VOLUME_MASK_LABEL_TOP));
	mask_label = new PixmapLabel(mask_vb,GET_QPIXMAP(THEME_EDIT_TOOLBAR__VOLUME_MASK_LABEL));
	mask_label->get_font().setPixelSize(GET_CONSTANT(CONSTANT_EDIT_TOOLBAR_FONT_HEIGHT));
	new PixmapLabel(mask_vb,GET_QPIXMAP(THEME_EDIT_TOOLBAR__VOLUME_MASK_LABEL_BOTTOM));
	mask_label->set_text("99");
	//QObject::connect(mask_label,SIGNAL(clicked_signal()),this,SLOT(change_volume_mask()));
	
	new PixmapLabel(p_hb_top,GET_QPIXMAP(THEME_EDIT_TOOLBAR__SEPARATOR));
	
	automation_envelope = new PixmapButton(p_hb_top,PixmapButton::Skin(GET_QPIXMAP(THEME_EDIT_TOOLBAR__AUTOMATION_MODE_ENVELOPE),GET_QPIXMAP(THEME_EDIT_TOOLBAR__AUTOMATION_MODE_ENVELOPE_ACTIVE)),PixmapButton::TYPE_TOGGLE);
	automation_stream = new PixmapButton(p_hb_top,PixmapButton::Skin(GET_QPIXMAP(THEME_EDIT_TOOLBAR__AUTOMATION_MODE_STREAM),GET_QPIXMAP(THEME_EDIT_TOOLBAR__AUTOMATION_MODE_STREAM_ACTIVE)),PixmapButton::TYPE_TOGGLE);
	
	new PixmapLabel(p_hb_top,GET_QPIXMAP(THEME_EDIT_TOOLBAR__SEPARATOR));
	
	new PixmapLabel(p_hb_top,GET_QPIXMAP(THEME_EDIT_TOOLBAR__CURSOR_STEPPING_ICON));
	
	CVBox *cursor_stepping_vb = new CVBox(p_hb_top);
	cursor_stepping_vb->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	new PixmapLabel(cursor_stepping_vb,GET_QPIXMAP(THEME_EDIT_TOOLBAR__CURSOR_STEPPING_DROPDOWN_TOP));
	cursor_stepping = new PixmapCombo(cursor_stepping_vb,GET_QPIXMAP(THEME_EDIT_TOOLBAR__CURSOR_STEPPING_DROPDOWN));
	cursor_stepping->get_font().setPixelSize(GET_CONSTANT(CONSTANT_EDIT_TOOLBAR_FONT_HEIGHT));
	for (int i=0;i<=9;i++) {
		cursor_stepping->add_item( QString::number(i) );
	}
	QObject::connect(cursor_stepping,SIGNAL(item_selected_signal( int )),this,SLOT(cursor_step_changed(int p_step)));
	cursor_stepping->select_item( 1 ); //1 preselected
	new PixmapLabel(cursor_stepping_vb,GET_QPIXMAP(THEME_EDIT_TOOLBAR__CURSOR_STEPPING_DROPDOWN_BOTTOM));
		
	new PixmapLabel(p_hb_top,GET_QPIXMAP(THEME_EDIT_TOOLBAR__SEPARATOR));
	
	midi_in_edit = new PixmapButton(p_hb_top,PixmapButton::Skin(GET_QPIXMAP(THEME_EDIT_TOOLBAR__MIDI_IN_CONFIG),GET_QPIXMAP(THEME_EDIT_TOOLBAR__MIDI_IN_CONFIG_PUSHED)),PixmapButton::TYPE_TOGGLE);
	QObject::connect(midi_in_edit,SIGNAL(mouse_toggled_signal( bool )),this,SLOT(midi_in_toggle( bool )));
	
	new PixmapLabel(p_hb_top,GET_QPIXMAP(THEME_EDIT_TOOLBAR__SEPARATOR));
	
	poly_input = new PixmapButton(p_hb_top,PixmapButton::Skin(GET_QPIXMAP(THEME_EDIT_TOOLBAR__POLY),GET_QPIXMAP(THEME_EDIT_TOOLBAR__POLY_ACTIVE)),PixmapButton::TYPE_TOGGLE);
	QObject::connect(poly_input,SIGNAL(mouse_toggled_signal( bool )),this,SLOT(poly_input_toggle( bool )));
	
	new PixmapLabel(p_hb_top,GET_QPIXMAP(THEME_EDIT_TOOLBAR__SPACER),PixmapLabel::EXPAND_TILE_H);
	
	
	CVBox *snap_vb = new CVBox(p_hb_top);
	snap_vb->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	new PixmapLabel(snap_vb,GET_QPIXMAP(THEME_EDIT_TOOLBAR__SNAP_DROPDOWN_TOP));
	snap_config = new PixmapCombo(snap_vb,GET_QPIXMAP(THEME_EDIT_TOOLBAR__SNAP_DROPDOWN));
	snap_config->get_font().setPixelSize(GET_CONSTANT(CONSTANT_EDIT_TOOLBAR_FONT_HEIGHT));
	
	new PixmapLabel(snap_vb,GET_QPIXMAP(THEME_EDIT_TOOLBAR__SNAP_DROPDOWN_BOTTOM));

	
	for (int i=0;i<MAX_DIVISORS;i++) {

		snap_config->add_item("1/"+QString::number(divisors[i])+" of Beat");
	}
	snap_config->select_item(3);
		
	QObject::connect(snap_config,SIGNAL(item_selected_signal( int )),this,SLOT(snap_changed_slot( int )));

	new PixmapLabel(p_hb_top,GET_QPIXMAP(THEME_EDIT_TOOLBAR__END));
	
}

void BlockListUIList::h_qscrollbar_range_changed(int p_min,int p_max) {
	
	//printf("range_changed %i - %i\n",p_min,p_max);
	h_scroll->set_max( p_max-p_min );
	//printf("vpw %i, hbw %i\n",scrollarea->viewport()->width(),hbox->width());
	if (hbox && hbox->width())
		h_scroll->set_pagesize( scrollarea->viewport()->width()*p_max/hbox->width() );
	if (p_max==0)
		h_scroll->hide();
	else
		h_scroll->show();
}
void BlockListUIList::h_qscrollbar_changed(int p_val) {
	
	//printf("val_changed %i\n",p_val);
	h_scroll->set_value( p_val );
}	
	

void BlockListUIList::update_top_bar() {
	
	edit_mask->set_pressed( editor->is_volume_mask_active() );
	mask_label->set_text( QString::number( editor->get_volume_mask() ) );
	cursor_stepping->select_item( editor->cursor_get_step() );
	for (int i=0;i<MAX_DIVISORS;i++) {
		
		if (editor->get_cursor().get_snap()==divisors[i]) {
			
			snap_config->select_item(i);
			break;
		}
	}
}
void BlockListUIList::update_play_position() {
	
	play_position->check_pos_changed();
}

BlockListUI_Base *BlockListUIList::get_blocklist_ui(int p_idx) {
	
	ERR_FAIL_INDEX_V(p_idx,block_list_ui_list.size(),NULL);
	return block_list_ui_list[p_idx];
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
	

	CVBox *track_top_vb = new CVBox(hb);
	new TrackTop(track_top_vb,&editor->get_song()->get_global_track(),editor,TrackTop::TYPE_GLOBAL); // Global Track	
	CHBox *track_top_hb = new CHBox(track_top_vb);
	row_display = new RowListDisplay(track_top_hb ,editor);
	play_position = new EditorPlayPosition(track_top_hb,editor);
	

	CVBox *scrollarea_vb = new CVBox(hb);
	scrollarea = new QScrollArea(scrollarea_vb);

	scrollarea->setFrameStyle(QFrame::NoFrame);
	scrollarea->viewport()->setContentsMargins(0,0,0,0);
	scrollarea->setContentsMargins(0,0,0,0);
	scrollarea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	QObject::connect(scrollarea->horizontalScrollBar(),SIGNAL(valueChanged( int )),this,SLOT(h_qscrollbar_changed( int )));
	QObject::connect(scrollarea->horizontalScrollBar(),SIGNAL(rangeChanged( int,int )),this,SLOT(h_qscrollbar_range_changed( int,int )));
	
	v_scroll=new PixmapScrollBar(hb,PixmapScrollBar::Skin(GET_SKIN(SKINBOX_THEME_SCROLLBAR_V_BG),GET_SKIN(SKINBOX_THEME_SCROLLBAR_GRABBER)),PixmapScrollBar::TYPE_VERTICAL);
	QObject::connect(v_scroll,SIGNAL(value_changed_signal( int )),this,SLOT(v_scrollbar_changed(int)));
	v_scroll->setFocusPolicy(Qt::ClickFocus); //very extremely clever hack so scrollign is smooth
	
	h_scroll = new PixmapScrollBar(scrollarea_vb,PixmapScrollBar::Skin(GET_SKIN(SKINBOX_THEME_SCROLLBAR_H_BG),GET_SKIN(SKINBOX_THEME_SCROLLBAR_GRABBER)),PixmapScrollBar::TYPE_HORIZONTAL);
	
	QObject::connect(h_scroll,SIGNAL(value_changed_signal( int )),scrollarea->horizontalScrollBar(),SLOT(setValue(int)));
	
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
	hb_top->layout()->setMargin(0);
	hb_top->layout()->setSpacing(0);
	
	setLineWidth(0);
	scrolling=false;
	
}


BlockListUIList::~BlockListUIList()
{
}


}

