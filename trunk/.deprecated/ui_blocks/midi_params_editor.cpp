

#include "midi_params_editor.h"
#include <Qt/qlayout.h>
#include <Qt/qscrollarea.h>
#include <Qt/qlabel.h>
#include <Qt/qpushbutton.h>
#include "ui_blocks/property_editors_qt.h"


namespace ReShaked {
	
	
int MidiParamsEditor::get_property_editor_count() {
	
	return property_editors.size();
}
PropertyEditor* MidiParamsEditor::get_property_editor(int p_which) {
	
	ERR_FAIL_INDEX_V(p_which,property_editors.size(),NULL);
	
	return property_editors[p_which];
}


MidiParamsEditor::MidiParamsEditor(QWidget *p_parent,MidiParameters *p_params) : QDialog(p_parent) {

	setLayout( new QVBoxLayout(this));
	QScrollArea *scroll = new QScrollArea(this);
	layout()->addWidget(scroll);
	
	QWidget * grid = new QWidget(scroll);
	QGridLayout *gl = new QGridLayout(grid);
	grid->setLayout(gl);
		
	scroll->setWidget(grid);
	scroll->setWidgetResizable(true);
	
	for (int i=0;i<p_params->get_parameter_count();i++) {
		
		
		
		gl->addWidget( new QLabel( QStrify( p_params->get_parameter(i)->get_caption() ),grid ),i,0);
		
		PropertyEditSpinBox *pe = new PropertyEditSpinBox(grid);
		gl->addWidget(pe,i,1);
		pe->set_property( p_params->get_parameter(i) );
		
		property_editors.push_back(pe);

	}

	CHBox *hb = new CHBox(this);
	layout()->addWidget(hb);
	
	(new QFrame(hb))->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	
	QPushButton *ab = new QPushButton("Accept",hb); //accept button
	QObject::connect(ab,SIGNAL(clicked()),this,SLOT(accept()));
	
	(new QFrame(hb))->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
		
	setMinimumWidth(400);
}


};