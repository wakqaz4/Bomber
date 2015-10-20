#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Bomb.h"
#include "navmesh\CCNavMesh.h"



class Player : public cocos2d::Node
{


	enum PLAYER_STATE
	{
		WAITING,
		ACTING,
		FIRING,

		STATE_COUNT
	};
public:
	static Player* createPlayerByID(int ID);
public:
	Player(int m_ID)
		:m_canAct(false), m_isDead(false), m_fired(0), m_bombDidLanded(0),
		m_tank(NULL), m_barrel(NULL), m_agent(NULL), m_rotationAngle(0),
		m_playerState(NULL), m_curBomb(NULL)
	{
		this->m_sPlayerInfo.m_ID = m_ID;
	}

public:
	Player(){};
	virtual ~Player();//**should be virtual
	virtual bool init();
	virtual void update(float dt);
	void setPlayerAngle(int angle);
	void moveAgent(const cocos2d::Vec3 &des);
	float calAngle(void);

	inline bool EnableAction(){ m_canAct = true; return true; }
	inline bool DisableAction(){ m_canAct = false; return true; }
	bool IsDead(){ return m_isDead; }

	int BombDidLanded(){ return m_bombDidLanded; };

	int Fired(){ return m_fired; };
	void SetFired(int res){ m_fired = res; };

public:
	cocos2d::Sprite3D*		m_tank;
	cocos2d::Sprite3D*		m_barrel;
	cocos2d::NavMeshAgent*	m_agent;
	float					m_rotationAngle;	

	//此部分为UI暴露给玩家所视的
	struct PlayerInfo
	{
		std::string m_Name;
		int m_ID;
		/*
		//影响maxPower, healthPoint;
		int m_power;
		//影响criticalAtkRatio
		int m_luck;
		//影响armor,Strength
		int m_agility;
		//影响炮弹精度，炮弹威力，可使用更多种类的炮弹，自动追踪度？？
		int m_intelligence;
		//玩家技能
		std::vector<CSkill*> m_skills;
		*/
	}m_sPlayerInfo;

	PLAYER_STATE*	m_playerState;
	bool			m_canAct;
	bool			m_isDead;
	int			m_fired;
	int			m_bombDidLanded;

	Bomb* m_curBomb;
	//std::vector<Bomb*> m_bombs;

	//m_bombs* equipBomb();





public:
	//get&set
	inline const  bool			getIsCanAct()		   { return this->m_canAct; }
	inline const  bool			getIsDead()		       { return this->m_isDead; }
	inline const unsigned int  getIsFired()			   { return this->m_fired; }
	inline const unsigned int  getIsBombDidLanded( )	   { return this->m_bombDidLanded; }

	inline void setIsCanAct(const bool m_canAct)				{ this->m_canAct = m_canAct; }
	inline void setIsDead(const bool m_isDead)					{ this->m_isDead = m_isDead; }
	inline void setIsFired(const unsigned int m_fired)			{ this->m_fired = m_fired; }
	inline void setIsBombDidLanded(const unsigned int userIsFired)		{ this->m_bombDidLanded = userIsFired; }


private:

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
#endif