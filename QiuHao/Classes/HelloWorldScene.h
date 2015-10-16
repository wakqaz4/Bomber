#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "UI\BarProgress.h"
#include "UI\MultiButton2.h"
#include "Emtity\Bomb.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);


private:
	BarProgress* barProgress;
	MultiFuncButton* multiBtn;
	Bomb* bomb;
	Camera* camera;

};

#endif // __HELLOWORLD_SCENE_H__
