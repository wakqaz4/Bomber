#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "cocos2d.h"
#include "../../Classes/mainLayer/entity/Player.h"
#include "../../Classes/UILayer/UIElements/MultiButton.h"

/**
* ���ȫ��ָ��
*/
class Global
{
public:
	static Global* getInstance();
	//������������ڵ�ͼ����ת������
	cocos2d::Camera *_camera;

	//Should have a player vector to restore all players
	Player* _playerObj;
	Player* player2;
	std::vector<Player*> m_playersVector;
	cocos2d::Physics3DWorld *_physics3DWorld;
	cocos2d::Vec3 hitPosition;
	MultiFuncButton *multiBtn;
//	cocos2d::Vec3 &hitPosition;
};
#endif