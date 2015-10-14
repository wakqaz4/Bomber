#include "MainScene.h"
#include "common.h"
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
	Size winSize = Director::getInstance()->getWinSize();
	//Should add gPlayer in version0.2, and use player's ID or name as identification

//Todo: 所有layer的create函数中都应当自动调用其init方法，并在init当中设置touch事件；


//MainLayer & its attaches	
	//创建mainLayer
	m_mainLayer = Layer::create();

	//创建地形
	/*

	*/
	//mainLayer->addChild(terrain)

	//创建角色(has a 炮管)	
	CPlayer* player1 = CPlayer::create();
	CPlayer* player2 = CPlayer::create();
	m_mainLayer->addChild(player1);
	m_mainLayer->addChild(player2);
	m_playerVector.push_back(player1);
	m_playerVector.push_back(player2);


	//创建炮弹
	/*

	*/
	//mainLayer->addChild(bomb);

	m_camera = Camera::createPerspective(60.0f, winSize.width / winSize.height, 1.0f, 1000.0f);
	m_camera->setCameraFlag(CameraFlag::USER1);
	m_mainLayer->addChild(m_camera);
	m_camera->setPosition3D(Vec3(0.0f, 50.0f, 100.0f));
	m_camera->lookAt(m_curPlayer->getPosition3D(), Vec3(0.0f, 1.0f, 0.0f));
	m_mainLayer->setCameraMask(2);
	
//UILayer & its attaches
	//UILayer
	m_UILayer = UILayer::create();
	//UIButtons
	CUIButton* uiButton = CUIButton::create();

//创建随机行动顺序表
	m_playersOrderList[0] = player1->m_sPlayerInfo.m_ID;
	m_playersOrderList[1] = player2->m_sPlayerInfo.m_ID;
	m_curPlayerOrder = 0;
	m_alivePlayersNum = 2;
	m_deadPlayersNum = 0;
	m_curTurnIsOver = false;
	m_curTurnIndex = 0;

//注册中断函数
	REGISTER_INTERRUPTIONS(m_curPlayer->Fired, onPlayerFired);
	REGISTER_INTERRUPTIONS(m_curPlayer->BombDidLanded, onBombDidLanded);
	
//Initialize MainScene's pointers	
	m_gameState = PREPARING;
	m_prevGameState = PREPARING;

//Main logic part is initialized in update
	this->scheduleUpdate();

	return true;
}


//update函数主要获取当前倒计时，并设置全局游戏状态、玩家状态、UI状态
void CMainScene::update(float delta)
{
	//checking game state has the highest priority
	if (m_gameState == PAUSED || m_gameState == FINISHED)
	{
		return;
	}	

	for (std::map<pFunc, pFunc>::iterator iterFunc = m_interruptionMap.begin(); iterFunc != m_interruptionMap.end(); iterFunc++)
	{
		//事件触发，则进行相应处理，从队列中删除事件，并随后添加注册事件；
		if (1 == iterFunc->first())
		{
			iterFunc->second();
			return;
		}
	}
	
	

	//调整previousState和timer，
	switch (m_gameState)
	{
	case PREPARING:
		//do nothing
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
		m_playerActionTimer = STATE_INTERVAL[RUNNING];
		m_gameState = RUNNING;
		m_curPlayer->DisableAction();

		//一轮结束，刷新队列，重置第一位玩家并使能，论数+，
		if (m_curPlayerOrder == m_alivePlayersNum - 1)
		{
			m_curTurnIsOver = true;
			refreshPlayerOrderList();
			m_curPlayerOrder = 0;
			m_curTurnIndex++;
			m_curTurnIsOver = false;
		}
		//否则指向下一个玩家并使能之
		else
		{
			m_curPlayerOrder++;
		}

		for (std::vector<CPlayer*>::iterator iterPlayer = m_playerVector.begin(); iterPlayer != m_playerVector.end(); iterPlayer++)
		{
			if ((*iterPlayer)->m_sPlayerInfo.m_ID == m_playersOrderList[m_curPlayerOrder])
			{
				m_curPlayer = (*iterPlayer);
				reRegisterInterruptions();
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
	m_playerWaitingTimer = STATE_INTERVAL[WAITING];
	m_curPlayer->SetFired(0);
	return 0;
}

int CMainScene::onBombDidLanded()
{
	//逐个对比，从队列中剔除死亡角色，设置为-1,
	for (std::vector<CPlayer*>::iterator iterPlayer = m_playerVector.begin(); iterPlayer != m_playerVector.end(); iterPlayer++)
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

	//是否胜利
	if (m_alivePlayersNum == 1)
	{
		Director::getInstance()->replaceScene(new Scene());
	}

	//是否一轮结束
	else
	{
		//一轮结束，刷新队列，重置第一位玩家并使能，论数+，
		if (m_curPlayerOrder == m_alivePlayersNum - 1)
		{
			m_curTurnIsOver = true;
			refreshPlayerOrderList();
			m_curPlayerOrder = 0;
			m_curTurnIndex++;
			m_curTurnIsOver = false;
		}
		//否则指向下一个玩家并使能之
		else
		{
			m_curPlayerOrder++;
		}

		for (std::vector<CPlayer*>::iterator iterPlayer = m_playerVector.begin(); iterPlayer != m_playerVector.end(); iterPlayer++)
		{
			if ((*iterPlayer)->m_sPlayerInfo.m_ID == m_playersOrderList[m_curPlayerOrder])
			{
				m_curPlayer = (*iterPlayer);
				reRegisterInterruptions();
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

void CMainScene::reRegisterInterruptions(void)
{
	m_interruptionMap.clear();
	REGISTER_INTERRUPTIONS(m_curPlayer->Fired, onPlayerFired);
	REGISTER_INTERRUPTIONS(m_curPlayer->BombDidLanded, onBombDidLanded);
}