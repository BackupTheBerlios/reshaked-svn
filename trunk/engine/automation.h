#ifndef AUTOMATION_H
#define AUTOMATION_H

#include <vector>
#include "types/value_stream.h"
#include "types/typedefs.h"
#include "engine/block_list.h"
#include "engine/data_pool.h"
#include "property.h"


namespace ReShaked {



class Automation : public BlockList {
public:

	enum DisplaySize {
		DISPLAY_SIZE_SMALL,
		DISPLAY_SIZE_MEDIUM,
		DISPLAY_SIZE_BIG	
	};
	
	enum BlockInterpolationMethod {

		INTERPOLATION_NONE,
		INTERPOLATION_LINEAR,
		INTERPOLATION_SPLINE,
	};
	
		
	struct AutomationValue {
		
		float value;
		float lfo_depth;
		
		AutomationValue(float p_value=0.0,float p_lfo_depth=0.0) { value=p_value; lfo_depth=p_lfo_depth; }
	};


	struct AutomationData : public ValueStream<Tick,AutomationValue> , public DataPool::Data {
	friend class AutomationBlock;
	friend class Automation;
		int length; //in beats
		int refcount;
		
		
	public:

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
		bool is_shared();

		AutomationData *get_data();
		AutomationBlock(AutomationData *p_data);
	};

private:
	DataPool *pool;
	String get_type_name();
	Block *create_duplicate_block(Block *p_block);
	Block *create_link_block(Block *p_block);
	BlockCreationBehavior get_block_creation_behavior();
	bool can_resize_from_begining();
	bool accepts_block(Block *p_block);
	Property *property;	
	
	DisplaySize display_size;

public:
	
	bool shares_block_data(Block *p_block);
	Property *get_property();
	
	AutomationBlock * get_block(int p_block);
	
	float get_tick_val(Tick p_tick); // 0.0 - 1.0
	
	void create_block(Tick p_pos,BlockCreationData *p_creation_data=NULL);

	
	DisplaySize get_display_size();

	Automation(DataPool *p_pool,Property *property);
};

}; /* end of namespace */

#endif
