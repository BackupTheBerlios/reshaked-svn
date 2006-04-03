//
// C++ Implementation: envelope
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "envelope.h"
#include "error_macros.h"

namespace ReShaked {

Envelope::Envelope() {

	interp_type=INTERP_LINEAR;
	max_value=1;
	min_value=-1;
	max_nodes=INFINITE_NODES;
	min_nodes=2;
	digits=0;
	prefix="";
	reset();
}

void Envelope::reset(bool p_add_default_nodes) {


	on=false;
	loop_on=false;
	loop_begin_node=0;
	loop_end_node=0;
	sustain_loop_on=false;
	sustain_loop_begin_node=0;
	sustain_loop_end_node=0;
	release_time=0;
	node.clear();
	loop_begin_node=0;
	loop_end_node=0;
	sustain_loop_begin_node=0;
	sustain_loop_end_node=0;
	
	if (p_add_default_nodes) {
		for (int i=0;i<min_nodes;i++)
			add_node_at_offset(i*200,0);
	}
	
	/* just for comfort, this goes back to zero after node addition */
	loop_begin_node=0;
	loop_end_node=0;
	sustain_loop_begin_node=0;
	sustain_loop_end_node=0;
	
}

float Envelope::get_value_at_pos(float p_pos) const {

	if (node.size()<2) return 0;
	if (p_pos>node[node.size()-1].offset)
		return node[node.size()-1].value;

	float begin_x,begin_y;
	float end_x,end_y,xdif;
	unsigned count=0;
	int limit=-1;

	//todo - use a binary search

	while ((count<node.size()) && (limit==-1)) {

		if (node[count].offset>=p_pos) limit=count;
		count++;
	}

	if (p_pos==0) return node[0].value;

	ERR_FAIL_COND_V(limit==-1,0);

	begin_x=node[limit-1].offset;
	end_x=node[limit].offset;
	begin_y=node[limit-1].value;
	end_y=node[limit].value;

	xdif=end_x-begin_x;
	return begin_y+((p_pos-begin_x)*(end_y-begin_y))/(xdif?xdif:1);
}

void Envelope::set_node(int p_node,int p_ofs,float p_value) {

	if (p_node>=node.size()) return;

	if (p_node==0) {

		p_ofs=0;

	} else if (p_ofs<=node[p_node-1].offset) {

		p_ofs=node[p_node-1].offset+1;

	} else if ((p_node<node.size()-1) && (p_ofs>=node[p_node+1].offset)) {

		p_ofs=node[p_node+1].offset-1;
	}

	if (p_ofs>=9999) p_ofs=9999;

	if (p_value>max_value) p_value=max_value;
	if (p_value<min_value) p_value=min_value;

	node[p_node].offset=p_ofs;
	node[p_node].value=p_value;

}

int Envelope::add_node_at_offset(int p_ofs,float p_value) {


	int i,new_node;

	if (node.size()>=max_nodes)
		return -1;

	// if this is assigning an existing node, let's quit.
	for (i=0;i<node.size();i++) if (p_ofs==node[i].offset) return -1;

	i=0;
	while ((i<node.size()) && (p_ofs>=node[i].offset)) i++;

	new_node=i;
	node.resize(node.size()+1);

	if (loop_begin_node>=new_node && node.size()>=2) loop_begin_node++;
	if (loop_end_node>=new_node && node.size()>=2) loop_end_node++;
	if (sustain_loop_begin_node>=new_node && node.size()>=2) sustain_loop_begin_node++;
	if (sustain_loop_end_node>=new_node && node.size()>=2) sustain_loop_end_node++;

	for (i=node.size()-1;i>new_node;i--) node[i]=node[i-1];

	set_node(new_node,p_ofs,p_value);

	return new_node;

}

void Envelope::set_loop_begin(int pos) {

	if ((pos<0) || (pos>=node.size())) return;

	loop_begin_node=pos;

	if (loop_end_node<loop_begin_node) loop_end_node=loop_begin_node;


}

void Envelope::set_loop_end(int pos) {

	if ((pos<0) || (pos>=node.size())) return;

	loop_end_node=pos;
	
	if (loop_end_node<loop_begin_node) loop_begin_node=loop_end_node;


}


void Envelope::set_sustain_loop_begin(int pos) {

	if ((pos<0) || (pos>=node.size())) return;

	sustain_loop_begin_node=pos;

	if (sustain_loop_end_node<sustain_loop_begin_node) sustain_loop_end_node=sustain_loop_begin_node;


}

void Envelope::set_sustain_loop_end(int pos) {

	if ((pos<0) || (pos>=node.size())) return;

	sustain_loop_end_node=pos;
	
	if (sustain_loop_end_node<sustain_loop_begin_node) sustain_loop_begin_node=sustain_loop_end_node;


}


void Envelope::delete_node(int p_node) {

	if ((node.size()<=min_nodes) || (p_node<=0) || (p_node>=node.size())) return;

	int i;

	if (loop_begin_node>=p_node) loop_begin_node--;
	if (loop_end_node>=p_node) loop_end_node--;
	if (sustain_loop_begin_node>=p_node) sustain_loop_begin_node--;
	if (sustain_loop_end_node>=p_node) sustain_loop_end_node--;

	for (i=p_node;i<node.size()-1;i++) node[i]=node[i+1];

	node.resize(node.size()-1);

}

int Envelope::get_node_count() const {

	return node.size();
}

float Envelope::get_node_value(int p_node) const {

	ERR_FAIL_COND_V( (p_node<0) || (p_node>=node.size()) ,0);


	return node[p_node].value;

}
int Envelope::get_node_offset(int p_node) const {

	ERR_FAIL_COND_V( (p_node<0) || (p_node>=node.size()) ,-1);
	
	return node[p_node].offset;
	
}

int Envelope::get_loop_begin() const {

	return loop_begin_node;	
}
int Envelope::get_sustain_loop_begin() const {

	return sustain_loop_begin_node;

}

int Envelope::get_loop_end() const {

	return loop_end_node;

}
int Envelope::get_sustain_loop_end() const {

	return sustain_loop_end_node;

}

bool Envelope::is_loop_enabled() const {


	return loop_on;
}
bool Envelope::is_sustain_loop_enabled() const {

	return sustain_loop_on;

}

bool Envelope::is_enabled() const {

	return on;
}


void Envelope::set_enabled (bool p_enabled) {


	on=p_enabled;
}

void Envelope::set_loop_enabled (bool p_enabled) {


	loop_on=p_enabled;
	printf("loop is %i\n",p_enabled);
}

void Envelope::set_sustain_loop_enabled (bool p_enabled) {


	sustain_loop_on=p_enabled;
}

void Envelope::set_max_nodes(int p_max) {

	if (p_max<-1)
		return;

	max_nodes=p_max;

}

void Envelope::set_min_nodes(int p_min) {

	min_nodes=p_min;

}

void Envelope::set_release_time(int p_time) {
	
	ERR_FAIL_COND(p_time<0);
	release_time=p_time;
}

void Envelope::set_prefix(String p_prefix,int p_digits) {
	
	prefix=p_prefix;
	digits=p_digits;
}
String Envelope::get_prefix() const {
	
	return prefix;	
}

int Envelope::get_digits() const {
	
	return digits;
}


int Envelope::get_release_time() const {

	return release_time;
}

}
