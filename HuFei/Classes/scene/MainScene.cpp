#include "MainScene.h"
#include "global\Global.h"
#include "layer\MainLayer.h"

USING_NS_CC;

MainScene::MainScene()
{

}

MainScene::~MainScene()
{

}

bool MainScene::init()
{
	if (!Scene::init())
		return false;
	if (initWithPhysics())
	{
		Global *global = Global::getInstance();

		//在主层初始化前设置相机，并给主场景增加相机
		Size size = Director::getInstance()->getWinSize();
		Size visibleSize = Director::getInstance()->getVisibleSize();
		global->_physics3DWorld = this->getPhysics3DWorld();
		global->_camera = Camera::createPerspective(30.0f, size.width / size.height, 1.0f, 1000.0f);
		global->_camera->setPosition3D(Vec3(0.0f, 80.0f, 160.0f));
		global->_camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
		global->_camera->setCameraFlag(CameraFlag::USER1);
		this->addChild(global->_camera);
		setPhysics3DDebugCamera(global->_camera);
		//给地图添加navmesh
		_navMesh = NavMesh::create("mesh/all_tiles_tilecache.bin", "mesh/geomset.txt");
		_navMesh->setDebugDrawEnable(true);
		setNavMesh(_navMesh);
		setNavMeshDebugCamera(global->_camera);

		//添加主层
		MainLayer *mainLayer = MainLayer::create();
		this->addChild(mainLayer);	

		//添加按钮层
		global->multiBtn = MultiFuncButton::create();
		global->multiBtn->setBoundingGear("images/bounding.png");
		global->multiBtn->setDirGear("images/testUI01.png");
		global->multiBtn->setPowerGear("images/testUI01.png");
		global->multiBtn->setFireButton("images/button.png");
		//还有一个装饰的大齿轮，素材没调好，不展示
		global->multiBtn->setIsEnable(true);
		global->multiBtn->setStep(90.f);
		global->multiBtn->setScale(0.5f);
		global->multiBtn->setPosition(visibleSize.width - 80, visibleSize.height / 2 - 120);//位置调整要在后
		this->addChild(global->multiBtn);
	}
	return true;
}