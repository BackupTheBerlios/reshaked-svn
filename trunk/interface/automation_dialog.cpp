//
// C++ Implementation: automation_dialog
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "automation_dialog.h"
#include <Qt/qhash.h>
#include "ui_blocks/helpers.h"
#include <math.h>
#include <Qt/qlabel.h>
#include <Qt/qpushbutton.h>

namespace ReShaked {


bool AutomationPopup::check_action(QAction *p_action)  {


	QActionAutomation *item = dynamic_cast<QActionAutomation*>(p_action);
				
	if (!item)
		return false;
	
	bool checked=item->isChecked();
	int idx=item->get_property_idx();
	if (idx==-1)
		return true;;
	
	bool has_auto=track->has_property_visible_automation( idx );
	
	if (checked==has_auto)
		return true; //nothing to do
	
	if (checked)
		attempt_automation_add_signal(idx);	
	else
		attempt_automation_remove_signal(idx);		
	
	return true;
}

void AutomationPopup::rebuild() {
	
	typedef QHash<QString,QMenu*> PathMap;
	PathMap hashmap;
	
	clear();
		
	for (int i=0;i<track->get_property_count();i++) {
		
		QString path=QStrify(track->get_property_visual_path( i));
		
		QString dir=path.left( path.lastIndexOf("/") );
		QString name=path.right( path.length()-(path.lastIndexOf("/")+1) );
		
		//printf("Dir %s, File %s\n",dir.toAscii().data(),name.toAscii().data());
		
		/* find if we have dir! */
		
		QActionAutomation * item;
		if (dir=="") { //toplevelAutomationTree( 
			
			item  = new QActionAutomation(this,i);
			addAction(item);

		} else {
			
			// have the path for this dir somewhere?
			PathMap::iterator I =hashmap.find(dir);
			if (I!=hashmap.end()) {
				item = new QActionAutomation(I.value(),i);
				I.value()->addAction(item);
				
			} else {
				
				/* build path */
				int from=1;
				QString dir2=dir+"/"; //so it finds the last / too
				QMenu*parent=NULL;
				QMenu*current=NULL;
				
				while (true) {
					
					if (from==dir2.length())
						break;
					int idx=dir2.indexOf("/",from);
					if (idx==-1)
						break;
					QString auxp=path.left( idx );
					from=idx+1; //so it skips the /
					printf("searching %s\n",auxp.toAscii().data());
					I=hashmap.find(auxp);
					if (I==hashmap.end()) { //we must create it!
						
						QString nodename=auxp.right( auxp.length()-(auxp.lastIndexOf("/")+1) );
						
						QMenu* node;
						if (parent) {
							node= new QMenu(parent);
							parent->addMenu(node)->setText(nodename);

						} else {
							node= new QMenu(this);
							addMenu(node)->setText(nodename);
							

						}
						
						//QObject::connect(node,SIGNAL(triggered(QAction*)),this,SLOT(action_activated(QAction*)));						
						//node->setText(0,nodename+"/");
						
						parent=node;
						hashmap[auxp]=node;
						printf("adding %s, nodename: %s\n",auxp.toAscii().data(),nodename.toAscii().data());
						
					} else { //exists
						 
						parent=I.value();
					}
				}
				
				item = new QActionAutomation(parent,i);
				parent->addAction(item);
			}
				
				
		}
		
		QString captionstr=QStrify( track->get_property( i)->get_caption());

		if (captionstr.lastIndexOf("/",-1)>=0) {
			
			captionstr=captionstr.right( captionstr.length()-(captionstr.lastIndexOf("/")+1));
		}
		
		if (track->get_property_automation(i)->get_block_count()) {
				
			item->setText( captionstr+"(*)" );
		
		} else {
		
			item->setText( captionstr );
		
		}
		item->setCheckable(true);
		
		//item->setText( 0, QStrify( track->get_property( i)->get_caption() ) );
		//item->setText( 1, QStrify( track->get_property( i)->get_text_value( track->get_property( i)->get_min() ) ) );
		//item->setText( 2, QStrify( track->get_property( i)->get_text_value( track->get_property( i)->get_max() ) ) );
		item->setChecked(track->has_property_visible_automation(i)?Qt::Checked:Qt::Unchecked);
		
		
	}
	
	
	addSeparator();
	
	manage = new QAction("Editor..",this);
	addAction(manage);
	QObject::connect(manage,SIGNAL(activated()),this,SIGNAL(editor_requested()));
	
}

AutomationPopup::AutomationPopup(QWidget *p_parent,Track *p_track) : QMenu(p_parent)
{
	
	track=p_track;
	//QObject::connect(this,SIGNAL(triggered(QAction*)),this,SLOT(action_activated(QAction*)));
}


AutomationPopup::~AutomationPopup()
{
}

/*************************************/


#define LABELS_BEGIN\
	CHBox *hb;\
	QLabel *label;\
	QFont font;\
	
	
#define LABEL(m_title) \
	label = new QLabel(m_title,this); \
	layout()->addWidget(label); \
	font=label->font(); \
	font.setBold(true); \
	label->setFont(font); \
	hb = new CHBox(this); \
	(new QFrame(hb))->setFixedWidth(20); \
	layout()->addWidget(hb); 




void AutomationSettingsLFO::set_lfo() {
	
	LFO lfo_local=blockdata->get_lfo();
	switch (wave->currentIndex()) {
		case 0: lfo_local.set_mode( LFO::MODE_SINE ); break;
		case 1: lfo_local.set_mode( LFO::MODE_SAW_UP ); break;
		case 2: lfo_local.set_mode( LFO::MODE_SAW_DOWN); break;
		case 3: lfo_local.set_mode( LFO::MODE_SQUARE); break;
	}
	
	lfo_local.set_rate( powf((float)rate->value()/512.0,2) );
	lfo_local.set_depth( (float)depth->value()/1024.0 );
	lfo_local.set_phase( ((float)phase->value()/1024.0) );
	lfo_local.set_random_depth( (float)random->value()/1024.0 );
	
	blockdata->get_lfo()=lfo_local;
	lfo_changed=true;
	lfo_changed_signal();
}

void AutomationSettingsLFO::wave_changed(int) {
	
	set_lfo();
}

void AutomationSettingsLFO::slider_changed(int) {
	
	set_lfo();
}


bool AutomationSettingsLFO::is_lfo_changed() {
	
	return lfo_changed;
}
AutomationSettingsLFO::AutomationSettingsLFO(QWidget *p_parent,Automation::AutomationData *p_blockdata) : QDialog(p_parent) {
	
	setLayout(new QVBoxLayout(this));
	
	
	blockdata=p_blockdata;
	
	LABELS_BEGIN
			
	LABEL("Wave");
	
	wave = new QComboBox(hb);
	wave->addItem("Sine");
	wave->addItem("Saw Up");
	wave->addItem("Saw Down");
	wave->addItem("Square");
	QObject::connect(wave,SIGNAL(activated(int)),this,SLOT(wave_changed( int )));
	wave->setCurrentIndex( p_blockdata->get_lfo().get_mode() );
	
	LABEL("Rate");
	
	rate = new QSlider(Qt::Horizontal,hb);
	rate->setRange(1,1024);
	rate->setValue( (sqrt(p_blockdata->get_lfo().get_rate()))*512.0 );
	QObject::connect(rate,SIGNAL(valueChanged(int)),this,SLOT(slider_changed( int )));
	
	LABEL("Base Depth");
	
	depth = new QSlider(Qt::Horizontal,hb);
	depth->setRange(0,1024);
	depth->setValue(p_blockdata->get_lfo().get_depth()*1024);
	QObject::connect(depth,SIGNAL(valueChanged(int)),this,SLOT(slider_changed( int )));
	
	LABEL("Phase");
	
	phase = new QSlider(Qt::Horizontal,hb);
	phase->setRange(0,1024);
	phase->setValue(p_blockdata->get_lfo().get_phase()*1024);
	QObject::connect(phase,SIGNAL(valueChanged(int)),this,SLOT(slider_changed( int )));

	
	LABEL("Random");
	
	random = new QSlider(Qt::Horizontal,hb);
	random->setRange(0,1024);
	random->setValue(p_blockdata->get_lfo().get_random_depth()*1024);
	QObject::connect(random,SIGNAL(valueChanged(int)),this,SLOT(slider_changed( int )));
	layout()->setMargin(10);
	layout()->setSpacing(2);
	
	lfo_changed=false;
	setWindowTitle("LFO Settings");
	
	QPushButton *acceptb = new QPushButton("Accept",this);
	acceptb->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	layout()->addWidget(acceptb);
	layout()->setAlignment(acceptb,Qt::AlignHCenter);
	
	QObject::connect(acceptb,SIGNAL(clicked()),this,SLOT(accept()));
	
}


}
