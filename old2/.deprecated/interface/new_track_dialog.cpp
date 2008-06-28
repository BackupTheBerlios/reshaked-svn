//
// C++ Implementation: new_track_dialog
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "new_track_dialog.h"
#include <Qt/qlayout.h>
#include <Qt/qlabel.h>
#include <Qt/qstringlist.h>


namespace ReShaked {


int NewTrackDialog::track_idx=1;
int NewTrackDialog::default_channels=2;


void NewTrackDialog::accept() {
	
	track_idx++;
	default_channels=get_channels();
	
	QDialog::accept();	
}

QString NewTrackDialog::get_name() {
	
	return line_edit->text();
		
}
int NewTrackDialog::get_channels() {
	
	return combo_channels->currentIndex()+1;
	
}


NewTrackDialog::NewTrackDialog(QWidget *p_parent) : QDialog(p_parent)
{
	
	QVBoxLayout *vl = new QVBoxLayout(this);
	setLayout(vl);
	
	setWindowTitle("New Track Options");
	
	QLabel *label = new QLabel("Name:",this);
	QFont font=label->font();
	font.setBold(true);
	label->setFont(font);
	vl->addWidget(label);
	
	QString new_name="New Track " + QString::number(track_idx);
	CHBox *hb = new CHBox(this);
	vl->addWidget(hb);
	(new QFrame(hb))->setFixedWidth(20);
	line_edit=new QLineEdit(new_name,hb);
	
	
	label = new QLabel("Channels:",this);
	font=label->font();
	font.setBold(true);
	label->setFont(font);
	vl->addWidget(label);
	
	hb = new CHBox(this);
	vl->addWidget(hb);
	(new QFrame(hb))->setFixedWidth(20);
	
	QStringList sl;
	sl << "Mono" << "Stereo" << "3" << "Quad" << "5" << "6" << "7" << "8"; //BluRay and HD-DVD support up to 8 :D
	
	
	combo_channels = new QComboBox(hb);
	combo_channels->addItems(sl);
	combo_channels->setCurrentIndex(default_channels-1);
	
	
	acceptb = new QPushButton("Accept",this);
	acceptb->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	vl->addWidget(acceptb);
	vl->setAlignment(acceptb,Qt::AlignHCenter);
	
	QObject::connect(acceptb,SIGNAL(clicked()),this,SLOT(accept()));

}


NewTrackDialog::~NewTrackDialog()
{
}


}
