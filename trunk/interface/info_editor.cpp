//
// C++ Implementation: info_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "info_editor.h"
#include <Qt/qlabel.h>
#include <Qt/qlayout.h>
#include <Qt/qfont.h>
#include "ui_blocks/helpers.h"

namespace ReShaked {


QString InfoEditor::get_title() {
	
	return title->text();
}
QString InfoEditor::get_author() {
	
	return author->text();
	
}
QString InfoEditor::get_notes() {
	
	return notes->toPlainText();
	
}

InfoEditor::InfoEditor(QWidget *p_parent,Song *p_song) : QDialog(p_parent)
{
	
	setLayout( new QVBoxLayout(this ) );
	
	CHBox *hb;
	QLabel *label;
	QFont font;
	
	label = new QLabel("Song Title:",this);
	font=label->font();
	font.setBold(true);
	label->setFont(font);
	layout()->addWidget(label);
	
	hb = new CHBox(this);
	layout()->addWidget(hb);
	(new QFrame(hb))->setFixedWidth(20);
	
	title = new QLineEdit(QStrify(p_song->get_info_title() ), hb );
	
	label = new QLabel("Author:",this);
	font=label->font();
	font.setBold(true);
	label->setFont(font);
	layout()->addWidget(label);
	
	hb = new CHBox(this);
	layout()->addWidget(hb);
	(new QFrame(hb))->setFixedWidth(20);
	
	author = new QLineEdit(QStrify(p_song->get_info_author() ), hb );
	
	label = new QLabel("Notes:",this);
	font=label->font();
	font.setBold(true);
	label->setFont(font);
	layout()->addWidget(label);
	
	hb = new CHBox(this);
	layout()->addWidget(hb);
	(new QFrame(hb))->setFixedWidth(20);
	
	notes = new QTextEdit(QStrify(p_song->get_info_notes() ), hb );
	
	setMinimumSize(500,300);
	
}


InfoEditor::~InfoEditor()
{
}


}
