
#include "audio_graph.h"
#include "error_macros.h"

namespace ReShaked {



bool AudioGraph::recompute_process_order() {

	/* This is my own algorithm for non-recursive graph solving, I call it Squirrel-Graph-Sort  */
	
	/* Fill the domain/image inverted process order list 
	   this list maps node->order, this is why it must
	   be filled with the inverted numbers */
	
	std::vector<int> inv_process_order; 
	inv_process_order.resize(nodes.size());
	for (int i=0;i<(int)nodes.size();i++)
		inv_process_order[i]=i;
	
	
	int iterations=0;
	int swaps;
	
	int worst_case=POW2(connections.size()); //worst case of a bubblesort is pow2
	do {
		swaps=0;
		
		
		for (int i=0;i<(int)connections.size();i++) {
			
			Connection conn=connections[i];
			
			if (inv_process_order[conn.node_from] > inv_process_order[conn.node_to]) {
				
				/* Swap */
			
				int aux=inv_process_order[conn.node_from];
				inv_process_order[conn.node_from]=inv_process_order[conn.node_to];
				inv_process_order[conn.node_to]=aux;
				swaps++;				
			}
		}
		
		iterations++;
	
	} while (iterations<=worst_case && swaps>0);
	/* I'm guessing as a worst case that the maximum iterations (worst case) 
	   is the amount of connections to process, just like the bubble sort, and if it
	   goes beyond this, then there must be a cyclic link. Another possibility
	   to detect cyclic links may be comparing if the input and output are the same 
	   and swaps>0.
	
	*/
	
	printf("Iterations needed to solve graph: %i ( %i being worst case  )\n",iterations,worst_case);
	
	if (iterations>=(int)nodes.size()) {
		
		printf("graph solving FAILED.\n");
		return true; /* we fucked it */
	}

	process_order.resize( inv_process_order.size() );

	/* invert domain/image and place into process_order */
	for (int i=0;i<(int)nodes.size();i++) {

		process_order[ inv_process_order[i] ] = i;
	}
	
	return false;
}


void AudioGraph::recompute_graph() {

	graph_process.clear();
	for (int i=0;i<nodes.size();i++)
		graph_process.add_node(nodes[i]);
	
	for (int i=0;i<connections.size();i++)
		graph_process.add_connection(
			connections[i].node_from,
			connections[i].plug_from,
			connections[i].node_to,
			connections[i].plug_to
			);
	
	graph_process.configure_connections();
	
}


int AudioGraph::get_node_count() {

	return nodes.size();	

}

AudioNode* AudioGraph::get_node(int p_index) {

	ERR_FAIL_INDEX_V(p_index,(int)nodes.size(),NULL);
	return nodes[p_index];
}

void AudioGraph::add_node(AudioNode *p_node,std::list<Connection> *p_node_connections) {

	nodes.push_back(p_node);
	if (p_node_connections) {
		
		foreach(I,(*p_node_connections)) {
			
			if (I->node_from==-1)
				I->node_from=nodes.size()-1;
			if (I->node_to==-1)
				I->node_to=nodes.size()-1;
			
			ERR_CONTINUE( connect_plugs( I->node_from,I->plug_from,I->node_to,I->plug_to) );
		}
		
	}
}


/**
 * Helper to adjust an index from the array, according to a deleted node. If the index
 * is greater than the deleted note, the index is decremented by 1
 * @param p_index index to adjust
 * @param p_deleted index being deleted
 * @return true if index to adjust equals index being deleted
 */
static bool index_adjust(int &p_index,int p_deleted) {
	if (p_index>p_deleted)
		p_index--;
	else if (p_index==p_deleted)
		return true;
	
	return false;
}

void AudioGraph::erase_node(int p_index,std::list<Connection> *p_connections_lost) {

	ERR_FAIL_INDEX(p_index,(int)nodes.size());

	int i=0;
	/* simple algorithm for erasing nodes */	
	
	
	/* Erase connections with that node */
	while (i<(int)connections.size()) {
	
		bool from_erased=false;
		bool to_erased=false;
		
		if (index_adjust(connections[i].node_from,p_index))
			from_erased=true;
		if (index_adjust(connections[i].node_to,p_index))
			to_erased=true;
			
		if (from_erased || to_erased) {
			
			/* this connection was lost, report loss if asked to */
			/* This is useful if we want to undo or redo the action */
			
			if (p_connections_lost) { //want report of connections lost?
				
				Connection lost_conn=connections[i];
				if (from_erased) 
					lost_conn.node_from=-1; //means it connected FROM this
				if (to_erased) 
					lost_conn.node_to=-1; //means it connected TO this
					
				p_connections_lost->push_back(lost_conn);			
			}
			connections.erase( connections.begin() + i );
			
			ERR_CONTINUE(from_erased && to_erased); //should never happen!
		} else
			i++;
	}
	
	/* erase node */
	nodes.erase( nodes.begin() + p_index );
	
	recompute_process_order();
	recompute_graph();
	
}

AudioGraph::ConnectError AudioGraph::connect_plugs(int p_node_from,int p_plug_from,int p_node_to,int p_plug_to) {

	ERR_FAIL_INDEX_V(p_node_from,(int)nodes.size(),CONNECT_INVALID_NODE);
	ERR_FAIL_INDEX_V(p_node_to,(int)nodes.size(),CONNECT_INVALID_NODE);
	ERR_FAIL_INDEX_V(p_plug_from,nodes[p_node_from]->get_output_plug_count(),CONNECT_INVALID_PLUG);
	ERR_FAIL_INDEX_V(p_plug_to,nodes[p_node_to]->get_input_plug_count(),CONNECT_INVALID_PLUG);
	
	/* Check if connection exists first */
	for (int i=0;i<(int)connections.size();i++) {
		
		Connection conn=connections[i];
		
		if (conn.node_from==p_node_from && conn.node_to==p_node_to &&
				  conn.plug_from==p_plug_from && conn.plug_to == p_plug_to ) {
			
			last_error=CONNECT_ALREADY_EXISTS;
			return CONNECT_ALREADY_EXISTS;
		}
	}
		
	if ( nodes[p_node_from]->get_output_plug(p_plug_from)->get_channels() !=
		    nodes[p_node_to]->get_input_plug(p_plug_to)->get_channels() ) {
		
		last_error=CONNECT_CHANNELS_DIFFER;
		return CONNECT_CHANNELS_DIFFER;
	}
	
	/* Then make connection */
	Connection conn;
	conn.node_from=p_node_from;
	conn.node_to=p_node_to;
	conn.plug_from=p_plug_from;
	conn.plug_to=p_plug_to;
		
	connections.push_back(conn);
	
	/* If connection causes a cyclic link, erase it */
	if (recompute_process_order()) {
		connections.erase( connections.begin() + ( connections.size() - 1 ) );
		
		if (recompute_process_order()) {
		
			ERR_PRINT("It's fucked, basically");
			connections.clear();
		}
		
		recompute_graph();
		last_error=CONNECT_CYCLIC_LINK;
		return CONNECT_CYCLIC_LINK;
		
	}
	
	recompute_graph();
	last_error=CONNECT_OK;
	return CONNECT_OK;
}

void AudioGraph::disconnect_plugs(int p_node_from,int p_plug_from,int p_node_to,int p_plug_to) {

	for (int i=0;i<(int)connections.size();i++) {
		
		Connection conn=connections[i];
		
		if (conn.node_from==p_node_from && conn.node_to==p_node_to &&
				  conn.plug_from==p_plug_from && conn.plug_to == p_plug_to ) {
			
			connections.erase( connections.begin() + i );
			i--;
		}
		
	}

	ERR_FAIL_COND( recompute_process_order() );
	recompute_graph();
}

int AudioGraph::get_connection_count() {

	return connections.size();

}
const AudioGraph::Connection* AudioGraph::get_connection(int p_index) {

	ERR_FAIL_INDEX_V(p_index,(int)connections.size(),NULL);
	return &connections[p_index];
}

int AudioGraph::get_node_index(AudioNode* p_fromnode) {
	
	for (int i=0;i<nodes.size();i++)
		if (nodes[i]==p_fromnode)
			return i;
	
	return -1;
}

AudioGraph::ConnectError AudioGraph::get_last_conect_error() {
	
	return last_error;
}
void AudioGraph::process(int p_frames) {
	
	graph_process.process(p_frames);	
}

AudioGraph::AudioGraph() {
	
	last_error=CONNECT_OK;
}
};
