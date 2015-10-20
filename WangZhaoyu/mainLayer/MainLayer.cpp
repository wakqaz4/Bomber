#include "MainLayer.h"
#include "3d\CCBundle3D.h"
#include <vector>
#include <map>
#include "entity/Player.h"
#include "entity/Bomb.h"

USING_NS_CC;
using namespace std;

struct AgentUserData
{
	float time;
};

bool isTouch = false;
bool isMoved = false;
int touchCounts = 0;
Vec2 m_startPoint;
Vec2 m_endPoint;
long long m_startTime;
bool m_longProgress;
int pressTimes = 0;
MainLayer::MainLayer(void)
{
	m_longProgress = false;
}

MainLayer::~MainLayer(void)
{

}

bool MainLayer::init()
{
	if (!Layer::init())
		return false;
	_angle = 0.0f;	
	this->setTouchEnabled(true);

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(MainLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(MainLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(MainLayer::onTouchEnded, this);
	listener->setSwallowTouches(true);//不向下传递触摸  
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	initLayer();
//	scheduleUpdate();
	return true;
}

void MainLayer::initLayer()
{
	//加载地图
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

	//加载主角
	Global *global = Global::getInstance();
	global->_playerObj = Player::createPlayerByID(1);
	this->addPlayer(*(global->_playerObj));
	global->player2 = Player::createPlayerByID(2);
	this->addPlayer(*(global->player2));

}



long long getCurrentTime()
{
	struct timeval tm;
	gettimeofday(&tm, NULL);
	return (long long)(tm.tv_sec * 1000 + tm.tv_usec / 1000);
}

void MainLayer::terrainRotation(Vec2 delta)
{
	Global *global = Global::getInstance();
	_angle -= CC_DEGREES_TO_RADIANS(delta.x);
	global->_camera->setPosition3D(Vec3(160.0f * sinf(_angle), 80.0f, 160.0f * cosf(_angle)));
	global->_camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));	
}

bool MainLayer::onTouchBegan(Touch* touch, Event  *event)
{
	m_startPoint = touch->getLocation();
	isTouch = true;
	m_startTime = getCurrentTime();
	return true;
	
}
void MainLayer::onSingleClick()
{
	log("on single click");

	Vec3 nearP(_singleClickLocation.x, _singleClickLocation.y, 0.0f);
	Vec3 farP(_singleClickLocation.x, _singleClickLocation.y, 1.0f);
	auto size = Director::getInstance()->getWinSize();
	
	Global *global = Global::getInstance();
	if (global->_playerObj->m_canAct == false)
	{
		return;
	}
	global->_camera->unproject(size, &nearP, &nearP);
	global->_camera->unproject(size, &farP, &farP);
	Physics3DWorld::HitResult result;
	global->_physics3DWorld->rayCast(nearP, farP, &result);	
	global->_playerObj->moveAgent(result.hitPosition);
	global->hitPosition = result.hitPosition;
	Vec2 p(-40.0f,30.0f);
	Vec2 des(result.hitPosition.x, result.hitPosition.z);
	global->_playerObj->m_rotationAngle = CalAngle::calangle(p,des);

}

void MainLayer::onTouchMoved(Touch* touch, Event  *event)
{
	isMoved = true;
	Point delta = touch->getDelta();
	terrainRotation(delta);
}

void MainLayer::updateSingleDelay(float ft)
{
	if (touchCounts == 1) {
		onSingleClick();
		touchCounts = 0;
	}
}

void MainLayer::onTouchEnded(Touch* touch, Event  *event)
{
	isTouch = false;
	pressTimes = 0;
//	this->unschedule(schedule_selector(TouchTest::updatelongprogress));

	//如果刚完成长按事件 则把按下次数清零 长按状态置空 直接返回 不继续执行  
	if (m_longProgress) {
		touchCounts = 0;
		m_longProgress = false;

		return;
	}

	m_endPoint = touch->getLocation();

	long long endTime = getCurrentTime();

	long long timeDis = endTime - m_startTime;

	//E_SWIP_DIR dir = GetSwipDir(m_startPoint, m_endPoint, timeDis);

	//	if (dir != E_INVAILD) {
	//	onSwip(m_startPoint, m_endPoint, dir);
	//		return;
	//	}

	//做连击判断  
	if (isMoved) {
		isMoved = false;
		return;
	}
	if (touchCounts == 2) {
		//	onThreeClick();
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

bool MainLayer::addPlayer(Player& player)
{
	if ((Global::getInstance()->m_playersVector).size() == MAX_PLAYER_COUNT)
	{
		return false;
	}
	else
	{
		Global::getInstance()->m_playersVector.push_back(&player);
		this->addChild(&player);
	}
}