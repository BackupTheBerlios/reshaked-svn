//
// C++ Interface: audio_graph
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef AUDIO_GRAPH_H
#define AUDIO_GRAPH_H


#include "engine/audio_graph_process.h"
#include <list>
#include "typedefs.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class AudioGraph{
public:

	enum ConnectError {
		CONNECT_OK, ///< AudioConnection Performed Succesfully
		CONNECT_INVALID_PORT, ///< An invalid port (not in range of the node) was specified
		CONNECT_INVALID_NODE, ///< An invalid node (not in 0 .. get_node_count()-1 ) was specified
		CONNECT_CHANNELS_DIFFER, ///< The ports being connected handle different amount of channels (ie stereo/mono)
		CONNECT_ALREADY_EXISTS, ///< The connection already exists and cant be made twice
		CONNECT_CYCLIC_LINK, ///< The connection will create a cyclic link in the graph, so it will fail
	};	
private:
	
	std::vector<AudioNode*> nodes;
	std::vector<int> process_order; /* process order of audionodes position -> node*/
	std::vector<int> inv_process_order; /* process order of audionodes, node -> position*/
	std::vector<AudioConnection> connections;
	AudioGraphProcess graph_process;
	ConnectError last_error;
	bool graph_order_valid;

	bool recompute_process_order(); /* return true if cyclic connection found */
	void recompute_graph();
		
	float sampling_rate;
public:

	
	int get_ordered_node(int p_index); ///< Get node in process orderd
	
	int get_node_count(); ///< Get amount of nodes in Graph
	AudioNode* get_node(int p_fromdex); ///< Get node
	int get_node_index(AudioNode* p_fromnode); ///< Get index of node
	
	void swap_node_indices(int p_idx,int p_with_idx);
	/**
	 * Add a node to the Audio Graph.
	 * A list of connections can be optionally set. This is useful for Undo/Redo operations, as well sound driver switching.
	 * @param p_node Audio Node
	 */
	void add_node(AudioNode *p_node,int p_at_index=-1);
	/**
	 * Remove a node from the Audio Graph
	 * A list of connections that the node had before being deleted can be optionally retrieved. 
	 * This is useful for Undo/Redo operations, as well sound driver switching.
	 * @param p_index Node Index
	 * @param p_connections_lost AudioConnections that will be lost upon deleting (NULL for not retrieving)
	 */
	void erase_node(int p_index);
	
	/**
	 *  Make a connection between too nodes, from an OUTPUT port to an INPUT port.
	 */
	ConnectError connect(const AudioConnection& p_connection);
	
	/**
	 * Disconnect two ports.
	 */
	
	void disconnect(const AudioConnection& p_connection);
	
	bool has_connection(const AudioConnection& p_connection);
	
	bool can_connect(const AudioConnection& p_connection);

	/**
	 * Get the last error code from a call to connect()
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
	 * @param p_fromdex AudioConnection index, from 0 to get_connection_count()
	 * @return the connection, or NULL on inalid connection.
	 */
	const AudioConnection* get_connection(int p_fromdex);

	/**
	 *  Process the graph This is called from the audio thread.
	 * @param p_frames Amount of frames to process
	 * @return Amount of frames actually processed, equal or less to p_frames
	 */
	int process(int p_frames);
	
	void clear();
	
	void set_mix_rate(float p_hz);
	
	AudioGraph();
	~AudioGraph();

};

#endif
