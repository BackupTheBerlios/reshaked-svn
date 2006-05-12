


#ifndef MIDI_PARAMETERS_H
#define MIDI_PARAMETERS_H


#include "property.h"


namespace ReShaked {


class MidiParameters {
public:
	enum {
		
		MAX_MIDI_CONTROLS=128,
		CONTROL_PITCH_BEND=128,
		CONTROL_AFTERTOUCH=129,
		MAX_CONTROLS=130,
		MAX_CHANGED_LIST=1024,
		CHANGES_DISABLED=-1
	};
	
	struct Changes {
		
		int count;
		
		struct Change {
			
			int control;
			int value;
		};
		
		Change list[MAX_CHANGED_LIST];
	
	};
	
private:		
	
	
	struct Parameter : public Property {
		
		bool active; //may be or not in use
		
		int val; //current value
		int max; //max value
		int min;
		int default_val;
		int index; //control index;
		
		bool stream; //discard repeated events
				
		Changes *changes;
		
		double get();
		void set(double p_val);
		double get_stepping();
		double get_min();
		double get_max();
		double get_default();
		String get_name();
		String get_caption();
		String get_postfix();
		String get_text_value(double p_for_value,bool p_no_postfix=false);
		bool has_text_value();
	
		DisplayMode get_display_mode();
	
		/* helpers */	
		String get_text_value(bool p_no_postfix=false); 		
		
		String display;
	
	};

	Changes changes;
	Parameter parameters[MAX_CONTROLS];
	
public:

	int get_parameter_count();
	bool is_parameter_active(int p_param);
	Property *get_parameter(int p_param);
	void set_parameter_display(int p_param,String p_display);
	
	/* track parameter changes */
	int get_changes_count();
	const Changes::Change* get_changes();
	void clear_changes();

	MidiParameters();
};



};

#endif