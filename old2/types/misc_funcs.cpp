
#include "misc_funcs.h"

namespace ReShaked {

String get_note_str(int p_note) {
	
	if (p_note<0)
		return "";
	
	/* Helpers */
	static const char* note_str[12] = {
		"C-",
		"C#",
		"D-",
		"D#",
		"E-",
		"F-",

		"F#",
		"G-",
		"G#",
		"A-",
		"A#",
		"B-"
	};

	
	String note=note_str[p_note%12];
	note+=String::num(p_note/12);
	
	return note;
	
	
}


std::vector<String> split_str(String::CharType p_key,String p_string) {
	
	std::vector<String> split_aux;
			
	String current;
	
	for (int i=0;i<p_string.length();i++) {
		
		if (p_string[i]==p_key) {
			
			split_aux.push_back(current);
			current="";
		} else {
			
			current+=p_string[i];
		}
		
	}
	
	split_aux.push_back(current);
	
	return split_aux;
		
}

int get_decimal_count(double p_number) {
	
	char buff[100];
	sprintf(buff,"%lg",p_number);
	
	int decimals=0;
	int idx=0;
	bool zero=false;
	while (buff[idx]!=0) {
		
		
		if (buff[idx]=='.')
			zero=true;
		else if (zero)
			decimals++;
		idx++;
	}
	
	return decimals;
}

}
