#include "../Entity/Bomb.h"
#include "Particle3D\CCParticleSystem3D.h"
#include "physics3d\CCPhysics3D.h"
#include "Particle3D\PU\CCPUParticleSystem3D.h"

Bomb::Bomb() : sprite3d(nullptr), rigidBody(nullptr), isBombDead(0), m_lifeSpan(0){}
Bomb::~Bomb() {}

//////////////////////////////////////////////////////////////////////
Bomb* Bomb::create()
{
	Bomb* pRet = new(std::nothrow) Bomb();
	if (pRet)
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

void Bomb::update(float delta)
{
	m_lifeSpan += delta;
	if (m_lifeSpan >= BOMB_LIFE_SPAN)
	{
		setBombIsDead(1);
		m_lifeSpan = 0;
	}
}

//////////////////////////////////////////////////////////////////////
void Bomb::set3DParams(const std::string& modelPath, const std::string&  texturePath,
					    Physics3DRigidBodyDes* rbDes, const Vec3& startPosition, const float scale)
{
	this->sprite3d = PhysicsSprite3D::create( modelPath,rbDes );
	this->sprite3d->setTexture( texturePath );
	this->sprite3d->setPosition3D(startPosition);
	this->sprite3d->setScale(scale);
	this->sprite3d->syncNodeToPhysics();
	this->sprite3d->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);	
}


//////////////////////////////////////////////////////////////////////
void Bomb::setRigidParams(const Vec3& linearFactor, const Vec3& angularVelocity,
						  const float motionThreshold, float sphereRadius)
{
	this->rigidBody = static_cast<Physics3DRigidBody*>(this->sprite3d->getPhysicsObj());
	this->rigidBody->setLinearFactor(linearFactor);		
	this->rigidBody->setAngularVelocity(angularVelocity);	
	this->rigidBody->setCcdMotionThreshold(motionThreshold);		
	this->rigidBody->setCcdSweptSphereRadius(sphereRadius);	



	////////添加粒子效果
	rigidBody->setCollisionCallback([=](const Physics3DCollisionInfo &ci){
		if (!ci.collisionPointList.empty()){
			if (ci.objA->getMask() != 0){
				
				auto ps = PUParticleSystem3D::create("Particle3D/scripts/mp_hit_04.pu", "Particle3D/materials/pu_example.material");
				ps->setPosition3D(ci.collisionPointList[0].worldPositionOnB);
				ps->setScale(0.05f);
				ps->startParticleSystem();
				ps->setCameraMask(2);
				this->addChild(ps);
				ps->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
					ps->removeFromParent();
				}), nullptr));
				ci.objA->setMask(0);
				
				//this->removeChild(this);/////////
				//this->removeFromParentAndCleanup(true);
				this->setBombIsDead( 1 );
			}
		}


	});

	this->addChild(this->sprite3d);
	this->setBombIsDead(0);
}


//////////////////////////////////////////////////////////////////////
void Bomb::setDirAndPower(const Vec3& des, float power)
{
	Vec3 linearVel = des;												
		 linearVel.normalize();											
		 linearVel *= power;		

	this->rigidBody->setLinearVelocity(linearVel);

	
}



