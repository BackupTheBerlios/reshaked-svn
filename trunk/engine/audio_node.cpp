


#include "audio_node.h"


namespace ReShaked {



bool AudioNode::is_input_plug_enabled(int p_idx) {
	
	return true;
}

bool AudioNode::is_output_plug_enabled(int p_idx) {
	
	return true;
}

String AudioNode::get_input_plug_caption(int p_which) {
	
	if (get_input_plug_count()==1) 
		return "In";
	else
		return String("In "+String::num(p_which+1));
}

String AudioNode::get_output_plug_caption(int p_which) {
	
	
	if (get_output_plug_count()==1) 
		return "Out";
	else
		return String("Out "+String::num(p_which+1));
	
}

bool AudioNode::is_system() {
	
	return false;	
}

}





