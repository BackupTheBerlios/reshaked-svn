//
// C++ Implementation: register_node_uis
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "register_node_uis.h"

#include "generic_node_ui.h"
#include "gui_common/audio_driver_node_ui.h"
#include "gui_common/node_ui_registry.h"
#include "gui_nodes/hq_reverb_node_ui.h"
#include "gui_nodes/chorus_node_ui.h"
#include "gui_nodes/compressor_node_ui.h"
#include "gui_nodes/filter_bank_node_ui.h"

void register_node_uis() {

	NodeUI_Registry::add_create_ui_func(HQ_ReverbNodeUI::ui_create_func);
	NodeUI_Registry::add_create_ui_func(ChorusNodeUI::ui_create_func);
	NodeUI_Registry::add_create_ui_func(CompressorNodeUI::ui_create_func);
	NodeUI_Registry::add_create_ui_func(FilterBankNodeUI::ui_create_func);
	
	NodeUI_Registry::add_create_ui_func(AudioDriverNodeUI::ui_create_func);
	
	NodeUI_Registry::add_create_ui_func(GenericNodeUI::ui_create_func);
}
