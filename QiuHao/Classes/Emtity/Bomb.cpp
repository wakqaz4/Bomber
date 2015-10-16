#include "Emtity\Bomb.h"

Bomb::Bomb() : sprite3d(nullptr), rigidBody(nullptr), isBombDead(0){}
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

	this->addChild(this->sprite3d);
}


//////////////////////////////////////////////////////////////////////
void Bomb::setDirAndPower(const Vec3& des, float power)
{
	Vec3 linearVel = des;												
		 linearVel.normalize();											
		 linearVel *= power;		

	this->rigidBody->setLinearVelocity(linearVel);

	
}



//////////////////////////////////////////////////////////////////////
int Bomb::bombDeadTime(){return this->isBombDead;}
