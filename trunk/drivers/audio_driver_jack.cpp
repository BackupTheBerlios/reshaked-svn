//
// C++ Implementation: audio_driver_jack
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "audio_driver_jack.h"
#include "engine/audio_driver_node.h"
#include "engine/audio_node_registry.h"


AudioDriverJACK*AudioDriverJACK::singleton=NULL;


struct AudioDriverNodeJACK : public AudioDriverNode {

	int node_idx;
	std::list< AudioDriverNodeJACK* >::iterator thisI;
	bool named;
	MusicEvent *ev_buffer;
	sample_t **audio_buffer; 
	
	const AudioNodeInfo *info;
	PortType type;
	PortFlow flow;
	int chans;
	
	struct JackPort {
		
		String named;
		int index;
		
		jack_port_t *port;
		
		JackPort() { port=NULL; index=0; }
	};
	
	JackPort *ports;
	
	void _disconnect_jack() {
	
		jack_client_t *c = AudioDriverJACK::singleton->client;
		if (!c)
			return;
		
		for (int i=0;i<chans;i++) {
		
			if (ports[i].port) {
				jack_port_unregister( c, ports[i].port );
				ports[i].port=NULL;
				
			}
		}
	
	}
	void _connect_jack() {
	
		jack_client_t *c = AudioDriverJACK::singleton->client;
		if (!c)
			return;
		
		for (int i=0;i<chans;i++) {
		
			if (ports[i].port) {
				jack_port_unregister( c, ports[i].port );
				ports[i].port=NULL;				
			}
			
			if (named) {
			
			
			} else {
			
				const char **result,**iter;
				
				result = jack_get_ports (c, NULL, (type==PORT_EVENT)?JACK_DEFAULT_MIDI_TYPE:JACK_DEFAULT_AUDIO_TYPE,JackPortIsPhysical|((flow==PORT_IN)?JackPortIsInput:JackPortIsOutput));
				

				if (result) {
				
					int j=0;
					iter=result;
					while(*iter) {				
					
						if (j==ports[i].index) {					
					
							
							ports[i].port=jack_port_register(c,String("node_"+String::num(node_idx)+"port_"+String::num(i)).ascii().get_data(),
								(type==PORT_EVENT)?JACK_DEFAULT_MIDI_TYPE:JACK_DEFAULT_AUDIO_TYPE,
								(flow==PORT_IN)?JackPortIsOutput:JackPortIsInput,
								0);
								
								
							if (!ports[i].port)
								break;
								
							printf("i try to connect %s to %s\n",jack_port_name(ports[i].port),*iter);
							if (flow==PORT_IN)
								jack_connect( c, jack_port_name(ports[i].port),*iter);
							else
								jack_connect( c, *iter,jack_port_name(ports[i].port));
							break;
						}
						
						iter++;
						j++;
					}
					
					free(result);
				}
			}
		}
	}
	
	
	virtual int get_port_count( PortType p_type, PortFlow p_flow ) const {
	
		if (p_type==type && p_flow==flow)
			return 1;
		else
			return 0;
	}
	virtual int get_audio_port_channel_count( PortFlow p_flow, int p_port) const {
	
		if (type==PORT_AUDIO && p_flow==flow)
			return chans;
		else 
			return 0;
		
	}
	
	virtual String get_port_name( PortType p_type, PortFlow p_flow,int p_port ) const {
	
		if (p_flow==PORT_OUT)
			return "Capture";
		else
			return "Playback";
	}
	
	virtual void connect_audio_buffer( PortFlow p_flow, int p_port, int p_channel, sample_t *p_buffer ) {
	
		if (type!=PORT_AUDIO || p_flow!=flow )
			return;
		if (p_channel>0 || p_channel >=chans)
			return;
		
		audio_buffer[p_channel]=p_buffer;
	}
	virtual void connect_event_buffer( PortFlow p_flow, int p_port, MusicEvent *p_buffer ) {
	
		if (type==PORT_EVENT && flow==p_flow)
			ev_buffer=p_buffer;
	}

	virtual ControlPort* get_control_port( PortFlow p_flow,int p_port ) { 
		return NULL; 
	}
	
	/* AudioNode Info */
	
	virtual const AudioNodeInfo *get_info() const { return info; }
	virtual int get_instanced_channels() const { return chans; }
	
	/* Process */
		
	virtual void process(const ProcessInfo& p_info) {
	
	
	}
	virtual void set_mix_rate(float p_hz) {
	
	
	}
	
	/* Load/Save */
	
	virtual Error save( TreeSaver * p_tree ) {
	 
		p_tree->add_string("connect_mode", (named?"named":"fixed") );
		if (named) {
		
			for (int i=0;i<chans;i++) {
				//?
			}
		}
		
		return OK;
	}
	virtual Error load( TreeLoader * p_tree ) {
	
	
		return OK;
	}
	

	virtual void get_named_connectable_list( std::list<String>* p_list ) const {
	
		jack_client_t *c = AudioDriverJACK::singleton->client;
		if (!c)
			return;
	
		const char **result,**iter;
				
		result = jack_get_ports (c, NULL, (type==PORT_EVENT)?JACK_DEFAULT_MIDI_TYPE:JACK_DEFAULT_AUDIO_TYPE,(flow==PORT_IN)?JackPortIsInput:JackPortIsOutput);
		

		if (result) {
		
			iter=result;
							
			while (*iter) {				
				
				p_list->push_back( *iter );					
				iter++;
			
			}			
			free(result);
		}
			
	}
	virtual void get_fixed_connectable_list( std::list<String>* p_list ) const {
	
		jack_client_t *c = AudioDriverJACK::singleton->client;
		if (!c)
			return;
	
		const char **result,**iter;
				
		result = jack_get_ports (c, NULL, (type==PORT_EVENT)?JACK_DEFAULT_MIDI_TYPE:JACK_DEFAULT_AUDIO_TYPE,JackPortIsPhysical|(flow==PORT_IN)?JackPortIsInput:JackPortIsOutput);
		

		if (result) {
		
			iter=result;
			
			for (int i=0;i<32;i++) {
			
				
				if (*iter) {				
				
					p_list->push_back( *iter );					
					iter++;
				} else {
				
					p_list->push_back("Unavailable (#"+String::num(i+1)+")");
				}
			}			
			free(result);
		}
	
	}
	
	virtual void connect_fixed(int p_index,int p_channel=-1) {
	
	
	}
	virtual void connect_named(String p_name,int p_channel=-1) {
	
	
	}
	virtual void disconnect() {
	
	
	}
	
	virtual ConnectionType get_connection_type() const  {
	
		return named?CONNECTION_NAMED:CONNECTION_FIXED;
	}
	
	virtual String get_named_connection() const {
	
		return "";
	}
	virtual int get_fixed_connection() const {
	
		return 0;
	}
	virtual int get_fixed_connection_channel() const {
	
		return 0;
	}
	
	virtual PortType get_driver_node_type() const { return type; }
	virtual PortFlow get_driver_node_flow() const { return flow; }


	AudioDriverNodeJACK(PortType p_type,PortFlow p_flow,int p_channels,const AudioNodeInfo *p_info) {
	
		info=p_info;
		flow=p_flow;
		type=p_type;
		printf("flow %i, type %i\n",flow,type);
		if (p_type==PORT_AUDIO) {
		
			audio_buffer= new sample_t*[p_channels];
			chans=p_channels;
		} else {
		
			chans=1;
			audio_buffer=NULL;
		}
		
		ports = new JackPort[chans];
		named=false;
		
		for (int i=0;i<chans;i++) {
		
			if (audio_buffer)
				audio_buffer[i]=0;
				
			ports[i].index=i;
		}
		
		
		if (!AudioDriverJACK::singleton)
			return;
			
		static int node_assign=1;
		
		node_idx=node_assign++;
		
		AudioDriverJACK::singleton->audio_nodes.push_front(this);
		thisI=AudioDriverJACK::singleton->audio_nodes.begin();
		 
		
		_connect_jack();
	}

	~AudioDriverNodeJACK() {
	
		_disconnect_jack();
		
		AudioDriverJACK::singleton->audio_nodes.erase( thisI );
		
		if(audio_buffer)
			delete[] audio_buffer;
		if (ports)
			delete[] ports;
	}
};


AudioNode * jack_create_audio_out(int p_channels,const AudioNodeInfo *p_info) {

	return new AudioDriverNodeJACK( AudioNode::PORT_AUDIO, AudioNode::PORT_IN, p_channels, p_info);
}

static const AudioNodeInfo *get_output_audio_creation_info() {

	static AudioNodeInfo _info;
	_info.caption="Hardware Audio Output";
	_info.short_caption="HW AudioOut";
	_info.description="Sends audio to soundcard.";
	_info.unique_ID="INTERNAL_HW_AUDIO_OUT";
	_info.creation_func=jack_create_audio_out;
	_info.icon_string="node_hw_audio_out";
	_info.category="Hardware";

	return &_info;
}

AudioNode * jack_create_audio_in(int p_channels,const AudioNodeInfo *p_info) {

	return new AudioDriverNodeJACK( AudioNode::PORT_AUDIO, AudioNode::PORT_OUT, p_channels, p_info);
}

static const AudioNodeInfo *get_input_audio_creation_info() {

	static AudioNodeInfo _info;
	_info.caption="Hardware Audio Input";
	_info.short_caption="HW AudioIn";
	_info.description="Capture audio from soundcard.";
	_info.unique_ID="INTERNAL_HW_AUDIO_IN";
	_info.creation_func=jack_create_audio_in;
	_info.icon_string="node_hw_audio_in";	
	_info.category="Hardware";

	return &_info;
}



AudioNode * jack_create_event_out(int p_channels,const AudioNodeInfo *p_info) {

	return new AudioDriverNodeJACK( AudioNode::PORT_EVENT, AudioNode::PORT_IN, p_channels, p_info);
}

static const AudioNodeInfo *get_output_event_creation_info() {

	static AudioNodeInfo _info;
	_info.caption="Hardware Event Output";
	_info.short_caption="HW EventOut";
	_info.description="Sends MIDI to soundcard.";
	_info.unique_ID="INTERNAL_HW_EVENT_OUT";
	_info.creation_func=jack_create_event_out;
	_info.icon_string="node_hw_event_out";	
	_info.category="Hardware";

	return &_info;
}

AudioNode * jack_create_event_in(int p_channels,const AudioNodeInfo *p_info) {

	return new AudioDriverNodeJACK( AudioNode::PORT_EVENT, AudioNode::PORT_OUT, p_channels, p_info);
}

static const AudioNodeInfo *get_input_event_creation_info() {

	static AudioNodeInfo _info;
	_info.caption="Hardware Event Input";
	_info.short_caption="HW EventIn";
	_info.description="Capture event from soundcard.";
	_info.unique_ID="INTERNAL_HW_EVENT_IN";
	_info.creation_func=jack_create_event_in;
	_info.icon_string="node_hw_event_in";	
	_info.category="Hardware";

	return &_info;
}

String AudioDriverJACK::get_name() {

	return "JACK Connection Kit";
}

void AudioDriverJACK::initialize() {

	AudioNodeRegistry::add_node_info( get_output_audio_creation_info() );
	AudioNodeRegistry::add_node_info( get_input_audio_creation_info() );
	AudioNodeRegistry::add_node_info( get_output_event_creation_info() );
	AudioNodeRegistry::add_node_info( get_input_event_creation_info() );

	
}
	
	
	
void AudioDriverJACK::finish() {
	
	if (!client)
		return;
	
	std::list< AudioDriverNodeJACK* >::iterator I=audio_nodes.begin();
	
	
	for(;I!=audio_nodes.end();I++) {
	
		(*I)->_disconnect_jack();	
	}
		
	jack_client_close(client);
	client=NULL;
		
}
	
	
int AudioDriverJACK::process(jack_nframes_t nframes) {


	return 0;
}
int AudioDriverJACK::set_mix_rate(jack_nframes_t nframes) {


	return 0;
}

bool AudioDriverJACK::restart() {


	if (client)
		void finish();

	if ((client = jack_client_new("Reshaked")) == 0) {
                error_text="Jack server not running.";
                return false;
        }

	/* Set Callbacks */
	jack_set_process_callback(client, _process, this);
        jack_set_sample_rate_callback(client, _srate, this);
        jack_on_shutdown(client, _shutdown, this);

	if (jack_activate (client)) {
		error_text="Cannot activate client.";
                return false;
        }

	error_text="JACK client active.";


	/* Connect all existing ports */
	
	
	std::list< AudioDriverNodeJACK* >::iterator I=audio_nodes.begin();
	
	
	for(;I!=audio_nodes.end();I++) {
	
		(*I)->_connect_jack();	
	}
	
	return true;
}
bool AudioDriverJACK::is_active() const {

	return client;
}

int AudioDriverJACK::get_param_count() {

	return 0;
}
ControlPort *AudioDriverJACK::get_param(int p_param) {

	return NULL;
}

void AudioDriverJACK::lock() {


}
void AudioDriverJACK::unlock() {


}


AudioDriverJACK::AudioDriverJACK() {

	singleton=this;
}


AudioDriverJACK::~AudioDriverJACK()
{
}


