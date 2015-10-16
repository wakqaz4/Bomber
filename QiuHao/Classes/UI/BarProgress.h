#ifndef __BAR_PROGRESS_H__
#define __BAR_PROGRESS_H__

#include "cocos2d.h"
USING_NS_CC;
/*
//使用范例：
BarProgress* barProgress = BarProgress::create();
barProgress->setBackground("background.png", ccp(50, 400), ccp(0, 0));
barProgress->setForeground("foreground.png", ccp(50, 400), ccp(0, 0));
barProgress->setDynamicParams(ccp(0, 0), ccp(1, 0));	//从左向右，且只是长度变化，高度不变
barProgress->setPercentage(67.f);						//0~100%，现在设置为50%
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
		const String& background ：设置背景素材路径
	*/

	void setBackground(const std::string& background);

private://下述方法被弃置掉
	void setBackground(const std::string& background, const Vec2& position);
	void setBackground(const std::string& background, const Vec2& position, const Vec2& anchorPoint);
	void setBackground(const std::string& background, const Vec2& position, const Vec2& anchorPoint, const float scaleX, const float scaleY);
	
	/***
		setForekground
		@params:
		const String& foreGround ：设置前景资源路径
	*/
public:
	void setForeground(const std::string& foreground);

private://下述方法被弃置掉
	void setForeground(const std::string& foreground, const Vec2& position);
	void setForeground(const std::string& foreground, const Vec2& position, const Vec2& anchorPoint);
	void setForeground(const std::string& foreground, const Vec2& position, const Vec2& anchorPoint, const float scaleX, const float scaleY);
	
	/***
		setDynamicParams
		@params:
		midPoint		 :
		中点（Midpoint）表示进度条变化的起始位置
		代表着进度条展开的方向。 进度条从中心位置向两边展开，因此：
		进度条是从左往右展开时，设置中点（Midpoint）为Vec2(0,y);
		进度条是从右往左展开时，设置中点（Midpoint）为Vec2(1,y);

		barChangeRate	:
		设置条形模式进度条非变化方向显示的比例（barChangeRate）
		设置为0可以保证进度条非变化方向100显示。
		例如，如果你需要一个从左往右的进度条，但是上下方向不需要100显示，
		那么设置barChangeRate为Vec2(0,1),
	*/

public:
	void setDynamicParams(const Vec2& midPoint, const Vec2& barChangeRate);

	/***
		setPercentage
		@params:
		curPercent:进度条当前值
	*/

	void setPercentage(const float curPercent);

	/***
		getPercentage
		@return:
		返回当前进度条的值
	*/

	float getPercentage() const;

	/***
		0.setPosition & setScale
		重写父类的方法。
	*/
	void setPosition(float x, float y);


	void setScale(float userScale);


private:
	Sprite* back;
	Sprite* fore;
	ProgressTimer* progressTimer;
};

#endif