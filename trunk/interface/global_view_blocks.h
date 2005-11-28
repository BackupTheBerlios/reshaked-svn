//
// C++ Interface: global_view_blocks
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDGLOBAL_VIEW_BLOCKS_H
#define RESHAKEDGLOBAL_VIEW_BLOCKS_H

#include <Qt/qpainter.h>
#include "editor.h"


namespace ReShaked {

/**
	@author red <red@killy>
*/
class GlobalViewBlocks {
public:
	
	static void paint_block_contents(QPainter &p, int p_x,int p_y,int p_w, int p_h, BlockList::Block *p_block,bool p_selected,int p_clip_to_len=-1);

};

}

#endif
