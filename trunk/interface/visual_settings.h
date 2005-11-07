//
// C++ Interface: visual_settings
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDVISUAL_SETTINGS_H
#define RESHAKEDVISUAL_SETTINGS_H

#include "ui_blocks/skin_box.h"
#include "ui_blocks/pixmap_font.h"
#include <map>

namespace ReShaked {

/**
	@author red <red@killy>
*/

    
			
enum SkinBoxList {				    
			    
	SKINBOX_GLOBALVIEW_PATTERN,
	SKINBOX_GLOBALVIEW_AUTOMATION,
	SKINBOX_GLOBALVIEW_AUDIO,
	SKINBOX_GLOBALVIEW_PATTERN_SELECTED,
	SKINBOX_GLOBALVIEW_AUTOMATION_SELECTED,
	SKINBOX_GLOBALVIEW_AUDIO_SELECTED,
	SKINBOX_EDITING_PATTERN,
	SKINBOX_EDITING_AUTOMATION,
};
				    
				    	    
				    
class VisualSettings{
	
	static VisualSettings * singleton;
			    
	typedef std::map<SkinBoxList,SkinBox*> SkinBoxMap;
	SkinBoxMap skin_box_map;
	PixmapFont pattern_font;
	int editing_row_height;
public:
	
	int get_editing_row_height();
	PixmapFont* get_pattern_font();
	static VisualSettings * get_singleton();
	
	SkinBox * get_skin_box(SkinBoxList p_which);
	
	VisualSettings();
	~VisualSettings();

};

}

#endif
