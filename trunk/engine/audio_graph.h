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
		CONNECT_OK, ///< Connection Performed Succesfully
		CONNECT_INVALID_PLUG, ///< An invalid plug (not in range of the node) was specified
		CONNECT_INVALID_NODE, ///< An invalid node (not in 0 .. get_node_count()-1 ) was specified
		CONNECT_CHANNELS_DIFFER, ///< The plugs being connected handle different amount of channels (ie stereo/mono)
		CONNECT_ALREADY_EXISTS, ///< The connection already exists and cant be made twice
		CONNECT_CYCLIC_LINK, ///< The connection will create a cyclic link in the graph, so it will fail
	};
private:	
	std::vector<AudioNode*> nodes;
	std::vector<int> process_order; /* process order of audionodes*/
	std::vector<Connection> connections;
	bool recompute_process_order(); /* return true if cyclic connection found */
	void recompute_graph();
	AudioGraphProcess graph_process;
	ConnectError last_error;
public:
	
	int get_ordered_node(int p_index);
	int get_node_count();
	AudioNode* get_node(int p_fromdex);
	int get_node_index(AudioNode* p_fromnode);
	
	/**
	 * Add a node to the Audio Graph.
	 * A list of connections can be optionally set. This is useful for Undo/Redo operations, as well sound driver switching.
	 * @param p_node Audio Node
	 * @param p_node_connections Connections to be set to this node upon creation (NULL for none)
	 */
	void add_node(AudioNode *p_node,std::list<Connection> *p_node_connections=NULL);
	/**
	 * Remove a node from the Audio Graph
	 * A list of connections that the node had before being deleted can be optionally retrieved. 
	 * This is useful for Undo/Redo operations, as well sound driver switching.
	 * @param p_index Node Index
	 * @param p_connections_lost Connections that will be lost upon deleting (NULL for not retrieving)
	 */
	void erase_node(int p_index,std::list<Connection> *p_connections_lost=NULL);
	
	/**
	 *  Make a connection between too nodes, from an OUTPUT plug to an INPUT plug.
	 * @param p_node_from Source Node
	 * @param p_plug_from Output Plug in source node
	 * @param p_node_to Destination Node
	 * @param p_plug_to Input Plug in destination node.
	 * @return  CONNECT_OK (equal zero) or a ConnectError otherwise
	 */
	ConnectError connect_plugs(int p_node_from,int p_plug_from,int p_node_to,int p_plug_to);
	
	
	/**
	 * Disconnect two plugs.
	 * @param p_node_from Source Node
	 * @param p_plug_from Output Plug in source node
	 * @param p_node_to Destination Node
	 * @param p_plug_to Input Plug in destination node.
	 */
	void disconnect_plugs(int p_node_from,int p_plug_from,int p_node_to,int p_plug_to);
	
	/**
	 * Get the last error code from a call to connect_plugs()
	 * @return CONNECT_OK (equal zero) or a ConnectError otherwise
	 */
	ConnectError get_last_conect_error();
	
	/**
	 * Get amount of connections in graph
	 * @return Amount of connections.
	 */
	int get_connection_count();
	
	/**
	 * Get a connection
	 * @param p_fromdex Connection index, from 0 to get_connection_count()
	 * @return the connection, or NULL on inalid connection.
	 */
	const Connection* get_connection(int p_fromdex);

	/**
	 *  Process the graph This is called from the audio thread.
	 * @param p_frames Amount of frames to process
	 * @return Amount of frames actually processed, equal or less to p_frames
	 */
	int process(int p_frames);
	
	AudioGraph();

};

}; /* end of namespace */

#endif
