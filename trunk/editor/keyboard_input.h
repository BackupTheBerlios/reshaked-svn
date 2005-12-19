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
// C++ Interface: $MODULE$
//
// Description:
//
//
// Author: Juan Linietsky <coding@reduz.com.ar>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KEYBOARD_INPUT_H
#define KEYBOARD_INPUT_H

#include "types/typedefs.h"

#include <vector>
	

/**
@author Juan Linietsky
*/


namespace ReShaked {

#define KEYBIND(m_str) Keyboard_Input::get_singleton_instance()->get_key_bind_by_name(m_str)

#define IS_KEYBIND(m_str,m_key) \
	(Keyboard_Input::get_singleton_instance()->get_key_bind_by_name(m_str)==m_key)

class Keyboard_Input {
public:
	
	
	enum {
		
		NO_KEY=-1
	};
	class External {
	public:
					
		virtual void set_bind(int p_key)=0;
		
		virtual ~External() {}
	};	
	
	static Keyboard_Input *singleton_instance;

	struct Bind {

		External *external;
		bool readonly;
		String category;
		String name;
		String caption_name;
		int key; // ui will take care of it
	};

	int number_key[10];

	std::vector<Bind> bind_list;

public:

	//	void load_settings(ConfigHandler *p_handler);
	//void save_settings(ConfigHandler *p_handler);

	static Keyboard_Input* get_singleton_instance();

	void add_key_bind(String p_name,String p_caption_name,int p_key=NO_KEY,bool p_readonly=false,External *p_external=NULL);


	int get_key_bind_count();

	const Bind& get_key_bind(int p_idx);
        void set_key_bind_key(int p_idx,int p_key);
	void set_key_bind_key_by_name(String p_name,int p_key);

	int get_key_bind_by_name(String p_name);
	int get_key_bind_key(int p_idx);
	int get_key_bind_key_nocase(int p_idx);

	bool is_number(int p_key);
	int get_number(int p_key);

	void set_number(int p_number,int p_key);
	Keyboard_Input();
	~Keyboard_Input();

};

}
#endif
