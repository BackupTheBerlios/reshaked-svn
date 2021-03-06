#ifndef AUTOMATION_H
#define AUTOMATION_H

#include <vector>
#include "types/value_stream.h"
#include "types/typedefs.h"
#include "engine/block_list.h"
#include "engine/data_pool.h"
#include "property.h"
#include "dsp/lfo.h"


namespace ReShaked {




class Automation : public BlockList {
public:

	enum DisplaySize {
		DISPLAY_SIZE_SMALL,
		DISPLAY_SIZE_MEDIUM,
		DISPLAY_SIZE_BIG	
	};
	
	enum BlockInterpolationMethod {

		INTERP_NONE,
		INTERP_LINEAR,
		INTERP_CUBIC,
	};
	

	struct AutomationValue {
		
		float value;
		float lfo_depth;
		
		AutomationValue(float p_value=0.0,float p_lfo_depth=1.0) { value=p_value; lfo_depth=p_lfo_depth; }
	};


	struct AutomationData : public ValueStream<Tick,AutomationValue> , public SharedData {
	friend class AutomationBlock;
	friend class Automation;
		int length; //in beats
		
		BlockInterpolationMethod interpolation;
		LFO lfo;
	public:

		BlockInterpolationMethod get_interpolation();
		void set_interpolation(BlockInterpolationMethod p_int);

		LFO& get_lfo();
		float get_tick_val(Tick p_tick);
		void set_default_value(float p_default);
		float get_default_value(); /* defalult value for the automation */
		AutomationData();

	};

	class AutomationBlock : public BlockList::Block {

		AutomationData *data;
	public:
		

		Tick get_length();
		void set_length(Tick p_length);

		AutomationData *get_data();
		AutomationBlock(AutomationData *p_data);
		~AutomationBlock();
	};

	
private:
	
	String get_type_name();
	BlockCreationBehavior get_block_creation_behavior();
	bool can_resize_from_begining();
	bool accepts_block(Block *p_block);
	Property *property;	
	
	DisplaySize display_size;

	double initial_value;
	
	bool swing_follow;
	
	struct MidiRecording {
		bool enabled;
		int channel; //channel to record
		int control; //control to record;			
	} recording;
	
	
public:
	
	Block *create_duplicate_block(Block *p_block);
	Block *create_link_block(Block *p_block);
	
	void set_initial_value(double p_value);
	double get_initial_value();
	
	bool shares_block_data(Block *p_block);
	Property *get_property();
	
	AutomationBlock * get_block(int p_block);
	
	float get_tick_val(Tick p_tick); // 0.0 - 1.0
	
	static AutomationBlock* create_automation_block();
	Block* create_block(BlockCreationData *p_creation_data=NULL);

	bool is_recording_enabled();
	void set_recording_enabled(bool p_enabled);
	
	int get_recording_channel();
	void set_recording_channel(int p_chan);
	int get_recording_control();
	void set_recording_control(int p_ctrl);
	
	void set_swing_follow_enabled(bool p_swing_follow);
	bool is_swing_follow_enabled();
	
	void apply(Tick p_pos);
	
	void clear();
	
	DisplaySize get_display_size();
	void set_display_size(DisplaySize p_size);

	Automation(Property *property);
};

}; /* end of namespace */

#endif
