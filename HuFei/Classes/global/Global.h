#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "cocos2d.h"
#include "entity\Player.h"
#include "entity\Enemyer.h"
#include "ui\MultiButton.h"
/**
* 存放全局指针
*/
class Global
{
public:
	static Global* getInstance();
	//主摄像机，用于地图的旋转、缩放
	cocos2d::Camera *_camera;
	Player* _playerObj;
	
	Enemyer *_enemyerObj;
	cocos2d::Physics3DWorld *_physics3DWorld;
	cocos2d::Vec3 hitPosition;
	MultiFuncButton *multiBtn;
//	cocos2d::Vec3 &hitPosition;
};
#endif