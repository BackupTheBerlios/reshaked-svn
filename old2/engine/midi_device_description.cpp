//
// C++ Implementation: midi_device_description
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "midi_device_description.h"

namespace ReShaked {

String MidiDeviceDescription::get_name() {
	
	return name;
}
void MidiDeviceDescription::set_name(String p_name) {
	
	name=p_name;
}

void MidiDeviceDescription::set_control_name(int p_which,String p_name) {
	
	ERR_FAIL_INDEX(p_which,MAX_CONTROLS);
	
	control_name[p_which]=p_name;
	
}
String MidiDeviceDescription::get_control_name(int p_which) {
	
	ERR_FAIL_INDEX_V(p_which,MAX_CONTROLS,"");
	
	return control_name[p_which];
	
}

void MidiDeviceDescription::add_bank() {
	
	Bank b;
	b.name="Empty Bank";
	banks.push_back( b );
		
}

void MidiDeviceDescription::erase_bank(int p_bank) {
	
	ERR_FAIL_INDEX(p_bank,banks.size());
	banks.erase( banks.begin() + p_bank );
	
}

int MidiDeviceDescription::get_bank_count() {
	
	return banks.size();
}

String MidiDeviceDescription::get_bank_name(int p_bank) {
	
	ERR_FAIL_INDEX_V(p_bank,banks.size(),"");
	
	return banks[p_bank].name;
}
void MidiDeviceDescription::set_bank_name(int p_bank,String p_name) {
	
	ERR_FAIL_INDEX(p_bank,banks.size());
	
	banks[p_bank].name=p_name;
	
	
}

int MidiDeviceDescription::get_bank_select_MSB(int p_bank) {
	
	ERR_FAIL_INDEX_V(p_bank,banks.size(),-1);
	
	return banks[p_bank].select.MSB;
	
	
}
void MidiDeviceDescription::set_bank_select_MSB(int p_bank,int p_MSB) {
	
	ERR_FAIL_INDEX(p_bank,banks.size());
	
	banks[p_bank].select.MSB=p_MSB;
	
	
	
}

int MidiDeviceDescription::get_bank_select_LSB(int p_bank) {
	
	ERR_FAIL_INDEX_V(p_bank,banks.size(),-1);
	
	return banks[p_bank].select.LSB;
	
	
}
void MidiDeviceDescription::set_bank_select_LSB(int p_bank,int p_LSB) {
	
	ERR_FAIL_INDEX(p_bank,banks.size());
	
	banks[p_bank].select.LSB=p_LSB;
	
	
	
}

String MidiDeviceDescription::get_bank_patch_name(int p_bank,int p_patch) {
	
	ERR_FAIL_INDEX_V(p_bank,banks.size(),"");
	ERR_FAIL_INDEX_V(p_patch,MAX_PATCHES,"");
	
	/* If not in list, empty */
	int idx=banks[p_bank].patch_list.get_exact_index(p_patch);
	if (idx<0)
		return ""; 
	
	return banks[p_bank].patch_list.get_index_value(idx);
	
}
void MidiDeviceDescription::set_bank_patch_name(int p_bank,int p_patch,String p_name) {
	
	ERR_FAIL_INDEX(p_bank,banks.size());
	ERR_FAIL_INDEX(p_patch,MAX_PATCHES);
	
	/* If not in list, empty */
	int idx=banks[p_bank].patch_list.get_exact_index(p_patch);
	
	if (idx<0 && p_name=="") //dont bother
		return; 
	
	if (p_name=="") { //erase
		
		banks[p_bank].patch_list.erase_index(idx);	
	} else { //insert
	
		banks[p_bank].patch_list.insert(p_patch,p_name);
	}
	
	
}

void MidiDeviceDescription::add_GM_bank() {
	
	
	
}


void MidiDeviceDescription::clear() {
	
	for (int i=0;i<MAX_CONTROLS;i++)
		control_name[i]="";
	name="";
	banks.clear();
	bank_select_method=BANK_SELECT_NONE;
}

void MidiDeviceDescription::set_bank_select_method(BankSelectMethod p_bank_select_method) {
	
	bank_select_method=p_bank_select_method;
}
MidiDeviceDescription::BankSelectMethod MidiDeviceDescription::get_bank_select_method() {
	
	return bank_select_method;
}

void MidiDeviceDescription::save(TreeSaver *p_save) {
	
	p_save->enter("global");
	
	p_save->add_string("name",name);
	p_save->add_int("bank_select_method",bank_select_method);
	
	p_save->exit();// global
	
	p_save->enter("control_names");
	
	for (int i=0;i<MAX_CONTROLS;i++) {
		
		if (control_name[i]=="")
			continue;
		p_save->add_string(String::num(i),control_name[i]);
	}
	
	p_save->exit();
	
	p_save->enter("banks");
	
	for (int i=0;i<banks.size();i++) {
		
		p_save->enter("bank_"+String::num(i));
		
		p_save->add_string("name",banks[i].name);
		p_save->add_int("select_MSB",banks[i].select.MSB);
		p_save->add_int("select_LSB",banks[i].select.LSB);
				
		p_save->enter("patch_list");
		
		for (int j=0;j<banks[i].patch_list.get_stream_size();j++) {
			
			int idx=banks[i].patch_list.get_index_pos(j);
			String name=banks[i].patch_list.get_index_value(j);
			p_save->add_string(String::num(idx),name);
		
		}
		p_save->exit(); //patch list
		p_save->exit(); //bank
		
	}
	
	p_save->exit();
	
	
}

void MidiDeviceDescription::load(TreeLoader *p_load) {
	
	clear();
	
	p_load->enter("global");
	
	name=p_load->get_string("name");
	switch(p_load->get_int("bank_select_method")) {
		
		case 0: bank_select_method=BANK_SELECT_NONE; break;
		case 1: bank_select_method=BANK_SELECT_MSB; break;
		case 2: bank_select_method=BANK_SELECT_LSB; break;
		case 3: bank_select_method=BANK_SELECT_MSB_LSB; break;
	}
	
	
	p_load->exit();// global
	
	p_load->enter("control_names");
	
	for (int i=0;i<MAX_CONTROLS;i++) {
		
		if (!p_load->is_var(String::num(i)))
			continue;
			
		control_name[i]=p_load->get_string(String::num(i));
	}
	
	p_load->exit();
	
	p_load->enter("banks");
	
	for (int i=0;i<p_load->get_child_count();i++) {
		
		
		p_load->enter(p_load->get_child_name(i));
		
		add_bank();
		
		banks[i].name=p_load->get_string("name");
		banks[i].select.MSB=p_load->get_int("select_MSB")&0x7F;
		banks[i].select.LSB=p_load->get_int("select_LSB")&0x7F;
				
		p_load->enter("patch_list");
		
		for (int j=0;j<128;j++) {
			
			if (!p_load->is_var(String::num(i)))
				continue;
			String name=p_load->get_string( String::num(i) );
			banks[i].patch_list.insert( i, name );
		
		}
		p_load->exit(); //patch list
		p_load->exit(); //bank
		
	}
	
	p_load->exit();

}


MidiDeviceDescription::MidiDeviceDescription() {
	
	bank_select_method=BANK_SELECT_NONE;
}


MidiDeviceDescription::~MidiDeviceDescription() {
	
}


}
