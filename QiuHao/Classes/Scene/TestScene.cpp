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


	/**** 0.���uiLayer*/
	this->uiLayer = UILayer::create();
	this->addChild(uiLayer);


	if (initWithPhysics())
	{
		/**** 1.init  */
		getPhysics3DWorld()->setDebugDrawEnable(false);//#include "physics3d\CCPhysics3D.h" : Enable or disable debug drawing.
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
		
		/**** 5.������ɫ�������� */
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
	des : Ŀ���λ��
*/
void MainScene::shootBullet(const cocos2d::Vec3 &des) 
{
	/**** 1.����physics BALL 3D����*/
	Physics3DRigidBodyDes rbDes;
		 rbDes.originalTransform.translate( this->camera->getPosition3D() );//����任
		 rbDes.mass = 1.f;													//����
		 rbDes.shape = Physics3DShape::createSphere( 0.5f );				//��״

		 Vec3 linearVel = des - this->camera->getPosition3D();				//******des = this->camera->getPosition3D() + dir*10.0f 
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
	/**** 1.����physics BALL 3D����*/
	Physics3DRigidBodyDes rbDes;
		rbDes.originalTransform.translate(this->camera->getPosition3D());   //����任
		rbDes.mass = 1.f;													//����
		rbDes.shape = Physics3DShape::createSphere(0.5f);				    //��״

		Vec3 linearVel = des ;												//******des 
			 linearVel.normalize();											//�����淶��
			 linearVel *= ( this->uiLayer->getPowerValue() / 250.0f );		//�ı��ٶȣ�power***********
			 log(" dir2:====%f,%f,%f ", linearVel.x, linearVel.y, linearVel.z);

	PhysicsSprite3D* sprite = PhysicsSprite3D::create("Sprite3DTest/box.c3t", &rbDes);
	sprite->setTexture("Images/Icon.png");

	/**** 2.���� 3D���� ��������*/
	Physics3DRigidBody* rigidBody = static_cast<Physics3DRigidBody*>(sprite->getPhysicsObj()); //class CC_DLL Physics3DRigidBody : public Physics3DObject
						rigidBody->setLinearFactor(Vec3::ONE);		//������������
						rigidBody->setLinearVelocity(linearVel);	//���������ٶ�*****
						rigidBody->setAngularVelocity(Vec3::ZERO);	//���ý��ٶ�
						rigidBody->setCcdMotionThreshold(0.5f);		//�����˶���
						rigidBody->setCcdSweptSphereRadius(0.4f);	//��������뾶

	/**** 3.*/
	this->addChild(sprite);
			sprite->setPosition3D(Vec3(START_POS_X, START_POS_Y, START_POS_Z)); //������ʼλ��
			sprite->setScale(0.5f);
			sprite->syncNodeToPhysics();

	//optimize, only sync node to physics
	sprite->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE); //sync node to physics

	sprite->setCameraMask((unsigned short)CameraFlag::USER1);

}


/////////////////////////////////
/*
	��.UI����ϵ/��Ļ  ����ϵ����ԭ�������Ͻǣ�X�����ң�Y�����¡�( ��ΪҪ�������¿�ʼ����ͼ�� )���������¼�
	��.GL����ϵ/cocos ����ϵ����ԭ�������½ǣ�X�����ң�Y�����ϡ��� ���� ��
	   GL����ϵ�£�
	������            ����ϵ�������� GL ����ϵ��
	������			  ����ϵ��������ڸ����������,Ҳ���ǽڵ㣨CCNode��������ϵ��ԭ���ڽڵ����½ǣ�x�����ң�y������,getPosition3D
	 http://www.cnblogs.com/pengyingh/articles/2513010.html
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

	if (!touches.empty())
	{
		Vec3 dir(0, this->uiLayer->getDirValue(), 5000);//���øı�Ƕȣ�z�����������̫С��Ч���᲻����
		log(" dir1:====%f,%f,%f ", dir.x, dir.y, dir.z);
		shootBullet2(dir);
		event->stopPropagation();
	}
}