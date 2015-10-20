#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "cocos2d.h"
#include "../../Classes/mainLayer/entity/Player.h"
#include "../../Classes/UILayer/UIElements/MultiButton.h"

/**
* 存放全局指针
*/
class Global
{
public:
	static Global* getInstance();
	//主摄像机，用于地图的旋转、缩放
	cocos2d::Camera *_camera;

	//Should have a player vector to restore all players
	Player* _playerObj;
	Player* player2;
	std::vector<Player*> m_playersVector;
	cocos2d::Physics3DWorld *_physics3DWorld;
	cocos2d::Vec3 hitPosition;
	MultiFuncButton *multiBtn;
//	cocos2d::Vec3 &hitPosition;
};
#endif