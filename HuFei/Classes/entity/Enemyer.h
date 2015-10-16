#ifndef __ENEMYER_H__
#define __ENEMYER_H__

#include "cocos2d.h"

class Enemyer : public cocos2d::Sprite3D
{
public:
	static Enemyer* create(const char* file);
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	cocos2d::Sprite3D *_enemyer;
};
#endif