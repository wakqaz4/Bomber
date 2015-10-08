#include "Scene\TestScene.h"
#include "physics3d\CCPhysics3D.h"

USING_NS_CC;
static cocos2d::Scene* physicsScene = nullptr;
/////////////////////////////////


MainScene::MainScene( void ) :camera( nullptr ),angle( 0.0f ){}

MainScene::~MainScene(void){}

bool MainScene::init( void )
{
	if ( !Scene::init() )	return false;


	/**** 0.���uiLayer*/
	this->uiLayer = UILayer::create();
	this->addChild(uiLayer);


	if (initWithPhysics())
	{
		/**** 1.init  */
		getPhysics3DWorld()->setDebugDrawEnable(false);//#include "physics3d\CCPhysics3D.h" : Enable or disable debug drawing.
		physicsScene = this;
		Size size = Director::getInstance()->sharedDirector()->getWinSize();


		/**** 2.����͸����� Perspective*/
		this->camera = Camera::createPerspective( 30.0f, size.width / size.height, 1.0f, 1000.0f );
		this->camera -> setPosition3D(Vec3(0.0f, 50.0f, 100.0f));
		/*lookAt
		ʹ�������Ŀ��
			@param:
			target	Ŀ���λ��
			up	������ϵ�����,ͨ������Y�� 
		*/
		this->camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
		this->camera->setCameraFlag(CameraFlag::USER1);
		this->addChild(camera);


		/**** 3.����¼�����*/
		EventListenerTouchAllAtOnce* listener = EventListenerTouchAllAtOnce::create();
									 listener->onTouchesBegan = CC_CALLBACK_2(MainScene::onTouchesBegan, this);
									 listener->onTouchesMoved = CC_CALLBACK_2(MainScene::onTouchesMoved, this);
									 listener->onTouchesEnded = CC_CALLBACK_2(MainScene::onTouchesEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


		/*** 4.����physics floor 3D����*/
		Physics3DRigidBodyDes rbDes; /*rb:rigidBody Description ��������*/
							  rbDes.mass = 0.0f; //������ Note: mass equals zero means static, default 0
							  rbDes.shape = Physics3DShape::createBox(Vec3(60.0f, 1.0f, 60.0f));//��״

		PhysicsSprite3D* floor = PhysicsSprite3D::create("Sprite3DTest/box.c3t", &rbDes);
						//floor->setTexture( "Sprite3DTest/plane.png" );
						floor->setTexture("Sprite3DTest/plane1.png");
						floor->setScaleX( 60 );
						floor->setScaleZ( 60 );
		this->addChild( floor );
						floor->setCameraMask((unsigned short)CameraFlag::USER1);
						floor->syncNodeToPhysics();//ͬ�� node ת��Ϊ physics
						//static object sync is not needed
						floor->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::NONE);
		/****debug---------------------------------------------------------------------. */
						Vec3 position = floor->getPosition3D();//��ȡ�ڵ��ڸ�����ϵ��λ�ã�X,Y,Z���� 
						//log("-------------------- %f,%f,%f ", position.x,position.y,position.z );
		/****7. */
		physicsScene->setPhysics3DDebugCamera(this->camera);
		this->angle = 0.0f;
		return true;
	}

	physicsScene = nullptr;
	return false;
}



/////////////////////////////////
/*
	@params:
	des : Ŀ���λ��
*/
void MainScene::shootBullet(const cocos2d::Vec3 &des) 
{
	/**** 1.����physics BALL 3D����*/
	Physics3DRigidBodyDes rbDes;
		 rbDes.originalTransform.translate( this->camera->getPosition3D() );//����任
		 rbDes.mass = 1.f;													//����
		 rbDes.shape = Physics3DShape::createSphere( 0.5f );				//��״

		 Vec3 linearVel = des - this->camera->getPosition3D();				//des = this->camera->getPosition3D() + dir*10.0f 
			  linearVel.normalize();										//�����淶��
		      linearVel *= 100.0f;											//�޸������ֵ���Ըı䵯��ȥ���ٶ�*****

	PhysicsSprite3D* sprite = PhysicsSprite3D::create("Sprite3DTest/box.c3t", &rbDes);
	sprite->setTexture( "Images/Icon.png" );

	/**** 2.���� 3D���� ��������*/
	Physics3DRigidBody* rigidBody = static_cast<Physics3DRigidBody*>( sprite->getPhysicsObj() ); //class CC_DLL Physics3DRigidBody : public Physics3DObject
						rigidBody->setLinearFactor(Vec3::ONE);		//������������
						rigidBody->setLinearVelocity(linearVel);	//���������ٶ�*****
						rigidBody->setAngularVelocity(Vec3::ZERO);	//���ý��ٶ�
						rigidBody->setCcdMotionThreshold(0.5f);		//�����˶���
						rigidBody->setCcdSweptSphereRadius(0.4f);	//��������뾶

	/**** 3.*/
	this->addChild( sprite );
		sprite->setPosition3D( this->camera->getPosition3D() );		//������ʼλ��
		sprite->setScale(0.5f);
		sprite->syncNodeToPhysics();

		//optimize, only sync node to physics
		sprite->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE); //sync node to physics

		sprite->setCameraMask((unsigned short)CameraFlag::USER1);
}

void MainScene::shootBullet2(const cocos2d::Vec3 &des)
{


}




/////////////////////////////////
/*
	��.UI����ϵ/��Ļ����ϵ��ԭ�������Ͻǣ�X�����ң�Y�����¡�
	��.GL����ϵ��ԭ�������½ǣ�X�����ң�Y�����ϡ�
	��.��������ϵ��ָ�����������Ļ������ϵ��(0,0)������Ļ�����½ǣ�(320,480)������Ļ�����Ͻǡ�
	��.��������ϵ������ڸ���������ꡣ
	http://www.cnblogs.com/pengyingh/articles/2513010.html

	CCNode���setPosition��������ʹ�õľ���GL����ϵ��
	�ڴ������¼�ʱCCTouch�����е����������Ļ����ϵ��
*/
void MainScene::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
	this->needShootBox = true;
	event->stopPropagation();//ֹͣ������ǰ�¼��� 
}

/* �ƶ������ */
void MainScene::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
	if ( touches.size() && this->camera )
	{
		Touch* touch = touches[ 0 ];
		Vec2 delta = touch->getDelta();//��ȡ��ǰ��������������Ļ����ϵ�µĲ�ֵ��

		//#define CC_DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__)* 0.01745329252f) // PI / 180
		this->angle -= CC_DEGREES_TO_RADIANS(delta.x); 
		this->camera->setPosition3D(Vec3(100.0f * sinf(this->angle), 50.0f, 100.0f * cosf(this->angle)));
		this->camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));

		if (delta.lengthSquared() > 16)//lengthSquared:�����ĳ��ȵ�ƽ����
		{
			this->needShootBox = false;
		}
		event->stopPropagation();
	}
}

void MainScene::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{

	if ( !this->needShootBox ) return;
	if ( !touches.empty() )
	{
		Vec2 location = touches[0]->getLocationInView();//getLocationInView : ��ǰ����������Ļ����ϵ�µ�����
		log("location:------------- %f,%f ", location.x, location.y);//509.000000,309.000000 
		Vec3 nearP(location.x, location.y, 0.0f), farP(location.x, location.y, 1.0f);
		nearP = this->camera->unproject( nearP );		//unproject : ��ָ����������Ļ����ת��Ϊ�������ꡣ ԭ����GL��Ļ����ϵ�����½ǡ� 
		farP  = this->camera->unproject( farP );
		log("nearP:------------- %f,%f,%f ", nearP.x, nearP.y,nearP.z);// 0.015909,49.561024,99.101456 
		log("farP:------------- %f,%f,%f ", farP.x, farP.y,farP.z);//15.910171,-388.993652,-798.583740 
		Vec3 dir( farP - nearP );
		log("dir:------------- %f,%f,%f ", dir.x, dir.y, dir.z);//15.894261,-438.554688,-897.685181 
		shootBullet( this->camera->getPosition3D() + dir*10.0f );


		log("camera:----------------- %f,%f,%f ", this->camera->getPosition3D().x, this->camera->getPosition3D().y, this->camera->getPosition3D().z);
		//0.000000,50.000000,100.000000 
		event->stopPropagation();
	}

}