#ifndef AUDIO_GRAPH_PROCESS_H
#define AUDIO_GRAPH_PROCESS_H

#include "engine/audio_node.h"

namespace ReShaked {

/**
 * This is an extension to AudioGraph that does the actual processing.
 * Only AudioGraph can use it, and onl
 */

class AudioGraphProcess {

	enum {
		
		BUFFER_SIZE=4096 /* Internal buffer size */
	};
	
	struct ProcessNode {
		/* Audio Node Being Processed */
		AudioNode *node;
	
		/* Which buffer will the output copy+add to */
		std::vector<AudioBuffer*> input_buffers; /* Buffer for each Input */
	};


	
	std::vector<ProcessNode*> process_nodes;
	std::vector<AudioBuffer*> thrash_buffers; /* Thrash Buffers, for sending unconnected outputs */

	bool unconfigured;	

	AudioBuffer* get_trash_buffer_for_given_channels(int p_channels);
	
friend class AudioGraph;	
	
	void clear();
	/* Add nodes first, in order */
	void add_node(AudioNode *p_node);
	/* Then Add Connections */
	void add_connection(int p_node_from, int p_plug_from, int p_node_to,int p_plug_to);

	/* reconnect everything, creates internal buffers and everything */
	void configure_connections(); 
		
	/* process */
	void process(int p_frames);
};


};


#endif
