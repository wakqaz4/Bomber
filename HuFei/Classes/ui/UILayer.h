#ifndef __UI_LAYER_H__
#define __UI_LAYER_H__

#include "cocos2d.h"

class UILayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(UILayer);
	virtual bool init();
	
	void angleUp(cocos2d::Ref *pSender);
	void angleDown(cocos2d::Ref *pSender);

	cocos2d::MenuItemImage *_angleUp;
	cocos2d::MenuItemImage *_angleDown;
};

#endif __UI_LAYER_H__