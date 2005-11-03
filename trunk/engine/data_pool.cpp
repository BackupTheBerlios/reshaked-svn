//
// C++ Implementation: data_pool
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "data_pool.h"

namespace ReShaked {



void DataPool::add_data(Data* p_data) {
	
	data_array.push_back(p_data);
	
}

DataPool::DataPool()
{
}


DataPool::~DataPool()
{
	
	for (int i=0;i<data_array.size();i++)
		delete data_array[i];
}


}
