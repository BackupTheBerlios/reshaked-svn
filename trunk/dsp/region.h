//
// C++ Interface: region
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef REGION_H
#define REGION_H

#include "value_stream.h"

namespace ReShaked {

struct Region {
				
	signed char base_note; //- 0.. 127 -1 is disabled */
	int source_idx;
};
			
typedef ValueStream<char,Region> RegionMap;

			
			
}









#endif
