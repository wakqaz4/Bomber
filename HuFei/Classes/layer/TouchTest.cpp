#include "TouchTest.h" 

bool isTouch = false;

bool isMoved = false;

int pressTimes = 0;

int touchCounts = 0;

TouchTest::TouchTest()
{
	m_longProgress = false;
}

TouchTest::~TouchTest()
{

}

bool TouchTest::init()
{
	if (!Layer::init())
	{
		return false;
	}
	log("++++++++++++++");
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(TouchTest::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(TouchTest::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(TouchTest::onTouchEnded, this);
	listener->setSwallowTouches(true);//不向下传递触摸  
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

long long TouchTest::getCurrentTime()
{
	struct timeval tm;
	gettimeofday(&tm, NULL);
	return (long long)(tm.tv_sec * 1000 + tm.tv_usec / 1000);
}

bool TouchTest::touchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{

	m_startPoint = touch->getLocation();

	isTouch = true;

	m_startTime = getCurrentTime();

	//处理长按事件  
	this->schedule(schedule_selector(TouchTest::updatelongprogress), 1);

	return true;
}

void TouchTest::touchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
	isMoved = true;
	Point curPoint = touch->getLocation();
	//onMove(curPoint);

}

void TouchTest::touchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
	isTouch = false;
	pressTimes = 0;
	this->unschedule(schedule_selector(TouchTest::updatelongprogress));

	//如果刚完成长按事件 则把按下次数清零 长按状态置空 直接返回 不继续执行  
	if (m_longProgress) {
		touchCounts = 0;
		m_longProgress = false;

		return;
	}

	m_endPoint = touch->getLocation();

	long long endTime = getCurrentTime();

	long long timeDis = endTime - m_startTime;

	//E_SWIP_DIR dir = GetSwipDir(m_startPoint, m_endPoint, timeDis);

//	if (dir != E_INVAILD) {
	//	onSwip(m_startPoint, m_endPoint, dir);
//		return;
//	}

	//做连击判断  
	if (isMoved) {
		isMoved = false;
		return;
	}
	if (touchCounts == 2) {
	//	onThreeClick();
		touchCounts = 0;
	}
	else if (touchCounts == 1) {
		this->scheduleOnce(schedule_selector(TouchTest::updateDoubleDelay), 0.25);
		touchCounts++;
	}
	else if (touchCounts == 0) {
		this->scheduleOnce(schedule_selector(TouchTest::updateSingleDelay), 0.25);
		touchCounts++;
	}
}

void TouchTest::updateSingleDelay(float ft)
{
	if (touchCounts == 1) {
		onSingleCLick();
		touchCounts = 0;
	}

}

void TouchTest::updateDoubleDelay(float ft)
{
	if (touchCounts == 2)
	{
	//	onDoubleClick();
		touchCounts = 0;
	}
}

void TouchTest::updatelongprogress(float ft)
{
	if (isTouch) {
		pressTimes++;

		if (pressTimes >= 2) {
			m_longProgress = true;
		//	onLongPressed();

		}
	}
	else
	{
		pressTimes = 0;
	}
}

void TouchTest::onSingleCLick()
{
	log("single click");
}