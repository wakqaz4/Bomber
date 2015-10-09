#ifndef __TEST_H__
#define __TEST_H__
#include "cocos2d.h"
#include "Layer\UILayer.h"

//Sprite3D : http://www.ziliao1.com/Article/Show/21C3FA1186723D01C4D565F9795F5299.html

class MainScene :public cocos2d::Scene
{
public:
	//init steps:
	
	/*
		c++ 11 override
		描述：override保留字表示当前函数重写了基类的虚函数。
		目的：1.在函数比较多的情况下可以提示读者某个函数重写了基类虚函数（表示这个虚函数是从基类继承，不是派生类自己定义的）；
			  2.强制编译器检查某个函数是否重写基类虚函数，如果没有则报错
	*/
	//static cocos2d::Scene* createScene(); //直接运行 scene ,没有layer
	CREATE_FUNC(MainScene);
	MainScene(void);
	virtual ~MainScene(void);
	virtual bool init() override;

	/*点击事件*/
	virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);

	/* 发射炮弹 */
	void shootBullet ( const cocos2d::Vec3 &des );
	void shootBullet2( const cocos2d::Vec3 &des );

private:
	cocos2d::Camera* camera;
	float angle;
	bool needShootBox;
	UILayer* uiLayer;//show the ui
};

#endif