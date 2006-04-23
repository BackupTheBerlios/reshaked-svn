//
// C++ Implementation: settings_keys
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "settings_keys.h"
#include "editor/keyboard_input.h"
#include <Qt/qmap.h>
#include <Qt/qheaderview.h>
#include <Qt/qmessagebox.h>
#include <Qt/qevent.h>
#include <Qt/qlabel.h>

namespace ReShaked {

void SettingsKeys::KeybindItem::update_bind() {
	
	ERR_FAIL_INDEX(bind_idx,Keyboard_Input::get_singleton_instance()->get_key_bind_count());
	
	

	setText(0,QStrify(Keyboard_Input::get_singleton_instance()->get_key_bind(bind_idx).caption_name));
	
	int bind=Keyboard_Input::get_singleton_instance()->get_key_bind(bind_idx).key;
	if (bind==Keyboard_Input::NO_KEY) {
		setText(1,"Undefined");

	} else {
		
		QString text=(QString)QKeySequence(bind);
		if (text==" ")
			text="Space Bar";
		if (Keyboard_Input::get_singleton_instance()->get_key_bind(bind_idx).readonly) {
			text+="   (readonly)";
			setFlags(0);

		} 
		setText(1,text);
	}
		
	
}

SettingsKeys::KeybindItem::KeybindItem(QTreeWidgetItem *p_parent,int p_bind_idx) : QTreeWidgetItem(p_parent) {
	
	bind_idx=p_bind_idx;
	update_bind();
	
}


void SettingsKeys::update_keybinds() {
	
	for (int i=0;i<items.size();i++) {
			
		items[i]->update_bind();
		
	}
	
}

void SettingsKeys::itemSelectionChangedSlot() {
	
	
	set_bind->setEnabled(true);
	clear_bind->setEnabled(true);
	
	QList<QTreeWidgetItem *> sel_items = tree->selectedItems ();
	if (sel_items.empty()) {
		set_bind->setEnabled(false);
		clear_bind->setEnabled(false);
		
		return;
	}
			
	QTreeWidgetItem * current=*sel_items.begin();
	

	KeybindItem *ki = dynamic_cast<KeybindItem*>(current);
	ERR_FAIL_COND(!ki);
	
	selected_idx=ki->bind_idx;;
	
}


void SettingsKeys::clear_item() {
	
	Keyboard_Input::Bind bind=Keyboard_Input::get_singleton_instance()->get_key_bind(selected_idx);
	QString name=QStrify(bind.caption_name);
	
	QWidget *parent=this;
	while (parent->parentWidget()!=NULL) {
		parent=parent->parentWidget();
	}
	
	if (QMessageBox::question ( parent, "Clear?", "Clear: "+name+" ?", QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes) {
		
		Keyboard_Input::get_singleton_instance()->set_key_bind_key(selected_idx,Keyboard_Input::NO_KEY);
		update_keybinds();
	}
	
}

class GetBind : public QDialog {
	
	int bind;
	
	QLabel *bind_show;
	
	bool event(QEvent *ev) {
		
		if (ev->type()!=QEvent::KeyPress)
			return QDialog::event(ev);
			
		
		QKeyEvent * e=(QKeyEvent*)ev;
			
			
		int key_value=e->key();
		
		if (key_value==Qt::Key_Backtab || key_value==Qt::Key_Alt || key_value==Qt::Key_Shift || key_value==Qt::Key_Control || key_value==Qt::Key_Meta || key_value==Qt::Key_Escape) 
			return QDialog::event(ev);
		
		bool alt=e->modifiers() & Qt::AltModifier;
		bool shift=e->modifiers() & Qt::ShiftModifier;
		bool control=e->modifiers() & Qt::ControlModifier;
	
		key_value|= (alt?Qt::ALT:0);
		key_value|= (shift?Qt::SHIFT:0);
		key_value|= (control?Qt::CTRL:0);
		
		
		bind=key_value;
		update_bind_show();
		e->accept();
	}
	
	void update_bind_show() { if (bind==Keyboard_Input::NO_KEY) bind_show->setText("Undefined"); else bind_show->setText((QString)QKeySequence(bind)); }
public:	
	
	int get_bind() { return bind; };
	GetBind(QWidget *p_parent,int p_current,QString p_name) : QDialog(p_parent) {
		
		setLayout( new QVBoxLayout(this) );
		bind=p_current;
		
		layout()->addWidget(new QLabel("Key For \""+p_name+"\":"));
		bind_show = new QLabel(this);
		QFont bsfont;
		bsfont.setBold(true);
		bind_show->setFont(bsfont);
		bind_show->setAlignment(Qt::AlignHCenter);
		layout()->addWidget(bind_show);
		setWindowTitle("Press Key(s)");
		QWidget *pb = new QPushButton("Change",this);
		layout()->addWidget(pb);
		QObject::connect(pb,SIGNAL(clicked()),this,SLOT(accept()));
		pb->setFocusPolicy(Qt::NoFocus);
		update_bind_show();
	}
};


void SettingsKeys::set_item() {
	
	
	Keyboard_Input::Bind bind=Keyboard_Input::get_singleton_instance()->get_key_bind(selected_idx);
	QString name=QStrify(bind.caption_name);
	
	QWidget *parent=this;
	while (parent->parentWidget()!=NULL) {
		parent=parent->parentWidget();
	}
	
	GetBind *get_bind = new GetBind(parent,bind.key,name);
	
	if (get_bind->exec()==QDialog::Accepted) {
		
		int new_bind=get_bind->get_bind();
		QString new_bind_text=(QString)QKeySequence(new_bind);
		
		int in_use_by=Keyboard_Input::get_singleton_instance()->get_key_bind_index_from_key(new_bind);
		
		if (in_use_by!=Keyboard_Input::NO_KEY && in_use_by!=selected_idx) {
			
			
			
			QString owner_name=QStrify(Keyboard_Input::get_singleton_instance()->get_key_bind(in_use_by).caption_name);
			
			if (Keyboard_Input::get_singleton_instance()->get_key_bind(in_use_by).readonly) {
				
				QMessageBox::warning ( parent, "Invalid", "Keybind \""+new_bind_text+"\"\nis in use by \""+owner_name+"\".\nIt can't be modified.", QMessageBox::Ok, QMessageBox::NoButton);
				delete get_bind;
				return;
				
				
			} else {
			
				if (QMessageBox::question ( parent, "Change?", "Keybind \""+new_bind_text+"\"\nis in use by action: \"" +owner_name+"\".\nDo you wish to replace it?", QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes) {
		
					Keyboard_Input::get_singleton_instance()->set_key_bind_key(in_use_by,Keyboard_Input::NO_KEY);
				} else {
					delete get_bind;
					return;
					
				}
			}
			
		}
		
		Keyboard_Input::get_singleton_instance()->set_key_bind_key(selected_idx,new_bind);
		

	}
	
	delete get_bind;
	update_keybinds();
}


SettingsKeys::SettingsKeys(QWidget *p_parent) : CVBox(p_parent) {
	
	
	QMap<QString,QString> path_translation;
	path_translation["cursor"]="Cursor";
	path_translation["editor"]="Editor";
	path_translation["global"]="Global";
	path_translation["note_entry"]="Note Entry";
	path_translation["actions"]="Actions";
	
	layout()->setMargin(10);
	layout()->setSpacing(4);
	
	tree = new QTreeWidget(this);
	
	tree->setColumnCount(2);
	QStringList strlist;
	strlist << "Action" << "Keybind";
	tree->setHeaderLabels(strlist);

	tree->header()->setResizeMode(0,QHeaderView::Stretch);
	
	QMap<QString,QTreeWidgetItem*> section_map;
	
	for (int i=0;i<Keyboard_Input::get_singleton_instance()->get_key_bind_count();i++) {
		
		Keyboard_Input::Bind bind=Keyboard_Input::get_singleton_instance()->get_key_bind(i);
		
		QString name=QStrify(bind.name);
		QString path;
		
		if (name.indexOf("/")!=-1)
			path=name.left(name.indexOf("/"));
		else
			path="cursor";
		
		printf("item %s - path %s\n",name.toAscii().data(),path.toAscii().data());
		QMap<QString,QTreeWidgetItem*>::iterator I = section_map.find(path);
		if (I==section_map.end()) {
			
			section_map[path]=new QTreeWidgetItem(tree);
			section_map[path]->setText(0,path_translation[path]);
			section_map[path]->setFlags(Qt::ItemIsEnabled);
			I = section_map.find(path);
			
		}
		
		items.push_back(new KeybindItem( I.value(), i ));
	
	}
	
	
	CHBox *hb = new CHBox(this);
	hb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	
	set_bind = new QPushButton("Set Keybind",hb);
	set_bind->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	
	clear_bind = new QPushButton("Clear Keybind",hb);
	clear_bind->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	
	set_bind->setEnabled(false);
	clear_bind->setEnabled(false);
	
	QObject::connect(set_bind,SIGNAL(clicked()),this,SLOT(set_item()));
	QObject::connect(clear_bind,SIGNAL(clicked()),this,SLOT(clear_item()));
	
	QObject::connect(tree,SIGNAL(itemSelectionChanged()),this,SLOT(itemSelectionChangedSlot()));
	
	selected_idx=0;
}


SettingsKeys::~SettingsKeys()
{
}


}
