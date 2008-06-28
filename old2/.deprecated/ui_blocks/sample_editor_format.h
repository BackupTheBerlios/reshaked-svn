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

#ifndef SAMPLE_EDITOR_FORMAT_H
#define SAMPLE_EDITOR_FORMAT_H


#include <Qt/qlabel.h>
#include <Qt/qlineedit.h>
#include <Qt/qcombobox.h>
#include <Qt/qspinbox.h>
#include "dsp/sample.h"
#include "ui_blocks/sample_viewer.h"
#include "ui_blocks/helpers.h"

/**
 *
 * Juan Linietsky
 **/
 
namespace ReShaked {
 
class SampleEditorFormat : public CVBox {

	Q_OBJECT

	bool updating;
	QLabel * label_size;
	QLabel * label_chans;
	QLabel * label_base_freq;

	QLabel * label_loop_type;
	QLabel * label_loop_begin;
	QLabel * label_loop_end;

	QLineEdit *entry_size;
	QLineEdit *entry_chans;

  	QComboBox* combo_loop_type;
	QSpinBox*spin_loop_begin;
	QSpinBox *spin_loop_end;
	QSpinBox *spin_base_freq;

	Sample *sample_data;
	SampleViewer *sample_viewer;

	void update_format();
	void update_loop();
protected slots:

	void loop_begin_changed_cbk(int p_idx);
	void loop_end_changed_cbk(int p_idx);
	void loop_type_changed_cbk(int p_idx);

	void base_freq_changed(int p_frq);

	void transpose_up_1_4_cbk();
	void transpose_up_1_2_cbk();
	void transpose_up_1_cbk();
	void transpose_up_12_cbk();

	void transpose_down_1_4_cbk();
	void transpose_down_1_2_cbk();
	void transpose_down_1_cbk();
	void transpose_down_12_cbk();

signals:

	void sample_base_freq_changed();
	void sample_modified();


public:




	void set_sample_data(Sample *p_sample_data);
	void set_sample_viewer(SampleViewer *p_sample_viewer);
	SampleEditorFormat(QWidget *p_grid);

	~SampleEditorFormat();
};


 }
#endif
