#include "Enemyer.h"
#include "global\Global.h"
#include <math.h>

#define PI 3.14159265
USING_NS_CC;

Enemyer* Enemyer::create(const char* file)
{
	auto sprite = new(std::nothrow)Enemyer();
	if (sprite && sprite->initWithFile(file))
	{
		auto dispatcher = Director::getInstance()->getEventDispatcher();
		auto listener = EventListenerTouchOneByOne::create();
		//listener->setSwallowTouches(true);//ÊÂ¼þµÄÆÁ±Î
		listener->onTouchBegan = CC_CALLBACK_2(Enemyer::onTouchBegan, sprite);
		dispatcher->addEventListenerWithSceneGraphPriority(listener, sprite);
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool Enemyer::onTouchBegan(Touch *touch,Event *event)
{
	Global *global = Global::getInstance();
	Vec3 v = global->_playerObj->_tank->getPosition3D();
	Vec2 des(0.0f,0.0f);
	Vec2 p(v.x, v.z);
	float tana = fabs(p.x - des.x) / fabs(p.y - des.y);
	float atan = std::atan(tana) * 180 / PI;

	if ((p.x - des.x) <= 0 && (p.y - des.y) >= 0)
		atan = 180 - atan;
	else if ((p.x - des.x) >= 0 && (p.y - des.y) >= 0)
		atan = 180 + atan;
	else if ((p.x - des.x) >= 0 && (p.y - des.y) <= 0)
		atan = 360 - atan;
	else if ((p.x - des.x) <= 0 && (p.y - des.y) <= 0)
		atan = atan;
	float fRadSeed = 3.14159f / 180.0f;
	kmMat4 kMat;
	kmMat4Identity(&kMat);
	kmMat4RotationY(&kMat, atan * fRadSeed);
	Quaternion quat(kMat);
	global->_playerObj->_agent->getOwner()->setRotationQuat(quat);
	return true;
}