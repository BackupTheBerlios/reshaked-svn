


#include "audio_node.h"


namespace ReShaked {




String AudioNode::get_input_plug_caption(int p_which) {
	
	if (get_input_plug_count()==1) 
		return "In";
	else
		return String("In "+String::num(p_which+1));
}

String AudioNode::get_out_plug_caption(int p_which) {
	
	
	if (get_output_plug_count()==1) 
		return "Out";
	else
		return String("Out "+String::num(p_which+1));
	
}



}





