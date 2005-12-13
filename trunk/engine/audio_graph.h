#ifndef AUDIO_GRAPH_H
#define AUDIO_GRAPH_H

#include "engine/audio_node.h"
#include "engine/audio_graph_process.h"
#include <vector>
#include <list>
/**
 * The core of the modular nature of this sequencer
 * It was programmed with simplicity of use and code,
 * but powerful in the sense that it does what it should.
 */

namespace ReShaked {

class AudioGraph {
public:
	struct Connection {
	
		int node_from;
		int plug_from;
		int node_to;
		int plug_to;
	};

	enum ConnectError {
		CONNECT_OK,
		CONNECT_INVALID_PLUG,
		CONNECT_INVALID_NODE,
		CONNECT_CHANNELS_DIFFER,
		CONNECT_ALREADY_EXISTS,
		CONNECT_CYCLIC_LINK,
	};
private:	
	std::vector<AudioNode*> nodes;
	std::vector<int> process_order; /* process order of audionodes*/
	std::vector<Connection> connections;
	bool recompute_process_order(); /* return true if cyclic connection found */
	void recompute_graph();
	AudioGraphProcess graph_process;
public:
	
	int get_node_count();
	AudioNode* get_node(int p_fromdex);
	int get_node_index(AudioNode* p_fromnode);
	void add_node(AudioNode *p_node,std::list<Connection> *p_node_connections=NULL);
	void erase_node(int p_fromdex,std::list<Connection> *p_connections_lost=NULL);
	
	ConnectError connect_plugs(int p_node_from,int p_plug_from,int p_node_to,int p_plug_to);
	void disconnect_plugs(int p_node_from,int p_plug_from,int p_node_to,int p_plug_to);
	
	int get_connection_count();
	const Connection* get_connection(int p_fromdex);

	void process(int p_frames);

};

}; /* end of namespace */

#endif
