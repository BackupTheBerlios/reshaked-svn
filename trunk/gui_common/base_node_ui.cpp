//
// C++ Implementation: base_node_ui
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "base_node_ui.h"
#include "common_skin.h" 
#include "widgets/label.h"
#include "gui_common/node_ui_label.h"
#include "gui_common/node_ui_value.h"
#include "widgets/knob.h"
#include "gui_common/common_skin.h"
#include "containers/grid_container.h"
#include "containers/center_container.h"
#include "widgets/label.h"
#include "widgets/slider.h"
#include "gui_common/control_port_range.h"
#include "gui_common/button_port_edit.h"

class _GenericNodeUI_KnobMargin : public GUI::Widget {

	virtual GUI::Size get_minimum_size_internal() {
	
		return GUI::Size( constant(C_GENERIC_NODE_UI_KNOB_MARGIN), 0 );
	}

public:
};

class _GenericNodeUI_VSliderMargin : public GUI::Widget {

	virtual GUI::Size get_minimum_size_internal() {
	
		return GUI::Size( constant(C_GENERIC_NODE_UI_VSLIDER_MARGIN), 0 );
	}

public:
};

static String _fix_name(String p_name) {

	int idx=p_name.find("/");
	if (idx!=-1) {
	
		return p_name.substr(idx+1,p_name.length() );
	}
	
	return p_name;
}

GUI::Frame* BaseNodeUI::make_knob(ControlPort *p_port) {

	GUI::VBoxContainer *vbc = new GUI::VBoxContainer;

	ControlPort *cp = p_port;
	
	NodeUI_Label * label = vbc->add( new NodeUI_Label );
	label->set_text( _fix_name(cp->get_name()) );
	
	ControlPortRange *r=new ControlPortRange( cp );
	register_range_for_updates(r);
	
	GUI::HBoxContainer *knob_hbc = vbc->add( new GUI::HBoxContainer );
	
	knob_hbc->add( new _GenericNodeUI_KnobMargin );
	GUI::Knob *knob = knob_hbc->add( new GUI::Knob );		
	knob_hbc->add( new _GenericNodeUI_KnobMargin );
	
	knob->set_range( r );
	
	
	NodeUI_Value * value = vbc->add( new NodeUI_Value );
	
	if (r)
		value->set_range( r, true );

	return vbc;
}

GUI::Frame* BaseNodeUI::make_knob(String p_port) {

	ControlPort *cp=_node->get_control_port_by_name( AudioNode::PORT_IN, p_port );
	
	ERR_FAIL_COND_V( !cp, new GUI::Widget );
	
	return make_knob( cp );	
	
}

GUI::Frame* BaseNodeUI::make_vslider(ControlPort *p_port) {

	GUI::VBoxContainer *vbc = new GUI::VBoxContainer;
	GUI::HBoxContainer *hbc = vbc->add( new GUI::HBoxContainer,1 );
	
	hbc->add( new _GenericNodeUI_VSliderMargin );
	NodeUI_Label * label = hbc->add( new NodeUI_Label( NodeUI_Label::VERTICAL ) );
	label->set_text( _fix_name(p_port->get_name()) );
	ControlPort *cp = p_port;
	ControlPortRange *r=new ControlPortRange( cp );
	register_range_for_updates(r);
	
	GUI::VSlider*vslider = hbc->add( new GUI::VSlider );
	
	hbc->add( new _GenericNodeUI_VSliderMargin );
	
	vslider->set_range( r );
	vslider->set_minimum_size( GUI::Size( 0, constant( C_GENERIC_NODE_UI_VSLIDER_MIN_HEIGHT ) ) );
		
	NodeUI_Value * value = vbc->add( new NodeUI_Value );
	
	if (r)
		value->set_range( r, true );


	return vbc;		

}

GUI::Frame* BaseNodeUI::make_vslider(String p_port) {

	ControlPort *cp=_node->get_control_port_by_name( AudioNode::PORT_IN, p_port );
	
	ERR_FAIL_COND_V( !cp, new GUI::Widget );
	
	return make_vslider( cp );	
	
}


GUI::Frame* BaseNodeUI::make_button(ControlPort *p_port) {

	GUI::CenterContainer *cc = new GUI::CenterContainer;
	
	ButtonPortEdit *bpe = cc->set( new ButtonPortEdit );
	bpe->set_range( new ControlPortRange( p_port ), true );
	bpe->set_text( _fix_name( p_port->get_name() ) );
	return cc;

}
GUI::Frame* BaseNodeUI::make_button(String p_port) {

	ControlPort *cp=_node->get_control_port_by_name( AudioNode::PORT_IN, p_port );
	
	ERR_FAIL_COND_V( !cp, new GUI::Widget );
	
	return make_button( cp );	

}


void BaseNodeUI::register_range_for_updates(ControlPortRange *p_range) {

	updatable_ranges.push_back(p_range);
}

void BaseNodeUI::presets_callback() {

	edited_signal.call(_node);
}

void BaseNodeUI::check_ranges_user() {


}

void BaseNodeUI::check_ranges() {

	for (std::list<ControlPortRange*>::iterator I=updatable_ranges.begin(); I != updatable_ranges.end() ; I++ )
		(*I)->check_for_changes();
		
	check_ranges_user();
}

BaseNodeUI::BaseNodeUI(AudioNode *p_node) {

	_node=p_node;
	GUI::HBoxContainer *hbc= add( new GUI::HBoxContainer );
	hbc->add( new GUI::Widget, 1 );
	GUI::Button *preset = hbc->add( new GUI::Button );
	preset->pressed_signal.connect( this, &BaseNodeUI::presets_callback );
	preset->add_bitmap_override( GUI::BITMAP_BUTTON_DEFAULT_ICON, BITMAP_GRAPH_NODE_PRESET );
			
	
}


BaseNodeUI::~BaseNodeUI() {

	
}


