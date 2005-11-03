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

	pool->add_data(a);

}

BlockList::BlockCreationBehavior Automation::get_block_creation_behavior() {

	return BLOCK_CREATE_AND_RESIZE;
}


BlockList::Block *Automation::create_duplicate_block(Block *p_block) {

	AutomationBlock *b = dynamic_cast<AutomationBlock*>( p_block );
	ERR_FAIL_COND_V( b==NULL , NULL );

	//new data
	AutomationData *nd = new AutomationData;
	*nd=*b->get_data(); //copy
	nd->refcount=0; //refcount to zero

	//new block
	pool->add_data(nd);
	AutomationBlock *nb = new AutomationBlock(nd);

	return nb;


}

BlockList::Block *Automation::create_link_block(Block *p_block) {

	AutomationBlock *b = dynamic_cast<AutomationBlock*>( p_block );
	ERR_FAIL_COND_V( b==NULL , NULL );

	//new block
	AutomationBlock *nb = new AutomationBlock(b->get_data());

	return nb;


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

bool Automation::can_resize_from_begining() {

	return true;
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
Automation::Automation(DataPool *p_pool) : BlockList(BLOCK_TYPE_FREE_MOVING) {

	pool=p_pool;
}


};
