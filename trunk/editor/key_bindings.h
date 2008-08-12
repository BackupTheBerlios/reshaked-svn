//
// C++ Interface: key_bindings
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KEY_BINDINGS_H
#define KEY_BINDINGS_H

#include "typedefs.h"
#include "rstring.h"

#include <vector>
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/


#define KEYBIND(m_str) KeyBindings::get_singleton_instance()->get_key_bind_by_name(m_str)

#define IS_KEYBIND(m_str,m_key) \
	(KeyBindings::get_singleton_instance()->get_key_bind_by_name(m_str)==m_key)

#define IS_NUMBIND(m_number,m_key) \
	(KeyBindings::get_singleton_instance()->get_number(m_key)==m_number)

class KeyBindings {
public:
	
	
	enum {
		
		NO_KEY=-1
	};
	class External {
	public:
					
		virtual void set_bind(int p_key)=0;
		
		virtual ~External() {}
	};	
	
	class ExternalDescription {
		public:
					
			virtual void set_description(String p_descr){}
			virtual void set_bind(int p_key) {};
		
		virtual ~ExternalDescription() {}
	};	
	
	static KeyBindings *singleton_instance;

	struct Bind {

		External *external;
		ExternalDescription *external_description;
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

	static KeyBindings* get_singleton_instance();

	void add_key_bind(String p_name,String p_caption_name,int p_key=NO_KEY,bool p_readonly=false,External *p_external=NULL,ExternalDescription *p_external_descr=NULL);


	int get_key_bind_count();

	const Bind& get_key_bind(int p_idx);
        void set_key_bind_key(int p_idx,int p_key);
	void set_key_bind_key_by_name(String p_name,int p_key);

	int get_key_bind_by_name(String p_name);
	int get_key_bind_key(int p_idx);
	int get_key_bind_key_nocase(int p_idx);
	int get_key_bind_index_from_key(int p_key);

	bool is_number(int p_key);
	int get_number(int p_key);

	void set_number(int p_number,int p_key);
	KeyBindings();
	~KeyBindings();

};

#endif
