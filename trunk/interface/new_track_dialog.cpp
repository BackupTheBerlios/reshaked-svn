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
	
	return combo->currentIndex()+1;
	
}


NewTrackDialog::NewTrackDialog()
{
	
	QVBoxLayout *vl = new QVBoxLayout(this);
	setLayout(vl);
	
	setWindowTitle("New Track");
	
	vl->addWidget(new QLabel("Name:",this));
	
	QString new_name="New Track " + QString::number(track_idx);
	line_edit=new QLineEdit(new_name,this);
	vl->addWidget(line_edit);
	
	vl->addWidget(new QLabel("Channels:",this));
	
	QStringList sl;
	sl << "Mono" << "Stereo" << "3" << "Quad" << "5" << "6" << "7" << "8"; //BluRay and HD-DVD support up to 8 :D
	
	combo = new QComboBox(this);
	combo->addItems(sl);
	combo->setCurrentIndex(default_channels-1);
	
	vl->addWidget(combo);
	
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
