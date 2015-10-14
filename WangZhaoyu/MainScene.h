#include "cocos2d.h"
#include "common.h"

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

class CEquipment : public Sprite
{
public:
	std::string m_name;
	EQUIP_ATTRIBUTE* m_attributeList;
	int m_attributeSize;

};

class CSkill
{

};

class CBomb : public Sprite3D
{
public:
	static CBomb* create();
};

class CPlayer : public Sprite3D
{
	enum PLAYER_STATE
	{
		WAITING,
		ACTING,
		FIRING,

		STATE_COUNT
	};

	typedef std::map<std::string, int> playerNameIDMap;

public:
	virtual bool UpdatePlayerAttrib(Ref* sender);
	static CPlayer* create();

	bool EnableAction();
	bool DisableAction();
	bool IsDead();

	int BombDidLanded();
	int Fired();
	void SetFired(int);

public:
	//此部分为UI暴露给玩家所视的
	struct
	{
		std::string m_Name;
		int m_ID;
		//影响maxPower, healthPoint;
		int m_power;
		//影响criticalAtkRatio
		int m_luck;
		//影响armor,Strength
		int m_agility;
		//影响炮弹精度，炮弹威力，可使用更多种类的炮弹，自动追踪度？？
		int m_intelligence;
		//玩家技能
		/*
		std::vector<CSkill*> m_skills;
		std::vector<CBomb*> m_bombs;
		*/

	}m_sPlayerInfo;
	//当前游戏状态
	PLAYER_STATE* m_playerState;
	bool m_canAct;
	bool m_isDead;
	//defaultly set as -1
	int m_actionOrderList[MAX_PLAYER_COUNT];
	int m_actionOrder;


private:
	//physical attributes
	float m_mass;
	float m_maxPower;
	float m_healthPoint;
	float m_maxStrength;

	//to be added in the second version
	/*
	float m_attack;
	float m_armor;
	float m_level;
	float m_criticalAttackRatio;

	float m_exp;
	float m_auxliaryMaxArmor;
	float m_auxliaryMaxPower;
	float m_auxliaryAttack;
	float m_auxiliaryMaxHealthPoint;
	float m_auxliaryMaxStrength;
	float m_auxliaryCritAtkRatio;

	//attached attributes
	std::vector<CEquipment*> m_equipments;
	std::vector<CSkill*>	m_skills;
	*/
};

class CUIButton
{
public:
	static CUIButton* create();
};
class UILayer : public Layer
{
public:
	static UILayer* create();
};
class TerrainLayer : public Layer
{
};
class EntityLayer : public Layer
{
};
class EnvLayer : public Layer
{
};

class CMainScene : public Scene
{

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

	typedef int(*(pFunc))();

#define REGISTER_INTERRUPTIONS(triggerFunc, callbackFunc) m_interruptionMap[triggerFunc] = callbackFunc;
#define UNREGISTER_INTERRUPTIONS(funcName) m_interruptionMap.erase(m_curPlayer->Fired);

public:

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
	void			reRegisterInterruptions();

	CREATE_FUNC(CMainScene);

public:

	UILayer*				m_UILayer;
	Layer*					m_mainLayer;

	CPlayer*				m_curPlayer;
	std::vector<CPlayer*>	m_playerVector;
	std::vector<CUIButton*> m_UIButton;
	std::vector<pFunc>		m_interruptionVector;
	std::map<pFunc, pFunc>  m_interruptionMap;

private:	
	int				m_playersOrderList[MAX_PLAYER_COUNT];
	int				m_curPlayerOrder;
	int				m_alivePlayersNum;
	int				m_deadPlayersNum;
	bool			m_curTurnIsOver;
	int				m_curTurnIndex;

	Camera*			m_camera;
	float			m_downCountingTimer;	
	float			m_playerActionTimer;
	GAME_STATE		m_gameState;
	GAME_STATE		m_prevGameState;

#ifdef _BOMB_DEBUG_
public:
	float			m_playerWaitingTimer;


#endif
};

