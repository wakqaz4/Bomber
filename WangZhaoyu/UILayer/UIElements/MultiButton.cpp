#include "MultiButton.h"

unsigned int MultiFuncButton::dirValue = 0;
unsigned int MultiFuncButton::powerValue = 0;

bool MultiFuncButton::isEnable = false;
bool MultiFuncButton::isFired = false;
bool MultiFuncButton::isPress = false;

void MultiFuncButton::setIsEnable(bool ifEnable){ isEnable = ifEnable; }
bool MultiFuncButton::getIsEnable(){ return isEnable; }

MultiFuncButton* MultiFuncButton::create()
{
	MultiFuncButton* pRet = new(std::nothrow) MultiFuncButton();
	if (pRet && pRet->init())
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

bool MultiFuncButton::init()
{
	/*注册事件*/
	this->touchListener = EventListenerTouchOneByOne::create();
	this->touchListener->onTouchBegan = CC_CALLBACK_2(MultiFuncButton::onTouchBegan, this);
	this->touchListener->onTouchEnded = CC_CALLBACK_2(MultiFuncButton::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	return true;

}

///////////////////////////////////////////////////////////////////////////////////////////
void MultiFuncButton::setPowerGear(const std::string& background)
{
	this->powerGear = Sprite::create(background);
	this->addChild(this->powerGear,100);
}

///////////////////////////////////////////////////////////////////////////////////////////
void MultiFuncButton::setDirGear(const std::string& background)
{
	this->dirGear = Sprite::create(background);
	this->addChild(this->dirGear,100);
}

///////////////////////////////////////////////////////////////////////////////////////////
void MultiFuncButton::setFireButton(const std::string& background)
{
	this->fireButton = Sprite::create(background);
	this->addChild(this->fireButton,200);
	this->fireButton->setScale(1.5f);
}


///////////////////////////////////////////////////////////////////////////////////////////
void MultiFuncButton::setDecorationGear(const std::string& background)
{
	this->decorationGear = Sprite::create(background);
	this->addChild(this->decorationGear);
}


///////////////////////////////////////////////////////////////////////////////////////////
void MultiFuncButton::setBoundingGear(const std::string& background)
{
	this->bounding = Sprite::create(background);
	this->addChild(this->bounding,50);
	this->bounding->setScale(2.5f);
}


///////////////////////////////////////////////////////////////////////////////////////////

bool MultiFuncButton::onTouchBegan(Touch *touch, Event *unused_event)
{
	this->touch_begin_pos = std::move(touch->getLocation());
	if (this->bounding->getBoundingBox().containsPoint(touch_begin_pos))
	{
		this->touchListener->setSwallowTouches(true);
		this->isPress = true;//边界检测
	}
	else{
		this->touchListener->setSwallowTouches(false);
		return false;
	}
	
}

void MultiFuncButton::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (!this->isPress) return;
	if (!this->getIsEnable()) return;//后期可以添加特效，让按钮变灰色------------------》

	this->isPress = false;//记得设置回来

	this->touch_end_pos = std::move(touch->getLocation());

	/*两点间的距离：正数*/
	Vec2 diff = touch_end_pos - touch_begin_pos;
	diff.normalize();

	auto a = std::move(Vec2(1, 0));//horizontal line
	/*
	dot（）两个点向量的点积/数量积(a*b = |a||b|cos@ )
	Returns the dot product of this vector and the specified vector.
	@Param:
	v	The vector to compute the dot product with.
	@Ret:
	The dot product.

	这个方法用开始触摸和结束位置的向量，与水平线的向量的点积（即求两线夹角的cos值）
	来判断用户是往左滑还是往右滑;
	这个方式要用两个坐标值来比对要精准一些，
	因为用户拉出来的线条通常都并不是横平竖直的。

	*/
	auto result = Vec2::dot(diff, a);//夹角


	ActionInterval*  actionBy = RotateBy::create(0.2, this->step);
	ActionInterval*  actionByRevser = actionBy->reverse();

	if (diff.length() >= 0.5) //滑动还是点击
	{
		const int TURN_LEFT = 1;
		const int TURN_RIGHT = 2;
		if (abs(result) > 0.7)// left or right 
		{
			if (result > 0)//right
			{
				//step += 45;
				
				if (this->getPowerValue() < 100)
				{
					this->setPowerValue(this->getPowerValue() + 10);
					log("rightPower:%d",this->getPowerValue());
					this->powerGear->runAction(actionBy);
				}
				this->setPowerValue(this->getPowerValue() + 10 );
				//ActionInterval*  actionBy = RotateBy::create(0.2, this->step / 2);
				//this->decorationGear->runAction(actionBy);
				log("Right");
			}
			else //left
			{
				
				if ( this->getPowerValue() > 0 )
				{
					this->setPowerValue(this->getPowerValue() - 10);
					log("leftPower:%d", this->getPowerValue());
					this->powerGear->runAction(actionByRevser);
				}
				//ActionInterval*  actionBy = RotateBy::create(0.2, -this->step / 2);
				//this->decorationGear->runAction(actionBy);
				log("Light");
			}
		}
		else //up of down
		{
			if (diff.y>0)
			{
				
				if ( this->getDirValue() < 90 )
				{
					this->setDirValue(this->getDirValue() + 10);
					log("upDir:%d", this->getDirValue());
					this->dirGear->runAction(actionBy);
				}
				//ActionInterval*  actionBy = RotateBy::create(0.2, this->step / 2);
				//this->decorationGear->runAction(actionBy);
				log("Up");
			}
			else
			{
				
				if (this->getDirValue() > 0)
				{
					this->setDirValue(this->getDirValue() - 10);
					log("downDir:%d", this->getDirValue());
					this->dirGear->runAction(actionByRevser);
				}
				//ActionInterval*  actionBy = RotateBy::create(0.2, -this->step / 2);
				//this->decorationGear->runAction(actionBy);
				log("Down");
			}
		}
	}
	else
	{
		log("click");
		ActionInterval*  action1 = ScaleBy::create(0.1f, 0.9f);//如果点击太过频繁，就会缩小，不过因为每回合只能点一次，所以不会有问题
		auto action2 = action1->reverse();
		this->fireButton->runAction(Sequence::create(action1, action2, NULL));

		this->setIsFired(true);//已开火
	}

}


///////////////////////////////////////////////////////////////////////////////////////////
void MultiFuncButton::setPosition(float x,float y)
{
	
	this->dirGear->setPosition(x - dirGear->getContentSize().width*scale/2, y);
	this->powerGear->setPosition(x + powerGear->getContentSize().width*scale/2, y);
	this->bounding->setPosition(x, y);
//	this->decorationGear->setPosition(x, y);
	this->fireButton->setPosition(x, y);

}

///////////////////////////////////////////////////////////////////////////////////////////
void MultiFuncButton::setScale(float userScale)
{
	this->scale = userScale;
	this->dirGear->setScale(scale);
	this->powerGear->setScale(scale);
	this->bounding->setScale(2.5*scale);
//	this->decorationGear->setScale(scale);
	this->fireButton->setScale(1.5*scale);
}

