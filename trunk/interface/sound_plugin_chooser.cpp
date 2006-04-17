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
#include <Qt/qpushbutton.h>
#include <Qt/qlabel.h>
#include "ui_blocks/helpers.h"
#include "pixmaps/note.xpm"

#include <Qt/qpainter.h>
#include <Qt/qfontmetrics.h>
#include <Qt/qevent.h>
#include "ui_blocks/visual_settings.h"
#include "ui_blocks/helpers.h"


namespace ReShaked {



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

SoundPluginChooserItem::SoundPluginChooserItem(QWidget *p_parent,QPixmap p_icon, QString p_title, QString p_description,QString p_text,int p_index) : QWidget(p_parent) {
	
	px=p_icon;
	title=p_title;
	description=p_description;
	index=p_index;
	
	if (!p_icon.isNull()) 
		setFixedHeight(p_icon.height()+GET_CONSTANT(CONSTANT_PLUGIN_CHOOSER_ITEM_MARGIN)*2);
	else	
		setFixedHeight(48+GET_CONSTANT(CONSTANT_PLUGIN_CHOOSER_ITEM_MARGIN)*2);
	
	setToolTip(p_text);
	selected=false;
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
}


bool SoundPluginChooser::append_to_output() {
	
	return append->isChecked();
}

SoundPluginChooser::SoundPluginChooser(QWidget *p_parent,bool p_show_synths,int p_track_channels) : QDialog(p_parent) {
	
	track_channels=p_track_channels;
	setLayout(new QVBoxLayout);
	
	scroll = new QScrollArea(this);
	layout()->addWidget(scroll);
	
	CVBox *vb = new CVBox(scroll);
		
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

}


SoundPluginChooser::~SoundPluginChooser()
{
}


}
