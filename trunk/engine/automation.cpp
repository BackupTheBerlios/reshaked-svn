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
	
	float val=0;
	float lfo_val=0;
	
	if (next==stream_size) {
		val=get_index_value( stream_size -1 ).value; //just send the last one, nothing else can be done
		lfo_val=get_index_value( stream_size -1 ).lfo_depth;
	} else if (prev==next) { //we got an exact index, just return it, no interp. needed
		val=get_index_value(prev).value;
		lfo_val=get_index_value(prev).lfo_depth;
	} else {
	
		switch (interpolation) {
			
			case Automation::INTERP_NONE: {
				
				val=get_index_value(prev).value;
				lfo_val=get_index_value(prev).lfo_depth;
				
			} break;
			case Automation::INTERP_LINEAR: {
				
				Tick prev_tick=get_index_pos( prev);
				float prev_val=get_index_value( prev).value;
				float prev_lfo_depth_val=get_index_value( prev ).lfo_depth;
				
				Tick next_tick=get_index_pos( next);
				float next_val=get_index_value( next).value;
				float next_lfo_depth_val=get_index_value( next ).lfo_depth;
				
				val=prev_val+((double)(p_tick-prev_tick)*(next_val-prev_val))/(double)(next_tick-prev_tick); //linear interpolation
				lfo_val=prev_lfo_depth_val+((double)(p_tick-prev_tick)*(next_lfo_depth_val-prev_lfo_depth_val))/(double)(next_tick-prev_tick); //linear interpolation
			} break;
			case Automation::INTERP_SPLINE: {
				
				
				Tick prev_tick=get_index_pos( prev);
				float prev_val=get_index_value( prev).value;
				float prev_lfo_depth_val=get_index_value( prev ).lfo_depth;
				
				Tick next_tick=get_index_pos( next);
				float next_val=get_index_value( next).value;
				float next_lfo_depth_val=get_index_value( next ).lfo_depth;
				
				val=prev_val+((double)(p_tick-prev_tick)*(next_val-prev_val))/(double)(next_tick-prev_tick); //linear interpolation
				lfo_val=prev_lfo_depth_val+((double)(p_tick-prev_tick)*(next_lfo_depth_val-prev_lfo_depth_val))/(double)(next_tick-prev_tick); //linear interpolation
				
				
			} break;
		}
	
	}
	
	val+=lfo_val*lfo.get_value( p_tick );
	
	if (val<0)
		val=0;
	if (val>1)
		val=1;
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

Automation::BlockInterpolationMethod Automation::AutomationData::get_interpolation() {
	
	
	return interpolation;
}
void Automation::AutomationData::set_interpolation(BlockInterpolationMethod p_int) {
	
	interpolation=p_int;
}


LFO& Automation::AutomationData::get_lfo() {
	
	return lfo;
}

bool Automation::accepts_block(Block *p_block) {
	
	return dynamic_cast<AutomationBlock*>(p_block)!=NULL;	
}

Automation::AutomationData *Automation::AutomationBlock::get_data() {

	return data;

}

Automation::AutomationData::AutomationData() {

	interpolation=INTERP_LINEAR;
	
	lfo.set_rate_unit_size( TICKS_PER_BEAT ); //a beat is rate :)
	lfo.set_depth(0.5); //maximum depth for having a range of 1
	lfo.set_rate(1); //1 cycle per track
	lfo.set_phase( 0 ); //phase 0 is default
	lfo.set_random_depth(0); //no randomness
	lfo.set_mode( LFO::MODE_SINE ); //sinewave is the most common
	lfo.set_delay( 0 );
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

void Automation::set_display_size(DisplaySize p_size) {
	
	display_size=p_size;
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
