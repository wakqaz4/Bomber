#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"

class MainScene : public cocos2d::Scene
{
public:
	CREATE_FUNC(MainScene);
	MainScene(void);
	~MainScene(void);

	virtual bool init() override;

	cocos2d::NavMesh *_navMesh;
};
#endif