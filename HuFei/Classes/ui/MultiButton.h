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
		��д����ķ�����
	*/
	void setPosition(float x, float y);


	void setScale(float userScale);


	/***
		1.setPowerGear
		@params:
		const String& background ���������������ز�·��
	*/
	void setPowerGear(const std::string& background);
	

	/***
		2.setDirGear
		@params:
		const String& background �����ø߶ȳ����ز�·��
	*/
	void setDirGear(const std::string& background);
	

	/***
		3.setFireButton
		@params:
		const String& background �����ÿ���ť�ز�·��
	*/
	void setFireButton(const std::string& background);
	

	/***
		4.setDecorationGear
		@params:
		const String& background ������װ���Գ����ز�·��
	*/
	void setDecorationGear(const std::string& background);
	

	/***
		5.setBoundingGear
		@params:
		const String& background �����ñ����ز�·��
	*/
	void setBoundingGear(const std::string& background);
	


	/***dir & power slider events
		�����¼�������:
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

	Sprite* powerGear;			  //��������
	Sprite* dirGear;			  //�߶ȳ���
	Sprite* fireButton;			  //����ť
	Sprite* decorationGear;		  //װ�γ��֣����ĳ��֣�
	Sprite* bounding;			  //�߽�

	static bool isEnable;		  //�ð�ť�Ƿ���Խ��л���
	unsigned int step;			  //������ת�Ķ���
	static bool isFired;		  //�Ƿ��·��䰴ť
	static bool isPress;		  //�ڲ����ݽ��б߽��⣨���ṩ�ӿڣ�

	//����
	EventListenerTouchOneByOne* touchListener ;
	//touchListener->setSwallowTouches(true);//�¼�������

};

#endif