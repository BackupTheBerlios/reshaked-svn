//
// C++ Implementation: sound_plugin_chooser
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sound_plugin_chooser.h"
#include "engine/sound_plugin_list.h"
#include <Qt/qlayout.h>
#include <Qt/qheaderview.h>
#include <Qt/qscrollbar.h>
#include <Qt/qpushbutton.h>
#include <Qt/qlabel.h>
#include <Qt/qevent.h>
#include "pixmaps/note.xpm"

#include <Qt/qpainter.h>
#include <Qt/qfontmetrics.h>
#include <Qt/qevent.h>
#include "interface/visual_settings.h"
#include "ui_blocks/helpers.h"


namespace ReShaked {


void SoundPluginChooserItem::keyPressEvent( QKeyEvent * event ) {
	
	if (event->text().length()>0)
		char_press_event_signal(event->text()[0]);
	//printf("text %s\n",event->text().toAscii().data());
}

void SoundPluginChooserItem::paintEvent(QPaintEvent *e) {
	
	int px_w=px.isNull()?48:px.width();
	
	int margin=GET_CONSTANT(CONSTANT_PLUGIN_CHOOSER_ITEM_MARGIN);
	
	QPainter p(this);
	
	
	if (selected) 
		p.fillRect(0,0,width(),height(),QColor(200,200,220));
	else		
		p.fillRect(0,0,width(),height(),QColor(255,255,255));
		
	
	if (!px.isNull()) {
		
		p.drawPixmap(margin,margin,px);
	}
	
	int text_ofs=margin*2+px_w;
	
	int title_h=height()*65/100;
	int desc_h=height()*35/100;
	
	QFont ft;
	
	ft.setPixelSize(title_h/2);
	ft.setBold(true);
	QFontMetrics fm(ft);
	p.setFont(ft);
	p.drawText(text_ofs,title_h/2+fm.ascent(),title);
	
	QFont fd;
	
	fd.setPixelSize(desc_h*2/3);
	QFontMetrics fmd(fd);
	
	p.setFont(fd);
	p.drawText(text_ofs,title_h+(desc_h-fmd.height())/2+fmd.ascent(),description);
	
	p.setPen(QColor(150,150,150));
	p.drawLine(0,height()-1,width(),height()-1);
	
}


void SoundPluginChooserItem::mousePressEvent(QMouseEvent *e) {
	
	if (e->button()!=Qt::LeftButton)
		return;
		
	selected_signal(this);
	
}


void SoundPluginChooserItem::set_selected(bool p_selected) {
	
	
	selected=p_selected;
	update();
}



int SoundPluginChooserItem::get_index() {
	
	return index;
	
}

QString SoundPluginChooserItem::get_title() {
	
	return title;
}


SoundPluginChooserItem::SoundPluginChooserItem(QWidget *p_parent,QPixmap p_icon, QString p_title, QString p_description,QString p_text,int p_index) : QWidget(p_parent) {
	
	px=p_icon;
	title=p_title;
	description=p_description;
	index=p_index;
	
	if (!p_icon.isNull()) 
		setFixedHeight(p_icon.height()+GET_CONSTANT(CONSTANT_PLUGIN_CHOOSER_ITEM_MARGIN)*2);
	else	
		setFixedHeight(48+GET_CONSTANT(CONSTANT_PLUGIN_CHOOSER_ITEM_MARGIN)*2);
	
	//setToolTip(p_text);
	selected=false;
	setFocusPolicy(Qt::ClickFocus);
}




int SoundPluginChooser::get_selected_plugin_idx() {
	
	return selected_idx;
	
}

int SoundPluginChooser::get_selected_channels() {
	
	int idx=channels->currentIndex();
	if (idx<0 || idx>=channels->count())
		return -1;
		
	int chans=channels->itemData(idx).toInt();
	
	if (chans<1 || chans>8)
		return -1;
		
	return chans;
}

void SoundPluginChooser::accept() {
	
	if (selected_idx==-1)
		return;
	
	QDialog::accept();
}
void SoundPluginChooser::selected_slot(SoundPluginChooserItem * p_item) {
	
	for (int i=0;i<items.size();i++) {
		
		items[i]->set_selected( items[i]==p_item );
	}
	
	selected_idx=p_item->get_index();
	
	const SoundPluginInfo *info=SoundPluginList::get_singleton()->get_plugin_info(selected_idx);
	bool synth_selected=info->is_synth;
	
	channels->clear();
	
	if (info->can_custom_channels) {
		
		int preselect_index=-1;
#define	ADD_COMBO_OPTION(mchans) {\
	QString text; \
	if (mchans==1)\
		text="1- Mono";\
	else if (mchans==2)\
		text="2- Stereo";\
	else if (mchans==4)\
		text="4- Quad";\
	else\
		text=QString::number(mchans);\
	\
	channels->addItem(text,QVariant(mchans));\
	if (track_channels==mchans)\
		preselect_index=channels->count()-1;\
	}
		
		if (info->custom_channels.size()) {
			

			for (int i=0;i<info->custom_channels.size();i++) {
				
				ADD_COMBO_OPTION(info->custom_channels[i]);
			}
			
		} else {
			
			for (int i=1;i<8;i++) {
				
				ADD_COMBO_OPTION(i);
			}
			
		}
					
#undef ADD_COMBO_OPTION

		if (preselect_index>=0)
			channels->setCurrentIndex(preselect_index);
					
	}
	
	append->setText(synth_selected?"Connect Synth to Output":"Append Effect to Output");
	ensure_selected_visible();
}


bool SoundPluginChooser::append_to_output() {
	
	return append->isChecked();
}

void SoundPluginChooser::kb_search_timeout_slot() {
	
	current_search="";
}


void SoundPluginChooser::ensure_selected_visible() {
	
	
	if (selected_idx<0 || selected_idx>=items.size())
		return;
	
	SoundPluginChooserItem* item=items[selected_idx];

	int vbox_h=vb->height(); //total height
	int vbox_pos=-vb->pos().y(); //total offset
	int vp_height=scroll->viewport()->height(); //window height
	int item_ofs=item->pos().y(); //item offset
	int item_h=item->height(); //item height
	
	int wanted_pos=-1;
	
	if ((item_ofs+item_h)>(vbox_pos+vp_height)) {
		
		wanted_pos=item_ofs-vp_height+item_h;
	}
	
	if (item_ofs<vbox_pos) {
		
		wanted_pos=item_ofs;
	}
	
	if (wanted_pos==-1)
		return; //nothing to fix
	
	//printf("vbox_h %i, vbox pos %i vp_h %i item_ofs %i item h %i\n",vbox_h,vbox_pos,vp_height,item_ofs,item_h);	
	int scroll_range=scroll->verticalScrollBar()->maximum();
	int scroll_pos=wanted_pos*scroll_range/(vbox_h-vp_height);
	scroll->verticalScrollBar()->setValue(scroll_pos);
	
	
	/*	
	int scrollbar_pos=
	
	
	
	
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
	*/
	
}

void SoundPluginChooser::update_current_search() {
	
	for (int i=0;i<items.size();i++) {
		
		if (items[i]->get_title().indexOf(current_search,0,Qt::CaseInsensitive)==0) {
			
			items[i]->setFocus();
			selected_slot(items[i]);
			ensure_selected_visible();
			break;
		}
	}
	
}

void SoundPluginChooser::char_press_event_slot(QChar p_char) {
	
	current_search+=p_char;
	//printf("search: %s\n",current_search.toAscii().data());
	update_current_search();
	keyb_search_timeout->start(); //restart timeout
}


SoundPluginChooser::SoundPluginChooser(QWidget *p_parent,bool p_show_synths,int p_track_channels) : QDialog(p_parent) {
	
	track_channels=p_track_channels;
	setLayout(new QVBoxLayout);
	
	scroll = new QScrollArea(this);
	layout()->addWidget(scroll);
	
	vb = new CVBox(scroll);
		
	scroll->setWidget(vb);
	scroll->setWidgetResizable(true);
	
	SoundPluginList *list=SoundPluginList::get_singleton();
	for (int i=0;i<list->get_plugin_count();i++) {
		
		if (list->get_plugin_info(i)->is_synth && !p_show_synths)
			continue;
		
		const SoundPluginInfo *info=list->get_plugin_info(i);
		
		QPixmap px=info->xpm_preview?QPixmap((const char **)info->xpm_preview):QPixmap();
		
		items.push_back( new SoundPluginChooserItem(vb,px,QStrify(info->caption),QStrify(info->description),QStrify(info->long_description),i));
		
		QObject::connect( items[ items.size() -1 ], SIGNAL(selected_signal(SoundPluginChooserItem*)),this,SLOT(selected_slot( SoundPluginChooserItem* )));
		
		QObject::connect( items[ items.size() -1 ], SIGNAL(char_press_event_signal(QChar)),this,SLOT(char_press_event_slot( QChar )));

	}
	
	(new QFrame(vb))->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	vb->show();
	
	CHBox *hb = new CHBox(this);
	layout()->addWidget(hb);
	
//	(new QFrame(hb))->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	
	append = new QCheckBox("Append Effect to Output",hb); //accept button
	append->setChecked(true);
	
	(new QFrame(hb))->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	
	hb = new CHBox(this);
	layout()->addWidget(hb);
	
//	(new QFrame(hb))->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	new QLabel("Channels to Instance",hb);
	
	(new QFrame(hb))->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	
	channels = new QComboBox(hb);
	channels->setMinimumWidth(100);	
	hb = new CHBox(this);
	layout()->addWidget(hb);
	
	(new QFrame(hb))->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	
	QPushButton *ab = new QPushButton("Accept",hb); //accept button
	QObject::connect(ab,SIGNAL(clicked()),this,SLOT(accept()));
	
	(new QFrame(hb))->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	
	setWindowTitle("Choose Sound Plugin");
	selected_idx=-1;
	
	setFixedSize(410,400);
	
	keyb_search_timeout = new QTimer(this);
	keyb_search_timeout->setInterval(1000); //2 seconds
	keyb_search_timeout->setSingleShot(true);

	QObject::connect(keyb_search_timeout,SIGNAL(timeout()),this,SLOT(kb_search_timeout_slot()));

}


SoundPluginChooser::~SoundPluginChooser()
{
}


}

