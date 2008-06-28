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

namespace ReShaked {


enum FileFieldType {
		
	FILE_FIELD_ENTER=0,
	FILE_FIELD_EXIT=1,
	FILE_FIELD_INT=2,
	FILE_FIELD_INT_ARRAY=3,
	FILE_FIELD_FLOAT=4,
	FILE_FIELD_FLOAT_ARRAY=5,
	FILE_FIELD_STRING=6,
	FILE_FIELD_RAW=7,
	FILE_FIELD_EOF=8,
};
	
	
};
#endif
