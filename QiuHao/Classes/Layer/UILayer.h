#ifndef __UILAYER_H__
#define __UILAYER_H__

#include <cocos2d.h>
//#include "extensions/cocos-ext.h"//其他头文件的集合包括一些扩展( extension )宏定义
#include "ui\UISlider.h"
#include "ui\UIText.h"

class UILayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(UILayer);
	virtual bool init();

	//dir & power slider events
	void DirSliderEvent(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type);
	void PowerSliderEvent(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type);

private:
	cocos2d::ui::Text* dirValueLabel;//to show the percent
	cocos2d::ui::Text* powerValueLabel;//to show the percent
};

#endif