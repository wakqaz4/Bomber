#include "Player.h"
#include "global\Global.h"
#include "../../UILayer/UIElements/MultiButton.h"

USING_NS_CC;

Player* Player::createPlayerByID(int ID)
{
	Player* pRet = new (std::nothrow) Player(ID);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

Player::~Player()
{

}
struct AgentUserData
{
	float time;
};

bool Player::init()
{
	m_rotationAngle = 0.f;

	if (!Node::init())
		return false;
	if (m_sPlayerInfo.m_ID == 1)
	{
		m_tank = Sprite3D::create("model/boss.c3b");
	}
	else
	{
		m_tank = Sprite3D::create("model/WoT_IS7/tank_buttom.c3b");
	}	
	NavMeshAgentParam param;
	param.radius = 2.0f;
	param.height = 8.0f;
	param.maxSpeed = 8.0f;
	m_agent = NavMeshAgent::create(param);
	//	sprite->_agent->setOrientationRefAxes(Vec3(-1.0f, 0.0f, 1.0f));
	AgentUserData *data = new AgentUserData{ 0.0f };
	m_agent->setUserData(data);
	m_tank->setScale(1.5f);
	m_tank->addComponent(m_agent);
	//	_tank->setTag(1);
	this->scheduleUpdate();
	m_barrel = Sprite3D::create("model/WoT_IS7/barrel.c3b");
	m_tank->addChild(m_barrel);

	m_barrel->setPosition3D(Vec3(0.30f, 2.0f, 1.6f));
	m_barrel->setAnchorPoint(ccp(0.5, 0.5));
	m_barrel->setScale(0.5f);
	m_tank->setPosition3D(Vec3(-40.0f, 1.0f, 30.0f));
	float fRadSeed = 3.14159f / 180.0f;
	kmMat4 kMat;
	kmMat4Identity(&kMat);
	kmMat4RotationX(&kMat, 270 * fRadSeed);
	Quaternion quat(kMat);
	m_barrel->setRotationQuat(quat);
	//将坦克模型加载到Node节点
	float width = m_barrel->getContentSize().width;
	float height = m_barrel->getContentSize().height;
	log("barrel width = %f height = %f", width, height);
	//	this->addChild(_barrel);
	this->addChild(m_tank);
	//	this->setPosition3D(Vec3(-40.0f, 1.0f, 30.0f));
	this->setCameraMask((unsigned short)CameraFlag::USER1);

	return true;
}
static int angleCount = 270;
void Player::setPlayerAngle(int angle)
{
	float fRadSeed = 3.14159f / 180.0f;
	kmMat4 kMat;
	kmMat4Identity(&kMat);
	kmMat4RotationX(&kMat, (angleCount - angle) * fRadSeed);
	Quaternion quat(kMat);
	m_barrel->setRotationQuat(quat);
}

float Player::calAngle()
{
	Vec3 vec = m_tank->getRotation3D();
	float tankAngle = 0.0f;
	if (vec.x == 0.000000  && vec.y > 0 && vec.z == -0.000000){
		//	log("右下角");
		tankAngle = vec.y;
	}
	else if (vec.x == 0.000000 && vec.y < 0 && vec.z == -0.000000)
	{
		//	log("左下角");
		tankAngle = 360 - fabs(vec.y);
	}
	else if (vec.x == 180 && vec.y > 0 && vec.z == -180)
	{
		//	log("右上角");
		tankAngle = 180 - fabs(vec.y);
	}
	else if (vec.x == -180 && vec.y < 0 && vec.z == -180)
	{
		//	log("左上交");
		tankAngle = 180 + fabs(vec.y);
	}
	return tankAngle;
}
void Player::update(float dt)
{
	Global *global = Global::getInstance();

	if (global->getInstance()->_playerObj != this)
	{
		return;
	}


	int a = global->multiBtn->getDirValue();
	float angle = a;

	//	log("----->%f %f %f", global->hitPosition.x, global->hitPosition.y, global->hitPosition.z);
	float rotation = calAngle();
	this->setPlayerAngle(a);

	//	log("-------> %f", rotation);
	//	log("-----------> %f %f %f", barrelV.x, barrelV.y, barrelV.z);
	//	rotation = rotation + 180 -90;
	if (global->multiBtn->getIsFired())
	{
		this->setIsFired(global->multiBtn->getIsFired());//////////////////////////////////////////
		Bomb *bomb = Bomb::create();
		m_curBomb = bomb;
		Physics3DRigidBodyDes rbDes;
		//	rbDes.originalTransform.translate(global->_camera->getPosition3D());
		rbDes.mass = 1.f;
		rbDes.shape = Physics3DShape::createSphere(0.5f);
		Vec3 start = m_tank->getPosition3D();
		bomb->set3DParams("model/box.c3t", "Icon.png", &rbDes, start, 0.5f);

		bomb->setRigidParams(Vec3::ONE, Vec3::ZERO, 0.5f, 0.4f);
		float l = 6.0f;
		float x0 = 0.0f, y0 = 0.0f, z0 = 0.0f;
		y0 = l * std::sin(angle * PI / 180);
		float l0 = l * std::cos(angle * PI / 180);
		x0 = start.x + l0 * std::sin(rotation * PI / 180);
		z0 = start.z + l0 * std::cos(rotation * PI / 180);

		Vec3 dir(x0, y0, z0);//方向向量
		dir = dir - start;
		float power = 100;				//power
		bomb->setDirAndPower(dir, power);
		bomb->setCameraMask((unsigned short)CameraFlag::USER1);
		this->addChild(bomb);
		
		//////////////////////////////////////////
		global->multiBtn->setIsFired(false);
	}
	if (m_curBomb == nullptr)
	{
		return;
	}
	if (m_curBomb->getBombIsDead())
	{
		setIsBombDidLanded(1);
		m_curBomb->setBombIsDead(0);
		//	this->removeFromParent();
	}

}

Vec3 jump(const Vec3* pV1, const Vec3* pV2, float height, float t)
{
	Vec3 pOut;
	pOut.x = pV1->x + t * (pV2->x - pV1->x);
	pOut.y = pV1->y + t * (pV2->y - pV1->y);
	pOut.z = pV1->z + t * (pV2->z - pV1->z);
	pOut.y += height * sinf(M_PI * t);
	return pOut;
}

//移动角色
void Player::moveAgent(const cocos2d::Vec3 &des)
{
	if (m_canAct == false)
	{
		return;
	}
	Global *global = Global::getInstance();

	if (global->getInstance()->_playerObj != this)
	{
		return;
	}
	NavMeshAgent::MoveCallback callback = [](NavMeshAgent *agent, float totalTimeAfterMove)
	{
		AgentUserData *data = static_cast<AgentUserData *>(agent->getUserData());
		if (agent->isOnOffMeshLink())
		{
			agent->setAutoTraverseOffMeshLink(false);
			agent->setAutoOrientation(false);
			OffMeshLinkData linkdata = agent->getCurrentOffMeshLinkData();

			agent->getOwner()->setPosition3D(jump(&linkdata.startPosition, &linkdata.endPosition, 10.0f, data->time));
			Vec3 dir = linkdata.endPosition - linkdata.startPosition;
			dir.y = 0.0f;
			dir.normalize();
			Vec3 axes;
			Vec3 refAxes = Vec3(-1.0f, 0.0f, 1.0f);
			refAxes.normalize();
			Vec3::cross(refAxes, dir, &axes);
			float angle = Vec3::dot(refAxes, dir);
			agent->getOwner()->setRotationQuat(Quaternion(axes, acosf(angle)));
			data->time += 0.01f;
			if (1.0f < data->time)
			{
				agent->completeOffMeshLink();
				agent->setAutoOrientation(true);
				data->time = 0.0f;
			}
		}
	};
	m_agent->move(des, callback);
}