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
		������override�����ֱ�ʾ��ǰ������д�˻�����麯����
		Ŀ�ģ�1.�ں����Ƚ϶������¿�����ʾ����ĳ��������д�˻����麯������ʾ����麯���Ǵӻ���̳У������������Լ�����ģ���
			  2.ǿ�Ʊ��������ĳ�������Ƿ���д�����麯�������û���򱨴�
	*/
	//static cocos2d::Scene* createScene(); //ֱ������ scene ,û��layer
	CREATE_FUNC(MainScene);
	MainScene(void);
	virtual ~MainScene(void);
	virtual bool init() override;

	/*����¼�*/
	virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);

	/* �����ڵ� */
	void shootBullet ( const cocos2d::Vec3 &des );
	void shootBullet2( const cocos2d::Vec3 &des );

private:
	cocos2d::Camera* camera;
	float angle;
	bool needShootBox;
	UILayer* uiLayer;//show the ui
};

#endif