#include "UILayer.h"

#include "2d\CCMenuItem.h"
#include "global\Global.h"

USING_NS_CC;

bool UILayer::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	_angleUp = MenuItemImage::create("images/up.png", "images/up.png",
		CC_CALLBACK_1(UILayer::angleUp, this));
	_angleUp->setPosition(Vec2(origin.x + visibleSize.width - _angleUp->getContentSize().width / 2,
		origin.y + _angleUp->getContentSize().height / 2));

	_angleDown = MenuItemImage::create("images/down.png", "images/down.png",
		CC_CALLBACK_1(UILayer::angleDown, this));
	_angleDown->setPosition(Vec2(_angleDown->getContentSize().width,
		origin.y + _angleDown->getContentSize().height / 2));

	auto menu = Menu::create(_angleUp, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	auto menu2 = Menu::create(_angleDown, NULL);
	menu2->setPosition(Vec2::ZERO);
	this->addChild(menu2, 1);
	return true;
}
static int angleCount = 270;
void UILayer::angleUp(cocos2d::Ref *pSender)
{
	Global *global = Global::getInstance();
	float fRadSeed = 3.14159f / 180.0f;
	kmMat4 kMat;
	kmMat4Identity(&kMat);
	angleCount+=10;
	kmMat4RotationX(&kMat, angleCount * fRadSeed);
	Quaternion quat(kMat);
	global->_playerObj->_barrel->setRotationQuat(quat);
	log("up ++  %d ",angleCount);
}

void UILayer::angleDown(cocos2d::Ref *pSender)
{
	Global *global = Global::getInstance();
	float fRadSeed = 3.14159f / 180.0f;
	kmMat4 kMat;
	kmMat4Identity(&kMat);
	angleCount -=10;
	kmMat4RotationX(&kMat, angleCount * fRadSeed);
	Quaternion quat(kMat);
	global->_playerObj->_barrel->setRotationQuat(quat);
	log("down --  %d ",angleCount);
}