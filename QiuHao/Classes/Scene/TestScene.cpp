#include "Scene\TestScene.h"
#include "physics3d\CCPhysics3D.h"

USING_NS_CC;
//static cocos2d::Scene* physicsScene = nullptr;

#define START_POS_X 3
#define START_POS_Y 4
#define START_POS_Z 5


/////////////////////////////////


MainScene::MainScene( void ) :camera( nullptr ),angle( 0.0f ){}

MainScene::~MainScene(void){}

bool MainScene::init( void )
{
	if ( !Scene::init() )	return false;


	/**** 0.添加uiLayer*/
	this->uiLayer = UILayer::create();
	this->addChild(uiLayer);


	if (initWithPhysics())
	{
		/**** 1.init  */
		getPhysics3DWorld()->setDebugDrawEnable(false);//#include "physics3d\CCPhysics3D.h" : Enable or disable debug drawing.
		Size size = Director::getInstance()->sharedDirector()->getWinSize();


		/**** 2.设置透视相机 Perspective*/
		this->camera = Camera::createPerspective( 30.0f, size.width / size.height, 1.0f, 1000.0f );
		this->camera -> setPosition3D(Vec3(0.0f, 50.0f, 100.0f));
		/*lookAt
		使相机看着目标
			@param:
			target	目标的位置
			up	相机向上的向量,通常这是Y轴 
		*/
		this->camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
		this->camera->setCameraFlag(CameraFlag::USER1);
		this->addChild(camera);


		/**** 3.添加事件监听*/
		EventListenerTouchAllAtOnce* listener = EventListenerTouchAllAtOnce::create();
									 listener->onTouchesBegan = CC_CALLBACK_2(MainScene::onTouchesBegan, this);
									 listener->onTouchesMoved = CC_CALLBACK_2(MainScene::onTouchesMoved, this);
									 listener->onTouchesEnded = CC_CALLBACK_2(MainScene::onTouchesEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


		/*** 4.创建physics floor 3D精灵*/
		Physics3DRigidBodyDes rbDes; /*rb:rigidBody Description 刚体描述*/
							  rbDes.mass = 0.0f; //质量： Note: mass equals zero means static, default 0
							  rbDes.shape = Physics3DShape::createBox(Vec3(60.0f, 1.0f, 60.0f));//形状

		PhysicsSprite3D* floor = PhysicsSprite3D::create("Sprite3DTest/box.c3t", &rbDes);
						//floor->setTexture( "Sprite3DTest/plane.png" );
						floor->setTexture("Sprite3DTest/plane1.png");
						floor->setScaleX( 60 );
						floor->setScaleZ( 60 );
		this->addChild( floor );
						floor->setCameraMask((unsigned short)CameraFlag::USER1);
						floor->syncNodeToPhysics();//同步 node 转换为 physics
						//static object sync is not needed
						floor->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::NONE);
		
		/**** 5.创建角色（武器） */
	    Sprite3D* weapon = Sprite3D::create("Sprite3DTest/boss.c3b");
		weapon->setRotation3D(Vec3(-90.0f, 0.0f, 0.0f));
		weapon->setPosition3D(Vec3(START_POS_X, START_POS_Y, START_POS_Z));
		weapon->setCameraMask(2);
	    this->addChild( weapon );

		/****7. */
		this->setPhysics3DDebugCamera(this->camera);
		this->angle = 0.0f;
		return true;
	}

	return false;
}



/////////////////////////////////
/*
	@params:
	des : 目标点位置
*/
void MainScene::shootBullet(const cocos2d::Vec3 &des) 
{
	/**** 1.创建physics BALL 3D精灵*/
	Physics3DRigidBodyDes rbDes;
		 rbDes.originalTransform.translate( this->camera->getPosition3D() );//矩阵变换
		 rbDes.mass = 1.f;													//质量
		 rbDes.shape = Physics3DShape::createSphere( 0.5f );				//形状

		 Vec3 linearVel = des - this->camera->getPosition3D();				//******des = this->camera->getPosition3D() + dir*10.0f 
			  linearVel.normalize();										//向量规范化
		      linearVel *= 100.0f;											//修改这个数值可以改变弹出去的速度*****
	
	PhysicsSprite3D* sprite = PhysicsSprite3D::create("Sprite3DTest/box.c3t", &rbDes);
	sprite->setTexture( "Images/Icon.png" );

	/**** 2.设置 3D精灵 刚体属性*/
	Physics3DRigidBody* rigidBody = static_cast<Physics3DRigidBody*>( sprite->getPhysicsObj() ); //class CC_DLL Physics3DRigidBody : public Physics3DObject
						rigidBody->setLinearFactor(Vec3::ONE);		//设置线性因子
						rigidBody->setLinearVelocity(linearVel);	//设置线性速度*****
						rigidBody->setAngularVelocity(Vec3::ZERO);	//设置角速度
						rigidBody->setCcdMotionThreshold(0.5f);		//设置运动阀
						rigidBody->setCcdSweptSphereRadius(0.4f);	//设置球面半径

	/**** 3.*/
	this->addChild( sprite );
		sprite->setPosition3D( this->camera->getPosition3D() );		//设置起始位置
		sprite->setScale(0.5f);
		sprite->syncNodeToPhysics();

		//optimize, only sync node to physics
		sprite->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE); //sync node to physics

		sprite->setCameraMask((unsigned short)CameraFlag::USER1);
}

/*
dir1:====0.000000,4541.000000,5000.000000
dir2:====0.000000,4.257077,4.687379

dir1:====0.000000,4541.000000,5000.000000
dir2:====0.000000,4.257077,4.687379

dir1:====0.000000,4541.000000,5000.000000
dir2:====0.000000,16.805103,18.503746

dir1:====0.000000,4541.000000,5000.000000
dir2:====0.000000,16.805103,18.503746

dir1:====0.000000,4541.000000,5000.000000
dir2:====0.000000,16.805103,18.503746

dir1:====0.000000,4541.000000,5000.000000
dir2:====0.000000,16.805103,18.503746

dir1:====0.000000,2083.000000,5000.000000
dir2:====0.000000,9.612535,23.073778

dir1:====0.000000,2083.000000,5000.000000
dir2:====0.000000,9.612535,23.073778

dir1:====0.000000,2083.000000,5000.000000
dir2:====0.000000,9.612535,23.073778

dir1:====0.000000,5083.000000,5000.000000
dir2:====0.000000,17.819733,17.528755

dir1:====0.000000,5083.000000,5000.000000
dir2:====0.000000,17.819733,17.528755

dir1:====0.000000,7082.000000,5000.000000
dir2:====0.000000,20.419649,14.416584

dir1:====0.000000,7082.000000,5000.000000
dir2:====0.000000,20.419649,14.416584

dir1:====0.000000,9124.000000,5000.000000
dir2:====0.000000,21.920330,12.012457

dir1:====0.000000,9124.000000,5000.000000
dir2:====0.000000,21.920330,12.012457

dir1:====0.000000,9124.000000,5000.000000
dir2:====0.000000,21.920330,12.012457

dir1:====0.000000,10000.000000,5000.000000
dir2:====0.000000,22.357103,11.178552

dir1:====0.000000,10000.000000,5000.000000
dir2:====0.000000,22.357103,11.178552

dir1:====0.000000,10000.000000,5000.000000
dir2:====0.000000,22.357103,11.178552

dir1:====0.000000,10000.000000,5000.000000
dir2:====0.000000,22.357103,11.178552

*/
void MainScene::shootBullet2( const cocos2d::Vec3 &des)
{
	/**** 1.创建physics BALL 3D精灵*/
	Physics3DRigidBodyDes rbDes;
		rbDes.originalTransform.translate(this->camera->getPosition3D());   //矩阵变换
		rbDes.mass = 1.f;													//质量
		rbDes.shape = Physics3DShape::createSphere(0.5f);				    //形状

		Vec3 linearVel = des ;												//******des 
			 linearVel.normalize();											//向量规范化
			 linearVel *= ( this->uiLayer->getPowerValue() / 250.0f );		//改变速度：power***********
			 log(" dir2:====%f,%f,%f ", linearVel.x, linearVel.y, linearVel.z);

	PhysicsSprite3D* sprite = PhysicsSprite3D::create("Sprite3DTest/box.c3t", &rbDes);
	sprite->setTexture("Images/Icon.png");

	/**** 2.设置 3D精灵 刚体属性*/
	Physics3DRigidBody* rigidBody = static_cast<Physics3DRigidBody*>(sprite->getPhysicsObj()); //class CC_DLL Physics3DRigidBody : public Physics3DObject
						rigidBody->setLinearFactor(Vec3::ONE);		//设置线性因子
						rigidBody->setLinearVelocity(linearVel);	//设置线性速度*****
						rigidBody->setAngularVelocity(Vec3::ZERO);	//设置角速度
						rigidBody->setCcdMotionThreshold(0.5f);		//设置运动阀
						rigidBody->setCcdSweptSphereRadius(0.4f);	//设置球面半径

	/**** 3.*/
	this->addChild(sprite);
			sprite->setPosition3D(Vec3(START_POS_X, START_POS_Y, START_POS_Z)); //设置起始位置
			sprite->setScale(0.5f);
			sprite->syncNodeToPhysics();

	//optimize, only sync node to physics
	sprite->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE); //sync node to physics

	sprite->setCameraMask((unsigned short)CameraFlag::USER1);

}


/////////////////////////////////
/*
	１.UI坐标系/屏幕  坐标系：　原点在左上角，X轴向右，Y轴向下。( 因为要从上向下开始排列图标 )：处理触摸事件
	２.GL坐标系/cocos 坐标系：　原点在左下角，X轴向右，Y轴向上。（ 右手 ）
	   GL坐标系下：
	·世界            坐标系：　就是 GL 坐标系。
	·本地			  坐标系：　相对于父对象的坐标,也就是节点（CCNode）的坐标系，原点在节点左下角，x轴向右，y轴向上,getPosition3D
	 http://www.cnblogs.com/pengyingh/articles/2513010.html
*/
void MainScene::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
	this->needShootBox = true;
	event->stopPropagation();//停止传播当前事件。 
}

/* 移动摄像机 */
void MainScene::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
	if ( touches.size() && this->camera )
	{
		Touch* touch = touches[ 0 ];
		Vec2 delta = touch->getDelta();//获取当前两个触摸点在屏幕坐标系下的差值。

		//#define CC_DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__)* 0.01745329252f) // PI / 180
		this->angle -= CC_DEGREES_TO_RADIANS(delta.x); 
		this->camera->setPosition3D(Vec3(100.0f * sinf(this->angle), 50.0f, 100.0f * cosf(this->angle)));
		this->camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));

		if (delta.lengthSquared() > 16)//lengthSquared:向量的长度的平方。
		{
			this->needShootBox = false;
		}
		event->stopPropagation();
	}
}

void MainScene::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
	if ( !this->needShootBox ) return;

	if (!touches.empty())
	{
		Vec3 dir(0, this->uiLayer->getDirValue(), 5000);//设置改变角度：z坐标如果设置太小，效果会不明显
		log(" dir1:====%f,%f,%f ", dir.x, dir.y, dir.z);
		shootBullet2(dir);
		event->stopPropagation();
	}
}