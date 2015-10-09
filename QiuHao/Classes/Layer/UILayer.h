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
	UILayer(  ):dirValue(0), powerValue(0) {}
	virtual bool init();

	//dir & power slider events
	void DirSliderEvent(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type);
	void PowerSliderEvent(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type);
	
	/*
	内联函数的定义对编译器必须是可见的, 这样编译器才可以在调用点进行内联展开该函数的代码.
	如果提供的头文件中,只有函数原型, 显然是不够的, 要把inline函数的函数体也放进头文件.
	inline的替换是在编译的时候就被执行的(到不了链接这步), 类似于宏,
	*/
	inline unsigned int getDirValue() { return this->dirValue; }
	inline void setDirValue(int dirValue){ this->dirValue = dirValue; }
	inline unsigned int getPowerValue(){ return this->powerValue; }
	inline void setPowerValue(int powerValue){ this->powerValue = powerValue; }
	
private:
	int dirValue;
	int powerValue;
};

#endif