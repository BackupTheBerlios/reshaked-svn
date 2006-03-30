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
// C++ Interface: sample_editor_clipboard
//
// Description: 
//
//
// Author: Juan Linietsky <coding@reduz.com.ar>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef SAMPLE_EDITOR_CLIPBOARD_H_
#define SAMPLE_EDITOR_CLIPBOARD_H_

#include "dsp/selection.h"
#include "dsp/sample.h"
#include "ui_blocks/helpers.h"

#include <Qt/qpushbutton.h>
#include <Qt/qlabel.h>

#include <vector>

namespace ReShaked {

class SampleEditorClipboard : public CHBox {

	Q_OBJECT

	Selection *selection;
	Sample *sdata;
	std::vector<FrameData> clipboard;

        QPushButton *copy;
        QPushButton *cut;
        QPushButton *paste;


private slots:

	void copy_cbk();
	void cut_cbk();
	void paste_cbk();

signals:

	void destructive_operation_begin();
	void destructive_operation_end();
	void sample_modified();

public:

	void set_selection(Selection *p_selection) { selection=p_selection; };
	void set_sample_data(Sample *p_sdata) { sdata=p_sdata; }

	void clear_clipboard();

	SampleEditorClipboard(QWidget *p_parent);
	~SampleEditorClipboard();

};

}

#endif

