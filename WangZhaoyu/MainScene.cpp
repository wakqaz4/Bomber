#include "MainScene.h"
#include "../Classes/mainLayer/entity/Player.h"
#include "common/CannonCommon.h"
#include <map>
USING_NS_CC;

Scene* CMainScene::createScene()
{
	Scene* pRet = new CMainScene();

	if (pRet && pRet->initWithPhysics())
	{
		pRet->init();		
		pRet->getPhysicsWorld()->setGravity(Vect(0, -9.8));
		pRet->autorelease();
	}
	//Debug mode
	pRet->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	return pRet;

};

bool CMainScene::init()
{
	if (!Scene::init())
		return false;
	Size winSize = Director::getInstance()->getWinSize();

//Should add gPlayer in version0.2, and use player's ID or name as identification
//MainLayer & its attaches	
	//create mainLayer
	m_mainLayer = MainLayer::create();
	this->addChild(m_mainLayer);

	//�������ʼ��ǰ��������������������������
	Global* global = Global::getInstance();
	global->multiBtn = MultiFuncButton::create();
	global->multiBtn->setBoundingGear("images/bounding.png");
	global->multiBtn->setDirGear("images/testUI01.png");
	global->multiBtn->setPowerGear("images/testUI01.png");
	global->multiBtn->setFireButton("images/button.png");
	//����һ��װ�εĴ���֣��ز�û���ã���չʾ
	global->multiBtn->setIsEnable(true);
	global->multiBtn->setStep(90.f);
	global->multiBtn->setScale(0.5f);
	global->multiBtn->setPosition(winSize.width - 80, winSize.height / 2 - 120);//λ�õ���Ҫ�ں�
	this->addChild(global->multiBtn);



	Size size = Director::getInstance()->getWinSize();


//UILayer & its attaches
	//UILayer	
	m_UILayer = UILayer::create();
	this->addChild(m_UILayer);
	
//��������ж�˳���
	m_playerVector = Global::getInstance()->m_playersVector;
	{
		int i = 0;
		for (auto iterPlayer = m_playerVector.begin(); iterPlayer != m_playerVector.end(); iterPlayer++)
		{
			m_playersOrderList[i++] = (*iterPlayer)->m_sPlayerInfo.m_ID;
		}
	}
	m_curPlayer = Global::getInstance()->_playerObj;
	m_curPlayerOrder = 0;
	m_alivePlayersNum = 2;
	m_deadPlayersNum = 0;
	m_curTurnIsOver = false;
	m_curTurnIndex = 0;

//ע���жϺ���
	//m_interruptionMap[&(Player::Fired)] = 1;//&(CMainScene::onPlayerFired);
	
	//REGISTER_INTERRUPTIONS(&(Player::Fired),&(CMainScene::onPlayerFired));
	//REGISTER_INTERRUPTIONS(&(Player::BombDidLanded), &(CMainScene::onBombDidLanded));
	
//Initialize MainScene's pointers	
	m_gameState = PREPARING;
	m_prevGameState = PREPARING;

	m_downCountingTimer = STATE_INTERVAL[DOWN_COUNTING];
	m_playerActionTimer = STATE_INTERVAL[RUNNING];

	global->_physics3DWorld = this->getPhysics3DWorld();
	global->_camera = Camera::createPerspective(30.0f, size.width / size.height, 1.0f, 1000.0f);
	global->_camera->setPosition3D(Vec3(0.0f, 80.0f, 160.0f));
	global->_camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	global->_camera->setCameraFlag(CameraFlag::USER1);
	this->addChild(global->_camera);
	setPhysics3DDebugCamera(global->_camera);

	_navMesh = NavMesh::create("mesh/all_tiles_tilecache.bin", "mesh/geomset.txt");
	_navMesh->setDebugDrawEnable(true);
	setNavMesh(_navMesh);
	setNavMeshDebugCamera(global->_camera);

//Main logic part is initialized in update
	this->scheduleUpdate();

	return true;
}


//update������Ҫ��ȡ��ǰ����ʱ��������ȫ����Ϸ״̬�����״̬��UI״̬
void CMainScene::update(float delta)
{
	//checking game state has the highest priority
	if (m_gameState == PAUSED || m_gameState == FINISHED)
	{
		return;
	}
	if (m_curPlayer == NULL)
	{
		return;
	}
	//�¼��������������Ӧ������m_curPlayer�Զ��任����˲���Ҫɾ���¼�����ע�᣻
	/*for (std::map<pFunc, pCallbackFunc>::iterator iterFunc = m_interruptionMap.begin(); iterFunc != m_interruptionMap.end(); iterFunc++)
	{		
		if ((m_curPlayer->*(iterFunc->first))() == 1)
		{
			(this->*(iterFunc->second))();
		}
	}*/
	
	if (m_curPlayer->Fired())
	{
		this->onPlayerFired();
		return;
	}
	if (m_curPlayer->BombDidLanded())
	{
		this->onBombDidLanded();
		return;
	}


	//����previousState��timer��
	switch (m_gameState)
	{
	case PREPARING:
		//do nothing
		m_gameState = DOWN_COUNTING;
		break;
	case DOWN_COUNTING:
		//start down counting
		if (m_prevGameState == DOWN_COUNTING - 1)
		{ 
			m_prevGameState = DOWN_COUNTING;	
			m_downCountingTimer = STATE_INTERVAL[DOWN_COUNTING]; 
		} 		
		else if (m_prevGameState == DOWN_COUNTING)
		{ 
			m_downCountingTimer -= delta;
		}
		else
		{
			CCLOG("m_curGameState is DOWN_COUNTING, while m_prevGameState is wrong.");
		}

		break;
	case RUNNING:
		if (m_prevGameState == RUNNING - 1)
		{
			m_prevGameState = RUNNING;	
			m_playerActionTimer = STATE_INTERVAL[RUNNING]; 
			m_curPlayer->EnableAction();
		}
		else if (m_prevGameState == RUNNING + 1)
		{
			m_prevGameState = RUNNING;
			m_playerActionTimer = STATE_INTERVAL[RUNNING];
			m_curPlayer->EnableAction();
		}
		else if (m_prevGameState == RUNNING)
		{
			m_playerActionTimer -= delta;
		}
		else
		{
			CCLOG("m_curGameState is RUNNING, while m_prevGameState is wrong.");
		}		
		break;
	case WAITING:
		if (m_prevGameState == WAITING - 1)
		{
			m_prevGameState = WAITING;
#ifdef _BOMB_DEBUG_
			m_playerWaitingTimer = STATE_INTERVAL[WAITING];
#endif
		}
		else if (m_prevGameState == WAITING)
		{
#ifdef _BOMB_DEBUG_
			m_playerWaitingTimer -= delta;
#endif
		}
		else
		{
			CCLOG("m_curGameState is WAITING, while m_prevGameState is wrong.");
		}
		break;

	case PAUSED:
		//do nothing;
		break;
	case FINISHED:
		//do nothing;
		break;

	default:
		CCLOG("Shouldn't be in other state! Logic Error1!");
		break;
	}

	if (m_downCountingTimer <= 0.0f)
	{
		m_downCountingTimer = STATE_INTERVAL[DOWN_COUNTING];
		m_gameState = RUNNING;
	}

	if (m_playerActionTimer <= 0.0f)
	{
		//Should've considered reaching the maxTurns of the game	

		//Current player didn't fire
//		m_playerActionTimer = STATE_INTERVAL[RUNNING];
		m_gameState = RUNNING;
		m_curPlayer->DisableAction();

		//һ�ֽ�����ˢ�¶��У����õ�һλ��Ҳ�ʹ�ܣ�����+��
		if (m_curPlayerOrder == m_alivePlayersNum - 1)
		{
			m_curTurnIsOver = true;
			refreshPlayerOrderList();
			m_curPlayerOrder = 0;
			m_curTurnIndex++;
			m_curTurnIsOver = false;
		}
		//����ָ����һ����Ҳ�ʹ��֮
		else
		{
			m_curPlayerOrder++;
		}

		for (std::vector<Player*>::iterator iterPlayer = m_playerVector.begin(); iterPlayer != m_playerVector.end(); iterPlayer++)
		{
			if ((*iterPlayer)->m_sPlayerInfo.m_ID == m_playersOrderList[m_curPlayerOrder])
			{
				m_curPlayer = (*iterPlayer);
				Global::getInstance()->_playerObj = m_curPlayer;
				//reRegisterInterruptions();
				break;
			}
		}
		m_curPlayer->EnableAction();
		m_playerActionTimer = STATE_INTERVAL[RUNNING];
	}
#ifdef _BOMB_DEBUG_
	if (m_playerWaitingTimer <= 0.0f)
	{
		m_playerWaitingTimer = STATE_INTERVAL[WAITING];		
	}
#endif

}

void CMainScene::refreshPlayerOrderList()
{
	//Yet to be implemented
}

int CMainScene::onPlayerFired()
{
	m_curPlayer->DisableAction();
	m_gameState = WAITING;
#ifdef _BOMB_DEBUG_
	m_playerWaitingTimer = STATE_INTERVAL[WAITING];
#endif
	m_curPlayer->SetFired(0);
	return 0;
}

int CMainScene::onBombDidLanded()
{
	m_gameState = RUNNING;
	m_curPlayer->setIsBombDidLanded(0);
	//����Աȣ��Ӷ������޳�������ɫ������Ϊ-1,
	for (std::vector<Player*>::iterator iterPlayer = m_playerVector.begin(); iterPlayer != m_playerVector.end(); iterPlayer++)
	{
		if ((*iterPlayer)->IsDead())
		{
			for (int i = 0; i < m_alivePlayersNum; i++)
			{
				if (m_playersOrderList[i] == (*iterPlayer)->m_sPlayerInfo.m_ID)
				{
					deleteOrderListWithIndex(i);
					m_alivePlayersNum--;
					m_deadPlayersNum++;
				}
			}
			m_playerVector.erase(iterPlayer);
		}
	}

	//�Ƿ�ʤ��
	if (m_alivePlayersNum == 1)
	{
		Director::getInstance()->replaceScene(new Scene());
	}

	//�Ƿ�һ�ֽ���
	else
	{
		//һ�ֽ�����ˢ�¶��У����õ�һλ��Ҳ�ʹ�ܣ�����+��
		if (m_curPlayerOrder == m_alivePlayersNum - 1)
		{
			m_curTurnIsOver = true;
			refreshPlayerOrderList();
			m_curPlayerOrder = 0;
			m_curTurnIndex++;
			m_curTurnIsOver = false;
		}
		//����ָ����һ����Ҳ�ʹ��֮
		else
		{
			m_curPlayerOrder++;
		}

		for (std::vector<Player*>::iterator iterPlayer = m_playerVector.begin(); iterPlayer != m_playerVector.end(); iterPlayer++)
		{
			if ((*iterPlayer)->m_sPlayerInfo.m_ID == m_playersOrderList[m_curPlayerOrder])
			{
				m_curPlayer = (*iterPlayer);
				Global::getInstance()->_playerObj = m_curPlayer;
				//reRegisterInterruptions();
				break;
			}
		}
		m_curPlayer->EnableAction();
		m_playerActionTimer = STATE_INTERVAL[RUNNING];
	}

	return 0;
}

void CMainScene::deleteOrderListWithIndex(int index)
{
	int i;
	for (i = index; i < m_alivePlayersNum; i++)
	{
		m_playersOrderList[i] = m_playersOrderList[i + 1];
	}
	m_playersOrderList[i] = -1;
}

/*
void CMainScene::reRegisterInterruptions(void)
{
	m_interruptionMap.clear();
	REGISTER_INTERRUPTIONS(&m_curPlayer->Fired, &this->onPlayerFired);
	REGISTER_INTERRUPTIONS(&m_curPlayer->BombDidLanded, &this->onBombDidLanded);
}
*/