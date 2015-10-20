#ifndef _CANNON_CONFIG_H_
#define _CANNON_CONFIG_H_

#define	MAX_PLAYER_COUNT		5
#define MAX_TURN_COUNT			10
#define PI						3.1415927f

enum GAME_STATE
{
	PREPARING,					//游戏开始之前的等待，等待所有玩家连接好
	DOWN_COUNTING,				//所有玩家就绪，倒计时三秒
	RUNNING,					//游戏进行中，当前玩家可行动
	WAITING,					//当前玩家已发射炮弹，等待炮弹落地
	PAUSED,						//游戏暂停中
	FINISHED,					//某一方胜利
	GAME_STATE_COUNT
};

static const float STATE_INTERVAL[GAME_STATE_COUNT] =
{
	10.0f,
	3.0f,
	9.0f,
	10.0f,
	20.0f,
	10.0f
};

const float BOMB_LIFE_SPAN = 5;
#endif //_CANNON_CONFIG_H_