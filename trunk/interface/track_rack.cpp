//
// C++ Implementation: track_rack
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "track_rack.h"
#include "interface/visual_settings.h"
#include "ui_blocks/helpers.h"
#include "ui_blocks/pixmap_label.h"
#include "ui_blocks/pixmap_slider.h"
#include "ui_blocks/property_editors.h"
#include <Qt/qscrollbar.h>
#include <Qt/qaction.h>
#include "dsp/formulas.h"
#include <Qt/qmenu.h>

namespace ReShaked {


void TrackRack::update_VUs() {
	
	if (vu_list.size()!=editor->get_song()->get_track_count())
		return;
	
	for (int i=0;i<vu_list.size();i++) {
		
		ERR_CONTINUE(vu_list[i].size()!=editor->get_song()->get_track(i)->get_channels());
		
		for (int j=0;j<vu_list[i].size();j++) {
			
			PixmapVU *vu=vu_list[i][j];
			float nrg=			editor->get_song()->get_track(i)->read_highest_energy(j);
			
			float db_nrg=energy_to_db(nrg);
			
			if (db_nrg<-60.0)
				db_nrg=-60.0;
			
			float vu_val=(db_nrg+60.0)/84.0; // -60 - +6 db display
			
			vu->set_value( vu_val );
		}
		
	}
}

void TrackRack::rack_height_chanegd_slot( int p_to_height ) {
	
	if (p_to_height>20) //for safety
		setMinimumHeight(p_to_height);
	
	
}

void TrackRack::add_label(QWidget *p_parent,QString p_name) {
	
	/* LABEL */
	QPixmap label_pixmap(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_LABEL));
	
	PixmapLabel * name = new PixmapLabel(p_parent,label_pixmap);
		
	name->set_pos(QPoint(6,label_pixmap.height()-8));
	name->get_font().setPixelSize(10);
	QString name_str=p_name;
	name->set_text( name_str );
	name->set_angle( -90 );
	name->set_color(QColor(0,0,22));
	
}

void TrackRack::add_track(Track *p_track) {
	
	CVBox *track_vb = new CVBox(rack_box);
	
	//setLayout(new QVBoxLayout(track_vb));
	CHBox *hb_top = new CHBox(track_vb);

	new PixmapLabel(hb_top,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_TOP_LEFT));
	PixmapLabel *title = new PixmapLabel(hb_top,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_TOP) ,PixmapLabel::EXPAND_TILE_H);
	title->set_text( QStrify(p_track->get_name())) ;
	title->setToolTip( QStrify(p_track->get_name())) ;
	new PixmapLabel(hb_top,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_TOP_RIGHT));
	
	
	CHBox *hb = new CHBox(track_vb);

	
	PixmapLabel *label_name = new PixmapLabel(hb,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_LEFT));
	label_name->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	
	QPixmap value_pixmap(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_VALUE));
	PixmapSlider::Skin slider_skin(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_SLIDER_BASE),GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_SLIDER_LIGHT),GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_SLIDER_GRABBER));
	
	PixmapVU::Skin vu_skin(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_VU_EMPTY),GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_VU_FILL));
	
	
	if (dynamic_cast<Track_Pattern*>(p_track)) {
		
		Track_Pattern*tp = dynamic_cast<Track_Pattern*>(p_track);
		
		{
	
			
			CVBox *vb_label = new CVBox(hb);
			
			add_label(vb_label,"Swing");
			
			new PixmapLabel(vb_label,GET_QPIXMAP(THEME_TRACK_PANEL__LABEL_BOTTOM));
			
			CVBox *vb_swing = new CVBox(hb);
			
			new PixmapLabel(vb_swing,GET_QPIXMAP(THEME_TRACK_PANEL__SWING_BASE));
			
			PropertyEditCombo *combo = new PropertyEditCombo(vb_swing,GET_QPIXMAP(THEME_TRACK_PANEL__SWING_COMBO));
			combo->set_property( &tp->swing_base() );
			add_editor(combo);
			new PixmapLabel(vb_swing,GET_QPIXMAP(THEME_TRACK_PANEL__SWING_LOCAL));
			
			PropertyEditButton *local_button = new PropertyEditButton(vb_swing,PixmapButton::Skin(GET_QPIXMAP(THEME_TRACK_PANEL__SWING_LOCAL_OFF),GET_QPIXMAP(THEME_TRACK_PANEL__SWING_LOCAL_ON)));
			
			local_button->set_property( &tp->swing_local() );
			add_editor(local_button);
			
			new PixmapLabel(vb_swing,GET_QPIXMAP(THEME_TRACK_PANEL__SWING_VALUE));
			
			Knob::Skin sk;
			sk.angle_begin=30;
			sk.base=GET_QPIXMAP(THEME_TRACK_PANEL__SWING_KNOB);
			sk.handle=GET_QPIXMAP(THEME_TRACK_PANEL__SWING_KNOB_POS);
			sk.handle_at_distance=10;
			
			PropertyEditKnob *knob = new PropertyEditKnob(vb_swing,sk);
			knob->set_property( &tp->swing() );
			add_editor(knob);
			
			new PixmapLabel(vb_swing,GET_QPIXMAP(THEME_TRACK_PANEL__SWING_BOTTOM));
			
			CVBox *vb_port = new CVBox(hb);
		
			CHBox *hb_port = new CHBox(vb_port);
		
			add_label(hb_port,"Volume");
		
			PropertyEditSlider * slider = new PropertyEditSlider(hb_port,slider_skin);
			slider->set_property(&tp->volume());
			add_editor(slider);

			PropertyEditLabel * value = new PropertyEditLabel( vb_port,value_pixmap );
			value->set_property(&tp->volume());
			value->set_suffix_visible( false );
			value->set_color(QColor(240,240,255));
			value->add_to_group(slider); //share group
			add_editor(value);
					
			//qt bug
			vb_port->layout()->setMargin(0);
			vb_port->layout()->setSpacing(0);
			hb_port->layout()->setMargin(0);
			hb_port->layout()->setSpacing(0);
			
			
			CVBox *vb_vus = new CVBox(hb);
		
			CHBox *hb_vus = new CHBox(vb_vus);
		
			new PixmapLabel(vb_vus,GET_QPIXMAP(THEME_TRACK_PANEL__LABEL_BOTTOM),PixmapLabel::EXPAND_TILE_H);			
			
			std::vector< PixmapVU* > vu_vector;
			for (int i=0;i<p_track->get_channels();i++) {
			
				PixmapVU * vu = new PixmapVU(hb_vus,vu_skin);
				vu->set_fall_time( 1.0 );
				vu_vector.push_back(vu);
			}

			vu_list.push_back(vu_vector);
			//qt bug
			
		}
	}
	new PixmapLabel(hb,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_RIGHT));
	
	CHBox *hb_bottom = new CHBox(track_vb);

	new PixmapLabel(hb_bottom,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_BOTTOM_LEFT));
	new PixmapLabel(hb_bottom,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_BOTTOM),PixmapLabel::EXPAND_TILE_H);
	new PixmapLabel(hb_bottom,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_BOTTOM_RIGHT));
	
	
	hb->layout()->setMargin(0);
	hb->layout()->setSpacing(0);	

	track_vb->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	
}


void TrackRack::property_changed(void *instance,Property *p_prop,double p_old_val) {
	TrackRack *_this = (TrackRack *)instance;
	
	Track *hint=NULL;
	
	for (int i=0;i<_this->editor->get_song()->get_track_count();i++) {
		
		Track *t=_this->editor->get_song()->get_track(i);
		for (int j=0;j<t->get_property_count();j++) {
			
			if (t->get_property(j)==p_prop) {
				hint=t;
				break;
			}
		}
		
	}
	
	if (!hint)
		return;
	
	_this->editor->property_changed(p_prop, p_old_val,hint );
	
}

void TrackRack::add_editor(PropertyEditor *p_editor) {
	
	editor_list.push_back(p_editor);
	updater->add_editor( p_editor );
	p_editor->set_changed_by_editor_callback(this,&TrackRack::property_changed);
	
	QWidget *w=dynamic_cast<QWidget*>(p_editor);
	if (w) {
		QObject::connect(w,SIGNAL(external_edit_signal(Property *)),this,SLOT(property_options_requested( Property* )),Qt::QueuedConnection); //queuing is necesary to avoid crashes
	}
	
	
}
		
void TrackRack::rebuild_track_rack() {
	
	
	
	spacer=NULL;
	for (int i=0;i<editor_list.size();i++) {
		
		updater->remove_editor( editor_list[i] );
	}
	
	editor_list.clear();
	
	vu_list.clear(); //vus deleted when hbox deleted
	
	if (rack_box) {
		rack_box->hide();
		delete rack_box; //all will be deleted
		viewport()->setBackgroundRole(QPalette::NoRole);
		
	}

	rack_box=NULL;
	
	if (editor==NULL)
		return;
	rack_box = new RackHBox(this);
	rack_box->hide();
	
	QObject::connect(rack_box,SIGNAL(new_height_signal(int)),this,SLOT(rack_height_chanegd_slot( int )));
	setWidget(rack_box);
	setWidgetResizable(true);
	
	for (int i=0;i<editor->get_song()->get_track_count();i++) {
		
		add_track(editor->get_song()->get_track(i));
	}
	
	spacer = new QFrame(rack_box);
	spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	rack_box->layout()->setMargin(0);
	rack_box->layout()->setSpacing(0);
	rack_box->adjustSize();
	rack_box->show();
	
	update_scrollbar();
	
}
void TrackRack::h_qscrollbar_range_changed( int,int ) {
	
	update_scrollbar();
}

void TrackRack::scrollbar_changed_slot(int p_val) {
	
	horizontalScrollBar()->setValue(p_val);
	
}

void TrackRack::resizeEvent(QResizeEvent * event) {
	
	QAbstractScrollArea::resizeEvent(event);
	update_scrollbar();
}


void TrackRack::update_scrollbar() {
	
	if (!spacer)
		return;
	if (!scrollbar)
		return;
	
	int tracklist_width=spacer->x();
	
	scrollbar->set_max( tracklist_width );
	
	scrollbar->set_pagesize( width() );
	
	if (tracklist_width<width())
		scrollbar->hide();
	else
		scrollbar->show();
	
	scrollbar->set_value( horizontalScrollBar()->value() );
}

void TrackRack::set_scrollbar(PixmapScrollBar *p_scroll) {
	
	scrollbar=p_scroll;
	update_scrollbar();
	
	QObject::connect(p_scroll,SIGNAL(value_changed_signal( int )),this,SLOT(scrollbar_changed_slot(int)));
	QObject::connect(horizontalScrollBar(),SIGNAL(valueChanged(int)),scrollbar,SLOT(set_value( int )));
	
}


void TrackRack::property_options_requested(Property *p_property) {
	
	
	Track *track=NULL;
	int prop_idx=-1;
	
	for (int i=-1;i<editor->get_song()->get_track_count();i++) {
		
		Track *t;
		if (i==-1)
			t=&editor->get_song()->get_global_track();
		else
			t=editor->get_song()->get_track(i);
		
		for (int j=0;j<t->get_property_count();j++) {
			
			if (t->get_property(j)==p_property) {
				
				prop_idx=j;
				track=t;
				break;
			}
		}
		
		if (track)
			break;
	}
	
	ERR_FAIL_COND(!track || prop_idx==-1);
	
	bool is_automated=track->has_property_visible_automation(prop_idx);
	
	QList<QAction*> ac_list;
	QAction *op_auto = new QAction(GET_QPIXMAP(PIXMAP_TRACK_SETTINGS_AUTOMATIONS),is_automated?"Hide Automation":"Show Automation",this);
	ac_list.push_back(op_auto);
	QAction *res = QMenu::exec(ac_list,QCursor::pos());
	
	if (res==op_auto) {
		
		if (is_automated)
			editor->hide_automation( prop_idx, track );
		else
			editor->show_automation( prop_idx, track );
	}
	
	foreach(I,ac_list) {
		delete *I;
	}
	
}

TrackRack::TrackRack(QWidget *p_parent,Editor *p_editor,PropertyEditUpdater *p_updater) :QScrollArea(p_parent){
	
	editor=p_editor;
	updater=p_updater;
	
	viewport()->setBackgroundRole(QPalette::NoRole);
	QPalette p=viewport()->palette();
	p.setColor(QPalette::Background,QColor(0,0,0));
	viewport()->setPalette(p);
	setLineWidth(0);
	setFrameStyle(QFrame::NoFrame);
	viewport()->setContentsMargins(0,0,0,0);
	setContentsMargins(0,0,0,0);

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	
	setFixedHeight(GET_CONSTANT(CONSTANT_RACK_MINIMUM_HEIGHT)-GET_QPIXMAP(THEME_RACK_PLUGIN_TOP__BEGIN).height());

	setBackgroundRole(QPalette::NoRole);
	p=palette();
	p.setColor(QPalette::Background,QColor(0,0,0));
	setPalette(p);
	
	scrollbar=NULL;
	spacer=NULL;
	QObject::connect(horizontalScrollBar(),SIGNAL(rangeChanged( int,int )),this,SLOT(h_qscrollbar_range_changed( int,int )));	
	rack_box=NULL;
	rebuild_track_rack();
	
}


TrackRack::~TrackRack()
{
}


}
