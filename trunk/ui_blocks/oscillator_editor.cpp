//
// C++ Implementation: oscillator_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "oscillator_editor.h"

#include <Qt/qlabel.h>
#include <Qt/qlayout.h>
#include <Qt/qframe.h>
#include <Qt/qscrollarea.h>



namespace ReShaked {


QBoxLayout * OscillatorEditor::add_section(QString p_section,bool p_add_frame) {
	
	QVBoxLayout *main_vbl=(QVBoxLayout*)layout();
	
	QLabel *l = new QLabel(p_section);
	
	QFont font=l->font();
	font.setBold(true);
	l->setFont(font);
	l->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	main_vbl->addWidget( l, 0, Qt::AlignLeft );
	
	QBoxLayout * hbl = new QBoxLayout(QBoxLayout::LeftToRight);
	
	
	main_vbl->addLayout(hbl);
	
	hbl->addSpacing( 20 ); //10 as margin!
	
	if (p_add_frame) {
		
		QFrame *qf = new QFrame;
		qf->setLineWidth(2);
		qf->setFrameStyle(QFrame::StyledPanel|QFrame::Sunken);
		hbl->addWidget(qf);
		
		QBoxLayout * hbl2 = new QBoxLayout(QBoxLayout::LeftToRight);
		qf->setLayout(hbl2);
		return hbl2;
		
	} else {
	
		return hbl;
	}
	
}

OscillatorEditor::OscillatorEditor(QWidget *p_parent) : QDialog(p_parent) {
	
	setLayout( new QVBoxLayout(this) );
	
	QBoxLayout *hbl;
	
	hbl = add_section("Display",true);
	
	
	OscillatorDisplay::Skin oscd_sk;
	oscd_sk.bg_color=QColor(0,0,0);
	oscd_sk.margin=0;
	oscd_sk.wave_color=QColor(255,255,255);
	
	display = new OscillatorDisplay(this,oscd_sk);
	
	display->setMinimumSize(400,120);
	
	
	hbl->addWidget(display);
	
	hbl = add_section("Wave Type");
	
	wave_types = new QComboBox;
	
	hbl->addWidget(wave_types);
	
	
	wave_parameter = new QSlider(Qt::Horizontal);
	wave_parameter->setRange(0,100);
	
	hbl->addWidget(wave_parameter);
	//std::vector<QSlider*> harmonics;
	
	hbl = add_section("Harmonics");
	
	QScrollArea *scra = new QScrollArea;
	hbl->addWidget(scra);
	
	QWidget *harmonics = new QWidget(scra);
	scra->setWidget(harmonics);
	
	QGridLayout *harm_grid = new QGridLayout;
	harmonics->setLayout( harm_grid );
	
	for (int i=0;i<MAX_HARMONICS;i++) {
		
		char harm_l[3]={'0','0',0};
		
		harm_l[0]='0'+(i+1)/10;
		harm_l[1]='0'+(i+1)%10;
		
		QSlider *qs = new QSlider(Qt::Vertical);
		harm_grid->addWidget( qs, 0, i );
		harmonic_array.push_back(qs);
		
		harm_grid->addWidget( new QLabel(harm_l), 1, i );
	}
	
	scra->setFixedHeight(130);
	scra->setWidgetResizable(true);	
	setWindowTitle("Waveform Creator");
	setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

	hbl = add_section("");
	
	hbl->addStretch(2); // ?
	
	QPushButton *accept_button =  new QPushButton("Create");
	QObject::connect(accept_button,SIGNAL(clicked()),this,SLOT(accept()));
	hbl->addWidget(accept_button);
	
	hbl->addSpacing(80);
	
	QPushButton *reject_button =  new QPushButton("Cancel");
	QObject::connect(reject_button,SIGNAL(clicked()),this,SLOT(reject()));
	hbl->addWidget(reject_button);
			
	hbl->addStretch(2); // ?
	
}


OscillatorEditor::~OscillatorEditor()
{
}


}

