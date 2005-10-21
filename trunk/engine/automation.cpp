#include "automation.h"





namespace ReShaked {



void Automation::create_block(Tick p_pos,BlockCreationData *p_creation_data) {
	
	if (!block_fits(p_pos,TICKS_PER_BEAT))
		return; //cant add thing
	
	AutomationData *a = new AutomationData;
	AutomationBlock *ab = new AutomationBlock(a);
	
	if (add_block(ab,p_pos)) { //no add? fuck must be a bug!?
		
		delete a;
		delete ab;
		ERR_PRINT("Failed adding pattern after previous check? WTF?");
		return;
	}
		

	automation_array.push_back(a);	
	
}

void Automation::copy_block(int p_from_which,Tick p_to_where) {

	copy_block(p_from_which,p_to_where);
}

void Automation::copy_block_ref(int p_from_which,Tick p_to_where) {

	AutomationBlock *b = dynamic_cast<AutomationBlock*>( get_block(p_from_which) );
	
	ERR_FAIL_COND( b==NULL );

		
	if (!block_fits(p_to_where,b->get_length()))
		return; //cant add thing
	
	AutomationBlock *pb = new AutomationBlock(b->get_data());
	
	if (add_block(pb,p_to_where)) { //no add? fuck must be a bug!?
		
		b->get_data()->refcount--; //WICKED SHIT!!
		delete pb;
		ERR_PRINT("Failed adding pattern after previous check? WTF? BUG! BUG! BUG!!!!");
		return;
	}
		
	
}
void Automation::erase_block(int p_which) {
	
	
	
}

Automation::AutomationData *Automation::AutomationBlock::get_data() {
	
	return data;
	
}

Automation::AutomationData::AutomationData() {

	refcount=0;	
	length=5;
}


Tick Automation::AutomationBlock::get_length() {
	
	return data->length*TICKS_PER_BEAT;
}

void Automation::AutomationBlock::set_length(Tick p_length) {
	
	if (is_shared())
		return;

	if (p_length<TICKS_PER_BEAT)
		return; //automation cant be less than a beat;
	
	data->length=p_length/TICKS_PER_BEAT;
}

bool Automation::AutomationBlock::is_shared() {
	
	return data->refcount>1;
}
String Automation::get_type_name() {
	
	return "automation";	
}
Automation::AutomationBlock::AutomationBlock(AutomationData *p_data) {
	
	data=p_data;
	p_data->refcount++;
}
Automation::Automation() : BlockList(BLOCK_TYPE_FREE_MOVING) {
	

}


};
