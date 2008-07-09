//
// C++ Interface: preset_browser
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PRESET_BROWSER_H
#define PRESET_BROWSER_H


#include "base/window.h"
#include "engine/audio_node.h"
#include "widgets/tree.h"
#include "bundles/window_box.h"
#include "base/file_system.h"
#include "bundles/input_dialog.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class PresetBrowser : public GUI::Window {

	enum Operation {
	
		OP_OPEN,
		OP_SAVE,
		OP_MKDIR,
		OP_RENAME,
		OP_ERASE,		
	};

	Operation current_op;
	
	GUI::WindowBox *window_box;
	GUI::Tree *tree;
	
	String current_path;

	void parse_dir( GUI::FileSystem *p_fs, GUI::TreeItem *p_item );
	void rescan_tree();
	
	GUI::StringInputDialog *string_input;
	
	String get_selected();
	
	void op_callback( Operation p_op );
	void dialog_callback(String p_text);

public:

	void edit_preset( AudioNode *p_node );

	PresetBrowser(GUI::Window *p_parent);
	~PresetBrowser();

};

#endif
