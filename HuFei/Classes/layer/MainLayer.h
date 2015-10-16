#ifndef __MAIN_LAYER_H__
#define __MAIN_LAYER_H__

#include "cocos2d.h"
#include "navmesh\CCNavMesh.h"

/**
 * ���㣬��ӵ�ͼ��������˵�ʵ��
 */
class MainLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(MainLayer);
	MainLayer(void);
	~MainLayer(void);

	virtual bool init() override;
	bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event);
	void onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event);
	void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event);

	void onSingleClick();
	void updateSingleDelay(float);
	void terrainRotation(cocos2d::Vec2);
protected:
	void initLayer();
	float _angle;
	cocos2d::Sprite3D *_terrain;
	cocos2d::Vec2 _singleClickLocation;
};
#endif