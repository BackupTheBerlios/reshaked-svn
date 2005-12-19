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
// C++ Implementation: $MODULE$
//
// Description:
//
//
// Author: Juan Linietsky <coding@reduz.com.ar>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include "keyboard_input.h"

namespace ReShaked {

Keyboard_Input *Keyboard_Input::singleton_instance=NULL;

void Keyboard_Input::add_key_bind(String p_name,String p_caption_name,int p_key,bool p_readonly,External *p_external) {

	Bind key_bind;

	key_bind.name=p_name;
	key_bind.caption_name=p_caption_name;
	key_bind.key=p_key;
	key_bind.readonly=p_readonly;
	key_bind.external=p_external;
	bind_list.push_back(key_bind);
}

int Keyboard_Input::get_key_bind_count() {

	return bind_list.size();
}

const Keyboard_Input::Bind& Keyboard_Input::get_key_bind(int p_idx) {

	static Bind nil_bind;
	if ((p_idx<0) || (p_idx>=bind_list.size()))
		return nil_bind;

	return bind_list[p_idx];


}
void Keyboard_Input::set_key_bind_key(int p_idx,int p_key) {
	if ((p_idx<0) || (p_idx>=bind_list.size()))
		return ;

	bind_list[p_idx].key=p_key;
	if (bind_list[p_idx].external)
		bind_list[p_idx].external->set_bind(p_key);

}

void Keyboard_Input::set_key_bind_key_by_name(String p_name,int p_key) {
	
	for (int i=0;i<bind_list.size();i++) {
		if (bind_list[i].name==p_name) {
			set_key_bind_key(i,p_key);
		}
	}
	
}
int Keyboard_Input::get_key_bind_key(int p_idx) {
	if ((p_idx<0) || (p_idx>=bind_list.size()))
		return -1;

	return bind_list[p_idx].key;

}


int Keyboard_Input::get_key_bind_by_name(String p_name) {

	for (int i=0;i<bind_list.size();i++) {
		if (bind_list[i].name==p_name) {
			return bind_list[i].key;

		}
	}

	return -1;
}
int Keyboard_Input::get_key_bind_key_nocase(int p_idx) {

	if ((p_idx<0) || (p_idx>=bind_list.size()))
		return -1;

	int key=bind_list[p_idx].key;

	if ( (key>='A') && (key<='Z') ) key+='a'-'A';

	return key;
}

Keyboard_Input* Keyboard_Input::get_singleton_instance() {

	return singleton_instance;
}

/*
void Keyboard_Input::load_settings(ConfigHandler *p_handler) {

	for (int i=0;i<bind_list.size();i++) {

		if (p_handler->property_exists("Tracker Keys",bind_list[i].name))
			bind_list[i].key=p_handler->get_int_property("Tracker Keys",bind_list[i].name);
	}
}

void Keyboard_Input::save_settings(ConfigHandler *p_handler) {

	p_handler->add_section("Tracker Keys");

	for (int i=0;i<bind_list.size();i++) {

		p_handler->add_property("Tracker Keys",bind_list[i].name,(int)bind_list[i].key);
	}


}
  */

bool Keyboard_Input::is_number(int p_key) {

	for (int i=0;i<10;i++)
		if (number_key[i]==p_key)
			return true;
	return false;
}
int Keyboard_Input::get_number(int p_key) {

	for (int i=0;i<10;i++)
		if (number_key[i]==p_key)
			return i;
	return -1;
}

void Keyboard_Input::set_number(int p_number,int p_key) {

	if (p_number<0 || p_number>=10)
		return;
	number_key[p_number]=p_key;
}

Keyboard_Input::Keyboard_Input() {

	if (singleton_instance!=NULL) {
		//printf("Error! Keyboard Input defined more than once! this is a singleton!");
		return;
	}

	singleton_instance=this;
	for (int i=0;i<10;i++)
		number_key[i]=-1;
}


Keyboard_Input::~Keyboard_Input() {
	
	/* delete externals */
	for (int i=0;i<bind_list.size();i++) 
		if (bind_list[i].external)
			delete bind_list[i].external;
}


}
