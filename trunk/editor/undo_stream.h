#ifndef UNDO_STREAM_H
#define UNDO_STREAM_H

#include "typedefs.h"
#include "rstring.h"
#include <list>
#include <vector>

namespace ReShaked {

class UndoData {
public:
	virtual ~UndoData() {};
};

template<class T>
class UndoDataTemplate : public UndoData {
	
	T* data;
public:	
	UndoDataTemplate(T *p_data) { data=p_data; }
	~UndoDataTemplate() { delete data; }	
};

class CommandFunc {

	/**
	 * Used by the undo createfunc, data that is erased with the redo history
	 */
	std::vector<UndoData *> create_data;
	/**
	 * Used by the undo createfunc, data that is erased when it is too old
	 */
	std::vector<UndoData *> delete_data;
	
public:
	template<class Td>
	void add_create_data(Td *p_data) {  create_data.push_back(new UndoDataTemplate<Td>(p_data)); }
	template<class Td>
	void add_delete_data(Td *p_data) {  delete_data.push_back(new UndoDataTemplate<Td>(p_data)); }
	
	void clear_create_data();
	void clear_delete_data();
	void erase_create_data();
	void erase_delete_data();
			
	virtual CommandFunc* exec(bool p_no_undo=false)=0;
	virtual ~CommandFunc();
};


template<class T,class P1>
class CommandFunc1 : public CommandFunc {

	typedef CommandFunc* (T::*Command)(bool,P1);
	P1 param1;
	
	Command command;
	T *instance;
	public:
		virtual CommandFunc* exec(bool p_no_undo=false) {
			return (instance->*command)(p_no_undo,param1);
		}
		CommandFunc1(T *p_instance,Command p_command,const P1 &p_param1) {
			instance=p_instance;
			command=p_command;
			param1=p_param1;
			
		}
};

#define Command1(m_self,m_method,m_p1) new CommandFunc1<typeof(*m_self),typeof(m_p1)>(m_self,m_method,m_p1)


template<class T,class P1,class P2>
class CommandFunc2 : public CommandFunc {

	typedef CommandFunc* (T::*Command)(bool,P1,P2);
	P1 param1;
	P2 param2;

	Command command;
	T *instance;
	public:
		virtual CommandFunc* exec(bool p_no_undo=false) {
			return (instance->*command)(p_no_undo,param1,param2);
		}
		CommandFunc2(T *p_instance,Command p_command,const P1 &p_param1,const P2 &p_param2) {
			instance=p_instance;
			command=p_command;
			param1=p_param1;
			param2=p_param2;
			
		}
};

#define Command2(m_self,m_method,m_p1,m_p2) new CommandFunc2<typeof(*m_self),typeof(m_p1),typeof(m_p2)>(m_self,m_method,m_p1,m_p2)



template<class T,class P1,class P2,class P3>
class CommandFunc3 : public CommandFunc {

	typedef CommandFunc* (T::*Command)(bool,P1,P2,P3);
	P1 param1;
	P2 param2;
	P3 param3;

	Command command;
	T *instance;
	public:
	virtual CommandFunc* exec(bool p_no_undo=false) {
		return (instance->*command)(p_no_undo,param1,param2,param3);
	}
	CommandFunc3(T *p_instance,Command p_command,const P1 &p_param1,const P2 &p_param2,const P3 &p_param3) {
		instance=p_instance;
		command=p_command;
		param1=p_param1;
		param2=p_param2;
		param3=p_param3;
	}
};

#define Command3(m_self,m_method,m_p1,m_p2,m_p3) new CommandFunc3<typeof(*m_self),typeof(m_p1),typeof(m_p2),typeof(m_p3)>(m_self,m_method,m_p1,m_p2,m_p3)


template<class T,class P1,class P2,class P3,class P4>
class CommandFunc4 : public CommandFunc {

	typedef CommandFunc* (T::*Command)(bool,P1,P2,P3,P4);
	P1 param1;
	P2 param2;
	P3 param3;
	P4 param4;

	Command command;
	T *instance;
	public:
		virtual CommandFunc* exec(bool p_no_undo=false) {
			return (instance->*command)(p_no_undo,param1,param2,param3,param4);
		}
		CommandFunc4(T *p_instance,Command p_command,const P1 &p_param1,const P2 &p_param2,const P3 &p_param3,const P4 &p_param4) {
			instance=p_instance;
			command=p_command;
			param1=p_param1;
			param2=p_param2;
			param3=p_param3;
			param4=p_param4;
		}
};

#define Command4(m_self,m_method,m_p1,m_p2,m_p3,m_p4) new CommandFunc4<typeof(*m_self),typeof(m_p1),typeof(m_p2),typeof(m_p3),typeof(m_p4)>(m_self,m_method,m_p1,m_p2,m_p3,m_p4)

template<class T,class P1,class P2,class P3,class P4,class P5>
class CommandFunc5 : public CommandFunc {

	typedef CommandFunc* (T::*Command)(bool,P1,P2,P3,P4,P5);
	P1 param1;
	P2 param2;
	P3 param3;
	P4 param4;
	P5 param5;

	Command command;
	T *instance;
	public:
		virtual CommandFunc* exec(bool p_no_undo=false) {
			return (instance->*command)(p_no_undo,param1,param2,param3,param4,param5);
		}
		CommandFunc5(T *p_instance,Command p_command,const P1 &p_param1,const P2 &p_param2,const P3 &p_param3,const P4 &p_param4,const P5 &p_param5) {
			instance=p_instance;
			command=p_command;
			param1=p_param1;
			param2=p_param2;
			param3=p_param3;
			param4=p_param4;
			param5=p_param5;
		}
};

#define Command5(m_self,m_method,m_p1,m_p2,m_p3,m_p4,m_p5) new CommandFunc5<typeof(*m_self),typeof(m_p1),typeof(m_p2),typeof(m_p3),typeof(m_p4),typeof(m_p5)>(m_self,m_method,m_p1,m_p2,m_p3,m_p4,m_p5)

class UndoStream {
	
	
	struct UndoElement {
		
		CommandFunc *redo;
		CommandFunc *undo;
	};
	
	struct UndoGroup {
		
		int collapses;
		unsigned int timestamp; //time at which the command happened
		String name;
		std::list<UndoElement> command_list;
	};
	
	std::list<UndoGroup> undo_stream;
	
	int inside_count;
	
	void undo_group(UndoGroup *p_group);
	void redo_group(UndoGroup *p_group);
	std::list<UndoGroup>::iterator get_block_iterator(int p_index);
	int current_index;
		
	void delete_redo_history();	
	
	int lock_count;
	unsigned int collapse_max_time_window; //in msecs
	
	static int max_undo_steps; //static, since it's a setting
	void fix_undo_history_length();
	
public:	
	
	String get_current_action_name();
	String get_current_action_text();
	int get_current_action_collapses();
	
	
	void begin(String p_name,bool p_can_collapse_to_previous=true);
	void add_command( CommandFunc *p_command );
	void end();
	
	/* no undo/redo is permitted if locked */
	void lock();
	void unlock();
	
	void undo();
	void redo();
	
	void clear();
	
	static void set_max_undo_steps(int p_max);
	
	UndoStream();
	~UndoStream();
	
};



} //end of namespace
#endif
