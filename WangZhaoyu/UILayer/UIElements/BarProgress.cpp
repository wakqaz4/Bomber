#include "BarProgress.h"

////////////////////////////
BarProgress* BarProgress::create()
{
	BarProgress* pRet = new(std::nothrow) BarProgress();
	if (pRet)
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

////////////////////////////
void BarProgress::setBackground(const std::string& background)
{
	this->back = Sprite::create(background);
	this->back->setAnchorPoint(ccp(0,0));
	this->addChild(back);
}

////////////////////////////
void BarProgress::setBackground(const std::string& background, const Vec2& position)
{
	this->setBackground(background);
	this->back->setPosition(position);
}

////////////////////////////
void BarProgress::setBackground(const std::string& background, const Vec2& position, const Vec2& anchorPoint)
{
	this->setBackground(background, position);
	this->back->setAnchorPoint(anchorPoint);
}

////////////////////////////
void BarProgress::setBackground(const std::string& background, const Vec2& position, const Vec2& anchorPoint, const float scaleX, const float scaleY)
{
	this->setBackground(background, position, anchorPoint);
	this->back->setScaleX(scaleX);
	this->back->setScaleY(scaleY);
}


////////////////////////////
void BarProgress::setForeground(const std::string& foreground)
{
	this->fore = Sprite::create(foreground);
	this->progressTimer = ProgressTimer::create(fore);
	this->progressTimer->setType(ProgressTimer::Type::BAR); //这里限定为 bar 而不是 radial
	this->progressTimer->setAnchorPoint(ccp(0,0));
	this->addChild(this->progressTimer);
}

////////////////////////////
void BarProgress::setForeground(const std::string& foreground, const Vec2& position)
{
	this->setForeground(foreground);
	this->setForeground(foreground);
	this->progressTimer->setPosition(position);
}

////////////////////////////
void BarProgress::setForeground(const std::string& foreground, const Vec2& position, const Vec2& anchorPoint)
{
	this->setForeground(foreground, position);
	//this->fore->setAnchorPoint(anchorPoint);
	this->progressTimer->setAnchorPoint(anchorPoint);
}

////////////////////////////
void BarProgress::setForeground(const std::string& foreground, const Vec2& position, const Vec2& anchorPoint, const float scaleX, const float scaleY)
{
	this->setForeground(foreground, position, anchorPoint);
	//this->fore->setScaleX(scaleX);
	//this->fore->setScaleY(scaleY);
	this->progressTimer->setScaleX(scaleX);
	this->progressTimer->setScaleY(scaleY);
}

////////////////////////////
void BarProgress::setDynamicParams(const Vec2& midPoint, const Vec2& barChangeRate)
{
	//progressTimer->setMidpoint(ccp(0, 0));
	//progressTimer->setBarChangeRate(ccp(1, 0)); //设置进度条为从左向右随进度增长而显现
	this->progressTimer->setMidpoint(midPoint);
	this->progressTimer->setBarChangeRate(barChangeRate);
}

////////////////////////////
void BarProgress::setPercentage(const float curPercent)
{
	this->progressTimer->setPercentage(curPercent);
}

////////////////////////////
float BarProgress::getPercentage() const
{
	return this->progressTimer->getPercentage();
}

////////////////////////////
void BarProgress::setPosition(float x, float y)
{
	this->back->setPosition( x,y );
	this->progressTimer->setPosition( x,y );
}

////////////////////////////
void BarProgress::setScale(float userScale)
{
	this->back->setScale(userScale);
	this->progressTimer->setScale(userScale);
}