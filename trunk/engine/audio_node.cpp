
#include "audio_node.h"


void ControlPort::set_visible(bool p_visible) {

	visible=p_visible;
}
bool ControlPort::is_visible() const {

	return visible;
}


void ControlPort::set_normalized(float p_val,bool p_make_initial) {

	p_val*=get_max()-get_min();
	p_val+=get_min();
	set(p_val,p_make_initial);

}

float ControlPort::get_normalized() const {

	float v=get();
	v-=get_min();
	v/=get_max()-get_min();
	return v;
}

String ControlPort::get_value_as_text(float p_value) const {

	return String::num(get());
}


ControlPort::Hint ControlPort::get_hint() const {

	return HINT_RANGE;
}

ControlPort::ControlPort() {

	visible=false;
}
ControlPort::~ControlPort() {

}

/*****************/

String AudioNode::get_port_name( PortType p_type, PortFlow p_flow,int p_port ) const {

	String str;
	
	switch (p_type) {
	
		case PORT_AUDIO: {
			
			str="Audio";			
		} break;
		case PORT_EVENT: {
			
			str="Event";			
		} break;
		case PORT_CONTROL: {
			
			if (p_port>=0 && p_port<get_port_count( PORT_CONTROL, p_flow )) {
			
				AudioNode *an=(AudioNode*)this;
			
				ControlPort *cp=an->get_control_port( p_flow, p_port );
				ERR_FAIL_COND_V( !cp, "");
				
				return cp->get_name();
			
			}
			str="Control";			
		} break;
	}
	
	switch (p_flow) {
	
		case PORT_IN: {
		
			str+=" IN";
		} break;
		case PORT_OUT: {
		
			str+=" OUT";
		} break;
	}
	
	
	str+=" #"+String::num(p_port);
	
	return str;
}
ControlPort* AudioNode::get_control_port_by_name( PortFlow p_flow,String p_name ) {

	for (int i=0;i<get_port_count( PORT_CONTROL, p_flow);i++) {
	
		if (get_control_port( p_flow , i )->get_name()==p_name )
			return get_control_port( p_flow , i );
	}
	
	return NULL;
}

void AudioNode::graph_enter() {

}
void AudioNode::graph_exit() {

}

String AudioNode::get_name() const {
	
	return _name;
}

void AudioNode::set_name(String p_name) {
	
	_name=p_name;
}

void AudioNode::set_x(int p_x) {

	_x=p_x;
}
int AudioNode::get_x() const {

	return _x;
}

void AudioNode::set_y(int p_y) {

	_y=p_y;
}
int AudioNode::get_y() const {

	return _y;
}


void AudioNode::set_layer(int p_layer) {

	if (p_layer==0)
		return; // don't let it dissapear
	_layer=p_layer;
}
int AudioNode::get_layer() const {

	return _layer;
}

Error AudioNode::save_file( String p_filename ) {

	TreeSaver *ts=TreeSaver::create();
	Error err = ts->save( "PRESET: "+get_info()->unique_ID, p_filename);
	if (err)
		delete ts;
	ERR_FAIL_COND_V( err, err );
	
	ts->add_int( "version", get_info()->version );
	ts->enter("data");
	save( ts );
	ts->exit();
	ts->close();
	
	delete ts;
	return OK;
}

Error AudioNode::load_file( String p_filename ) {

	TreeLoader *tl=TreeLoader::create();
	Error err = tl->open( "PRESET: "+get_info()->unique_ID, p_filename);
	if (err)
		delete tl;
	ERR_FAIL_COND_V( err, err );
	
	if (tl->get_int("version")>get_info()->version ) {
		
		delete tl;
		return ERR_VERSION_MISMATCH;
	}
	tl->enter("data");
	load( tl );
	tl->exit();
	tl->close();
	
	delete tl;
	return OK;
}

void AudioNode::set_ui_data(AudioNodeUIData *p_ui_data) {

	_ui_data=p_ui_data;
}
AudioNodeUIData *AudioNode::get_ui_data() {

	return _ui_data;
}

AudioNode::AudioNode() {


	_x=-1;
	_y=-1;
	_layer=1;	
	_ui_data=NULL;
}

AudioNode::~AudioNode() {

	if (_ui_data)
		delete _ui_data;
}
