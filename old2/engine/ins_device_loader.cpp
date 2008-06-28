//
// C++ Implementation: ins_device_loader
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ins_device_loader.h"
#include <stdio.h>
#include "error_macros.h"
namespace ReShaked {


void InsDeviceLoader::remove_trailing_space(std::string *p_string) {

	while ((p_string->length()>0) && ((*p_string)[0]<33)) {

		*p_string=p_string->substr(1,p_string->length()-1);
	}

	while ((p_string->length()>0) && ((*p_string)[p_string->length()-1]<33)) {

		*p_string=p_string->substr(0,p_string->length()-1);
	}

}

bool InsDeviceLoader::load(const char *p_filename) {

	std::string subsection="";
	std::string section="";

	FILE *f;

	if ( !(f=fopen(p_filename,"rb")) ) return true;

	while(1) {

		std::string auxstring;
		char axloadzone[500];

		fscanf(f," %[^\n]",axloadzone);

		if (feof(f)) break;

		auxstring=axloadzone;

		auxstring=auxstring.substr(0,auxstring.find(";"));

		if (auxstring.length()==0) continue;

		remove_trailing_space(&auxstring);

		
		if (auxstring[0]=='.') {

			section=auxstring;
			subsection="";
			
		} else if (auxstring[0]=='[') {

			auxstring=auxstring.substr(1,auxstring.length()-2);

			subsection=auxstring;
			
		} else if (auxstring.find("=")<auxstring.length()) {

			std::string key,data;

			key=auxstring.substr(0,auxstring.find("="));

			remove_trailing_space(&key);
			data=auxstring.substr(auxstring.find("=")+1,(auxstring.length()-auxstring.find("="))-1);
		
			remove_trailing_space(&data);
			
			Pair p;
			p.key=key;
			p.value=data;
			
			if (section=="" || subsection=="")
				continue;
			
			//printf("%s : %s - %s = %s\n",section.c_str(),subsection.c_str(),key.c_str(),data.c_str());
			section_list[section].subsection_list[subsection].property_list.push_back(p);

			////printf("got \"%s\":\"%s\"\n",key.c_str(),data.c_str());
		}
	}

	fclose(f);

	return false;
}

int InsDeviceLoader::get_device_count() {
	
	Section_List::iterator sI=section_list.find(".Instrument Definitions");
	if (sI==section_list.end())
		return 0;
	
	return sI->second.subsection_list.size();
}
std::string InsDeviceLoader::get_device_name(int p_device) {
	
	ERR_FAIL_INDEX_V(p_device,get_device_count(),"");
	
	
	Section_List::iterator sI=section_list.find(".Instrument Definitions");
	if (sI==section_list.end())
		return "";
	
	SubSection_List::iterator ssI = sI->second.subsection_list.begin();
	
	for (int i=0;i<get_device_count();i++,ssI++) {
		
		if (i==p_device)
			return ssI->first;
	}
	
	return "";
}

InsDeviceLoader::Property_List* InsDeviceLoader::get_property_list(std::string p_section,std::string p_subsection) {
	
	Section_List::iterator sI=section_list.find(p_section);
	if (sI==section_list.end())
		return NULL;
	SubSection_List::iterator ssI = sI->second.subsection_list.find(p_subsection);
	
	if (ssI==sI->second.subsection_list.end())
		return NULL;
	
	return &ssI->second.property_list;
	
}


void InsDeviceLoader::set_to_device_description(int p_device,MidiDeviceDescription *p_desc) {
	
	
	
	ERR_FAIL_INDEX(p_device,get_device_count());
	
	/* Detect bank select */
	bool found_msb=false;
	bool found_lsb=false;
	bool found_banksel=false;
	
	desc=p_desc;
	p_desc->clear();
	
	Property_List *definition = get_property_list( ".Instrument Definitions", get_device_name( p_device ) );
	
	p_desc->set_name( get_device_name(p_device).c_str() );
	
	for (int i=0;i<definition->size();i++) {
		
		Pair p = (*definition)[i];
		
		if (p.key.find("Patch")!=std::string::npos) {
			/* Patch is a tricky one */
			std::string num=p.key;
			if (num.find("[")==std::string::npos)
				continue;
			num=num.substr(num.find("[")+1,(num.length()-num.find("["))-1);
			if (num.find("]")==std::string::npos)
				continue;
			num=num.substr(0,num.find("]"));
			
			
			int MSB=0,LSB=0;
			if (num=="")
				continue;
			if (num!="*") {
			
				int int_num=atoi( num.c_str() );
				
				MSB=int_num>>7;
				LSB=int_num&0x7F;
				//printf("%s at MSB %i, LSB %i\n",p.value.c_str(),MSB,LSB);
			}
			if (MSB>0)
				found_msb=true;
			if (LSB>0)
				found_lsb=true;
			load_bank(MSB,LSB,p.value);
			
		} if (p.key.find("BankSelMethod")!=std::string::npos) {

			int method=atoi(p.value.c_str());
			switch (method) {
				case 0: desc->set_bank_select_method( MidiDeviceDescription::BANK_SELECT_NONE ); break;
				case 1: desc->set_bank_select_method( MidiDeviceDescription::BANK_SELECT_MSB ); break;
				case 2: desc->set_bank_select_method( MidiDeviceDescription::BANK_SELECT_LSB ); break;				
				case 3: desc->set_bank_select_method( MidiDeviceDescription::BANK_SELECT_MSB_LSB ); break;
			}
			
			found_banksel=true;
		} else if (p.key.find("Control")!=std::string::npos) {
			
			
			Property_List *definition = get_property_list( ".Controller Names", p.value);
	
			if (!definition)
				continue;
			
			for (int i=0;i<definition->size();i++) {
		
				Pair p = (*definition)[i];
		
				int ctrl_idx=atoi(p.key.c_str());
				if (ctrl_idx<0 || ctrl_idx>127)
					continue;
				desc->set_control_name( ctrl_idx, p.value.c_str() );
			}

		}
		
		
	}
	
	if (!found_banksel) { /* Autodetect bank select method */
		
		if (!found_msb && !found_lsb)
			desc->set_bank_select_method( MidiDeviceDescription::BANK_SELECT_NONE );
		else if (found_msb && !found_lsb)
			desc->set_bank_select_method( MidiDeviceDescription::BANK_SELECT_MSB );
		else if (!found_msb && found_lsb)
			desc->set_bank_select_method( MidiDeviceDescription::BANK_SELECT_LSB );
		else
			desc->set_bank_select_method( MidiDeviceDescription::BANK_SELECT_MSB_LSB );
	}
}

void InsDeviceLoader::load_bank(int p_MSB,int p_LSB,std::string p_name) {
	
	Property_List *definition = get_property_list( ".Patch Names", p_name );
	
	if (!definition)
		return;
	
	desc->add_bank();
	int bank_idx=desc->get_bank_count() -1;
	desc->set_bank_name( bank_idx, p_name.c_str() );
	desc->set_bank_select_LSB( bank_idx, p_LSB );
	desc->set_bank_select_MSB( bank_idx, p_MSB );
	//printf("BANK %s\n",desc->get_bank_name(bank_idx).ascii().get_data());
	for (int i=0;i<definition->size();i++) {
		
		Pair p = (*definition)[i];
		
		int patch_idx=atoi(p.key.c_str());
		//printf("PATCH: %i (%s) - %s\n",patch_idx,p.key.c_str(),p.value.c_str());
		if (patch_idx<0 || patch_idx>127)
			continue;
		desc->set_bank_patch_name( bank_idx, patch_idx, p.value.c_str() );
	}
	

}

InsDeviceLoader::InsDeviceLoader()
{
}


InsDeviceLoader::~InsDeviceLoader()
{
}


}
