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
#include "ui_blocks/helpers.h"
#include "pixmaps/note.xpm"

namespace ReShaked {

void SoundPluginChooser::focusOutEvent(QFocusEvent*e) {
	
	reject();
}
void SoundPluginChooser::accept() {
	
	QTreeWidgetItem *twi=tree->currentItem();
	if (twi==NULL)
		return;
	selected_idx=twi->data(2,ROLE_ID).toInt();
	printf("selected_idx is %i\n",selected_idx);
	QDialog::accept();
}

int SoundPluginChooser::get_selected_plugin_idx() {
	
	return selected_idx;
	
}

SoundPluginChooser::SoundPluginChooser(QWidget *p_parent,bool p_show_synths) {
	
	setLayout(new QVBoxLayout);
	tree = new QTreeWidget(this);
	layout()->addWidget(tree);
	tree->setColumnCount(3);
	
	QStringList strlist;
	strlist << "Preview" << "Synth" << "Plugin";
	tree->setHeaderLabels(strlist);

	tree->header()->setResizeMode(3,QHeaderView::Stretch);
	
	
	SoundPluginList *list=SoundPluginList::get_singleton();
	for (int i=0;i<list->get_plugin_count();i++) {
		
		if (list->get_plugin_info(i)->is_synth && !p_show_synths)
			continue;
		
		QTreeWidgetItem *ti = new QTreeWidgetItem(tree);
		
		ti->setData(2,ROLE_ID,i);
		ti->setData(2,ROLE_INDEX,QStrify(list->get_plugin_info(i)->unique_ID));
		ti->setText(2, QStrify( list->get_plugin_info(i)->caption ) );
		ti->setIcon(1,QPixmap( (const char**)note_xpm ) );
	}
	QPushButton *ab = new QPushButton("Accept",this); //accept button
	layout()->addWidget(ab);
	QObject::connect(ab,SIGNAL(clicked()),this,SLOT(accept()));
	
	selected_idx=-1;
}


SoundPluginChooser::~SoundPluginChooser()
{
}


}
