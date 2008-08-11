
//
// C++ Implementation: key_bindings
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "key_bindings.h"


KeyBindings *KeyBindings::singleton_instance=NULL;

void KeyBindings::add_key_bind(String p_name,String p_caption_name,int p_key,bool p_readonly,External *p_external,ExternalDescription *p_external_descr) {

	Bind key_bind;

	key_bind.name=p_name;
	key_bind.caption_name=p_caption_name;
	key_bind.key=p_key;
	key_bind.readonly=p_readonly;
	key_bind.external=p_external;
	key_bind.external_description=p_external_descr;
	if (p_external) {
		p_external->set_bind( p_key );
	}
	if (p_external_descr) {
		
		p_external_descr->set_bind( p_key );
		p_external_descr->set_description( p_caption_name );
		
	}
	bind_list.push_back(key_bind);
}

int KeyBindings::get_key_bind_count() {

	return bind_list.size();
}

const KeyBindings::Bind& KeyBindings::get_key_bind(int p_idx) {

	static Bind nil_bind;
	if ((p_idx<0) || (p_idx>=bind_list.size()))
		return nil_bind;

	return bind_list[p_idx];


}
void KeyBindings::set_key_bind_key(int p_idx,int p_key) {
	if ((p_idx<0) || (p_idx>=bind_list.size()))
		return ;

	bind_list[p_idx].key=p_key;
	if (bind_list[p_idx].external)
		bind_list[p_idx].external->set_bind(p_key);

	if (bind_list[p_idx].external_description) 
		bind_list[p_idx].external_description->set_bind(p_key);

}

void KeyBindings::set_key_bind_key_by_name(String p_name,int p_key) {
	
	for (int i=0;i<bind_list.size();i++) {
		if (bind_list[i].name==p_name) {
			set_key_bind_key(i,p_key);
		}
	}
	
}
int KeyBindings::get_key_bind_key(int p_idx) {
	if ((p_idx<0) || (p_idx>=bind_list.size()))
		return -1;

	return bind_list[p_idx].key;

}


int KeyBindings::get_key_bind_by_name(String p_name) {

	for (int i=0;i<bind_list.size();i++) {
		if (bind_list[i].name==p_name) {
			return bind_list[i].key;

		}
	}

	return -1;
}
int KeyBindings::get_key_bind_key_nocase(int p_idx) {

	if ((p_idx<0) || (p_idx>=bind_list.size()))
		return -1;

	int key=bind_list[p_idx].key;

	if ( (key>='A') && (key<='Z') ) key+='a'-'A';

	return key;
}

KeyBindings* KeyBindings::get_singleton_instance() {

	return singleton_instance;
}

/*
void KeyBindings::load_settings(ConfigHandler *p_handler) {

	for (int i=0;i<bind_list.size();i++) {

		if (p_handler->property_exists("Tracker Keys",bind_list[i].name))
			bind_list[i].key=p_handler->get_int_property("Tracker Keys",bind_list[i].name);
	}
}

void KeyBindings::save_settings(ConfigHandler *p_handler) {

	p_handler->add_section("Tracker Keys");

	for (int i=0;i<bind_list.size();i++) {

		p_handler->add_property("Tracker Keys",bind_list[i].name,(int)bind_list[i].key);
	}


}
  */

bool KeyBindings::is_number(int p_key) {

	for (int i=0;i<10;i++)
		if (number_key[i]==p_key)
			return true;
	return false;
}
int KeyBindings::get_number(int p_key) {

	for (int i=0;i<10;i++)
		if (number_key[i]==p_key)
			return i;
	return -1;
}


int KeyBindings::get_key_bind_index_from_key(int p_key) {
	
	for (int i=0;i<get_key_bind_count();i++)
		if (bind_list[i].key==p_key)
			return i;
	
	return NO_KEY;
	
	
}

void KeyBindings::set_number(int p_number,int p_key) {

	if (p_number<0 || p_number>=10)
		return;
	number_key[p_number]=p_key;
}

KeyBindings::KeyBindings() {

	if (singleton_instance!=NULL) {
		//printf("Error! Keyboard Input defined more than once! this is a singleton!");
		return;
	}

	singleton_instance=this;
	for (int i=0;i<10;i++)
		number_key[i]=-1;
}


KeyBindings::~KeyBindings() {
	
	/* delete externals */
	for (int i=0;i<bind_list.size();i++) {
		if (bind_list[i].external)
			delete bind_list[i].external;
		if (bind_list[i].external_description)
			delete bind_list[i].external_description;
	}
}
