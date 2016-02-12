#include "DanShuang.h"
#include "DanShuangView.h"
#include "SimpleAudioEngine.h"

CDanShuangGame::CDanShuangGame(CGameDirector *pDirector):m_pDirector(pDirector)
{
	//限制信息
	m_lMeMaxScore = 0;
	m_lAllMaxScore = 0;
	m_lAreaLimitScore = 0;
	m_lApplyBankerCondition = 0;

	//下注信息
	memset(m_lMeScore, 0, sizeof(m_lMeScore));
	memset(m_lAllScore, 0, sizeof(m_lAllScore));

	//庄家信息
	m_wCurrentBanker = INVALID_CHAIR;
	m_lBankerScore = 0;
	m_bEnableSysBanker = true;

	//状态变量
	m_bMeApplyBanker=false;
	m_bMePlaceJetton = false;

	//辅助变量
	m_nGameStatus = GS_INVALIDE;

	m_pDanShuangGameView = NULL;

	return;
}

CDanShuangGame::~CDanShuangGame()
{
	if (m_pDanShuangGameView)
	{
		m_pDanShuangGameView->removeFromParent();
		m_pDanShuangGameView = NULL;
	}
}

bool CDanShuangGame::Init()
{
	//限制信息
	m_lMeMaxScore = 0;
	m_lAllMaxScore = 0;

	//下注信息
	memset(m_lMeScore, 0, sizeof(m_lMeScore));
	memset(m_lAllScore, 0, sizeof(m_lAllScore));

	//状态变量
	m_bMePlaceJetton = false;

	return true;
}

void CDanShuangGame::Uninit()
{

}
bool CDanShuangGame::IsInGame()
{
	if(m_pDanShuangGameView && m_pDanShuangGameView->isVisible()) return true;
	else return false;
}

bool CDanShuangGame::EnterGame()
{
	///////////////////////////////////////
	//发送非旁观消息，不发就会被视为旁观
	//变量定义
	CMD_GF_Info Info;
	//ZeroMemory(&Info,sizeof(Info));
	memset(&Info,0,sizeof(Info));
	//构造数据
	Info.bAllowLookon = FALSE;
	//发送数据
	m_pDirector->SendRoomData(MDM_GF_FRAME,SUB_GF_INFO,&Info,sizeof(Info));
	///////////////////////////////////////

	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

	if (m_pDanShuangGameView)
	{
		m_pDanShuangGameView->removeFromParent();
	}

	m_pDanShuangGameView = CDanShuangGameView::create(this);
	m_pDirector->m_pGameScene->addChild(m_pDanShuangGameView);
	CCDirector::sharedDirector()->replaceScene(m_pDirector->m_pGameScene);
	string strPath = SOUND_PATH;
	strPath += "BACK_GROUND.mp3";
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic(strPath.c_str(),true);

	return true;
}

void CDanShuangGame::LeaveGame()
{
}

void CDanShuangGame::OnGB2312Message(const char* sMsg, int nType)
{
	std::string sUTF8 = CStringHelper::GB2312ToUTF8(sMsg);
	OnUTF8Message(sUTF8.c_str(), nType);
}

void CDanShuangGame::OnUTF8Message(const char* sMsg, int nType)
{
	if (0 != (SMT_INFO & nType))
	{
		m_pDanShuangGameView->ShowMessage(sMsg, false);
	}
	else if (0 != (SMT_EJECT & nType))
	{
		if (m_pDanShuangGameView)
		{
			m_pDanShuangGameView->unschedule(schedule_selector(CDanShuangGameView::OnDisconnected));
			m_pDirector->ShowNotify(sMsg, eNotifyFlag_OK, m_pDanShuangGameView->getParent(), 0, false, NULL);
		}
	}
}

bool CDanShuangGame::OnEventGameScene(bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	switch (m_nGameStatus)
	{
	case GS_GAME_FREE:	//空闲状态
	case GS_GAME_ANIMATION:	//动画状态
		{
			//效验数据
			CC_ASSERT(wDataSize == sizeof(CMD_S_StatusFree));
			if (wDataSize != sizeof(CMD_S_StatusFree))
				return false;

			//消息处理
			CMD_S_StatusFree * pStatusFree = (CMD_S_StatusFree *)pBuffer;

			//庄家信息
			SetBankerInfo(pStatusFree->wBankerUser, pStatusFree->lBankerScore);
			m_pDanShuangGameView->SetBankerScore(pStatusFree->wBankerTime, pStatusFree->lBankerWinScore);
			m_bEnableSysBanker = pStatusFree->bEnableSysBanker;

			//控制信息
			m_lApplyBankerCondition = pStatusFree->lApplyBankerCondition;
			m_lAreaLimitScore = pStatusFree->lAreaLimitScore;
			char szApplyBankerCondition[256];
			sprintf(szApplyBankerCondition,"上庄条件:%lld分",m_lApplyBankerCondition);
			//m_pDanShuangGameView->ShowMessage(szApplyBankerCondition);

			//游戏记录
			m_pDanShuangGameView->SetGameHistory(pStatusFree->cbOpenRecord);

			//设置时间
			m_pDanShuangGameView->SetGameTime(pStatusFree->cbTimeLeave, m_nGameStatus);

			if (m_nGameStatus == GS_GAME_FREE)
			{
				//清理桌面
				m_pDanShuangGameView->SetWinnerSide(0xFF);
				m_pDanShuangGameView->ClearAllJetton();
			}
			else if (m_nGameStatus == GS_GAME_ANIMATION)
			{
				//停止抛币动画
				m_pDanShuangGameView->BeginThrowCoin(false);
			}

			//更新控制
			UpdateButtonControl(false);

			return true;
		}
	case GS_GAME_START:	//游戏状态
		{
			//效验数据
			CC_ASSERT(wDataSize == sizeof(CMD_S_StatusJetton));
			if (wDataSize != sizeof(CMD_S_StatusJetton))
				return false;

			//消息处理
			CMD_S_StatusJetton * pStatusJetton = (CMD_S_StatusJetton *)pBuffer;

			//下注信息
			WORD wMeChairID=m_pDirector->GetMeData()->nUserChair;
			for (BYTE i =0; i < AREA_COUNT; i++)
			{
				m_lAllScore[i] += pStatusJetton->lAllScore[i];
				m_pDanShuangGameView->PlaceUserJetton(i + JA_RED, pStatusJetton->lAllScore[i] - pStatusJetton->lUserScore[i], INVALID_CHAIR);
				m_lMeScore[i] += pStatusJetton->lUserScore[i];
				m_pDanShuangGameView->PlaceUserJetton(i + JA_RED, pStatusJetton->lUserScore[i], wMeChairID);
				if (m_lMeScore[i] > 0)
				{
					m_bMePlaceJetton = true;
				}
			}

			//玩家积分
			m_lMeMaxScore = pStatusJetton->lUserMaxScore;
			m_lAllMaxScore = pStatusJetton->lAllMaxScore;

			//控制信息
			m_lApplyBankerCondition = pStatusJetton->lApplyBankerCondition;
			m_lAreaLimitScore = pStatusJetton->lAreaLimitScore;
			char szApplyBankerCondition[256];
			sprintf(szApplyBankerCondition,"上庄条件:%lld分",m_lApplyBankerCondition);
			//m_pDanShuangGameView->ShowMessage(szApplyBankerCondition);

			//庄家信息
			SetBankerInfo(pStatusJetton->wBankerUser, pStatusJetton->lBankerScore);
			m_pDanShuangGameView->SetBankerScore(pStatusJetton->wBankerTime, pStatusJetton->lBankerWinScore);
			m_bEnableSysBanker = pStatusJetton->bEnableSysBanker;

			//游戏记录
			m_pDanShuangGameView->SetGameHistory(pStatusJetton->cbOpenRecord);

			//设置时间
			m_pDanShuangGameView->SetGameTime(pStatusJetton->cbTimeLeave, m_nGameStatus);

			//停止抛币动画
			m_pDanShuangGameView->BeginThrowCoin(false);

			//更新控制
			UpdateButtonControl(true);
			return true;
		}
	case GS_GAME_END:	//结束状态
		{
			//效验数据
			CC_ASSERT(wDataSize == sizeof(CMD_S_StatusEnd));
			if (wDataSize != sizeof(CMD_S_StatusEnd))
				return false;

			//消息处理
			CMD_S_StatusEnd * pStatusEnd = (CMD_S_StatusEnd *)pBuffer;

			//下注信息
			WORD wMeChairID=m_pDirector->GetMeData()->nUserChair;
			for (BYTE i =0; i < AREA_COUNT; i++)
			{
				memcpy(m_lAllScore, pStatusEnd->lAllScore, sizeof(pStatusEnd->lAllScore));
				m_pDanShuangGameView->PlaceUserJetton(i + JA_RED, pStatusEnd->lAllScore[i] - pStatusEnd->lUserScore[i], INVALID_CHAIR);
				memcpy(m_lMeScore, pStatusEnd->lUserScore, sizeof(pStatusEnd->lUserScore));
				m_pDanShuangGameView->PlaceUserJetton(i + JA_RED, pStatusEnd->lUserScore[i], wMeChairID);
			}

			//控制信息
			m_lApplyBankerCondition = pStatusEnd->lApplyBankerCondition;
			m_lAreaLimitScore = pStatusEnd->lAreaLimitScore;
			char szApplyBankerCondition[256];
			sprintf(szApplyBankerCondition,"上庄条件:%lld分",m_lApplyBankerCondition);
			//m_pDanShuangGameView->ShowMessage(szApplyBankerCondition);

			//庄家信息
			SetBankerInfo(pStatusEnd->wBankerUser, pStatusEnd->lBankerScore);
			m_pDanShuangGameView->SetBankerScore(pStatusEnd->wBankerTime, pStatusEnd->lBankerWinScore);
			m_bEnableSysBanker = pStatusEnd->bEnableSysBanker;

			//游戏记录
			m_pDanShuangGameView->SetGameHistory(pStatusEnd->cbOpenRecord);

			//结束信息
			m_pDanShuangGameView->SetCardInfo((enOpenCardType)pStatusEnd->cbOpenArea, false);
			m_pDanShuangGameView->FinishOpenAnimation(false);
			m_pDanShuangGameView->SetCurGameScore(pStatusEnd->lEndUserScore, pStatusEnd->lEndUserReturnScore,
				pStatusEnd->lEndBankerScore, pStatusEnd->lEndRevenue);

			//设置时间
			m_pDanShuangGameView->SetGameTime(pStatusEnd->cbTimeLeave, m_nGameStatus);

			//更新控制
			UpdateButtonControl(false);
			return true;
		}
	}
	return true;
}

bool CDanShuangGame::OnGameMessage(WORD wMainCmd, WORD wSubCmd, const void * pBuffer, WORD wDataSize)
{
	CCLOG("GameMsg:%d",wSubCmd);
	switch(wSubCmd)
	{
	case SUB_S_GAME_STATUS:					//游戏状态
		{
			//效验数据
			CC_ASSERT(wDataSize==sizeof(CMD_S_GameStatus));
			if (wDataSize!=sizeof(CMD_S_GameStatus))
				return false;

			CMD_S_GameStatus * pGameStatus = (CMD_S_GameStatus *)pBuffer;
			m_nGameStatus = pGameStatus->cbGameStatus;

			return true;
		}
	case SUB_S_GAME_FREE:					//游戏空闲
		{
			return OnSubGameFree(pBuffer,wDataSize);
		}
	case SUB_S_GAME_ANIMATION:				//游戏动画
		{
			return OnSubGameAnimation(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:					//游戏开始
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON:				//用户加注
		{
			return OnSubPlaceJetton(pBuffer,wDataSize,true);
		}
	case SUB_S_GAME_END:					//游戏结束
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_APPLY_BANKER:				//申请坐庄
		{
			return OnSubUserApplyBanker(pBuffer, wDataSize);
		}
	case SUB_S_CHANGE_BANKER:				//切换庄家
		{
			return OnSubChangeBanker(pBuffer, wDataSize);
		}
	case SUB_S_CANCEL_BANKER:				//取消坐庄
		{
			return OnSubUserCancelBanker(pBuffer, wDataSize);
		}
	case SUB_S_CLEAN:						//取消下注
		{
			return OnSubClearJetton(pBuffer,wDataSize);
		}
	case SUB_S_SCORE_RECORD:				//查看分数记录
		{
			return OnSubScoreRecord(pBuffer,wDataSize);
		}
	default:break;
	}

	CC_ASSERT(false);
	return false;
}

//设置庄家
void CDanShuangGame::SetBankerInfo(WORD wBanker, LONGLONG lScore)
{
	m_wCurrentBanker=wBanker;
	m_lBankerScore=lScore;
	m_pDanShuangGameView->SetBankerInfo(m_wCurrentBanker,m_lBankerScore);
}

//游戏空闲
bool CDanShuangGame::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	CC_ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;

	//消息处理
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;

	//设置时间
	m_pDanShuangGameView->SetGameTime(pGameFree->cbTimeLeave, GS_GAME_FREE);

	//清理桌面
	m_pDanShuangGameView->SetWinnerSide(0xFF);
	m_pDanShuangGameView->ClearAllJetton();

	//初始化桌子
	this->Init();
	m_nGameStatus = GS_GAME_FREE;

	//更新控件
	UpdateButtonControl(false);

	return true;
}

//游戏抛币
bool CDanShuangGame::OnSubGameAnimation(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	CC_ASSERT(wDataSize==sizeof(CMD_S_GameAnimation));
	if (wDataSize!=sizeof(CMD_S_GameAnimation)) return false;

	//消息处理
	CMD_S_GameAnimation * pGameAnimation=(CMD_S_GameAnimation *)pBuffer;

	//设置时间
	m_pDanShuangGameView->SetGameTime(pGameAnimation->cbTimeLeave, GS_GAME_ANIMATION);

	//设置状态
	m_nGameStatus = GS_GAME_ANIMATION;

	//进行抛币动画
	m_pDanShuangGameView->BeginThrowCoin(true);

	//更新控件
	UpdateButtonControl(false);

	return true;
}

//游戏开始
bool CDanShuangGame::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	CC_ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	memset(m_lMeScore,0, sizeof(m_lMeScore));
	memset(m_lAllScore, 0,sizeof(m_lAllScore));

	//庄家信息
	SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

	//玩家信息
	m_lMeMaxScore=pGameStart->lUserMaxScore;
	m_lAllMaxScore = pGameStart->lAllMaxScore;

	//设置时间
	m_pDanShuangGameView->SetGameTime(pGameStart->cbTimeLeave,GS_GAME_START);

	//停止抛币动画
	m_pDanShuangGameView->BeginThrowCoin(false);

	//设置状态
	m_nGameStatus = GS_GAME_START;

	//更新控制
	UpdateButtonControl(true);

	//播放声音
	string strPath = SOUND_PATH;
	strPath += "GAME_START.ogg";
	SimpleAudioEngine::sharedEngine()->playEffect(strPath.c_str());

	return true;
}

//用户加注
bool CDanShuangGame::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize,bool bGameMes)
{
	//效验数据
	CC_ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) return false;

	//消息处理
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;
	if ((pPlaceJetton->cbJettonArea < JA_RED ) || (pPlaceJetton->cbJettonArea > JA_BLUE) || (pPlaceJetton->lJettonScore <= 0))
	{
		return true;
	}

	//定义变量
	BYTE cbJettonIndex = pPlaceJetton->cbJettonArea - JA_RED;	//下注区索引

	//更新下注信息
	m_lAllScore[cbJettonIndex] += pPlaceJetton->lJettonScore;
	if (pPlaceJetton->wChairID == m_pDirector->m_nMeChairID)
	{
		m_lMeScore[cbJettonIndex] += pPlaceJetton->lJettonScore;
		m_bMePlaceJetton = true;
	}

	//更新界面
	m_pDanShuangGameView->PlaceUserJetton(pPlaceJetton->cbJettonArea, pPlaceJetton->lJettonScore, pPlaceJetton->wChairID);

	//更新控件
	UpdateButtonControl();

	//播放声音
	if (pPlaceJetton->wChairID != m_pDirector->m_nMeChairID)
	{
		if (pPlaceJetton->lJettonScore == 5000000)
		{
			string strPath = SOUND_PATH;
			strPath += "ADD_GOLD_EX.ogg";
			SimpleAudioEngine::sharedEngine()->playEffect(strPath.c_str());
		}
		else
		{
			string strPath = SOUND_PATH;
			strPath += "ADD_GOLD.ogg";
			SimpleAudioEngine::sharedEngine()->playEffect(strPath.c_str());
		}
	}

	return true;
}

//游戏结束
bool CDanShuangGame::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	CC_ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd))
		return false;

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//设置开奖结果
	m_pDanShuangGameView->SetCardInfo((enOpenCardType)pGameEnd->cbOpenArea, true);

	//设置庄家分数
	m_pDanShuangGameView->SetBankerScore(pGameEnd->wBankerTime, pGameEnd->lBankerTotalScore);

	//设置成绩信息
	m_pDanShuangGameView->SetCurGameScore(pGameEnd->lUserScore, pGameEnd->lUserReturnScore, pGameEnd->lBankerNowScore, pGameEnd->lRevenue);

	//设置时间
	m_pDanShuangGameView->SetGameTime(pGameEnd->cbTimeLeave, GS_GAME_END);

	//设置游戏状态
	m_nGameStatus = GS_GAME_END;

	//更新控件
	UpdateButtonControl(false);

	return true;
}

//申请做庄
bool CDanShuangGame::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	CC_ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;

	//消息处理
	CMD_S_ApplyBanker * m_pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;

	//插入玩家
	if (m_wCurrentBanker != m_pApplyBanker->wApplyUser)
	{
		tagApplyUser ApplyUser;
		ApplyUser.wChairID = m_pApplyBanker->wApplyUser;
		ApplyUser.strUserName = m_pDirector->GetTableUserData(m_pApplyBanker->wApplyUser)->sUserName;
		m_pDanShuangGameView->AddApplyBanker(ApplyUser);
	}

	//自己判断
	if (m_pDirector->m_nMeChairID == m_pApplyBanker->wApplyUser)
		m_bMeApplyBanker=true;

	//更新控件
	UpdateButtonControl();

	return true;
}

//切换庄家
bool CDanShuangGame::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	CC_ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	//消息处理
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

	WORD wPrevBanker = m_wCurrentBanker;	//备份上个庄家

	//自己判断
	if (m_wCurrentBanker == m_pDirector->m_nMeChairID && pChangeBanker->wBankerUser != m_pDirector->m_nMeChairID) 
	{
		m_bMeApplyBanker=false;
	}
	else if (pChangeBanker->wBankerUser == m_pDirector->m_nMeChairID)
	{
		m_bMeApplyBanker=true;
	}

	//庄家信息
	SetBankerInfo(pChangeBanker->wBankerUser, pChangeBanker->lBankerScore);

	//显示图片
	m_pDanShuangGameView->ShowChangeBanker(wPrevBanker != pChangeBanker->wBankerUser);
	m_pDanShuangGameView->SetBankerScore(0, 0);

	//删除上庄列表玩家
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		tagApplyUser ApplyUser;
		ApplyUser.wChairID = m_wCurrentBanker;
		m_pDanShuangGameView->CancelApplyBanker(ApplyUser);
	}

	//更新控件
	UpdateButtonControl();

	return true;
}

//取消做庄
bool CDanShuangGame::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	CC_ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;

	//消息处理
	CMD_S_CancelBanker * pCancelBanker=(CMD_S_CancelBanker *)pBuffer;

	//删除玩家
	tagApplyUser ApplyUser;
	ApplyUser.wChairID = pCancelBanker->wCancelUser;
	m_pDanShuangGameView->CancelApplyBanker(ApplyUser);

	//自己判断
	if (m_pDirector->m_nMeChairID == pCancelBanker->wCancelUser)
		m_bMeApplyBanker = false;

	//更新控件
	UpdateButtonControl();

	return true;
}

//查看分数记录
bool CDanShuangGame::OnSubScoreRecord(const void * pBuffer, WORD wDataSize)
{
	//校验数据
	CC_ASSERT(false);

	//CC_ASSERT(wDataSize==sizeof(CMD_S_HisInfo));
	//if(wDataSize!=sizeof(CMD_S_HisInfo)) return false;
	//CMD_S_HisInfo * pData = (CMD_S_HisInfo *)pBuffer;

	//m_pSicBoGameView->SetHistoryInfo(*pData);

	return true;
}

//取消下注
bool CDanShuangGame::OnSubClearJetton(const void * pBuffer, WORD wDataSize)
{
	//校验数据
	CC_ASSERT(wDataSize==sizeof(CMD_S_ClearJetton));
	if(wDataSize!=sizeof(CMD_S_ClearJetton)) return false;

	//消息处理
	CMD_S_ClearJetton * pClearJetton = (CMD_S_ClearJetton *)pBuffer;

	//更新下注信息
	memcpy(m_lAllScore, pClearJetton->lAllJettonScore, sizeof(pClearJetton->lAllJettonScore));
	if (m_pDirector->m_nMeChairID == pClearJetton->wChairID)
	{
		memset(m_lMeScore, 0, sizeof(m_lMeScore));
		m_bMePlaceJetton = false;
	}

	//清除玩家下注
	m_pDanShuangGameView->ClearUserJetton(pClearJetton->wChairID, pClearJetton->lAllJettonScore);

	//更新控件
	UpdateButtonControl();

	return true;
}

//加注消息
bool CDanShuangGame::OnPlaceJetton(BYTE cbJettonArea, LONGLONG lJettonScore)
{
	//下注判断
	if (m_wCurrentBanker == m_pDirector->m_nMeChairID || lJettonScore <= 0 || m_nGameStatus != GS_GAME_START)
		return true;

	//分数判断
	LONGLONG lMaxPlayerScore = GetMaxPlaceJetton(cbJettonArea);	//可用最大下注

	if (lJettonScore > lMaxPlayerScore)
	{
		return true;
	}

	//构造变量
	CMD_C_PlaceJetton PlaceJetton;
	memset(&PlaceJetton, 0, sizeof(PlaceJetton));
	PlaceJetton.cbJettonArea = cbJettonArea;
	PlaceJetton.lJettonScore = lJettonScore;

	//发送消息
	m_pDirector->SendRoomData(MDM_GF_GAME, SUB_C_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));

	//更新按钮
	UpdateButtonControl();

	//播放音效
	if (lJettonScore==5000000)
	{
		string strPath = SOUND_PATH;
		strPath += "ADD_GOLD_EX.ogg";
		SimpleAudioEngine::sharedEngine()->playEffect(strPath.c_str());
	}
	else
	{
		string strPath = SOUND_PATH;
		strPath += "ADD_GOLD.ogg";
		SimpleAudioEngine::sharedEngine()->playEffect(strPath.c_str());
	}

	return 0;
}

//取消下注
bool CDanShuangGame::OnClearJetton()
{
	//取消判断
	if (m_wCurrentBanker == m_pDirector->m_nMeChairID || m_nGameStatus != GS_GAME_START)
		return true;

	//判断玩家是否下注
	bool bIsJetton = false;	//是否有下注
	for (BYTE i = 0; i < AREA_COUNT; ++i)
	{
		if (m_lMeScore[i] > 0)
		{
			bIsJetton = true;
			break;
		}
	}
	if (bIsJetton == false)
		return true;

	//发送消息
	m_pDirector->SendRoomData(MDM_GF_GAME, SUB_C_CLEAR_JETTON);

	//更新按钮
	UpdateButtonControl();

	return true;
}

//申请坐庄
bool CDanShuangGame::OnApplyBanker()
{
	//申请判断
	if (m_bMeApplyBanker == true)
		return true;
	LONGLONG lUserScore = m_pDirector->GetMeData()->nUserScore;	//玩家可用分数
	for (BYTE i =0; i < AREA_COUNT; i++)
	{
		lUserScore -= m_lMeScore[i];
	}
	if (lUserScore < m_lApplyBankerCondition)
	{
		CC_ASSERT(false);
		return true;
	}

	//发送消息
	m_pDirector->SendRoomData(MDM_GF_GAME, SUB_C_APPLY_BANKER);

	//更新按钮
	UpdateButtonControl();

	return true;
}

//取消坐庄
bool CDanShuangGame::OnCancelBanker()
{
	//取消判断
	if (m_bMeApplyBanker == false)
		return true;
	if ((m_wCurrentBanker == m_pDirector->m_nMeChairID) && (m_nGameStatus != GS_GAME_FREE))
		return true;

	//发送消息
	m_pDirector->SendRoomData(MDM_GF_GAME, SUB_C_CANCEL_BANKER);

	//更新按钮
	UpdateButtonControl();

	return true;
}

//更新控制
void CDanShuangGame::UpdateButtonControl(bool bEnablePlaceJetton /*= true*/)
{
	//筹码可用判断
	if (bEnablePlaceJetton)
	{
		if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR)
			bEnablePlaceJetton = false;
		if (m_nGameStatus != GS_GAME_START)
			bEnablePlaceJetton = false;
		if (m_wCurrentBanker == m_pDirector->m_nMeChairID)
			bEnablePlaceJetton = false;
	}

	//设置取消、下注按钮
	if (bEnablePlaceJetton==true)
	{
		//计算区域可用最大分数
		LONGLONG lCanUserMaxScore = 0;
		for (int i=0; i<AREA_COUNT; ++i)
		{
			LONGLONG lLeaveScoreTemp = GetMaxPlaceJetton(i + JA_RED);
			if (lLeaveScoreTemp > lCanUserMaxScore)
			{
				lCanUserMaxScore = lLeaveScoreTemp;
			}
		}

		m_pDanShuangGameView->m_pJetton0->setEnabled(true);
		m_pDanShuangGameView->m_pJetton1->setEnabled(true);
		m_pDanShuangGameView->m_pJetton2->setEnabled(true);
		m_pDanShuangGameView->m_pJetton3->setEnabled(true);

		//设置筹码按钮状态
		m_pDanShuangGameView->m_pJetton0->setDisplayView(lCanUserMaxScore >= g_dwJettonValue[0] ? true : false);
		m_pDanShuangGameView->m_pJetton1->setDisplayView(lCanUserMaxScore >= g_dwJettonValue[1] ? true : false);
		m_pDanShuangGameView->m_pJetton2->setDisplayView(lCanUserMaxScore >= g_dwJettonValue[2] ? true : false);
		m_pDanShuangGameView->m_pJetton3->setDisplayView(lCanUserMaxScore >= g_dwJettonValue[3] ? true : false);

		//设置取消按钮状态
		if (m_nGameStatus == GS_GAME_START && m_bMePlaceJetton == true && m_pDanShuangGameView->GetGameTime() > 5)
		{
			m_pDanShuangGameView->m_pClearJetton->setEnabled(true);
		}
		else
		{
			m_pDanShuangGameView->m_pClearJetton->setEnabled(false);
		}
	}
	else
	{
		//禁止按钮
		m_pDanShuangGameView->m_pJetton0->setEnabled(false);
		m_pDanShuangGameView->m_pJetton1->setEnabled(false);
		m_pDanShuangGameView->m_pJetton2->setEnabled(false);
		m_pDanShuangGameView->m_pJetton3->setEnabled(false);
		m_pDanShuangGameView->m_pClearJetton->setEnabled(false);
	}

	//设置上庄、下庄按钮显示
	if (m_bMeApplyBanker == true)
	{
		m_pDanShuangGameView->m_pApplyBanker->setVisible(false);
		m_pDanShuangGameView->m_pCancelBanker->setVisible(true);
	}
	else
	{
		m_pDanShuangGameView->m_pApplyBanker->setVisible(true);
		m_pDanShuangGameView->m_pCancelBanker->setVisible(false);
	}

	//设置上庄、下庄按钮状态
	if (m_wCurrentBanker == m_pDirector->m_nMeChairID)
	{
		if (m_nGameStatus == GS_GAME_FREE)
		{
			m_pDanShuangGameView->m_pCancelBanker->setEnabled(true);
		}
		else
		{
			m_pDanShuangGameView->m_pCancelBanker->setEnabled(false);
		}
	}
	else if (m_bMeApplyBanker == true)
	{
		m_pDanShuangGameView->m_pCancelBanker->setEnabled(true);
	}
	else
	{
		//判断可否上庄
		LONGLONG lUserScore = m_pDirector->GetMeData()->nUserScore;	//玩家可用分数
		for (BYTE i =0; i < AREA_COUNT; i++)
		{
			lUserScore -= m_lMeScore[i];
		}

		if (lUserScore < m_lApplyBankerCondition)
		{
			m_pDanShuangGameView->m_pApplyBanker->setEnabled(false);
		}
		else
		{
			m_pDanShuangGameView->m_pApplyBanker->setEnabled(true);
		}
	}

	return;
}

//取得区域最大下注
LONGLONG CDanShuangGame::GetMaxPlaceJetton(BYTE cbArea)
{
	//参数校验
	CC_ASSERT(cbArea >= JA_RED && cbArea <= JA_BLUE);
	if (cbArea < JA_RED || cbArea > JA_BLUE)
		return 0;

	//定义变量
	LONGLONG lUserAllScore = 0;	//玩家已下注码
	LONGLONG lAllUserAllScore = 0;	//所有玩家已下注码
	LONGLONG lAreaAllLossScore = m_lAllScore[cbArea - JA_RED] * m_GameLogic.GetAreaLossPercent(cbArea);	//该区域庄家需赔注码

	for (BYTE i = 0; i < AREA_COUNT; ++i)
	{
		lUserAllScore += m_lMeScore[i];
		lAllUserAllScore += m_lAllScore[i];
	}

	//获取注码上限
	LONGLONG lAllMaxScore = m_lAllMaxScore;	//庄家可用注码
	LONGLONG lUserMaxScore = m_lMeMaxScore;	//玩家可用注码

	//计算庄家可赔注码
	lAllMaxScore = lAllMaxScore + lAllUserAllScore - lAreaAllLossScore ;
	if (lAllMaxScore <= 0)
	{
		return 0;
	}
	lAllMaxScore = lAllMaxScore / ((std::max)((WORD)2, m_GameLogic.GetAreaLossPercent(cbArea)) - 1);
	if (lAllMaxScore <= 0)
	{
		return 0;
	}

	//计算玩家可用注码
	lUserMaxScore -= lUserAllScore;
	if (lUserMaxScore<=0)
	{
		return 0;
	}

	//考虑区域限制
	LONGLONG lWithAreaLimitScore = m_lAreaLimitScore - m_lAllScore[cbArea - JA_RED];
	if (lWithAreaLimitScore <= 0)
	{
		return 0;
	}

	//计算返回分数
	LONGLONG lReturnScore = min(lAllMaxScore, lUserMaxScore);
	lReturnScore = min(lReturnScore, lWithAreaLimitScore);

	return lReturnScore;

}

void CDanShuangGame::OnMeScoreChange(LONGLONG lScore)
{
	m_pDanShuangGameView->DrawUserScore();

	return ;
	//char szScore[256];
	//sprintf(szScore,"%lld",lScore);
	//m_pDanShuangGameView->m_pLabelUserScore->setString(szScore);
}

void CDanShuangGame::OnMeInsureScoreChange(LONGLONG lScore)
{
	return ;
	//char szScore[256];
	//sprintf(szScore,"%lld",lScore);
	//m_pDanShuangGameView->m_pSubLayer->m_pBankStorageScore->setString(szScore);
	//m_pDanShuangGameView->m_pSubLayer->m_pGiftStorageScore->setString(szScore);
}