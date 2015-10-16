#ifndef __BAR_PROGRESS_H__
#define __BAR_PROGRESS_H__

#include "cocos2d.h"
USING_NS_CC;
/*
//ʹ�÷�����
BarProgress* barProgress = BarProgress::create();
barProgress->setBackground("background.png", ccp(50, 400), ccp(0, 0));
barProgress->setForeground("foreground.png", ccp(50, 400), ccp(0, 0));
barProgress->setDynamicParams(ccp(0, 0), ccp(1, 0));	//�������ң���ֻ�ǳ��ȱ仯���߶Ȳ���
barProgress->setPercentage(67.f);						//0~100%����������Ϊ50%
//float update = barProgress->getPercentage();

this->addChild(barProgress);
*/

class BarProgress : public Node
{
public:
	/***
		create():then class Porgress object is autorelease
	*/

	static BarProgress*  create();
	BarProgress();
	~BarProgress();
	
	/***
		setBackground
		@params:
		const String& background �����ñ����ز�·��
	*/

	void setBackground(const std::string& background);

private://�������������õ�
	void setBackground(const std::string& background, const Vec2& position);
	void setBackground(const std::string& background, const Vec2& position, const Vec2& anchorPoint);
	void setBackground(const std::string& background, const Vec2& position, const Vec2& anchorPoint, const float scaleX, const float scaleY);
	
	/***
		setForekground
		@params:
		const String& foreGround ������ǰ����Դ·��
	*/
public:
	void setForeground(const std::string& foreground);

private://�������������õ�
	void setForeground(const std::string& foreground, const Vec2& position);
	void setForeground(const std::string& foreground, const Vec2& position, const Vec2& anchorPoint);
	void setForeground(const std::string& foreground, const Vec2& position, const Vec2& anchorPoint, const float scaleX, const float scaleY);
	
	/***
		setDynamicParams
		@params:
		midPoint		 :
		�е㣨Midpoint����ʾ�������仯����ʼλ��
		�����Ž�����չ���ķ��� ������������λ��������չ������ˣ�
		�������Ǵ�������չ��ʱ�������е㣨Midpoint��ΪVec2(0,y);
		�������Ǵ�������չ��ʱ�������е㣨Midpoint��ΪVec2(1,y);

		barChangeRate	:
		��������ģʽ�������Ǳ仯������ʾ�ı�����barChangeRate��
		����Ϊ0���Ա�֤�������Ǳ仯����100��ʾ��
		���磬�������Ҫһ���������ҵĽ��������������·�����Ҫ100��ʾ��
		��ô����barChangeRateΪVec2(0,1),
	*/

public:
	void setDynamicParams(const Vec2& midPoint, const Vec2& barChangeRate);

	/***
		setPercentage
		@params:
		curPercent:��������ǰֵ
	*/

	void setPercentage(const float curPercent);

	/***
		getPercentage
		@return:
		���ص�ǰ��������ֵ
	*/

	float getPercentage() const;

	/***
		0.setPosition & setScale
		��д����ķ�����
	*/
	void setPosition(float x, float y);


	void setScale(float userScale);


private:
	Sprite* back;
	Sprite* fore;
	ProgressTimer* progressTimer;
};

#endif