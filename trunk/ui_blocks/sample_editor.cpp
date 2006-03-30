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
#include "sample_editor.h"
#include <Qt/qgroupbox.h>

namespace ReShaked {
/*
void SampleEditorDialog::add_edit_effect(Edit_Effect *p_edit_effect) {


	sample_editor_effects->add_edit_effect(p_edit_effect);
}
*/
void SampleEditorDialog::data_modification_begin_notify_cbk() {


}

void SampleEditorDialog::data_modification_end_notify_cbk() {

	sample_viewer->set_zoom_offset( sample_viewer->get_zoom_offset() );
	sample_viewer->sample_changed_notification();
	sample_viewer_zoom->update_zoom_bar();
	sample_editor_format->set_sample_data(sample_data);

}


void SampleEditorDialog::set_sample_data(Sample *p_sample_data) {
	sample_viewer->set_sample_data(p_sample_data);
	sample_viewer_zoom->set_sample_data(p_sample_data);
	sample_editor_format->set_sample_data(p_sample_data);
	sample_editor_clipboard->set_sample_data(p_sample_data);
	//sample_editor_effects->set_sample_data(p_sample_data);

	sample_data=p_sample_data;
	selection.clear();

}

void SampleEditorDialog::destructive_operation_end_cbk() {

	if (sample_data && sample_data->get_length()) {
		if (selection.get_point_1()>=sample_data->get_length()) {

			selection.set_point_1(sample_data->get_length()-1);
		}
		if (selection.get_point_2()>=sample_data->get_length()) {

			selection.set_point_2(sample_data->get_length()-1);
		}
		if (selection.get_point_2()==selection.get_point_1()) {
			selection.clear();
		}
	} else {
		selection.clear();
	}

	//sample_editor_clipboard->set_sample_data(sample_data);
	sample_viewer->set_zoom_offset( sample_viewer->get_zoom_offset() );
	sample_viewer->sample_changed_notification();
//	sample_editor_effects->set_sample_data(sample_data); //if destructive, sorry!

	sample_viewer_zoom->update_zoom_bar();

	sample_editor_format->set_sample_data(sample_data);


	destructive_operation_end();
}

SampleEditorDialog::SampleEditorDialog(QWidget *p_parent,Sample *p_sample_data) : QDialog(p_parent)   {

	QVBoxLayout *vbl=new QVBoxLayout(this);
	setLayout(vbl);
	
	sample_data=NULL;

	
	sample_viewer = new SampleViewer(this);
	layout()->addWidget(sample_viewer);
	sample_viewer->setMinimumHeight(200);	

	sample_viewer_zoom = new SampleViewerZoom(this);

	sample_viewer_zoom->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	
	layout()->addWidget(sample_viewer_zoom);
	
	//sample_viewer_zoom->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
	sample_viewer_zoom->set_sample_viewer(sample_viewer);

	tabs = new QTabWidget(this);
	tabs->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	layout()->addWidget(tabs);
	
//	setStretchFactor(tabs,2);


	CVBox *auxvb2 = new CVBox(tabs);
	auxvb2->layout()->setMargin(5);
	CHBox *auxvb = new CHBox(auxvb2);
	auxvb->layout()->setMargin(5);
	sample_editor_format=new SampleEditorFormat(auxvb);

	sample_editor_format->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Maximum));
	sample_editor_format->set_sample_viewer(sample_viewer);
	sample_viewer->set_selection(&selection);
	tabs->addTab(auxvb2,"Format");
	QObject::connect(sample_editor_format,SIGNAL(sample_base_freq_changed()),this,SIGNAL(sample_freq_changed()));
	QObject::connect(sample_editor_format,SIGNAL(sample_modified()),this,SIGNAL(sample_modified()));


	CVBox *auxvb2_2 = new CVBox(tabs);
	auxvb2_2->layout()->setMargin(5);
	CVBox *auxvb_2 = new CVBox(auxvb2_2);
	auxvb_2->layout()->setMargin(5);

	sample_editor_clipboard = new SampleEditorClipboard(auxvb_2);
	sample_editor_clipboard->set_selection(&selection);
	tabs->addTab(auxvb2_2,"Edit");
	sample_editor_clipboard->layout()->setMargin(5);

	sample_editor_clipboard->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Maximum));

	QObject::connect(sample_editor_clipboard,SIGNAL(destructive_operation_begin()),this,SIGNAL(destructive_operation_begin()));
	QObject::connect(sample_editor_clipboard,SIGNAL(destructive_operation_end()),this,SLOT(destructive_operation_end_cbk()));
	QObject::connect(sample_editor_clipboard,SIGNAL(sample_modified()),this,SIGNAL(sample_modified()));

	/*
	sample_editor_effects = new SampleEditorDialog_Effects(tabs);
	sample_editor_effects->setMargin(5);
	tabs->addTab(sample_editor_effects,"Effects");
	sample_editor_effects->set_selection(&selection);

	
	QObject::connect(sample_editor_effects,SIGNAL(data_modification_begin_notify()),this,SLOT(data_modification_begin_notify_cbk()));
	QObject::connect(sample_editor_effects,SIGNAL(data_modification_end_notify()),this,SLOT(data_modification_end_notify_cbk()));

	QObject::connect(sample_editor_effects,SIGNAL(destructive_operation_begin()),this,SIGNAL(destructive_operation_begin()));
	QObject::connect(sample_editor_effects,SIGNAL(destructive_operation_end()),this,SLOT(destructive_operation_end_cbk()));
	QObject::connect(sample_editor_effects,SIGNAL(sample_modified()),this,SIGNAL(sample_modified()));
	*/
	/*
	add_edit_effect(&edit_effects.sel_to_loop);
	add_edit_effect(&edit_effects.fade_in);
	add_edit_effect(&edit_effects.fade_out);
	add_edit_effect(&edit_effects.reverse);
	add_edit_effect(&edit_effects.center);
	add_edit_effect(&edit_effects.normalize);
	add_edit_effect(&edit_effects.pre_cut);
	add_edit_effect(&edit_effects.post_cut);
	add_edit_effect(&edit_effects.tunner);
	add_edit_effect(&edit_effects.toggle_sign);
	add_edit_effect(&edit_effects.toggle_depth);
	*/
	
	set_sample_data( p_sample_data);
	
}

SampleEditorDialog::~SampleEditorDialog() {
	
}

}
