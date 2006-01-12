

#include "audio_graph_process.h"

namespace ReShaked {

/**
 * Possible optimization: move unused input buffers to silence buffers 
 * Use pointers to array instead of vector[] ?
 */
void AudioGraphProcess::clear() {

	for (int i=0;i<(int)process_nodes.size();i++) {

		for (int j=0;j<(int)process_nodes[i]->input_buffers.size();j++) {
			
			if (process_nodes[i]->input_buffers[j])
				delete process_nodes[i]->input_buffers[j];
		}
		for (int j=0;j<(int)process_nodes[i]->output_buffers.size();j++) {
			
			if (process_nodes[i]->output_buffers[j])
				delete process_nodes[i]->output_buffers[j];
		}
		
		delete process_nodes[i];		
	}
	
	process_nodes.clear();
	
	for (int i=0;i<(int)thrash_buffers.size();i++) {
		
		delete thrash_buffers[i];
	}
	thrash_buffers.clear();
	
	for (int i=0;i<(int)silence_buffers.size();i++) {
		
		delete silence_buffers[i];
	}
	silence_buffers.clear();
	
	
	
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
	AudioBuffer * new_buffer = new AudioBuffer(p_channels,BUFFER_SIZE);
	thrash_buffers.push_back(new_buffer);
	
	return new_buffer;
}

AudioBuffer* AudioGraphProcess::get_silence_buffer_for_given_channels(int p_channels) {
	
	/* if we have one, give back that */
	for (int i=0;i<(int)silence_buffers.size();i++) {
		
		if (silence_buffers[i]->get_channels()==p_channels) {
			
			return silence_buffers[i];
		}
	}
	/* if not, give what we have */
	AudioBuffer * new_buffer = new AudioBuffer(p_channels,BUFFER_SIZE);
	silence_buffers.push_back(new_buffer);
	
	return new_buffer;
}

void AudioGraphProcess::add_node(AudioNode *p_node) {
	
	ProcessNode * process_node = new ProcessNode;
	
	process_node->node=p_node;

	/* by default the input plugs read from silence buffers */
	process_node->input_buffers.resize(p_node->get_input_plug_count());
	for (int i=0;i<p_node->get_input_plug_count();i++) {
		
		int plug_channels=p_node->get_input_plug(i)->get_channels();
		AudioBuffer* silence_buff=get_silence_buffer_for_given_channels(plug_channels);
		p_node->get_input_plug(i)->set_buffer(silence_buff);
		process_node->input_buffers[i]=NULL; //not connected by default, reads from silenace buffer
		
	}		
	/* by default, set the output plugs to write from thrash buffers */
	
	process_node->output_buffers.resize(p_node->get_output_plug_count());
	for (int i=0;i<p_node->get_output_plug_count();i++) {
		
		int plug_channels=p_node->get_output_plug(i)->get_channels();
		AudioBuffer* thrash_buff=get_trash_buffer_for_given_channels(plug_channels);
		p_node->get_output_plug(i)->set_buffer(thrash_buff);
		process_node->output_buffers[i]=NULL; //not connected by default, writes to thrash buffer
	}		
	/* We'll store hoe many sources does each input have here */
	process_node->input_sources.resize(p_node->get_input_plug_count());
	process_nodes.push_back(process_node);
}

/* Then Add Connections */
void AudioGraphProcess::add_connection(int p_node_from, int p_plug_from, int p_node_to,int p_plug_to) {
	
	//printf("connecting %lls %i to %lls %i\n",process_nodes[p_node_from]->node->get_caption().c_str(),p_plug_from,process_nodes[p_node_to]->node->get_caption().c_str(),p_plug_to);
	
	ERR_FAIL_INDEX(p_node_from,(int)process_nodes.size()); 
	ERR_FAIL_INDEX(p_node_to,(int)process_nodes.size());
	ERR_FAIL_INDEX(p_plug_from,process_nodes[p_node_from]->node->get_output_plug_count());
	ERR_FAIL_INDEX(p_plug_to,(int)process_nodes[p_node_to]->input_buffers.size());
	
	/* FIRST: Check if source node (an output) has a writing-to buffer, if not, create it  */
	
	if (process_nodes[p_node_from]->output_buffers[p_plug_from]==NULL) {
		
		int chans=process_nodes[p_node_from]->node->get_output_plug(p_plug_from)->get_channels();
		AudioBuffer * buff = new AudioBuffer(chans,BUFFER_SIZE);
		process_nodes[p_node_from]->node->get_output_plug(p_plug_from)->set_buffer(buff);
		process_nodes[p_node_from]->output_buffers[p_plug_from]=buff;
		
	}
	
	/* SECOND - Check if destination node (INPUT plug) is either empty/reads from buffer(1 connection) or reads from many */
	
	if (process_nodes[p_node_to]->input_sources[p_plug_to].empty()) { 
		/** No one connected to this node 
		 *  Since so far after this it will have only ONE connection, 
		 *  we can set it to read directly.
		 */
		AudioBuffer *buff=process_nodes[p_node_from]->output_buffers[p_plug_from];
		process_nodes[p_node_to]->node->get_input_plug(p_plug_to)->set_buffer(buff);
		process_nodes[p_node_to]->input_sources[p_plug_to].push_back(buff);
		
		
	} else {
		
		/**
		 * IF it only has one connection, it means it's reading directly.
		 * Since now it's going to have more than one, we need a buffer to
		 * mix the data from the source connections.
		 */
		
		if (process_nodes[p_node_to]->input_sources[p_plug_to].size()==1) {
			
			int chans=process_nodes[p_node_to]->node->get_input_plug(p_plug_to)->get_channels();
			AudioBuffer * buff=new AudioBuffer(chans,BUFFER_SIZE);
			ERR_FAIL_COND(process_nodes[p_node_to]->input_buffers[p_plug_to]!=NULL);
			process_nodes[p_node_to]->input_buffers[p_plug_to]=buff;
			process_nodes[p_node_to]->node->get_input_plug(p_plug_to)->set_buffer(buff);
		}
		
		/**
		 * Add the Source Buffer to the listof Source Buffers
		 */
		
		AudioBuffer *src_buff=process_nodes[p_node_from]->output_buffers[p_plug_from];
		process_nodes[p_node_to]->input_sources[p_plug_to].push_back(src_buff);
		
	}
	
}


void AudioGraphProcess::configure_connections() {
	
	unconfigured=false;
	
}

int AudioGraphProcess::process(int p_frames) {
	 
	if (unconfigured)
		return p_frames; // nothing processed
	
	if (p_frames>BUFFER_SIZE)
		p_frames=BUFFER_SIZE;
	
	
	/**
	 * Process the audio in the topologically solved graph 
	 */
	
	for (int i=0;i<(int)process_nodes.size();i++) {
		
		/** STEP 1: Check the Input Buffers.. */
		
		for (int j=0;j<process_nodes[i]->input_buffers.size();j++) {
			
			AudioBuffer *buff=process_nodes[i]->input_buffers[j];
			
			if (buff==NULL)
				continue; ///< Doesnt use an input buffer
		  
			/** STEP 2 - CLEAR The buffer */
			
			buff->clear(p_frames);
			
			/** STEP 3 - MIX The outputs into this input */
					 
			for (int k=0;k<process_nodes[i]->input_sources[j].size();k++) {
				
				buff->add_from(process_nodes[i]->input_sources[j][k],p_frames);
			}
		}
		
		process_nodes[i]->node->process(p_frames);
	}


	/* done ! */
		
	return p_frames;
}

AudioGraphProcess::AudioGraphProcess() {
	
	unconfigured=true;


}
	
	
AudioGraphProcess::~AudioGraphProcess() {
	
	
	clear();
}
	
};














