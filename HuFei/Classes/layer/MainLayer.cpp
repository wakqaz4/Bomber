#include "MainLayer.h"
#include "3d\CCBundle3D.h"
#include "physics3d\CCPhysics3DObject.h"
#include "physics3d\CCPhysics3DShape.h"
#include "physics3d\CCPhysics3DComponent.h"
#include "physics3d\CCPhysics3DWorld.h"
#include "global\Global.h"

USING_NS_CC;

MainLayer::MainLayer(void)
{
}

MainLayer::~MainLayer(void)
{
}

bool MainLayer::init()
{
	if (!Layer::init())
		return false;
	_angle = 0.0f;

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(MainLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(MainLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(MainLayer::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	initLayer();
	return true;
}

void MainLayer::initLayer()
{
	//加载地图刚体
	std::vector<Vec3> trianglesList = Bundle3D::getTrianglesList("mesh/map.obj");
	Physics3DRigidBodyDes rbDes;
	rbDes.mass = 0.0f;
	rbDes.shape = Physics3DShape::createMesh(&trianglesList[0], (int)trianglesList.size() / 3);
	auto rigidBody = Physics3DRigidBody::create(&rbDes);
	auto component = Physics3DComponent::create(rigidBody);
	_terrain = Sprite3D::create("mesh/map.obj");
	_terrain->addComponent(component);
	_terrain->setCameraMask((unsigned short)CameraFlag::USER1);
	this->addChild(_terrain);

	Global *global = Global::getInstance();
	//加载主角
	global->_playerObj = Player::create();
	this->addChild(global->_playerObj);
	Player *player = Player::create();
	this->addChild(player);
	//加载敌人
	global->_enemyerObj = Enemyer::create();
	global->_enemyerObj->setPosition3D(Vec3(0.0f,0.0f,0.0f));
	global->_enemyerObj->setScale(0.08f);
	global->_enemyerObj->setCameraMask((unsigned short)CameraFlag::USER1);
	this->addChild(global->_enemyerObj);
}

void MainLayer::terrainRotation(Vec2 delta)
{
	Global *global = Global::getInstance();
	_angle -= CC_DEGREES_TO_RADIANS(delta.x);
	global->_camera->setPosition3D(Vec3(160.0f * sinf(_angle), 80.0f, 160.0f * cosf(_angle)));
	global->_camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
}

bool isTouch = false, isMoved = false,m_longProgress;
Vec2 m_startPoint, m_endPoint;
long long m_startTime;
int touchCounts = 0, pressTimes = 0;

long long getCurrentTime()
{
	struct timeval tm;
	gettimeofday(&tm, NULL);
	return (long long)(tm.tv_sec * 1000 + tm.tv_usec / 1000);
}

void MainLayer::onSingleClick()
{
	log("on single click");
	Global *global = Global::getInstance();
	Vec3 nearP(_singleClickLocation.x, _singleClickLocation.y, 0.0f);
	Vec3 farP(_singleClickLocation.x, _singleClickLocation.y, 1.0f);
	auto size = Director::getInstance()->getWinSize();
	global->_camera->unproject(size, &nearP, &nearP);
	global->_camera->unproject(size, &farP, &farP);
	Physics3DWorld::HitResult result;
	global->_physics3DWorld->rayCast(nearP, farP, &result);
	global->_playerObj->moveAgent(result.hitPosition);
}

void MainLayer::updateSingleDelay(float)
{
	if (touchCounts == 1)
	{
		onSingleClick();
		touchCounts = 0;
	}
}
void MainLayer::onTouchMoved(Touch *touch, Event *event)
{
	isMoved = true;
	Point delta = touch->getDelta();
	terrainRotation(delta);
}

bool MainLayer::onTouchBegan(Touch *touch, Event *event)
{
	m_startPoint = touch->getLocation();
	isTouch = true;
	m_startTime = getCurrentTime();
	return true;
}

void MainLayer::onTouchEnded(Touch *touch, Event *event)
{
	isTouch = false;
	pressTimes = 0;
	if (m_longProgress) {
		touchCounts = 0;
		m_longProgress = false;
		return;
	}
	m_endPoint = touch->getLocation();
	long long endTime = getCurrentTime();
	long long timeDis = endTime - m_startTime;
	//做连击判断  
	if (isMoved) {
		isMoved = false;
		return;
	}
	if (touchCounts == 2) {
		touchCounts = 0;
	}
	else if (touchCounts == 1) {
		touchCounts++;
	}
	else if (touchCounts == 0) {
		_singleClickLocation = touch->getLocationInView();
		this->scheduleOnce(schedule_selector(MainLayer::updateSingleDelay), 0);
		touchCounts++;
	}
}