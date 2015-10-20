#ifndef _CANNON_MACROS_H_
#define _CANNON_MACROS_H_

#define SINGLE_INSTANCE(className)	\
private:\
	static className* m_singleInstance; \
public:\
	inline static className* create()\
	{\
		if (m_singleInstance == nullptr)\
		{\
			m_singleInstance = new className(); \
			m_singleInstance->init(); \
			return m_singleInstance; \
		}\
		else\
		{\
			return m_singleInstance; \
		}\
	}
/*
#define MEM_B( x ) ( *( (byte *) (x) ) )

#define MEM_W( x ) ( *( (word *) (x) ) )

#define MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )

#define MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )

#define UPCASE( c ) ( ((c) >= ''a'' && (c) <= ''z'') ? ((c) - 0x20) : (c) )
*/
#endif //_CANNON_MACROS_H_
