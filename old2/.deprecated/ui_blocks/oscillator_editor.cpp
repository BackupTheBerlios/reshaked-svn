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
#include <math.h>


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

void OscillatorEditor::regenerate_wave() {
	
	
	int wavetype=wave_types->currentIndex();
	
	static float cache[Oscillator::BASE_SIZE];
	
	for (int i=0;i<Oscillator::BASE_SIZE;i++) {
		
		cache[i]=0;
		
	}
	
	float max=0;
	
	for (int i=0;i<MAX_HARMONICS;i++) {
		
		float h_gain=(float)harmonic_array[i]->value()/100.0;
		
		if (h_gain==0.0)
			continue;
		
		for (int j=0;j<Oscillator::BASE_SIZE;j++) {
			
			float ph=fmodf(((float)j/(float)Oscillator::BASE_SIZE)*(float)(i+1),1.0);
			
			cache[j]+=get_wave_value( ph, wavetype ) * h_gain;
			
			if (cache[j]>max)
				max=cache[j];
		}
	}
	
	if (max<1.0)
		max=1.0;
	
	for (int i=0;i<Oscillator::BASE_SIZE;i++) {
		
		
		osc.set_frame( i, cache[i]/max);
		
	}
	
	
	osc.update_submaps();
	
	if (edited_osc)
		*edited_osc=osc; //copy it!
	display->update();
}


float OscillatorEditor::get_wave_value(float p_phase,int p_wave_type) {
	
	
	float param=(float)wave_parameter->value()/1000.0;
	float phase_mod=wave_phase->value()/1000.0;
	p_phase=fmodf(p_phase+phase_mod,1.0);
	
	switch (p_wave_type) {
		
		
		case WAVE_TYPE_SINE: {  
			
			return sinf(p_phase*2.0*M_PI);
		} break;
		case WAVE_TYPE_SAW: {  
			
			return 2.0*(1.0-p_phase)-1.0;
		} break;
		case WAVE_TYPE_TRIANGLE: {  
			
			float t=fmodf(p_phase,0.25)*4.0;
			
			if (p_phase<0.25)
				return t;
			else if (p_phase<0.50)
				return 1.0-t;
			else if (p_phase<0.75)
				return -t;
			else
				return -(1.0-t);
			
		} break;
		case WAVE_TYPE_SQUARE: {  
			if (p_phase<param)
				return 1.0;
			else
				return -1.0;
		} break;
		case WAVE_TYPE_CIRCULAR: {  
			
			float xp=fmodf(p_phase,0.5)*4.0-1.0;
			
			float v=sqrtf(1.0-xp*xp);
			if (p_phase>0.5)
				v=-v;
			
			return v;
		} break;
		case WAVE_TYPE_CHIRP: {  
			
			//taken from znddsbfx
			float x=fmodf(p_phase,1.0)*2.0*M_PI;
			float a=(param-0.5)*4;
			if (a<0.0) a*=2.0;
			a=powf(3.0,a);
			return (sinf(x/2.0)*sinf(a*x*x));
		} break;
		case WAVE_TYPE_CUSTOM: {  
			
			return custom.get_value( p_phase );
		} break;
		case WAVE_TYPE_MAX: {  
		} break;
		
		
	}
	
	return 0;
	
}

void OscillatorEditor::wave_changed_slot(int) {
	
	regenerate_wave();
}

int OscillatorEditor::exec(Oscillator *p_edited_osc) {
	
	edited_osc=p_edited_osc;
	
	edited_osc_original=*edited_osc;
	
	regenerate_wave();
	
	int execcode=QDialog::exec();
	
	switch (execcode) {
		
		case Accepted: {
			
			// nothing?
		} break;
		case Rejected: {
			
			*edited_osc=edited_osc_original;
			
		} break;
	}
	
	edited_osc=NULL;
	
	return execcode;
}


OscillatorEditor::OscillatorEditor(QWidget *p_parent) : QDialog(p_parent) {
	
	edited_osc=NULL;
	
	setLayout( new QVBoxLayout(this) );
	
	QBoxLayout *hbl;
	
	hbl = add_section("Display",true);
	
	OscillatorDisplay::Skin oscd_sk;
	oscd_sk.bg_color=QColor(0,0,0);
	oscd_sk.margin=10;
	oscd_sk.wave_color=QColor(255,255,255);
	
	display = new OscillatorDisplay(NULL,oscd_sk);
	
	display->setMinimumSize(400,120);
	
	display->set_oscillator( &osc );
	
	hbl->addWidget(display);
	
	hbl = add_section("Wave Type & Custom Shape");
	
	QVBoxLayout *param_vbl = new QVBoxLayout;
	
	hbl->addLayout(param_vbl);
	
	wave_types = new QComboBox;
	
	wave_types->addItem("Sine Wave");
	wave_types->addItem("Saw Wave");
	wave_types->addItem("Triangle Wave");
	wave_types->addItem("Square Wave");
	wave_types->addItem("Circular Wave");
	wave_types->addItem("Chirp Wave");
	wave_types->addItem("Custom Shape");
	
	QObject::connect(wave_types,SIGNAL(currentIndexChanged(int)),this,SLOT(wave_changed_slot( int )));
	
	param_vbl->addWidget(wave_types);
	
	param_vbl->addWidget(new QLabel("Width:"));
	wave_parameter = new QSlider(Qt::Horizontal);
	wave_parameter->setRange(0,1000);
	wave_parameter->setValue(500);
	param_vbl->addWidget(wave_parameter);
	
	param_vbl->addWidget(new QLabel("Phase:"));
	wave_phase = new QSlider(Qt::Horizontal);
	wave_phase->setRange(0,1000);
	wave_phase->setValue(0);
	
	QObject::connect(wave_phase,SIGNAL(valueChanged(int)),this,SLOT(wave_changed_slot( int )));
	param_vbl->addWidget(wave_phase);
	
	//std::vector<QSlider*> harmonics;
	
	ShapeEditor::Skin se_sk;
	se_sk.bg_col=QColor(0,0,0);
	se_sk.line_col=QColor(255,255,255);
	se_sk.point_col=QColor(255,100,100);
	se_sk.zero_col=QColor(100,100,100);
	
	hbl->addSpacing(20);
	
	shape_editor = new ShapeEditor(this,se_sk);
	shape_editor->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	shape_editor->set_shape( &custom );
	hbl->addWidget(shape_editor);
	
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
		if (i==0)
			qs->setValue(100); //only first harmonic
		
		QObject::connect(qs,SIGNAL(valueChanged(int)),this,SLOT(wave_changed_slot( int )));
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
	
	custom.set_min(-1);
	custom.set_max(1);
	
	QObject::connect(shape_editor,SIGNAL(shape_changed_signal()),this,SLOT(regenerate_wave()));
	regenerate_wave();
	
}


OscillatorEditor::~OscillatorEditor()
{
}


}

