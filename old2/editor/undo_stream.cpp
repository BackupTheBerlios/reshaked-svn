#include "undo_stream.h"
#include "typedefs.h"
#include "get_time.h"

namespace ReShaked {

void CommandFunc::clear_create_data() {
	
	create_data.clear();
	
}
void CommandFunc::clear_delete_data() {
	
	delete_data.clear();
	
}


void CommandFunc::erase_create_data() {
	
	for (int i=0;i<create_data.size();i++) {
			
		delete create_data[i];
	}
	create_data.clear();
	
}
void CommandFunc::erase_delete_data() {
	
	for (int i=0;i<delete_data.size();i++) {
			
		delete delete_data[i];
	}
	delete_data.clear();
	
}

CommandFunc::~CommandFunc() {
	
	ERR_FAIL_COND(!delete_data.empty());
	ERR_FAIL_COND(!create_data.empty());
}
void UndoStream::delete_redo_history() {
	
	int to_erase=(int)undo_stream.size()-(current_index+1);
	while(to_erase--) {
		
		UndoGroup&ug=undo_stream.back();
		std::list<UndoElement>::iterator I=ug.command_list.begin();
		for (;I!=ug.command_list.end();I++) {
			I->undo->erase_create_data(); //since this undo is no longer needed, we erase the create data
			I->undo->clear_delete_data();
			delete I->undo;
			delete I->redo;
		}
		undo_stream.pop_back(); //remove this
	}
	
}

void UndoStream::fix_undo_history_length() {
	
	if (max_undo_steps<5)
		return;
	
	int to_erase=(int)undo_stream.size()-max_undo_steps;
	
	if (to_erase<=0)
		return;
	
	current_index-=to_erase;
	if (current_index<0)
		current_index=0;
	
	while (to_erase--) {
		
		
		UndoGroup&ug=undo_stream.front();
		//printf("Erasing %s\n",ug.name.ascii().get_data());
		std::list<UndoElement>::iterator I=ug.command_list.begin();
		for (;I!=ug.command_list.end();I++) {
			I->undo->erase_delete_data(); //since this undo is no longer needed, we erase the create data
			I->undo->clear_create_data();
			delete I->undo;
			delete I->redo;
		}
		undo_stream.pop_front(); //remove this
		
	}
}

void UndoStream::set_max_undo_steps(int p_max) {
	
	max_undo_steps=p_max;
}

void UndoStream::begin(String p_name,bool p_can_collapse_to_previous) {

        //printf("Undo Begin!\n");
	inside_count++;
	if (inside_count>1) //already into group, merge
		return;
	
	if (current_index<(int)(undo_stream.size()-1))  // we have been undoing, so delete redo history
		delete_redo_history();
	
	/* Shall we create a new group, or use the previous one? */
	if (p_can_collapse_to_previous && !undo_stream.empty() && undo_stream.back().name==p_name && (GetTime::get_time_msec()-undo_stream.back().timestamp)<collapse_max_time_window) {
		/* can collapse it to previous one! */
		////printf("collapsing %lls\n",p_name.c_str());
		undo_stream.back().collapses++;
		
	} else {
	
		UndoGroup undo_group;
		undo_group.name=p_name;
		undo_group.timestamp=GetTime::get_time_msec();
		undo_group.collapses=0;
		////printf("adding at time %i\n",undo_group.timestamp);
		
		undo_stream.push_back( undo_group );
		current_index++;
		fix_undo_history_length();
	}
	
	
}

void UndoStream::add_command( CommandFunc *p_command ) {

        ////printf("Command Begin\n");
	ERR_FAIL_COND(inside_count==0);
	ERR_FAIL_COND(undo_stream.empty());
	
	UndoElement e;
	e.redo=p_command;
	e.undo=p_command->exec();

	if (e.undo==NULL) { //the command failed, abort. Commands shouldnt fail, but I just do it for app safety
		delete p_command;
		return;
	}
	undo_stream.back().command_list.push_back(e);

}
void UndoStream::end() {

	// if inside count is 0, someone called end() without begin
	ERR_FAIL_COND(inside_count==0);
	
	// decrease inside count;
	inside_count--;

	//if still greater than zero, means we are inside another block, so ignore
	if (inside_count>0)
		return;
	
	//i could put something here someday..

}


String UndoStream::get_current_action_name() {
	
	std::list<UndoGroup>::iterator I=get_block_iterator(current_index);
	if (I==undo_stream.end())
		return "";
	return I->name;
	
}
int UndoStream::get_current_action_collapses() {
	
	
	std::list<UndoGroup>::iterator I=get_block_iterator(current_index);
	if (I==undo_stream.end())
		return 0;
	return I->collapses;
	
}

String UndoStream::get_current_action_text() {
	
	int coll=get_current_action_collapses()+1;
	
	String text=get_current_action_name();
	if (coll>1)
		text+=" x"+String::num(coll);
	
	return text;
	
}

std::list<UndoStream::UndoGroup>::iterator UndoStream::get_block_iterator(int p_index) {

	int undo_stream_size=(int)undo_stream.size();

	ERR_FAIL_INDEX_V(p_index,undo_stream_size,undo_stream.end());
	std::list<UndoGroup>::iterator I=undo_stream.begin();
	for (int i=0;i<p_index;i++,I++); // this is cool!
	return I;

}
void UndoStream::undo_group(UndoGroup *p_group) {

	ERR_FAIL_COND(p_group->command_list.empty());

	std::list<UndoElement>::iterator op_I=--p_group->command_list.end();

	while (true) {
		op_I->undo->exec(true); //dont create undo for this!
		if (op_I==p_group->command_list.begin())
			break;
		op_I--;
	}
}
void UndoStream::redo_group(UndoGroup *p_group) {

	ERR_FAIL_COND(p_group->command_list.empty());

	std::list<UndoElement>::iterator op_I=p_group->command_list.begin();

	for (;op_I!=p_group->command_list.end();op_I++)
		op_I->redo->exec(true); //true means no UNDO
}

void UndoStream::undo() {
	
	
	if (lock_count)
		return;
	if (current_index<0)
		return;
	ERR_FAIL_COND(inside_count>0);
	
	std::list<UndoGroup>::iterator I=get_block_iterator( current_index );
	ERR_FAIL_COND(I==undo_stream.end());
	////printf("UNDO: %lls\n",I->name.c_str());
	undo_group( &(*I) );
	current_index--;
}


void UndoStream::redo() {

	if (lock_count)
		return;
	
	if (current_index>=(int)(undo_stream.size()-1)) //no redo
		return;
	
	
	std::list<UndoGroup>::iterator I=get_block_iterator( current_index+1 );
	ERR_FAIL_COND(I==undo_stream.end());
	////printf("REDO: %lls\n",I->name.c_str());
	redo_group( &(*I) );
	current_index++;
}

void UndoStream::lock() {
	lock_count++;

	
}
void UndoStream::unlock() {
	
	ERR_FAIL_COND(lock_count==0);
	lock_count--;
	
}

void UndoStream::clear() {
	
	foreach(I,undo_stream) {
			
		foreach(J,I->command_list) {
			
			delete J->undo;
			delete J->redo;
		}
		
	}
	
	undo_stream.clear();
	lock_count=0;
	current_index=-1;
	inside_count=0;

}

int UndoStream::max_undo_steps=-1; //static, since it's a setting

UndoStream::UndoStream() {

	max_undo_steps=-1; //infinite
	lock_count=0;
	current_index=-1;
	inside_count=0;
	collapse_max_time_window=2000; //2 seconds
}

UndoStream::~UndoStream() {
	
	clear();
}
} //end of namespace
