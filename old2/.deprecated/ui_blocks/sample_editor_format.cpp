/***************************************************************************
    This file is part of the CheeseTronic Music Tools
    url                  : http://reduz.com.ar/cheesetronic
    copyright            : (C) 2003 by Juan Linietsky
    email                : coding@reduz.com.ar
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
//
//
// C++ Implementation: cpp
//
// Description:
//
//
// Author: Juan Linietsky <coding@reduz.com.ar>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sample_editor_format.h"
#include "pixmaps/transp_12_down.xpm"
#include "pixmaps/transp_12_up.xpm"
#include "pixmaps/transp_14_down.xpm"
#include "pixmaps/transp_14_up.xpm"
#include "pixmaps/transp_octave_down.xpm"
#include "pixmaps/transp_octave_up.xpm"
#include "pixmaps/transp_semitone_down.xpm"
#include "pixmaps/transp_semitone_up.xpm"
#include <Qt/qpushbutton.h>
#include <math.h>

namespace ReShaked {

void SampleEditorFormat::transpose_up_1_4_cbk() {

	if (!sample_data || !sample_data->get_length())
		return;

	float freq=sample_data->get_base_freq();
	freq*=powf(2.0,1.0/96.0);
	sample_data->set_base_freq(freq);
	update_format();
	sample_base_freq_changed();
}


void SampleEditorFormat::transpose_up_1_2_cbk() {

	if (!sample_data || !sample_data->get_length())
		return;

	float freq=sample_data->get_base_freq();
	freq*=pow(2.0,1.0/48.0);
	sample_data->set_base_freq(freq);
	update_format();
	sample_base_freq_changed();

}

void SampleEditorFormat::transpose_up_1_cbk() {

	if (!sample_data || !sample_data->get_length())
		return;

	float freq=sample_data->get_base_freq();
	freq*=pow(2.0,1.0/12.0);
	sample_data->set_base_freq(freq);
	update_format();
	sample_base_freq_changed();


}
void SampleEditorFormat::transpose_up_12_cbk() {

	if (!sample_data || !sample_data->get_length())
		return;

	float freq=sample_data->get_base_freq();
	freq*=2.0;
	sample_data->set_base_freq(freq);
	update_format();
	sample_base_freq_changed();

}


void SampleEditorFormat::transpose_down_1_4_cbk() {

	if (!sample_data || !sample_data->get_length())
		return;

	float freq=sample_data->get_base_freq();
	freq/=pow(2.0,1.0/96.0);
	sample_data->set_base_freq(freq);
	update_format();
	sample_base_freq_changed();
}
void SampleEditorFormat::transpose_down_1_2_cbk() {

	if (!sample_data || !sample_data->get_length())
		return;

	float freq=sample_data->get_base_freq();
	freq/=pow(2.0,1.0/48.0);
	sample_data->set_base_freq(freq);
	update_format();
	sample_base_freq_changed();

}
void SampleEditorFormat::transpose_down_1_cbk() {

	if (!sample_data || !sample_data->get_length())
		return;

	float freq=sample_data->get_base_freq();
	freq/=pow(2.0,1.0/12.0);
	sample_data->set_base_freq(freq);
	update_format();
	sample_base_freq_changed();

}
void SampleEditorFormat::transpose_down_12_cbk() {

	if (!sample_data || !sample_data->get_length())
		return;

	float freq=sample_data->get_base_freq();
	freq/=2.0;
	sample_data->set_base_freq(freq);
	update_format();
	sample_base_freq_changed();

}

void SampleEditorFormat::base_freq_changed(int p_frq) {
	if (updating) return;

	if (!sample_data || !sample_data->get_length())
		return;

	sample_data->set_base_freq(p_frq);
	sample_modified();

}

void SampleEditorFormat::loop_begin_changed_cbk(int p_idx) {

	if (updating) return;
	if (!sample_data || !sample_data->get_length())
		return;
	sample_data->set_loop_begin(spin_loop_begin->value());
	update_loop();

}
void  SampleEditorFormat::loop_end_changed_cbk(int p_idx) {

	if (updating) return;
	if (!sample_data || !sample_data->get_length())
		return;
	sample_data->set_loop_end(spin_loop_end->value());
	update_loop();
}

void  SampleEditorFormat::loop_type_changed_cbk(int p_idx) {
	if (updating) return;
	switch (p_idx) {

		case 0:{
			sample_data->set_loop_type( Sample::LOOP_NONE );
		} break;
		case 1:{
			
			sample_data->set_loop_type( Sample::LOOP_FORWARD );
		} break;
		case 2:{
			sample_data->set_loop_type( Sample::LOOP_BIDI );
		} break;
	}
	sample_modified();
}

void SampleEditorFormat::update_format() {
	
	if (!sample_data)
		return;
	
	updating=true;

	char auxbuf[40];

	sprintf(auxbuf,"%i",sample_data->get_length());
	entry_size->setText(auxbuf);
	if (sample_data->get_channels()==1) {
		
		entry_chans->setText("Mono");

	} else if (sample_data->get_channels()==2) {
		
		entry_chans->setText("Stereo");
		
	} else {
		
		entry_chans->setText(QString::number(sample_data->get_channels()));
		
	}
	spin_base_freq->setValue(sample_data->get_base_freq());

	combo_loop_type->setCurrentIndex( sample_data->get_loop_type() );

	updating=false;
	sample_modified();
}

void SampleEditorFormat::update_loop() {
	if (!sample_data)
		return;

	updating=true;

	spin_loop_begin->setMinimum(0);
	spin_loop_begin->setMaximum(sample_data->get_loop_end()-1);
	spin_loop_begin->setValue(sample_data->get_loop_begin());




	spin_loop_end->setMinimum(sample_data->get_loop_begin()+1);
	spin_loop_end->setMaximum(sample_data->get_length());
	spin_loop_end->setValue(sample_data->get_loop_end());


	if (sample_viewer)
		sample_viewer->update();

	updating=false;
	sample_modified();
	
}

void SampleEditorFormat::set_sample_data(Sample *p_sample_data) {

	sample_data=p_sample_data;
	update_format();
	update_loop();

}
void SampleEditorFormat::set_sample_viewer(SampleViewer *p_sample_viewer) {

	sample_viewer=p_sample_viewer;
}

SampleEditorFormat::SampleEditorFormat(QWidget *p_parent) : CVBox(p_parent) {


	CHBox *main_hb = new CHBox(this);
	QHBoxLayout *hbl=(QHBoxLayout*)main_hb->layout();
	CVBox *vb1 = new CVBox(main_hb);
	hbl->setStretchFactor(vb1,1);
	CVBox *vb2 = new CVBox(main_hb);
	hbl->setStretchFactor(vb2,1);
	hbl->setSpacing(10);
	
	/* left */
	

	label_base_freq = new QLabel ("C5 Freq:",vb1);
	spin_base_freq = new QSpinBox(vb1);
	spin_base_freq->setMinimum(500);
	spin_base_freq->setMaximum(0xFFFFFFF);
	
	
	label_size = new QLabel("Length:",vb1);
	entry_size = new QLineEdit(vb1);
	entry_size->setReadOnly(true);
	
	label_chans = new QLabel("Channels:",vb1);
	entry_chans = new QLineEdit(vb1);
	entry_chans->setReadOnly(true);

	label_loop_type = new QLabel("Loop Type:",vb2);
  	combo_loop_type = new QComboBox(vb2);
	combo_loop_type->addItem("Disabled");
	combo_loop_type->addItem("Forward ->");
	combo_loop_type->addItem("Bidi <->");

	//second row
	label_loop_begin = new QLabel("Loop Begin:",vb2);
	spin_loop_begin = new QSpinBox(vb2);


	label_loop_end = new QLabel("Loop End:",vb2);
	spin_loop_end = new QSpinBox(vb2);

	QObject::connect( combo_loop_type , SIGNAL(activated(int)),this, SLOT(loop_type_changed_cbk(int)) );
	QObject::connect( spin_loop_begin, SIGNAL(valueChanged(int)),this, SLOT(loop_begin_changed_cbk(int)) );
	QObject::connect( spin_loop_end, SIGNAL(valueChanged(int)),this, SLOT(loop_end_changed_cbk(int)) );
	QObject::connect( spin_base_freq, SIGNAL(valueChanged(int)),this, SLOT(base_freq_changed(int)) );
	updating=false;
	sample_viewer=NULL;


	vb1->layout()->setSpacing(3);
	vb2->layout()->setSpacing(3);

	CHBox * grid2 = new CHBox(this);
	grid2->layout()->setSpacing(2);

	QPushButton *auxbut;
	auxbut = new QPushButton(QPixmap((const char**)transp_14_down_xpm),"",grid2);
//	auxbut->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
	QObject::connect( auxbut, SIGNAL(clicked()), this, SLOT(transpose_down_1_4_cbk()));
	auxbut = new QPushButton(QPixmap((const char**)transp_14_up_xpm),"",grid2);
//	auxbut->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
	QObject::connect( auxbut, SIGNAL(clicked()), this, SLOT(transpose_up_1_4_cbk()));

	(new QFrame(grid2))->setFrameStyle(QFrame::VLine+QFrame::Sunken);

	auxbut = new QPushButton(QPixmap((const char**)transp_12_down_xpm),"",grid2);
//	auxbut->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
	QObject::connect( auxbut, SIGNAL(clicked()), this, SLOT(transpose_down_1_2_cbk()));
	auxbut = new QPushButton(QPixmap((const char**)transp_12_up_xpm),"",grid2);
//	auxbut->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
	QObject::connect( auxbut, SIGNAL(clicked()), this, SLOT(transpose_up_1_2_cbk()));


	(new QFrame(grid2))->setFrameStyle(QFrame::VLine+QFrame::Sunken);

	auxbut = new QPushButton(QPixmap((const char**)transp_semitone_down_xpm),"",grid2);
//	auxbut->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
	QObject::connect( auxbut, SIGNAL(clicked()), this, SLOT(transpose_down_1_cbk()));
	auxbut = new QPushButton(QPixmap((const char**)transp_semitone_up_xpm),"",grid2);
//	auxbut->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
	QObject::connect( auxbut, SIGNAL(clicked()), this, SLOT(transpose_up_1_cbk()));


	(new QFrame(grid2))->setFrameStyle(QFrame::VLine+QFrame::Sunken);

	auxbut = new QPushButton(QPixmap((const char**)transp_octave_down_xpm),"",grid2);
	//auxbut->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
	QObject::connect( auxbut, SIGNAL(clicked()), this, SLOT(transpose_down_12_cbk()));
	auxbut = new QPushButton(QPixmap((const char**)transp_octave_up_xpm),"",grid2);
//	auxbut->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));
	QObject::connect( auxbut, SIGNAL(clicked()), this, SLOT(transpose_up_12_cbk()));

	layout()->setSpacing(4);
	sample_data=NULL;
	
	setWindowTitle("Sample Editor");
	
}

SampleEditorFormat::~SampleEditorFormat()
{
}

}
