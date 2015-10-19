#ifndef __MAIN_LAYER_H__
#define __MAIN_LAYER_H__

#include "cocos2d.h"

/**
 * 主层，添加地图、人物、敌人等实体
 */
class MainLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(MainLayer);
	MainLayer(void);
	~MainLayer(void);

	virtual bool init() override;
protected:
	void initLayer();
	void terrainRotation(cocos2d::Vec2);
	void onSingleClick();
	void updateSingleDelay(float);
	bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event);
	void onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event);
	void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event);

	cocos2d::Sprite3D *_terrain;
	float _angle;
	cocos2d::Vec2 _singleClickLocation;
};

#endif