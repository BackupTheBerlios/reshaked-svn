#ifndef AUTOMATION_H
#define AUTOMATION_H

#include <vector>
#include "types/value_stream.h"
#include "types/typedefs.h"
#include "engine/block_list.h"
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


	struct AutomationData : public ValueStream<Tick,AutomationValue> {
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
		bool pos_snaps_to_beat();
		bool is_shared();
	
		AutomationData *get_data();
		AutomationBlock(AutomationData *p_data);
	};

private:

	std::vector<AutomationData*> automation_array;
public:	
	void create_block(Tick p_pos,BlockCreationData *p_creation_data=NULL);
	void copy_block(int p_from_which,Tick p_to_where);
	void copy_block_ref(int p_from_which,Tick p_to_where);
	void erase_block(int p_which);

	Automation();	
};

}; /* end of namespace */

#endif
