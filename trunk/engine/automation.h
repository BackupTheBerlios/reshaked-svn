#ifndef AUTOMATION_H
#define AUTOMATION_H

#include <vector>
#include "types/value_stream.h"
#include "types/typedefs.h"
#include "engine/block_list.h"
#include "engine/data_pool.h"
namespace ReShaked {



class Automation : public BlockList {
public:

	struct AutomationValue {
		
		enum InterpolationMethod {
			
			INTERPOLATION_NONE,
			INTERPOLATION_LINEAR,
			INTERPOLATION_SPLINE,		
		};
		
		float value;	
		InterpolationMethod interpolation;
	};


	struct AutomationData : public ValueStream<Tick,AutomationValue> , public DataPool::Data {
	friend class AutomationBlock;
	friend class Automation;
		int length; //in beats
		int refcount;
	public:	
	
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
	
	bool can_resize_from_begining();
	
public:	
	void create_block(Tick p_pos,BlockCreationData *p_creation_data=NULL);
	void erase_block(int p_which);

	Automation(DataPool *p_pool);	
};

}; /* end of namespace */

#endif
