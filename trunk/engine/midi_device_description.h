//
// C++ Interface: midi_device_description
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDMIDI_DEVICE_DESCRIPTION_H
#define RESHAKEDMIDI_DEVICE_DESCRIPTION_H

#include "typedefs.h"
#include "value_stream.h"


namespace ReShaked {

/**
	@author red <red@killy>
*/
class MidiDeviceDescription{
public:	
	enum {
		
		MAX_CONTROLS=128,
		MAX_PATCHES=128
	};
	
	enum BankSelectMethod {
		BANK_SELECT_NONE,
		BANK_SELECT_MSB, //  Variation in GS
		BANK_SELECT_LSB,
		BANK_SELECT_MSB_LSB // 14 bits
	};
	
private:	
	String name;
		
	struct Bank {
			
		String name;
		
		struct Select {
			int MSB;
			int LSB;
			
			Select() { MSB=LSB=0; }
		} select;
						
		ValueStream<int,String> patch_list;
			
		
	};
	
	std::vector<Bank> banks;
			
	String control_name[MAX_CONTROLS];
	
	BankSelectMethod bank_select_method;
	
public:
	
	void set_bank_select_method(BankSelectMethod p_bank_select_method);
	BankSelectMethod get_bank_select_method();
	
	String get_name();
	void set_name(String p_name);
	
	void set_control_name(int p_which,String p_name);
	String get_control_name(int p_which);
	
	void add_bank();
	void add_GM_bank(); //standard GM BANK
	void erase_bank(int p_bank);
	
	int get_bank_count();
	
	String get_bank_name(int p_bank);
	void set_bank_name(int p_bank,String p_name);
	
	int get_bank_select_MSB(int p_bank);
	void set_bank_select_MSB(int p_bank,int p_MSB);

	int get_bank_select_LSB(int p_bank);
	void set_bank_select_LSB(int p_bank,int p_LSB);
	
	String get_bank_patch_name(int p_bank,int p_patch);	
	void set_bank_patch_name(int p_bank,int p_patch,String p_name);	

	
	void clear();
	
	
	MidiDeviceDescription();
	
	~MidiDeviceDescription();

};

}

#endif
