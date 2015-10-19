#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "navmesh\CCNavMeshAgent.h"

class Player : public cocos2d::Node
{
public:
	Player();
	~Player();
	virtual bool init();
	CREATE_FUNC(Player);
	void moveAgent(const cocos2d::Vec3 &des);

	cocos2d::Sprite3D *_tank;
	cocos2d::Sprite3D *_barrel;
	cocos2d::NavMeshAgent *_agent;
	float rotation;
protected:
	virtual void update(float dt);
	void setBarrelAngle(int angle);
	float calAngle();
};
#endif