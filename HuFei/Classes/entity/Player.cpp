#include "Player.h"
#include "global\Global.h"
#include "ui\Bomb.h"
USING_NS_CC;

Player::Player()
{

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
	if (!Node::init())
		return false;
	_tank = Sprite3D::create("model/WoT_IS7/tank_buttom.c3b");
	NavMeshAgentParam param;
	param.radius = 2.0f;
	param.height = 8.0f;
	param.maxSpeed = 8.0f;
	_agent = NavMeshAgent::create(param);
//	sprite->_agent->setOrientationRefAxes(Vec3(-1.0f, 0.0f, 1.0f));
	AgentUserData *data = new AgentUserData{ 0.0f };
	_agent->setUserData(data);
	_tank->setScale(1.5f);
	_tank->addComponent(_agent);
//	_tank->setTag(1);
	this->scheduleUpdate();
	_barrel = Sprite3D::create("model/WoT_IS7/barrel.c3b");
	_tank->addChild(_barrel);

	_barrel->setPosition3D(Vec3(0.30f,2.0f,1.6f));
	_barrel->setAnchorPoint(ccp(0.5,0.5));
	_barrel->setScale(0.5f);
	_tank->setPosition3D(Vec3(-40.0f, 1.0f, 30.0f));
	float fRadSeed = 3.14159f / 180.0f;
	kmMat4 kMat;
	kmMat4Identity(&kMat);
	kmMat4RotationX(&kMat, 270 * fRadSeed);
	Quaternion quat(kMat);
	_barrel->setRotationQuat(quat);
	//将坦克模型加载到Node节点
	
//	this->addChild(_barrel);
	this->addChild(_tank);
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
	_barrel->setRotationQuat(quat);
	log("down --  %d ", (angleCount+angle));
}
void Player::update(float dt)
{
	Global *global = Global::getInstance();
	int angle = global->multiBtn->getDirValue();
	this->setPlayerAngle(angle);
	log("update %d" ,angle);
	if (global->multiBtn->getIsFired())
	{
		log("fa she pao dan");
		Bomb *bomb = Bomb::create();
		Physics3DRigidBodyDes rbDes;
		rbDes.originalTransform.translate(global->_camera->getPosition3D());
		rbDes.mass = 1.f;
		rbDes.shape = Physics3DShape::createSphere(0.5f);
		bomb->set3DParams("model/box.c3t", "Icon.png", &rbDes, _tank->getPosition3D(), 0.5f);

		bomb->setRigidParams(Vec3::ONE,Vec3::ZERO,0.5f,0.4f);
		Vec3 dir(0.f, 5291.0f, 5000.0f);//方向向量
		float power = 30;				//power
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