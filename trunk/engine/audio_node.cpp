
#include "audio_node.h"


void ControlPort::set_visible(bool p_visible) {

	visible=p_visible;
}
bool ControlPort::is_visible() const {

	return visible;
}


void ControlPort::set_normalized(float p_val) {

	p_val*=get_max()-get_min();
	p_val+=get_min();
	set(p_val);

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

	visible=true;
}
ControlPort::~ControlPort() {

}

/*****************/

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


void AudioNode::set_layer(unsigned int p_layer) {

	if (p_layer==0)
		return; // don't let it dissapear
	_layer=p_layer;
}
unsigned int AudioNode::get_layer() const {

	return _layer;
}


AudioNode::AudioNode() {


	_x=-1;
	_y=-1;
	_layer=1;	
}

AudioNode::~AudioNode() {

}
