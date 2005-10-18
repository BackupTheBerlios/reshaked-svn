#ifndef UNDO_STREAM_H
#define UNDO_STREAM_H

#include <list>
#include "typedefs.h"


class UndoRedoOp;
class UndoStream;

class UndoRedoOwner {

	UndoStream *undo_stream;
protected:	
	UndoStream *get_undo_stream();
public:

	virtual void undo(UndoRedoOp *p_item)=0;
	virtual void redo(UndoRedoOp *p_item)=0;
	UndoRedoOwner(UndoStream *p_undo_stream);
	virtual ~UndoRedoOwner() {};
};


class UndoRedoOp {

	UndoRedoOwner *owner;

	UndoRedoOp();
public:

	UndoRedoOwner *get_owner();

	UndoRedoOp(UndoRedoOwner *p_owner);
	virtual ~UndoRedoOp();
};



class UndoStream {

	struct UndoRedoBlock {

		String name;
		std::list<UndoRedoOp*> operations;
	};

	std::list<UndoRedoBlock> undo_list;

	int current_block;

	void undo_block(UndoRedoBlock *p_block);
	void redo_block(UndoRedoBlock *p_block);
	std::list<UndoRedoBlock>::iterator get_block_iterator(int p_index);
public:

	void new_block(String p_block_name);
	void add_operation(UndoRedoOp *p_operation);

	int get_block_count();
	String get_block_name(int p_index);

	void undo(int p_to_block=-1);
	void redo(int p_to_block=-1);

	UndoStream();
};

#endif
