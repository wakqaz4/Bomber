//现在暂时是不可以设置子弹的形状( 固定 )( 没有素材 )
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
		set3DParams()：
			设置三维的属性，比如模型，材质等
		@params:
		modelPath		:模型素材路径
		texturePath		:贴图素材路径
		rbDes			:一些刚体必要的描述属性
		startPosition	:炮弹发射起始位置
		scale			:大小尺寸
	*/

	virtual void set3DParams(const std::string& modelPath, const std::string&  texturePath,
					Physics3DRigidBodyDes* rbDes, const Vec3& startPosition, const float scale);
	/***
		setRigidParams():
			设置刚体的属性
		@params:
		linearFactor	:线性因子
		angularVelocity	:运动角速度
		motionThreshold :运动阀
		sphereRadius	:球面半径
	*/
	virtual void setRigidParams(const Vec3& linearFactor, const Vec3& angularVelocity,
					    const float motionThreshold, float sphereRadius);

	/***
		setDirAndPower()：
			设置炮弹的角度和蓄力
		@params:
		dir  :高度 + 旋转角度
		power:炮弹的蓄力
	*/
	virtual void setDirAndPower(const Vec3& des, float power);

	/***
		bombDeadTime():
			炮弹的是否最终结束( 落地 + 粒子结束 )
	*/
	inline int getBombIsDead(){  return this->isBombDead; }
	inline void setBombIsDead(int isDead) { this->isBombDead = isDead; }



	/***
	
	*/
	virtual void update(float delta) override;


private:

	PhysicsSprite3D*		sprite3d;
	Physics3DRigidBody*     rigidBody;
	int						isBombDead;//炮弹是否已经结束了
	float					m_lifeSpan;
};

#endif