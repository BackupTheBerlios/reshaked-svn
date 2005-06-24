#ifndef ERROR_MACROS_H
#define ERROR_MACROS_H


#define ERR_FAIL_INDEX(m_index,m_size) \
	 {if ((m_index)<0 || (m_index)>=(m_size)) { \
		std::cout << "index out of size " << std::endl;	\
		return;	\
	}}	\

#define ERR_FAIL_INDEX_V(m_index,m_size,m_retval) \
	 {if ((m_index)<0 || (m_index)>=(m_size)) { \
		std::cout << "index out of size " << std::endl;	\
		return (m_retval);	\
	}}	\

#define ERR_FAIL_COND(m_cond) \
	{ if ( m_cond ) {	\
		std::cout << " ## m_cond ## failed." << std::endl;	\
		return;	 \
	} }	\

#define ERR_PRINT(m_string) \
	{ \
			std::cout << m_string << std::endl;	\
	} \




#endif
