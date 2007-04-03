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
#include <Qt/qcombobox.h>
#include <Qt/qtimer.h>
#include "ui_blocks/helpers.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/


class SoundPluginChooserItem : public QWidget {
	
	Q_OBJECT
	
			
	int index;
	bool selected;
	bool draw_separator;
	void mousePressEvent(QMouseEvent *e);
	void paintEvent(QPaintEvent *e);
	
	QPixmap px;
	QString title;
	QString description;
	void keyPressEvent ( QKeyEvent * event );	
	
signals:
			
	void char_press_event_signal(QChar p_char);
	void selected_signal(SoundPluginChooserItem *);
public:	
	
	QString get_title();
	
	void set_selected(bool p_selected);
	
	int get_index();	
	
	SoundPluginChooserItem(QWidget *p_parent,QPixmap p_icon, QString p_title, QString p_description,QString p_text,int p_index,bool p_draw_separator=true);
	
};

class SoundPluginChooser : public QDialog {

	Q_OBJECT
			    
			
			
	
	CVBox *vb;
			
	QScrollArea *scroll;
	QCheckBox *append;
	QComboBox *channels;
	
	std::vector<SoundPluginChooserItem*> items;
			
	int selected_idx;
	virtual void accept();		

	int track_channels;
	
	QString current_search;
	
	QTimer *keyb_search_timeout;
	
	void update_current_search();
	void ensure_selected_visible();
	
public slots:	
	
	void char_press_event_slot(QChar p_char);
	void selected_slot(SoundPluginChooserItem * p_item);
	void kb_search_timeout_slot();
public:
	int get_selected_channels();
	int get_selected_plugin_idx();
	bool append_to_output();
	
	SoundPluginChooser(QWidget *p_parent, bool p_show_synths,int p_track_channels);
	~SoundPluginChooser();

};

}

#endif
