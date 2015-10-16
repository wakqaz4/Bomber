#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "navmesh\CCNavMeshAgent.h"

class Player : public cocos2d::Sprite3D
{
public:
	static Player* create(const char* file);
	void adjustAngle(float angle);
	void moveAgent(const cocos2d::Vec3 &des);
private:
	bool _removeable;
	cocos2d::NavMeshAgent *_agent;
	cocos2d::Sprite3D *_barrel;
	float _movePoints;
	float _rotationAngle;
};
#endif