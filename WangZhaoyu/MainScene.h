#ifndef _MAIN_SCENE_H_
#define _MAIN_SCENE_H_

#include "cocos2d.h"
#include "UILayer\UILayer.h"
#include "mainLayer\MainLayer.h"
#include "mainLayer\entity\Player.h"
#include "mainLayer\entity\Bomb.h"

USING_NS_CC;

enum EQUIP_ATTRIBUTE
{
	//Base attributes
	POWER,
	LUCK,
	AGILITY,
	INTELLIGENCE,

	//attributes that effects
	MASS,
	MAX_POWER,
	MAX_STRENGTH,
	MAX_HEALTH_POINT,

	//yet to be implemented
	ATTACK,
	ARMOR,
	EXP,
	CRIT_ATK_RATIO,
	DAMAGE_REFLECTION,
	DAMAGE_DECRESE,
	DAMAGE_ENHANCED,

	ATTRIB_COUNT
};

class CMainScene : public Scene
{
	//typedef int(Player::*pFunc)(void);
	//typedef int(CMainScene::*pCallbackFunc)(void);

//#define REGISTER_INTERRUPTIONS(triggerFunc, callbackFunc) m_interruptionMap[triggerFunc] = callbackFunc
//#define UNREGISTER_INTERRUPTIONS(funcName) m_interruptionMap.erase(funcName)


public:

	CMainScene(){};
	~CMainScene(){};
	static Scene*	createScene();
	virtual bool	init() override;
	virtual void	update(float delta) override;
	bool			setGameState(GAME_STATE gameState);
	GAME_STATE		getGameState();
	GAME_STATE		getPrevGameState();
	void			nextPlayerTurn(void);
	void			refreshPlayerOrderList();
	int				onPlayerFired();
	int				onBombDidLanded();
	void			deleteOrderListWithIndex(int index);
	//void			reRegisterInterruptions();

	CREATE_FUNC(CMainScene);

public:

	UILayer*				m_UILayer;
	MainLayer*				m_mainLayer;

	Player*					m_curPlayer;
	std::vector<Player*>	m_playerVector;
	//std::map<pFunc, int>  m_interruptionMap;

private:	
	int				m_playersOrderList[MAX_PLAYER_COUNT];
	int				m_curPlayerOrder;
	int				m_alivePlayersNum;
	int				m_deadPlayersNum;
	bool			m_curTurnIsOver;
	int				m_curTurnIndex;

	float			m_downCountingTimer;	
	float			m_playerActionTimer;
	GAME_STATE		m_gameState;
	GAME_STATE		m_prevGameState;

	cocos2d::NavMesh *_navMesh;

#ifdef _BOMB_DEBUG_
public:
	float			m_playerWaitingTimer;


#endif
};

#endif//_MAIN_SCENE_H_