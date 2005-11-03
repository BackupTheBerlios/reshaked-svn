//
// C++ Interface: pattern_pool
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDDATA_POOL_H
#define RESHAKEDDATA_POOL_H

#include <vector>

namespace ReShaked {

/**
	
*/
class DataPool {
public:
		
	class Data {
	public:		
		
		virtual ~Data() {};
	};
	
private:
	std::vector<Data*> data_array;

	
public:
	void add_data(Data* p_data);
	
	DataPool();
	~DataPool();

};

}

#endif
