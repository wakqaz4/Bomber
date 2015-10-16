#ifndef __MUTI_FUNC_BUTTON_H__
#define __MUTI_FUNC_BUTTON_H__

#include "cocos2d.h"

USING_NS_CC;

class MultiFuncButton :public Node
{
public:
	static MultiFuncButton*  create();
	MultiFuncButton()
		:  step(0),
		dirGear(nullptr), powerGear(nullptr), bounding(nullptr), fireButton(nullptr), decorationGear(nullptr),
		touch_begin_pos(Vec2(0, 0)), touch_end_pos(Vec2(0, 0)), scale(1.f){}

	virtual~MultiFuncButton(){}
	virtual bool init();


	/***
		0.setPosition & setScale
		重写父类的方法。
	*/
	void setPosition(float x, float y);


	void setScale(float userScale);


	/***
		1.setPowerGear
		@params:
		const String& background ：设置蓄力齿轮素材路径
	*/
	void setPowerGear(const std::string& background);
	

	/***
		2.setDirGear
		@params:
		const String& background ：设置高度齿轮素材路径
	*/
	void setDirGear(const std::string& background);
	

	/***
		3.setFireButton
		@params:
		const String& background ：设置开火按钮素材路径
	*/
	void setFireButton(const std::string& background);
	

	/***
		4.setDecorationGear
		@params:
		const String& background ：设置装饰性齿轮素材路径
	*/
	void setDecorationGear(const std::string& background);
	

	/***
		5.setBoundingGear
		@params:
		const String& background ：设置背景素材路径
	*/
	void setBoundingGear(const std::string& background);
	


	/***dir & power slider events
		触摸事件处理函数:
	*/
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	/***
		get&set powerData/dirData/step
	*/
	inline const unsigned int getDirValue()			{ return this->dirValue; }
	inline const unsigned int getPowerValue()		{ return this->powerValue; }
	inline const unsigned int getStep()				{ return this->step; }
	inline const bool getIsFired()					{ return this->isFired; }

	inline void setDirValue(const int dirValue)				{ this->dirValue = dirValue; }
	inline void setPowerValue(const int powerValue)			{ this->powerValue = powerValue; }
	inline void setStep(const unsigned int step)			{ this->step = step; }
	inline void setIsFired(const bool userIsFired)			{ this->isFired = userIsFired; }
	/***
		get&set isEnable
	*/
	static void setIsEnable(bool isEnable);
	static bool getIsEnable();
	float scale;//-------------------------------->temp




private:
	static unsigned int dirValue;
	static unsigned int powerValue;
	Vec2 touch_begin_pos;
	Vec2 touch_end_pos;

	Sprite* powerGear;			  //蓄力齿轮
	Sprite* dirGear;			  //高度齿轮
	Sprite* fireButton;			  //开火按钮
	Sprite* decorationGear;		  //装饰齿轮（最大的齿轮）
	Sprite* bounding;			  //边界

	static bool isEnable;		  //该按钮是否可以进行互动
	unsigned int step;			  //齿轮旋转的度数
	static bool isFired;		  //是否按下发射按钮
	static bool isPress;		  //内部数据进行边界检测（不提供接口）

	//监听
	EventListenerTouchOneByOne* touchListener ;
	//touchListener->setSwallowTouches(true);//事件的屏蔽

};

#endif