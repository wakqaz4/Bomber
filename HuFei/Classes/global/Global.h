#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "cocos2d.h"
#include "entity\Enemyer.h"
#include "entity\Player.h"
#include "ui\MultiButton.h"

class Global
{
public:
	static Global* getInstance();

	//Ö÷ÉãÏñ»ú
	cocos2d::Camera *_camera;
	cocos2d::Physics3DWorld *_physics3DWorld;
	Enemyer *_enemyerObj;
	Player *_playerObj;
	MultiFuncButton *multiBtn;
};
#endif