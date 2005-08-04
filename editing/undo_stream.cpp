 #include "undo_stream.h"


UndoRedoOwner *UndoRedoOp::get_owner() {

	return owner;

}

UndoRedoOp::UndoRedoOp(UndoRedoOwner *p_owner) {

	owner=p_owner;
}
UndoRedoOp::~UndoRedoOp() {

	/* empty for inheritance */
}

void UndoStream::new_block(String p_block_name) {

	/* this algorithm erases the undoed operations of the list
	when a new operation is done, so they cant be redone */

	int undo_list_size=undo_list.size();

	if (current_block<(undo_list_size-1))
		for(int i=current_block;i<(undo_list_size-1);i++)
			undo_list.erase( --undo_list.end() );



	/* check that last operation isnt empty */

	if (!undo_list.empty() && (--undo_list.end())->operations.empty())
		WARN_PRINT("Operation didnt contain data:" + (--undo_list.end())->name );

	/* Create new block */

	UndoRedoBlock new_block;
	new_block.name=p_block_name;

	undo_list.push_back(new_block);
	current_block++;

}
void UndoStream::add_operation(UndoRedoOp *p_operation) {


	ERR_FAIL_COND( undo_list.empty() );
	ERR_FAIL_COND( current_block<((int)undo_list.size()-1) );

	undo_list.back().operations.push_back(p_operation);



}

int UndoStream::get_block_count() {

	return undo_list.size();
}

std::list<UndoStream::UndoRedoBlock>::iterator UndoStream::get_block_iterator(int p_index) {

	int undo_list_size=undo_list.size();

	ERR_FAIL_INDEX_V(p_index,undo_list_size,undo_list.end());
	std::list<UndoRedoBlock>::iterator I=undo_list.begin();
	for (int i=0;i<p_index;i++,I++); // this is cool!
	return I;


}

String UndoStream::get_block_name(int p_index) {

	std::list<UndoRedoBlock>::iterator I=get_block_iterator(p_index);
	ERR_FAIL_COND_V(I==undo_list.end(),"");
	return I->name;
}

void UndoStream::undo_block(UndoRedoBlock *p_block) {

	if (p_block->operations.empty())
		return;

	std::list<UndoRedoOp*>::iterator op_I=--p_block->operations.end();

	while (true) {
		(*op_I)->get_owner()->undo(*op_I);
		if (op_I==p_block->operations.begin())
			break;
		op_I--;
	}
}

void UndoStream::undo(int p_to_block) {

	int target_block=(p_to_block==-1)?(current_block-1):(p_to_block);
	ERR_FAIL_COND(target_block<-1);

	if (target_block>current_block) {
		redo(p_to_block);
		return;
	} else if (target_block==current_block) {
		return; /* nothing to do */
	}

	std::list<UndoRedoBlock>::iterator tg_I=get_block_iterator(target_block+1);
	ERR_FAIL_COND(tg_I==undo_list.end());

	std::list<UndoRedoBlock>::iterator src_I=get_block_iterator(current_block);
	ERR_FAIL_COND(src_I==undo_list.end());

	while (true) { /* loops with iterators suck, hehe */
		undo_block(&(*src_I));
		src_I--;
		if (src_I==tg_I)
			break;
	}
}

void UndoStream::redo_block(UndoRedoBlock *p_block) {

	if (p_block->operations.empty())
		return;

	std::list<UndoRedoOp*>::iterator op_I=p_block->operations.begin();

	for (;op_I!=p_block->operations.end();op_I++)
		(*op_I)->get_owner()->redo(*op_I);
}

void UndoStream::redo(int p_to_block) {

	int target_block=(p_to_block==-1)?(current_block+1):(p_to_block);

	ERR_FAIL_COND(target_block>=(int)undo_list.size());

	if (target_block<current_block) {
		undo(p_to_block);
		return;
	} else if (target_block==current_block) {
		return; /* nothing to do */
	}

	std::list<UndoRedoBlock>::iterator tg_I=get_block_iterator(target_block);
	ERR_FAIL_COND(tg_I==undo_list.end());

	std::list<UndoRedoBlock>::iterator src_I=get_block_iterator(current_block+1);
	ERR_FAIL_COND(src_I==undo_list.end());

	while (true) { /* loops with iterators suck, hehe */
		redo_block(&(*src_I));
		src_I++;
		if (src_I==tg_I)
			break;
	}
}

UndoStream::UndoStream() {

	current_block=-1;
}
