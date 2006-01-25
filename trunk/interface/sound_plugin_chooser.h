//
// C++ Interface: sound_plugin_chooser
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSOUND_PLUGIN_CHOOSER_H
#define RESHAKEDSOUND_PLUGIN_CHOOSER_H


#include <Qt/qscrollarea.h>
#include <Qt/qdialog.h>
#include <Qt/qpixmap.h>
#include <Qt/qcheckbox.h>

namespace ReShaked {

/**
	@author red <red@killy>
*/


class SoundPluginChooserItem : public QWidget {
	
	Q_OBJECT
	
			
	int index;
	bool selected;
	
	void mousePressEvent(QMouseEvent *e);
	void paintEvent(QPaintEvent *e);
	
	QPixmap px;
	QString title;
	QString description;
	
signals:
			
	void selected_signal(SoundPluginChooserItem *);
public:	
	
	void set_selected(bool p_selected);
	
	int get_index();	
	
	SoundPluginChooserItem(QWidget *p_parent,QPixmap p_icon, QString p_title, QString p_description,QString p_text,int p_index);
	
};

class SoundPluginChooser : public QDialog {

	Q_OBJECT
			    
			
			
	
			
	QScrollArea *scroll;
	QCheckBox *append;
	
	
	std::vector<SoundPluginChooserItem*> items;
			
	int selected_idx;
	virtual void accept();		

public slots:	
	
	void selected_slot(SoundPluginChooserItem * p_item);
public:
	int get_selected_plugin_idx();
	bool append_to_output();
	
	SoundPluginChooser(QWidget *p_parent, bool p_show_synths);
	~SoundPluginChooser();

};

}

#endif
