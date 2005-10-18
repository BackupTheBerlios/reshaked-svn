

#include "audio_graph_process.h"

namespace ReShaked {

/**
 * Possible optimization: move unused input buffers to silence buffers 
 * Use pointers to array instead of vector[] ?
 */
void AudioGraphProcess::clear() {

	for (int i=0;i<(int)process_nodes.size();i++) {

		for (int j=0;j<(int)process_nodes[i]->input_buffers.size();j++) {
			
			delete process_nodes[i]->input_buffers[j];
		}
		delete process_nodes[i];		
	}
	
	process_nodes.clear();
	
	for (int i=0;i<(int)thrash_buffers.size();i++) {
		
		delete thrash_buffers[i];
	}
	thrash_buffers.clear();
	unconfigured=true;
}
/* Add nodes first, in order */


AudioBuffer* AudioGraphProcess::get_trash_buffer_for_given_channels(int p_channels) {
	
	/* if we have one, give back that */
	for (int i=0;i<(int)thrash_buffers.size();i++) {
		
		if (thrash_buffers[i]->get_channels()==p_channels) {
			
			return thrash_buffers[i];
		}
	}
	/* if not, give what we have */
	AudioBuffer * new_buffer = new AudioBuffer(BUFFER_SIZE,p_channels);
	thrash_buffers.push_back(new_buffer);
	
	return new_buffer;
}

void AudioGraphProcess::add_node(AudioNode *p_node) {
	
	ProcessNode * process_node = new ProcessNode;
	
	process_node->node=p_node;

	/* add channel input buffers */
	for (int i=0;i<p_node->get_input_plug_count();i++) {
		
		AudioBuffer * in_buff=new AudioBuffer(BUFFER_SIZE,p_node->get_input_plug(i)->get_channels());
		process_node->input_buffers.push_back(in_buff);
		p_node->get_input_plug(i)->set_buffer(in_buff);
		
	}		
	/* by default, set the output plugs to write to the thrash buffer */
	
	for (int i=0;i<p_node->get_output_plug_count();i++) {
		
		int plug_channels=p_node->get_output_plug(i)->get_channels();
		AudioBuffer* thrash_buff=get_trash_buffer_for_given_channels(plug_channels);
		p_node->get_output_plug(i)->set_buffer(thrash_buff);
	}		
	
	process_nodes.push_back(process_node);
}

/* Then Add Connections */
void AudioGraphProcess::add_connection(int p_node_from, int p_plug_from, int p_node_to,int p_plug_to) {
	
	ERR_FAIL_INDEX(p_node_from,(int)process_nodes.size());
	ERR_FAIL_INDEX(p_node_to,(int)process_nodes.size());
	ERR_FAIL_INDEX(p_plug_from,process_nodes[p_node_from]->node->get_output_plug_count());
	ERR_FAIL_INDEX(p_plug_to,(int)process_nodes[p_node_to]->input_buffers.size());
	
	
	/* Perform the connection */
	AudioPlug *from=process_nodes[p_node_from]->node->get_output_plug(p_plug_from);
	from->set_buffer(process_nodes[p_node_to]->input_buffers[p_plug_to]);
	
}

/* Then Add Connections */
void AudioGraphProcess::configure_connections() {
	
	unconfigured=false;
	
}

void AudioGraphProcess::process(int p_frames) {
	 
	if (unconfigured)
		return;
	
	/* pass 1, clean the input buffers */
	for (int i=0;i<(int)process_nodes.size();i++)
		for (int j=0;j<(int)process_nodes[i]->input_buffers.size();j++)
			process_nodes[i]->input_buffers[j]->clear();
					
	
	/* pass 2, process the audio in the topologically solved graph */
	for (int i=0;i<(int)process_nodes.size();i++) 
		process_nodes[i]->node->process(p_frames);


	/* done ! */
		
}




};
