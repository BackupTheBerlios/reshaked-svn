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

#ifndef SAMPLE_EDITOR_H
#define SAMPLE_EDITOR_H


#include "ui_blocks/helpers.h"
#include "ui_blocks/sample_viewer_zoom.h"
#include "ui_blocks/sample_editor_format.h"
#include "ui_blocks/sample_editor_clipboard.h"
#include "ui_blocks/sample_editor_effects.h"
#include <Qt/qpushbutton.h>
#include <Qt/qpixmap.h>
#include <Qt/qlabel.h>
#include <Qt/qscrollbar.h>
#include <Qt/qslider.h>
#include <Qt/qtabwidget.h>
#include <Qt/qdialog.h>

//#include "plugins/edit_effects/simple_edit_effects.h"
namespace ReShaked {

/**
 *
 * Juan Linietsky
 **/
class SampleEditorDialog : public QDialog {

	Q_OBJECT


	SampleEditorDialog();

	SampleViewer * sample_viewer;
	CHBox *format_box;
	Sample *sample_data;
	SampleViewerZoom * sample_viewer_zoom;
	SampleEditorFormat *sample_editor_format;
	QTabWidget *tabs;

	SampleEditorClipboard *sample_editor_clipboard;
	//SampleEditorDialog_Effects *sample_editor_effects;

	Selection selection;

	/*
	struct Edit_Effects {

		Edit_Effect_Reverse reverse;
		Edit_Effect_PreLoop_Cut pre_cut;
		Edit_Effect_PostLoop_Cut post_cut;
		Edit_Effect_SelToLoop sel_to_loop;
		Edit_Effect_Center center;
		Edit_Effect_Normalize normalize;
		Edit_Effect_Toggle_Sign toggle_sign;
		Edit_Effect_Toggle_Depth toggle_depth;
		Edit_Effect_Tunner tunner;

		Edit_Effect_FadeIn fade_in;
		Edit_Effect_FadeOut fade_out;

	} edit_effects;
	*/

protected slots:

	void destructive_operation_end_cbk();
	void data_modification_begin_notify_cbk();
	void data_modification_end_notify_cbk();

signals:
	void destructive_operation_begin();
	void destructive_operation_end();
	void sample_freq_changed();
	void sample_modified();
public:

	//void add_edit_effect(Edit_Effect *p_edit_effect);

	SampleViewer* get_sample_viewer() { return sample_viewer; }
	void set_sample_data(Sample *p_sample_data);
	QTabWidget *get_tabs() { return tabs; }

	SampleEditorDialog(QWidget *p_parent,Sample *p_sample_data);
	~SampleEditorDialog();
};




}
#endif

