#include <iostream>  
#include "cocos2d.h"  
USING_NS_CC;

/*

	void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
	void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
	void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
	void onTouchesCancelled(const std::vector<Touch*>&touches, Event *unused_event);

public:

};
*/

#ifndef __TOUCH_TEST_H__
#define __TOUCH_TEST_H__

USING_NS_CC;
const int minSwipdistance = 100;
const int minSwiptime = 1000;    //毫秒  
const int maxClickedDis = 20;
enum E_SWIP_DIR
{
	E_INVAILD,
	E_LEFT,
	E_RIGHT,
	E_UP,
	E_DOWN
};

class TouchTest : public Layer
{
public:
	CREATE_FUNC(TouchTest);
	TouchTest(void);
	~TouchTest(void);

	bool touchBegan(Touch * touch, Event * event);
	void touchMoved(Touch * touch, Event * event);
	void touchEnded(Touch * touch, Event * event);
//	void touchCancel(Touch * touch, Event * event);

	void updateSingleDelay(float);
	void updateDoubleDelay(float);

	void updatelongprogress(float);
	virtual bool init() override;
//	void stopSchedule();

	long long getCurrentTime();

//	E_SWIP_DIR GetSwipDir(Point start, Point end, long long timeDis);
public:
	virtual void onSingleCLick();        //单击  
///	virtual void onDoubleClick();        //双击  
//	virtual void onThreeClick();         //3连击  
//	virtual void onLongPressed();        //长按  
//	virtual void onMove(Point);          //移动  
//	virtual void onSwip(Point, Point, E_SWIP_DIR);    //滑动  


	bool m_longProgress;
	Vec2 m_startPoint, m_endPoint;
	long long m_startTime;
};

#endif