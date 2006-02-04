#include "automation.h"





namespace ReShaked {



float Automation::AutomationData::get_tick_val(Tick p_tick) {
	
	int stream_size=get_stream_size();
	
	if (stream_size==0)
		return -1;
	
	int prev = get_prev_index( p_tick );
	int next = get_next_index( p_tick );
	
	
		
	
	if (prev==-1) 
		return -1;
	
	if (next==stream_size)
		return get_index_value( stream_size -1 ).value; //just send the last one, nothing else can be done
	
	if (prev==next) //we got an exact index, just return it, no interp. needed
		return get_index_value(prev).value;
	
	Tick prev_tick=get_index_pos( prev);
	float prev_val=get_index_value( prev).value;
	
	Tick next_tick=get_index_pos( next);
	float next_val=get_index_value( next).value;
	
	float val=prev_val+((double)(p_tick-prev_tick)*(next_val-prev_val))/(double)(next_tick-prev_tick); //linear interpolation
	
	
	return val;
	
}

float Automation::get_tick_val(Tick p_tick) {
	
	int block=get_block_idx_at_pos( p_tick );
	if (block<0)
		return -1.0;
	
	AutomationBlock *b=(AutomationBlock*)BlockList::get_block(block); //cant use dynamic cast here, too slow, sorry there are other checks :(
	
	Tick block_tick=p_tick-get_block_pos(block);
	
	float res=b->get_data()->get_tick_val(block_tick);
	//printf("for tick p_tick %g , val is %g\n",(float)p_tick/(float)TICKS_PER_BEAT,res);
	return res;
	
	
	
	
}

Automation::AutomationBlock* Automation::create_automation_block() {
	
	
	AutomationData *a = new AutomationData;
	AutomationBlock *ab = new AutomationBlock(a);

	return ab;
}

BlockList::Block* Automation::create_block(BlockCreationData *p_creation_data) {

	return create_automation_block();
	
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
	nd->reset();

	//new block
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

bool Automation::accepts_block(Block *p_block) {
	
	return dynamic_cast<AutomationBlock*>(p_block)!=NULL;	
}

Automation::AutomationData *Automation::AutomationBlock::get_data() {

	return data;

}

Automation::AutomationData::AutomationData() {

	length=5;
}

bool Automation::can_resize_from_begining() {

	return true;
}


Tick Automation::AutomationBlock::get_length() {

	return data->length*TICKS_PER_BEAT;
}

void Automation::AutomationBlock::set_length(Tick p_length) {

	if (is_active_shared())
		return;

	if (p_length<TICKS_PER_BEAT)
		return; //automation cant be less than a beat;

	data->length=p_length/TICKS_PER_BEAT;
}

String Automation::get_type_name() {

	return "automation";
}

Automation::AutomationBlock * Automation::get_block(int p_block) {
	
	return dynamic_cast<AutomationBlock*>( BlockList::get_block(p_block) );	
	
}

Automation::DisplaySize Automation::get_display_size() {
	
	return display_size;	
}

Property *Automation::get_property() {
	
	return property;	
}

bool Automation::shares_block_data(Block *p_block) {
	
	AutomationBlock *b=dynamic_cast<AutomationBlock*>(p_block);
	if (!b)
		return false; //not same type
	for (int i=0;i<get_block_count();i++)
		if (b->get_data()==get_block(i)->get_data())
			return true; //shared!
		
	return false;
	
}

Automation::AutomationBlock::AutomationBlock(AutomationData *p_data) : Block(p_data) {

	data=p_data;
	
}



void Automation::apply(Tick p_pos) {
	
	float val=get_tick_val( p_pos );
	if (val<0)
		return;
	property->set_coeff_value(val); 	
}

Automation::AutomationBlock::~AutomationBlock() {

}

void Automation::set_initial_value(double p_value) {
	
	initial_value=p_value;
}
double Automation::get_initial_value() {
	
	return initial_value;
}

void Automation::clear() {
	
	clear_blocks();
	display_size=DISPLAY_SIZE_SMALL;
	initial_value=property->get();
}
Automation::Automation(Property *p_property) : BlockList(BLOCK_TYPE_FIXED_TO_BEAT) {

	property=p_property;
	clear();
}


};
