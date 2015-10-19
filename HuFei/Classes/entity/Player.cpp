#include "Player.h"
#include "global\Global.h"
#include "ui\Bomb.h"

USING_NS_CC;


struct AgentUserData
{
	float time;
};

Player::Player()
{
}

Player::~Player()
{
}

bool Player::init()
{
	if (!Node::init())
		return false;
	_tank = Sprite3D::create("model/tank/tank_buttom.c3b");
	NavMeshAgentParam param;
	param.radius = 2.0f;
	param.height = 8.0f;
	param.maxSpeed = 8.0f;
	_agent = NavMeshAgent::create(param);
	AgentUserData *data = new AgentUserData{ 0.0f };
	_agent->setUserData(data);
	_tank->setScale(1.5f);
	_tank->addComponent(_agent);
	_tank->setPosition3D(Vec3(-40.0f, 1.0f, 30.0f));

	_barrel = Sprite3D::create("model/tank/barrel.c3b");
	_tank->addChild(_barrel);
	_barrel->setPosition3D(Vec3(0.30f, 2.0f, 1.6f));
	_barrel->setAnchorPoint(ccp(0.5, 0.5));
	_barrel->setScale(0.5f);
	float fRadSeed = 3.14159f / 180.0f;
	kmMat4 kMat;
	kmMat4Identity(&kMat);
	kmMat4RotationX(&kMat, 270 * fRadSeed);
	Quaternion quat(kMat);
	_barrel->setRotationQuat(quat);
	this->scheduleUpdate();

	this->addChild(_tank);
	this->setCameraMask((unsigned short)CameraFlag::USER1);
	return true;
}
int angleCount = 270;
#define PI 3.1415926

float Player::calAngle()
{
	Vec3 vec = _tank->getRotation3D();
	float tankAngle = 0.0f;
	if (vec.x == 0.000000  && vec.y > 0 && vec.z == -0.000000){
		tankAngle = vec.y;
	}
	else if (vec.x == 0.000000 && vec.y < 0 && vec.z == -0.000000)
	{
		tankAngle = 360 - fabs(vec.y);
	}
	else if (vec.x == 180 && vec.y > 0 && vec.z == -180)
	{
		tankAngle = 180 - fabs(vec.y);
	}
	else if (vec.x == -180 && vec.y < 0 && vec.z == -180)
	{
		tankAngle = 180 + fabs(vec.y);
	}
	return tankAngle;
}

void Player::setBarrelAngle(int angle)
{
	float fRadSeed = 3.14159f / 180.0f;
	kmMat4 kMat;
	kmMat4Identity(&kMat);
	kmMat4RotationX(&kMat, (angleCount - angle) * fRadSeed);
	Quaternion quat(kMat);
	//调整炮管上下角度
	_barrel->setRotationQuat(quat);
}

Global *global = Global::getInstance();
void Player::update(float dt)
{
	int a = global->multiBtn->getDirValue();
	float angle = a;
	this->setBarrelAngle(a);
	//发射大炮
	rotation = calAngle();
	if (global->multiBtn->getIsFired())
	{
		Bomb *bomb = Bomb::create();
		Physics3DRigidBodyDes rbDes;
		rbDes.mass = 1.f;
		rbDes.shape = Physics3DShape::createSphere(0.5f);
		Vec3 start = _tank->getPosition3D();
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
		global->multiBtn->setIsFired(false);
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
	_agent->move(des, callback);
}