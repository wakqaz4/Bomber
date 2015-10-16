#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "navmesh\CCNavMeshAgent.h"
/*
class Player : public cocos2d::Node
{
public:
	static Player* create(const char* file);
	void adjustAngle(float angle);
	
private:
	bool _removeable;
	cocos2d::NavMeshAgent *_agent;
	cocos2d::Sprite3D *_barrel;
	float _movePoints;
};*/
class Player : public cocos2d::Node
{
public:
	Player();
	~Player();
	virtual bool init();
	void moveAgent(const cocos2d::Vec3 &des);
	CREATE_FUNC(Player);
	

	cocos2d::Sprite3D *_tank;
	cocos2d::Sprite3D *_barrel;
	cocos2d::NavMeshAgent *_agent;
	float _rotationAngle;

	virtual void update(float dt);
	
	void setPlayerAngle(int angle);
};
#endif