#include "Player.h"
USING_NS_CC;

struct AgentUserData
{
	float time;
};

Player* Player::create(const char* file)
{
	auto sprite = new(std::nothrow)Player();
	if (sprite && sprite->initWithFile(file))
	{
		NavMeshAgentParam param;
		param.radius = 2.0f;
		param.height = 8.0f;
		param.maxSpeed = 8.0f;
		sprite->_agent = NavMeshAgent::create(param);
	//	sprite->_agent->setOrientationRefAxes(Vec3(-1.0f, 0.0f, 1.0f));
		AgentUserData *data = new AgentUserData{ 0.0f };
		sprite->_agent->setUserData(data);
		sprite->setScale(1.5f);
		sprite->addComponent(sprite->_agent);
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
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

//ÒÆ¶¯½ÇÉ«
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