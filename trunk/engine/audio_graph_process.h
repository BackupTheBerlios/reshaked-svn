#ifndef AUDIO_GRAPH_PROCESS_H
#define AUDIO_GRAPH_PROCESS_H

#include <vector>
#include "engine/audio_node.h"
#include "engine/music_event.h"
#include "engine/audio_connection.h"
#include "error_macros.h"

/**
 * This is an extension to AudioGraph that does the actual processing.
 * Only AudioGraph can use it, and onl
 */

class AudioGraphProcess {

	enum {
		
		AUDIO_BUFFER_SIZE=512, /* Internal buffer size */
		EVENT_BUFFER_SIZE=256
	};
	
	
	struct AudioBuffer {
	
		std::vector< std::vector<sample_t> > buffer; // buffer for each chan
				
		void connect_in(AudioNode *p_node,int p_port) {
		
			ERR_FAIL_INDEX( p_port, p_node->get_port_count( AudioNode::PORT_AUDIO, AudioNode::PORT_IN ) );
			ERR_FAIL_COND( p_node->get_audio_port_channel_count( AudioNode::PORT_IN, p_port) != buffer.size() );
			for (int i=0;i<buffer.size();i++) {
			
				p_node->connect_audio_buffer( AudioNode::PORT_IN, p_port,i, &buffer[i][0] );
			}
		}
		
		void connect_out(AudioNode *p_node,int p_port) {
		
			ERR_FAIL_INDEX( p_port, p_node->get_port_count( AudioNode::PORT_AUDIO, AudioNode::PORT_OUT ) );
			ERR_FAIL_COND( p_node->get_audio_port_channel_count( AudioNode::PORT_OUT, p_port) != buffer.size() );
			for (int i=0;i<buffer.size();i++) {
			
				p_node->connect_audio_buffer( AudioNode::PORT_OUT, p_port,i, &buffer[i][0] );
			}
		}
		
		
		inline void clear(int p_frames=AUDIO_BUFFER_SIZE) {
		
			ERR_FAIL_INDEX(p_frames,AUDIO_BUFFER_SIZE);
			
			
			for (int i=0;i<buffer.size();i++) {
			
				sample_t *buf=&buffer[i][0];
				for (int j=0;j<p_frames;j++)
					buf[j]=0;
			}
		
		}
		
		inline void add_from(AudioBuffer *p_src,int p_frames=AUDIO_BUFFER_SIZE) {
		
			ERR_FAIL_INDEX(p_frames,AUDIO_BUFFER_SIZE);
			ERR_FAIL_COND(buffer.size()!=p_src->buffer.size());
			
			
			for (int i=0;i<buffer.size();i++) {
			
				sample_t *dst_buf=&buffer[i][0];
				sample_t *src_buf=&p_src->buffer[i][0];
				
				for (int j=0;j<p_frames;j++)
					dst_buf[j]+=src_buf[j];
			}
		
		}
		
		
		AudioBuffer(int p_chans) {
		
			buffer.resize(p_chans);
			for (int i=0;i<buffer.size();i++)
				buffer[i].resize( AUDIO_BUFFER_SIZE );				
		
		}
	};
	
	
	typedef std::vector<MusicEvent> EventBuffer;
	
	struct ProcessNode {
		/* Audio Node Being Processed */
		AudioNode *node;
	
	
		/* Buffer for each input/output. Empty if not connected */
		
		// Audio
		std::vector< AudioBuffer* > output_audio_buffers; /* Buffer for each Output */
		std::vector< AudioBuffer* > input_audio_buffers; /* Buffer for each Input */
		std::vector< std::vector< AudioBuffer*> > input_audio_sources;
		
		// Event
		std::vector<EventBuffer*> output_event_buffers; /* Buffer for each Output */
		std::vector<EventBuffer*> input_event_buffers; /* Buffer for each Input */
		std::vector< std::vector<EventBuffer*> > input_event_sources;
		
		// Control
		
		struct ControlConnect {
		
			int node_from;
			int port_from;
			int port_to;
		};
		
		std::vector<ControlConnect> control_connections;
	};


	AudioNodeConfig audio_config;
	
	std::vector<ProcessNode*> process_nodes;
	std::vector<sample_t> audio_trash_buffer;
	std::vector<sample_t> audio_silence_buffer;
	
	EventBuffer event_trash_buffer;
	EventBuffer event_silence_buffer;

	bool unconfigured;	
	
friend class AudioGraph;	
	
	void clear();
	/* Add nodes first, in order */
	void add_node(AudioNode *p_node);
	/* Then Add Connections */
	void add_connection(const AudioConnection& p_connection);

	/* reconnect everything, creates internal buffers and everything */
	void configure_connections(); 
		
	/* process */
	int process(int p_frames);
		
	AudioGraphProcess();
	~AudioGraphProcess();
};




#endif
