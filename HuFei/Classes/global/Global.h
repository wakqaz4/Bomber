#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "cocos2d.h"
#include "entity\Player.h"
#include "entity\Enemyer.h"
#include "ui\MultiButton.h"
/**
* ���ȫ��ָ��
*/
class Global
{
public:
	static Global* getInstance();
	//������������ڵ�ͼ����ת������
	cocos2d::Camera *_camera;
	Player* _playerObj;
	
	Enemyer *_enemyerObj;
	cocos2d::Physics3DWorld *_physics3DWorld;
	cocos2d::Vec3 hitPosition;
	MultiFuncButton *multiBtn;
//	cocos2d::Vec3 &hitPosition;
};
#endif