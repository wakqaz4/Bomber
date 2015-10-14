#ifndef _COMMON_H_
#define _COMMON_H_

#define	MAX_PLAYER_COUNT		5

#define _BOMB_DEBUG_

#define CHANGE_GAME_STATE(curState) \
{	\
	m_prevGameState = curState;	\
	m_timer = STATE_INTERVAL[curState]; \
}
#define CHECK_GAME_STATE(curState) \
	if (m_prevGameState == curState - 1) \
	{ \
		CHANGE_GAME_STATE(curState); \
	} \
	else if (m_prevGameState == curState) \
	{ \
	}

#define floatEqual(x, y, EPSILON)		return ((((float)(x)- (y)) < EPSILON) && (((float)(x)- (y)) > -EPSILON))		

#endif