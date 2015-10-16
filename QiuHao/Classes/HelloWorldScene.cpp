#include "HelloWorldScene.h"


USING_NS_CC;

Scene* HelloWorld::createScene()
{
  
    return HelloWorld::create();
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
	//closeItem->setAnchorPoint(Vec2(0,0));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

  
	/**** test the demo ：创建示例**/
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//初始化cmaera
	this->camera = Camera::createPerspective(30.0f, visibleSize.width / visibleSize.height, 1.0f, 1000.0f);
	this->camera->setPosition3D(Vec3(0.0f, 50.0f, 100.0f));
	this->camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	this->camera->setCameraFlag(CameraFlag::USER1);
	this->addChild(this->camera);
	
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//血条/移动点数
	this->barProgress = BarProgress::create();
	this->barProgress->setBackground("background.png");
	this->barProgress->setForeground("foreground.png");
	this->barProgress->setDynamicParams(ccp(0, 0), ccp(1, 0));	//从左向右，且只是长度变化，高度不变
	this->barProgress->setPercentage(100.f);						//0~100%，现在设置为50%
	this->barProgress->setPosition(300, 500);
	this->barProgress->setScale(1.5f);
	this->addChild(barProgress);
	
	
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//多功能按钮	
	multiBtn = MultiFuncButton::create();
	multiBtn->setBoundingGear("bounding.png");
	multiBtn->setDirGear("testUI01.png");
	multiBtn->setPowerGear("testUI01.png");
	multiBtn->setFireButton("button.png");
	//还有一个装饰的大齿轮，素材没调好，不展示
	multiBtn->setIsEnable(true);
	multiBtn->setStep(90.f);
	this->multiBtn->setScale(0.5f);
	this->multiBtn->setPosition(visibleSize.width - 500, visibleSize.height / 2 + 100);//位置调整要在后

	this->addChild(this->multiBtn);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//子弹类
	
	
	if (initWithPhysics())//initWithPhysics()只有 scene 有该方法
	{
		this->bomb = Bomb::create();
	
		Physics3DRigidBodyDes	rbDes;
								rbDes.originalTransform.translate(this->camera->getPosition3D());
								rbDes.mass = 1.f;
								rbDes.shape = Physics3DShape::createSphere(0.5f);
								this->bomb->set3DParams("Sprite3DTest/box.c3t", "Icon.png", &rbDes,
									Vec3(2,2,2),0.5f);
							
		this->bomb->setRigidParams(Vec3::ONE, Vec3::ZERO,0.5f,0.4f);//刚体等物理属性的设置必须放在scene

		Vec3 dir(0.f, 5291.0f, 5000.0f);//方向向量
		float power = 30;				//power
		this->bomb->setDirAndPower( dir,power);
		this->bomb->setCameraMask((unsigned short)CameraFlag::USER1);

		this->addChild( this->bomb );
	}
	
	auto sprite = Sprite3D::create();
	sprite->setAnchorPoint(Vec2(0,0));



	/*
	if (initWithPhysics())
	{
		this->getPhysics3DWorld()->setDebugDrawEnable(false);

		Physics3DRigidBodyDes rbDes;
		rbDes.originalTransform.translate(this->camera->getPosition3D());   //矩阵变换
		rbDes.mass = 1.f;													//质量
		rbDes.shape = Physics3DShape::createSphere(0.5f);				    //形状

		Vec3 linearVel(0.f, 5291.0f, 5000.0f);												//******des 
		linearVel.normalize();											//向量规范化
		linearVel *= 20;		//改变速度：power***********
		log(" dir2:====%f,%f,%f ", linearVel.x, linearVel.y, linearVel.z);
		PhysicsSprite3D* sprite = PhysicsSprite3D::create("Sprite3DTest/box.c3t", &rbDes);
		sprite->setTexture("Images/Icon.png");

	

		Physics3DRigidBody* rigidBody = static_cast<Physics3DRigidBody*>(sprite->getPhysicsObj()); //class CC_DLL Physics3DRigidBody : public Physics3DObject
		rigidBody->setLinearFactor(Vec3::ONE);		//设置线性因子
		rigidBody->setLinearVelocity(linearVel);	//设置线性速度*****
		rigidBody->setAngularVelocity(Vec3::ZERO);	//设置角速度
		rigidBody->setCcdMotionThreshold(0.5f);		//设置运动阀
		rigidBody->setCcdSweptSphereRadius(0.4f);	//设置球面半径

		

		this->addChild(sprite);
		sprite->setPosition3D(Vec3(3, 4, 5)); //设置起始位置
		sprite->setScale(0.5f);
		sprite->syncNodeToPhysics();

		//optimize, only sync node to physics
		sprite->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE); //sync node to physics

		sprite->setCameraMask((unsigned short)CameraFlag::USER1);
	}

	*/

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
   // Director::getInstance()->end();
	this->barProgress->setPercentage(50.F);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
