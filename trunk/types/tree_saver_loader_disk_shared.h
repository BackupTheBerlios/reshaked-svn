//
// C++ Interface: tree_saver_loader_disk_shared
//
// Description:
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//


#ifndef TREE_SAVER_LOADER_DISK_SHARED_H
#define TREE_SAVER_LOADER_DISK_SHARED_H

/**
 * Some values used by both tree saver/loader (check: tree_saver.h / tree_loader.h in this same folder)
 */

enum FileFieldType {

	FILE_FIELD_ENTER=0,
	FILE_FIELD_EXIT=1,
	FILE_FIELD_EOF=2,
	FILE_FIELD_INT=3,
	FILE_FIELD_INT_ARRAY=4,
	FILE_FIELD_FLOAT=5,
	FILE_FIELD_FLOAT_ARRAY=6,
	FILE_FIELD_STRING=7,
	FILE_FIELD_STRING_ARRAY=8,
	FILE_FIELD_RAW=9,
	
};

enum TreeConstants {
	
	MAX_ENTRY_NAME_LEN=32	/// default entry name length used as name for the varials.
	
};


#endif
