//
// C++ Interface: ins_device_loader
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDINS_DEVICE_LOADER_H
#define RESHAKEDINS_DEVICE_LOADER_H

#include <map>
#include <string>
#include <vector>
#include "engine/midi_device_description.h"
namespace ReShaked {

/**
	@author red <red@killy>
*/
class InsDeviceLoader{
	
	struct Pair {

		std::string key;
		std::string value;
	};

	typedef std::vector<Pair> Property_List;

	
	struct SubSection {

		Property_List property_list;
	};

	typedef std::map<std::string,SubSection> SubSection_List;
	

	
	struct Section {

		SubSection_List subsection_list;		
	};

	typedef std::map<std::string,Section> Section_List;
	
	Section_List section_list;

	void remove_trailing_space(std::string *p_string);

	MidiDeviceDescription *desc;
	
	void load_bank(int p_MSB,int p_LSB,std::string p_name);
	Property_List* get_property_list(std::string p_section,std::string p_subsection);
	
	
public:
	
	int get_device_count();
	std::string get_device_name(int p_device);
	
	bool load(const char *p_filename);
	void set_to_device_description(int p_device,MidiDeviceDescription *p_desc);
	InsDeviceLoader();
	~InsDeviceLoader();

};

}

#endif
