//
// C++ Implementation: midi_device_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "midi_device_editor.h"
#include <Qt/qlabel.h>
#include <Qt/qlayout.h>
#include <Qt/qpushbutton.h>
#include <Qt/qheaderview.h>
#include <Qt/qfiledialog.h>
#include "pixmaps/icon_add.xpm"
#include "pixmaps/icon_remove.xpm"
#include "pixmaps/icon_file_open.xpm"
#include "pixmaps/icon_file_save.xpm"
#include "engine/ins_device_loader.h"
#include <Qt/qtimer.h>
#include <Qt/qmessagebox.h>
#include <Qt/qinputdialog.h>

namespace ReShaked {


void MidiDeviceEditor::add_bank_slot() {
	
	desc->add_bank();
	update_bank_list();
}
void MidiDeviceEditor::erase_bank_slot() {
	
	if (current_bank>=0)
		desc->erase_bank(current_bank);
	update_bank_list();
}
void MidiDeviceEditor::add_gm_bank() {
	
	desc->add_GM_bank();
	update_bank_list();
	
}
void MidiDeviceEditor::set_gm_controls() {
	
	update_device();	
}

void MidiDeviceEditor::patch_name_changed(int p_row,int p_col) {
	
	int patch_idx=p_row;
	QString text=patches->item(p_row,p_col)->text();
	
	banks->blockSignals(true);
	desc->set_bank_patch_name(banks->currentRow(),patch_idx,DeQStrify(text) );
	banks->blockSignals(false);
}

void MidiDeviceEditor::control_name_changed(int p_row,int p_col) {
	
	int ctrl_idx=p_row;
	QString text=controls->item(p_row,p_col)->text();
	
	banks->blockSignals(true);
	desc->set_control_name( ctrl_idx,DeQStrify(text) );
	banks->blockSignals(false);
}

void MidiDeviceEditor::update_current_bank() {

	if (current_bank==-1) {
			
		bank_select_MSB->setEnabled(false);
		bank_select_LSB->setEnabled(false);
		patches->setEnabled(false);
		banks->setEnabled(false);
	} else {
		bank_select_MSB->setEnabled(true);
		bank_select_LSB->setEnabled(true);
		patches->setEnabled(true);
		banks->setEnabled(true);
		
		bank_select_MSB->setValue(desc->get_bank_select_MSB(current_bank));
		bank_select_LSB->setValue(desc->get_bank_select_LSB(current_bank));
		
		patches->blockSignals(true);
		for (int i=0;i<128;i++) {
			
			patches->item(i,0)->setText( QStrify(desc->get_bank_patch_name(current_bank,i)) );
		}
		patches->blockSignals(false);
		
	}
}



void MidiDeviceEditor::update_bank_list() {
	
	banks->blockSignals(true);

	banks->clear();
	for (int i=0;i<desc->get_bank_count();i++) {
		QListWidgetItem * new_item = new QListWidgetItem(QStrify(desc->get_bank_name(i)));
		new_item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
		banks->addItem( new_item );
	}
	banks->blockSignals(false);
	
	if (current_bank<0)
		current_bank=0;
	
	if (current_bank>=desc->get_bank_count())
		current_bank=desc->get_bank_count()-1;
	
	banks->setCurrentRow(current_bank);
	
	update_current_bank();
}


void MidiDeviceEditor::bank_selected_slot(int p_bank) {
	
	current_bank=p_bank;
	update_current_bank();
	
}

void MidiDeviceEditor::device_name_changed(const QString& p_new_text) {
	
	desc->set_name( DeQStrify( p_new_text ) );
}


void MidiDeviceEditor::bank_name_changed(const QString&p_new_name) {
	
	/* QT4 CANT GVIE CALLBACKS TO BANK NAME EDITED ON A QLISTWIDGET :( */
	//printf("MEEH %s\n",p_new_name.toAscii().data());
	desc->set_bank_name(current_bank,DeQStrify(p_new_name));
	
}

void MidiDeviceEditor::bank_select_MSB_changed(int p_to_val) {
	
	desc->set_bank_select_MSB(current_bank, p_to_val );
}
void MidiDeviceEditor::bank_select_LSB_changed(int p_to_val) {
	
	desc->set_bank_select_LSB(current_bank, p_to_val );

}

void MidiDeviceEditor::check_bank_names_changed_horrible_qt4_hack() {
	
	for (int i=0;i<banks->count();i++) {
		
		desc->set_bank_name(i,DeQStrify(banks->item(i)->text()));
	}

	
}

void MidiDeviceEditor::load_device_slot() {
	
	QString file=QFileDialog::getOpenFileName ( this, "Open Bank",".", "Bank Files (*.bnk *.ins)");
			
	if (file=="")
		return;
	
	
	if (file.toLower().indexOf(".ins")>=0) {
		
		
		InsDeviceLoader ins_loader;
		
		ins_loader.load( file.toAscii().data() );
		
		int device_selected=0;
		
		if (ins_loader.get_device_count()==0) {
			
			QMessageBox::information ( this, "Info", "File contains no banks" , QMessageBox::Ok);
			return;
		} else if (ins_loader.get_device_count()>1) {
			
			QStringList sl;
			for (int i=0;i<ins_loader.get_device_count();i++) {
				
				sl << QString(ins_loader.get_device_name(i).c_str());
			}
	
			QString item= QInputDialog::getItem ( this, "Select Device", "Multiple Devices Found:",sl,0,false);
			
			int dev=-1;
			
			for (int i=0;i<ins_loader.get_device_count();i++) {
				
				if (item==QString(ins_loader.get_device_name(i).c_str()))
					dev=i;
			}
			
			if (dev==-1)
				
				return;
			
			device_selected=dev;

			
		}
		
		ins_loader.set_to_device_description( device_selected, desc );
	}
	
	update_device();
			
}
void MidiDeviceEditor::save_device_slot() {
	
	
}

void MidiDeviceEditor::bank_select_method_changed(int p_method) {
	
	switch (p_method) {
		case 0: desc->set_bank_select_method( MidiDeviceDescription::BANK_SELECT_NONE ); break;
		case 1: desc->set_bank_select_method( MidiDeviceDescription::BANK_SELECT_MSB ); break;
		case 2: desc->set_bank_select_method( MidiDeviceDescription::BANK_SELECT_LSB ); break;				
		case 3: desc->set_bank_select_method( MidiDeviceDescription::BANK_SELECT_MSB_LSB ); break;
	}
		
}

void MidiDeviceEditor::update_device() {
	
	device_name->setText( QStrify(desc->get_name() ) );	
		
	controls->blockSignals(true);
	for (int i=0;i<128;i++) {
		
		controls->item(i,0)->setText( QStrify(desc->get_control_name( i)) );
	}
	controls->blockSignals(false);
	bank_select_method->setCurrentIndex(desc->get_bank_select_method());

	update_bank_list();
	
}

#define LABELS_BEGIN\
	QWidget *hb;\
	QLabel *label;\
	QFont font;\
	
	
#define LABEL(m_title,m_parent,m_widget) \
	label = new QLabel(m_title); \
	m_parent->layout()->addWidget(label);\
	font=label->font(); \
	font.setBold(true); \
	label->setFont(font); \
	if (m_widget) \
		m_parent->layout()->addWidget(m_widget);\




MidiDeviceEditor::MidiDeviceEditor(QWidget *p_parent,MidiDeviceDescription *p_desc) : QDialog(p_parent) {
	
	
	/**** A LOT OF CODE BORKED AND DIED IN HERE, HAD TO HAX QUITE A LOT, AND DONT KNOW WHY */
	
	LABELS_BEGIN
	
	current_bank=0;
	desc=p_desc;
	
	setLayout(new QVBoxLayout(this));
	
	QWidget *main_hb = new QWidget(this);
	main_hb->setLayout(new QHBoxLayout(main_hb));
	layout()->addWidget(main_hb);
			
	
	QWidget *left_vb = new QWidget(main_hb);
	left_vb->setLayout(new QVBoxLayout(left_vb));
	main_hb->layout()->setAlignment(left_vb,Qt::AlignTop);
	main_hb->layout()->addWidget(left_vb);
	main_hb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	QWidget *device_hb = new QWidget;
	device_hb->setLayout(new QHBoxLayout(left_vb));
	
	LABEL("Device Name",left_vb,device_hb);			
	
	device_name = new QLineEdit;
	device_hb->layout()->addWidget(device_name);
	device_name->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	QPushButton *load_button = new QPushButton(QPixmap( (const char **)icon_file_open_xpm),"",device_hb);
	device_hb->layout()->addWidget(load_button);
	QObject::connect( load_button ,SIGNAL( clicked() ), this, SLOT(load_device_slot() ) );
		
	QPushButton *save_button = new QPushButton(QPixmap( (const char **)icon_file_save_xpm),"",device_hb);
	device_hb->layout()->addWidget(save_button);
	QObject::connect( save_button ,SIGNAL( clicked() ), this, SLOT(save_device_slot() ) );
	
	device_name->setText( QStrify(desc->get_name() ) );
	QObject::connect(device_name,SIGNAL(textChanged(const QString&)),this,SLOT(device_name_changed( const QString& )));		
	
	CVBox *right_vb = new CVBox(main_hb);
	right_vb->setLayout(new QVBoxLayout(right_vb));
	main_hb->layout()->addWidget(right_vb);
	main_hb->layout()->setAlignment(right_vb,Qt::AlignTop);
	
	bank_select_method = new QComboBox;
	LABEL("Bank Select Method",right_vb,bank_select_method);
	bank_select_method->addItem("None");
	bank_select_method->addItem("MSB/Variation (CC 0)");
	bank_select_method->addItem("LSB (CC 32)");
	bank_select_method->addItem("MSB/LSB (14 Bits)");
	
	QObject::connect(bank_select_method,SIGNAL(currentIndexChanged(int)),this,SLOT(bank_select_method_changed(int)));
	
	tabs = new QTabWidget(this);
	layout()->addWidget(tabs);
	tabs->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	QWidget *banks_hb = new QWidget(tabs);
	banks_hb->setLayout(new QHBoxLayout(banks_hb));
	tabs->addTab(banks_hb,"Bank List");
			
	left_vb = new QWidget(banks_hb);
	left_vb->setLayout(new QVBoxLayout(left_vb));
	banks_hb->layout()->addWidget(left_vb);
	
	banks = new QListWidget;
	LABEL("Bank List",left_vb,banks);
	
	banks->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	
	QObject::connect(banks,SIGNAL(currentRowChanged(int)),this,SLOT(bank_selected_slot(int)));
	QObject::connect(banks,SIGNAL(currentTextChanged(const QString&)),this,SLOT(bank_name_changed(const QString&)));
	
	CHBox *bank_actions_hb = new CHBox(left_vb);
	left_vb->layout()->addWidget(bank_actions_hb);
			
	QObject::connect( new QPushButton(QPixmap( (const char **)icon_add_xpm),"",bank_actions_hb) ,SIGNAL( clicked() ), this, SLOT(add_bank_slot() ) );
	
	QObject::connect( new QPushButton(QPixmap( (const char **)icon_add_xpm),"GM",bank_actions_hb) ,SIGNAL( clicked() ), this, SLOT(add_gm_bank() ) );
	
	QObject::connect( new QPushButton(QPixmap( (const char **)icon_remove_xpm),"",bank_actions_hb) ,SIGNAL( clicked() ), this, SLOT(erase_bank_slot() ) );
	
	bank_actions_hb->layout()->setMargin(10);
	
	bank_select_MSB = new QSpinBox;
	LABEL("Bank Select MSB (CC 0):",left_vb,bank_select_MSB);
	
	bank_select_MSB->setMinimum(0);	
	bank_select_MSB->setMaximum(127);
	bank_select_MSB->setValue(0);
	QObject::connect(bank_select_MSB,SIGNAL(valueChanged(int)),this,SLOT(bank_select_MSB_changed(int)));
	
	bank_select_LSB = new QSpinBox;
	
	LABEL("Bank Select LSB (CC 32):",left_vb,bank_select_LSB);
	
	bank_select_LSB->setMinimum(0);	
	bank_select_LSB->setMaximum(127);
	bank_select_LSB->setValue(0);
	QObject::connect(bank_select_LSB,SIGNAL(valueChanged(int)),this,SLOT(bank_select_LSB_changed(int)));
	
	right_vb = new CVBox(banks_hb);
	right_vb->setLayout(new QVBoxLayout(right_vb));
	banks_hb->layout()->addWidget(right_vb);
	right_vb->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
	

	LABEL("Patch List in Bank",right_vb,NULL);
	patches = new QTableWidget(128,1);
	right_vb->layout()->addWidget(patches);

	right_vb->layout()->addWidget(new QLabel("^ this is a Qt4 Bug! ^"));
	
	QStringList headers;
	headers << "Name";
	patches->setHorizontalHeaderLabels(headers);
	patches->horizontalHeader()->setResizeMode ( 0, QHeaderView::Stretch );
	patches->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	for (int i=0;i<128;i++) {
		
		QTableWidgetItem *name = new QTableWidgetItem( "" );
		
		name->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
		patches->setItem(i,0,name );
		
	}

	QObject::connect(patches,SIGNAL(cellChanged(int,int)),this,SLOT(patch_name_changed(int,int)));
	
	
	QWidget *controls_vb = new QWidget(tabs);
	controls_vb->setLayout(new QVBoxLayout(banks_hb));
	tabs->addTab(controls_vb,"Controls");
	
	LABEL("Control List",controls_vb,NULL);
	controls = new QTableWidget(128,1);
	controls_vb->layout()->addWidget(controls);

	//controls_vb->layout()->addWidget(new QLabel("^ this is a Qt4 Bug! ^");
	
	headers=QStringList();
	headers << "Name";
	controls->setHorizontalHeaderLabels(headers);
	controls->horizontalHeader()->setResizeMode ( 0, QHeaderView::Stretch );
	controls->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	QStringList idxnums;
	for (int i=0;i<128;i++) {
		
		QTableWidgetItem *name = new QTableWidgetItem( "" );
		
		name->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
		controls->setItem(i,0,name );
		idxnums << QString::number(i);
		
	}
	controls->setVerticalHeaderLabels(idxnums);

	QObject::connect(controls,SIGNAL(cellChanged(int,int)),this,SLOT(control_name_changed(int,int)));
	
	QPushButton *set_gm_ctrls = new QPushButton("Set GM Names",this);
	set_gm_ctrls->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	QObject::connect(set_gm_ctrls,SIGNAL(clicked()),this,SLOT(set_gm_controls()));
	controls_vb->layout()->addWidget(set_gm_ctrls);
	controls_vb->layout()->setAlignment(set_gm_ctrls,Qt::AlignHCenter);
	
	QPushButton *acceptb = new QPushButton("Close",this);
	acceptb->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	QObject::connect(acceptb,SIGNAL(clicked()),this,SLOT(accept()));
	layout()->addWidget(acceptb);
	layout()->setAlignment(acceptb,Qt::AlignHCenter);
		
	
	setWindowTitle("MIDI Bank Management");
	
	setMinimumSize( QSize(500,550) );
	
	QTimer *t = new QTimer(this);
	QObject::connect(t,SIGNAL(timeout()),this,SLOT(check_bank_names_changed_horrible_qt4_hack()));
	t->start(500);
	
	update_device();
}


MidiDeviceEditor::~MidiDeviceEditor()
{
}


}

