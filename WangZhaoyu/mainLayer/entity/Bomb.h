//������ʱ�ǲ����������ӵ�����״( �̶� )( û���ز� )
//
#ifndef __BOMB_H__
#define __BOMB_H__
#include "cocos2d.h"
#include "physics3d\CCPhysics3D.h"
#include "Particle3D\CCParticleSystem3D.h"
#include ".././../Classes/config/CannonConfig.h"

USING_NS_CC;

class Bomb : public Node
{
public :
	static Bomb* create( );
	Bomb();
	virtual ~Bomb();
	
	/***
		set3DParams()��
			������ά�����ԣ�����ģ�ͣ����ʵ�
		@params:
		modelPath		:ģ���ز�·��
		texturePath		:��ͼ�ز�·��
		rbDes			:һЩ�����Ҫ����������
		startPosition	:�ڵ�������ʼλ��
		scale			:��С�ߴ�
	*/

	virtual void set3DParams(const std::string& modelPath, const std::string&  texturePath,
					Physics3DRigidBodyDes* rbDes, const Vec3& startPosition, const float scale);
	/***
		setRigidParams():
			���ø��������
		@params:
		linearFactor	:��������
		angularVelocity	:�˶����ٶ�
		motionThreshold :�˶���
		sphereRadius	:����뾶
	*/
	virtual void setRigidParams(const Vec3& linearFactor, const Vec3& angularVelocity,
					    const float motionThreshold, float sphereRadius);

	/***
		setDirAndPower()��
			�����ڵ��ĽǶȺ�����
		@params:
		dir  :�߶� + ��ת�Ƕ�
		power:�ڵ�������
	*/
	virtual void setDirAndPower(const Vec3& des, float power);

	/***
		bombDeadTime():
			�ڵ����Ƿ����ս���( ��� + ���ӽ��� )
	*/
	inline int getBombIsDead(){  return this->isBombDead; }
	inline void setBombIsDead(int isDead) { this->isBombDead = isDead; }



	/***
	
	*/
	virtual void update(float delta) override;


private:

	PhysicsSprite3D*		sprite3d;
	Physics3DRigidBody*     rigidBody;
	int						isBombDead;//�ڵ��Ƿ��Ѿ�������
	float					m_lifeSpan;
};

#endif