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
#include "engine/song.h"


AudioDriverJACK*AudioDriverJACK::singleton=NULL;


struct AudioDriverNodeJACK : public AudioDriverNode {

	bool inside_graph;

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
		int _in_ev_offset; //offset to reading input events for the slice
		
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
	
		if (!inside_graph)
			return;
			
		jack_client_t *c = AudioDriverJACK::singleton->client;
		if (!c)
			return;
		
		for (int i=0;i<chans;i++) {
		
			if (ports[i].port) {
				jack_port_unregister( c, ports[i].port );
				ports[i].port=NULL;				
			}

							
			ports[i].port=jack_port_register(c,String("node_"+String::num(node_idx)+"port_"+String::num(i)).ascii().get_data(),
								(type==PORT_EVENT)?JACK_DEFAULT_MIDI_TYPE:JACK_DEFAULT_AUDIO_TYPE,
								(flow==PORT_IN)?JackPortIsOutput:JackPortIsInput,
								0);
								
			if (!ports[i].port)
				continue;
				
			if (named) {
			
			
			} else {
			
				const char **result,**iter;
				
				result = jack_get_ports (c, NULL, (type==PORT_EVENT)?JACK_DEFAULT_MIDI_TYPE:JACK_DEFAULT_AUDIO_TYPE,JackPortIsPhysical|((flow==PORT_IN)?JackPortIsInput:JackPortIsOutput));
				

				if (result) {
				
					int j=0;
					iter=result;
					while(*iter) {				
					
						if (j==ports[i].index) {					
					
							
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
	
	virtual void graph_enter() {
	
		inside_graph=true;
		_connect_jack();
	}
	virtual void graph_exit() {
	
		_disconnect_jack();
		inside_graph=false;
	
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
	
		ERR_FAIL_COND(type!=PORT_AUDIO || p_flow!=flow );
		ERR_FAIL_INDEX(p_channel,chans);
		
		audio_buffer[p_channel]=p_buffer;
	}
	virtual void connect_event_buffer( PortFlow p_flow, int p_port, MusicEvent *p_buffer ) {
	
		ERR_FAIL_COND(type!=PORT_EVENT || flow!=p_flow);
			
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
	
		switch( type ) {
		
			case PORT_AUDIO: {
			
	
				int mix_frames=p_info.audio_buffer_size;
		
				for (int c=0;c<chans;c++) {
			
					if (!ports[c].port)
						continue;
						
					jack_default_audio_sample_t *jack_buff = &((jack_default_audio_sample_t*)jack_port_get_buffer( ports[c].port , AudioDriverJACK::singleton->callback_nframes))[AudioDriverJACK::singleton->process_offset];
				
					sample_t *audio_buff=audio_buffer[c];
				
					ERR_FAIL_COND(!audio_buff);
					
					//printf("mix frames %i, dir is %i, chan is %i\n",mix_frames,flow,c);
					
					if (flow==PORT_IN) {
					
						for (int i=0;i<mix_frames;i++) {
						
							jack_buff[i]=audio_buff[i];
						}					
					
					} else {
				
						for (int i=0;i<mix_frames;i++) {
						
							audio_buff[i]=jack_buff[i];
						}					
				
					}
				}			
			} break;
			case PORT_EVENT: {
			
					
				for (int c=0;c<chans;c++) { // always one anyway.. but..	
			
					if (!ports[c].port)
						continue;
					
					void *jack_buff = jack_port_get_buffer( ports[c].port , AudioDriverJACK::singleton->callback_nframes);
					MusicEvent *event_buffer=ev_buffer;
					ERR_CONTINUE(!ev_buffer);
						
					if (flow==PORT_OUT) { // read from jack, output events
					
						jack_nframes_t event_count = jack_midi_get_event_count(jack_buff);
						
						if ( AudioDriverJACK::singleton->process_offset==0) {
						
							ports[c]._in_ev_offset=0;
						}
						
						int limit=AudioDriverJACK::singleton->process_offset+p_info.audio_buffer_size;
						int idx=0;
						while( ports[c]._in_ev_offset < event_count ) {
						
							jack_midi_event_t src_ev;
							if (!jack_midi_event_get ( &src_ev,jack_buff,ports[c]._in_ev_offset))
								break;
							
							if (src_ev.time>=limit)
								break;
							
							if (idx>=p_info.event_buffer_size)
								break;
								
							MusicEvent &dst_ev = event_buffer[idx];
							
							unsigned char *bufdata = (unsigned char*)src_ev.buffer;
							
							if ((*bufdata)&0x80) {
								//meaningful event
								dst_ev.type=(*bufdata)>>4;
								dst_ev.channel=(*bufdata)&0xF;
								
								bufdata++;
								
								dst_ev.raw.param1=*bufdata;
								
								if (
									dst_ev.type==MusicEvent::MIDI_NOTE_OFF ||
									dst_ev.type==MusicEvent::MIDI_NOTE_ON ||
									dst_ev.type==MusicEvent::MIDI_NOTE_PRESSURE ||
									dst_ev.type==MusicEvent::MIDI_CONTROLLER ||
									dst_ev.type==MusicEvent::MIDI_PITCH_BEND ||
									dst_ev.type==MusicEvent::MIDI_CONTROLLER ) {					
									
									} {
									
									bufdata++;
									
									dst_ev.raw.param2=*bufdata;
								}
								
								dst_ev.frame=src_ev.time-AudioDriverJACK::singleton->process_offset;
								idx++;		
							}
							
							ports[c]._in_ev_offset++;
						}
						
						if (idx>=p_info.event_buffer_size) {
							idx=p_info.event_buffer_size-1;
						}
			
						event_buffer[idx]=MusicEvent( MusicEvent::STREAM_TAIL, 0,0,0);
					}
					
				}
			} break;
		}
		
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
	
		ERR_FAIL_INDEX( p_channel, chans );
		ports[p_channel].index=p_index;
		_connect_jack();
	}
	virtual void connect_named(String p_name,int p_channel=-1) {
	
		ERR_FAIL_INDEX( p_channel, chans );
		ports[p_channel].named=p_name;
		_connect_jack();
	
	}
	virtual void disconnect() {
	
	
		_disconnect_jack();
	}
	
	virtual ConnectionType get_connection_type() const  {
	
		return named?CONNECTION_NAMED:CONNECTION_FIXED;
	}
	
	virtual String get_named_connection(int p_channel=-1) const {
	
		ERR_FAIL_INDEX_V( p_channel, chans,"" );
		
		return ports[p_channel].named;
	}
	
	virtual int get_fixed_connection(int p_channel=1) const {
	
		ERR_FAIL_INDEX_V( p_channel, chans,0 );
		
		return ports[p_channel].index;
	
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
		 
		
		inside_graph=false;
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

	AudioNodeRegistry::add_node_info( get_input_audio_creation_info() );
	AudioNodeRegistry::add_node_info( get_output_audio_creation_info() );
	AudioNodeRegistry::add_node_info( get_input_event_creation_info() );
	AudioNodeRegistry::add_node_info( get_output_event_creation_info() );

	
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


	if (pthread_mutex_trylock(&mutex)!=0) {
		printf("jack locked\n");
		return 0; // couldn't process audio (locked)
	}

	callback_nframes=nframes;

	if (Song::get_singleton()) {
	
		process_offset=0;
		
		while(nframes) { 
			int processed = Song::get_singleton()->process(nframes);
			process_offset+=processed;
			nframes-=processed;
		}		
	}
	
	pthread_mutex_unlock(&mutex);
	return 0;
}
int AudioDriverJACK::set_mix_rate(jack_nframes_t nframes) {

	if (Song::get_singleton())
		Song::get_singleton()->get_audio_graph()->set_mix_rate( nframes );
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

	pthread_mutex_lock(&mutex);
}
void AudioDriverJACK::unlock() {

	pthread_mutex_unlock(&mutex);
}


AudioDriverJACK::AudioDriverJACK() {

	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&mutex,&attr);	
	singleton=this;
}


AudioDriverJACK::~AudioDriverJACK()
{
}


