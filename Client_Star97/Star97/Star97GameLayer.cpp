#include "stdafx.h"
#include "ResManager.h"
#include "ResourceDef.h"
#include "CMD_Game.h"
#include "Star97GameConstant.h"
#include "Star97GameLayer.h"
#include "Star97UILayer.h"
#include "Star97ResultLayer.h"
#include "Star97PrizeLayer.h"
#include "Star97RollNode.h"
#include "Star97SettingLayer.h"

namespace GameStar97
{

CStar97GameLayer::CStar97GameLayer(CStar97Game * pGame, CResManager * pResManager)
	:m_pGame(pGame)
	,m_pResManager(pResManager)
	,m_pResultLayer(NULL)
	,m_pUILayer(NULL)
	,m_pPrizeLayer(NULL)
	,m_bInit(false)
	,m_bEnableSound(true)
	,m_nCellScore(0)
	,m_lCreditScore(0)
	,m_nBetScore(0)
	,m_cbStarRatio(0)
	,m_cbHandselRatio(0)
	,m_lHandselPoolScore(0)
	,m_nGameStatus(0)
	,m_bAutoPlay(false)
	,m_nBetLevel(-1)
	,m_bTouchBegin(false)
{
	memset(&m_winScore, 0, sizeof(m_winScore));
	memset(m_cbResult, 0, sizeof(m_cbResult));
	memset(m_bOddsLine, 0,sizeof(m_bOddsLine));
}

CStar97GameLayer::~CStar97GameLayer()
{

}

void CStar97GameLayer::OnInit()
{
	//构造界面
#ifndef ANDROID_GAME
	CCSprite * pBgSprite = NULL;
	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Star97_Bg_Pc, pBgSprite))
	{
		pBgSprite->setAnchorPoint(CCPointZero);
		pBgSprite->setPosition(CCPointZero);
		this->addChild(pBgSprite, -1);
	}

	//快捷键
	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Star97_Bg_Shortcut_Key, pBgSprite))
	{
		pBgSprite->setAnchorPoint(CCPointZero);
		pBgSprite->setPosition(ccp(28, 2));
		this->addChild(pBgSprite);
	}

	m_pResultLayer = CStar97ResultLayer::create(this, m_pResManager);
	m_pResultLayer->setPosition(ccp(27 + 42, 79 + 32));
	//m_pResultLayer->setPosition(ccp(127, 179));
	m_pResultLayer->setScale(g_fPcScale);
	this->addChild(m_pResultLayer, 10);

#else
	CCSprite * pBgSprite = NULL;
	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Star97_Bg_Phone, pBgSprite))
	{
		pBgSprite->setAnchorPoint(CCPointZero);
		pBgSprite->setPosition(CCPointZero);
		this->addChild(pBgSprite, -1);
	}

	m_pResultLayer = CStar97ResultLayer::create(this, m_pResManager);
	m_pResultLayer->setPosition(ccp(242, 23));
	this->addChild(m_pResultLayer, 10);

#endif

	//UI层
	m_pUILayer = CStar97UILayer::create(this, m_pResManager);
	m_pUILayer->setPosition(CCPointZero);
	this->addChild(m_pUILayer, 5);

	//大奖图层
	m_pPrizeLayer = CStar97PrizeLayer::create(m_pResManager);
	m_pPrizeLayer->setPosition(CCPointZero);
	m_pPrizeLayer->setVisible(false);
	this->addChild(m_pPrizeLayer, 20);

	//设置图层
	m_pSettingLayer = CStar97SettingLayer::create(m_pResManager);
	m_pSettingLayer->setPosition(CCPointZero);
	m_pSettingLayer->setVisible(false);
	this->addChild(m_pSettingLayer, 100);

	//准备完毕
	m_bInit = true;
	CMD_GF_Info Info;
	memset(&Info, 0, sizeof(Info));
	Info.bAllowLookon = FALSE;
	m_pGameDirector->SendRoomData(MDM_GF_FRAME, SUB_GF_INFO, &Info, sizeof(Info));

#if (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
	SetCursor(LoadCursor(NULL, IDC_ARROW));
#endif

	//PlayMusic(Star97_Sound_Back_Music, true);

	return ;
}

void CStar97GameLayer::Clear()
{
	CGameLayer::Clear();
	removeAllChildren();
}

bool CStar97GameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return false;
}

void CStar97GameLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{

}

void CStar97GameLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

}

bool CStar97GameLayer::OnEventGameScene(bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	if (!m_bInit)
		return true;

	switch (GetGameStatus())
	{
	case GS_WK_FREE:
		{
			if (wDataSize != sizeof(CMD_S_StatusFree))
				return false;

			return OnEventGameSceneFree(bLookonOther, static_cast<const CMD_S_StatusFree *>(pBuffer));
		}
	case GS_WK_START:
	case GS_WK_PLAYING:
		{
			if (wDataSize != sizeof(CMD_S_StatusPlay))
				return false;

			return OnEventGameScenePlay(bLookonOther, static_cast<const CMD_S_StatusPlay *>(pBuffer));
		}
	}

	return true;
}

template<class T>
static const T * ToMsgData(const void * pBuffer, WORD wDataSize, const char * szType)
{
	CCLOG("----------Receive Message: [%s]", szType);
	CC_ASSERT(wDataSize == sizeof(T));
	return static_cast<const T *>(pBuffer);
}

#define TO_MSG_DATA(TYPE) ToMsgData<TYPE>(pBuffer, wDataSize, #TYPE)

bool CStar97GameLayer::OnGameMessage(WORD wSubCmd, const void * pBuffer, WORD wDataSize)
{
	if (!m_bInit)
		return true;

	switch (wSubCmd)
	{
	case SUB_S_GAME_START:
		OnSubGameStart(TO_MSG_DATA(CMD_S_GameStart));
		break;

	case SUB_S_BEGIN_ROLL:
		OnSubBeginRoll(TO_MSG_DATA(CMD_S_BeginGame));
		break;

	case SUB_S_GAME_END:
		OnSubGameEnd(TO_MSG_DATA(CMD_S_GameEnd));
		break;

	case SUB_S_BIGPRIZE_RECORD:
		OnSubBigprizeRecord(TO_MSG_DATA(CMD_S_BigPrizeRecord));
		break;

	case SUB_S_AUTOPLAY:
		break;

	case SUB_S_CLOSE:
		break;

	case SUB_GF_BIGPRIZE:
		{
			//处理中奖消息
			const CMD_GF_BIGPRIZEINFO * pBigPrize = TO_MSG_DATA(CMD_GF_BIGPRIZEINFO);

			CCLayerColor * pCLayer = CCLayerColor::create(ccc4(0, 0, 0, 128), WINDOW_WIDTH * 3, 30);
			float fEndPosY = 0.0f;
			float fFontSize = 0.0f;
#ifndef ANDROID_GAME
			fEndPosY = CONTENTS_HEIGHT - 30.0f;
			fFontSize = 20.0f;
#else
			fEndPosY = CONTENTS_HEIGHT - 93.0f;
			fFontSize = 26.0f;
#endif
			pCLayer->setPosition(ccp(0, fEndPosY));
			this->addChild(pCLayer, 30);

			char szChar[512] = {0};
			snprintf(szChar, sizeof(szChar), "%s", CStringHelper::GB2312ToUTF8(pBigPrize->szPrizeInfo).c_str());
			CCLabelTTF * pPrize = CCLabelTTF::create(szChar, "黑体", fFontSize, CCSizeZero, kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
			pPrize->setColor(ccc3(237, 237, 20));
			pPrize->setAnchorPoint(ccp(0, 0.5f));
			pPrize->setPosition(ccp(WINDOW_WIDTH -50, 15));
			pCLayer->addChild(pPrize);

			CCAction * action = CCSequence::create(CCMoveTo::create(4.0f, ccp(100 - WINDOW_WIDTH, fEndPosY)), CCDelayTime::create(2.0f),
				CCCallFunc::create(pCLayer, callfunc_selector(CCNode::removeFromParent)), NULL);
			pCLayer->runAction(action);

			break;
		}
	}

	return true;
}

void CStar97GameLayer::SendUserReady()
{
	if (GetUserStatus(0) != US_READY)
	{
		m_pGameDirector->SendRoomData(MDM_GF_FRAME, SUB_GF_USER_READY);
	}
}

void CStar97GameLayer::SendGameMessage(WORD wSubCmd, const void* pData, WORD wDataSize)
{
	m_pGameDirector->SendRoomData(MDM_GF_GAME, wSubCmd, pData, wDataSize);
}

bool CStar97GameLayer::OnEventGameSceneFree(bool bLookon, const CMD_S_StatusFree * pStatusFree)
{
	//更新信息
	SetGameStatus(GS_WK_FREE);
	m_nCellScore = pStatusFree->lCellScore;
	m_lHandselPoolScore = pStatusFree->lWinPoolScore;
	m_pPrizeLayer->updatePrizeCount(pStatusFree->bigPrizeCount);
	m_pPrizeLayer->updateAllJettonWin(pStatusFree->lAllPlaceJetton / m_nCellScore, pStatusFree->lAllWinJetton / m_nCellScore);
	m_pPrizeLayer->setExtraScoreInfo(pStatusFree->extraScoreInfo);
	m_pResultLayer->setHandselPoolScore(m_lHandselPoolScore / m_nCellScore);
	m_pUILayer->setBtnEnable(GBTag_Start, false);
	m_pUILayer->setBtnEnable(GBTag_BetScore, false);
	m_pUILayer->setUserScore(GetUserScore());
	m_bTouchBegin = false;

	if (bLookon)
	{
		m_pUILayer->setBtnEnable(GBTag_AutoStart, false);
		m_pUILayer->setBtnEnable(GBTag_CancelAuto, false);
		m_pUILayer->setBtnEnable(GBTag_CreditScore, false);
		m_pUILayer->setBtnEnable(GBTag_BetScore, false);
	}

	return true;
}

bool CStar97GameLayer::OnEventGameScenePlay(bool bLookOn, const CMD_S_StatusPlay * pStatusPlay)
{
	//更新信息
	SetGameStatus(GS_WK_FREE);
	m_nCellScore = pStatusPlay->lCellScore;
	m_lHandselPoolScore = pStatusPlay->lWinPoolScore;
	m_pPrizeLayer->updatePrizeCount(pStatusPlay->bigPrizeCount);
	m_pPrizeLayer->updateAllJettonWin(pStatusPlay->lAllPlaceJetton / m_nCellScore, pStatusPlay->lAllWinJetton / m_nCellScore);
	m_pPrizeLayer->setExtraScoreInfo(pStatusPlay->extraScoreInfo);
	m_pResultLayer->setHandselPoolScore(m_lHandselPoolScore / m_nCellScore);
	m_pUILayer->setBtnEnable(GBTag_Start, false);
	m_pUILayer->setBtnEnable(GBTag_BetScore, false);
	m_pUILayer->setUserScore(GetUserScore());
	m_bTouchBegin = false;

	if (bLookOn)
	{
		m_pUILayer->setBtnEnable(GBTag_Start, false);
		m_pUILayer->setBtnEnable(GBTag_AutoStart, false);
		m_pUILayer->setBtnEnable(GBTag_CancelAuto, false);
		m_pUILayer->setBtnEnable(GBTag_CreditScore, false);
		m_pUILayer->setBtnEnable(GBTag_BetScore, false);
	}

	return true;
}

bool CStar97GameLayer::OnSubGameStart(const CMD_S_GameStart * pGameStart)
{
	SetGameStatus(GS_PLAYING);
	m_nCellScore = pGameStart->lCellScore;

	//初始化变量
	memset(&m_winScore, 0, sizeof(m_winScore));
	memset(m_cbResult, 0, sizeof(m_cbResult));
	memset(&m_bOddsLine, 0, sizeof(m_bOddsLine));
	m_cbStarRatio = 0;
	m_cbHandselRatio = 0;

	m_pUILayer->setBtnEnable(GBTag_Start, false);
	m_pUILayer->setBtnEnable(GBTag_BetScore, false);

	return true;
}

bool CStar97GameLayer::OnSubBeginRoll(const CMD_S_BeginGame * pBeginGame)
{
	//更新游戏结果
	SetGameStatus(GS_WK_PLAYING);
	memcpy(&m_winScore, &pBeginGame->winScore, sizeof(m_winScore));
	memcpy(m_cbResult, pBeginGame->cbResult, sizeof(m_cbResult));
	memcpy(m_bOddsLine, pBeginGame->bOddsLine, sizeof(m_bOddsLine));
	m_cbStarRatio = pBeginGame->cbStarRatio;
	m_cbHandselRatio = pBeginGame->cbHandselRatio;
	m_lHandselPoolScore = pBeginGame->lWinPoolScore;

	//启动滚动
	m_pResultLayer->setRollResult(m_cbResult, m_cbStarRatio);
	m_pResultLayer->setStatusInfo(GSInfo_Roll);
	PlaySound(Star97_Sound_Fruit_Roll);
	schedule(schedule_selector(CStar97GameLayer::rollIconSoundSchedule), 0.45f);

	//设置彩金池分数
	m_pResultLayer->setHandselPoolScore(m_lHandselPoolScore / m_nCellScore);

	//自动停止滚动判断
	if (!IsLookon() && m_bAutoPlay)
	{
		scheduleOnce(schedule_selector(CStar97GameLayer::allStopRollSchedule), 2.0f);
	}

	return true;
}

bool CStar97GameLayer::OnSubGameEnd(const CMD_S_GameEnd * pGameEnd)
{
	//更新分数
	SetGameStatus(GS_WK_FREE);
	m_lCreditScore += pGameEnd->lUserScore;
	m_lHandselPoolScore = pGameEnd->lHandselPoll;
	m_bTouchBegin = false;

	//更新按钮状态
	if (!IsLookon() && !m_bAutoPlay)
	{
		if (m_lCreditScore - m_nBetScore >= 0)	//判断是否够下当前注码
		{
			m_pUILayer->setBtnEnable(GBTag_Start, true);
			m_pUILayer->setBtnEnable(GBTag_BetScore, true);
		}
		else if (m_lCreditScore - g_nJettonLevel[0] * m_nCellScore >= 0)	//判断是否够下最小注码
		{
			m_pUILayer->setBtnEnable(GBTag_Start, true);
			m_pUILayer->setBtnEnable(GBTag_BetScore, true);
			m_nBetScore = g_nJettonLevel[0] * m_nCellScore;
		}
		else
		{
			m_pUILayer->setBtnEnable(GBTag_Start, false);
			m_pUILayer->setBtnEnable(GBTag_BetScore, false);
		}
	}

	//更新界面
	m_nBetLevel = -1;
	m_pResultLayer->betScore(0, 0);
	m_pResultLayer->setCreditScore(m_lCreditScore / m_nCellScore);
	m_pResultLayer->setHandselPoolScore(m_lHandselPoolScore / m_nCellScore);
	bool bFlashFrameShow[CELL_NUM] = {0};
	m_pResultLayer->setFlashFrameVisible(bFlashFrameShow);
	bool bLinesVisible[LINE_NUM] = {0};
	m_pResultLayer->setLinesVisible(bLinesVisible);
	m_pResultLayer->resetScoreInfo();
	m_pResultLayer->setStatusInfo(GSInfo_Bet);
	m_pPrizeLayer->updateAllJettonWin(pGameEnd->lAllPlaceJetton / m_nCellScore, pGameEnd->lAllWinJetton / m_nCellScore);

	//自动开始判断
	if (!IsLookon() && m_bAutoPlay)
	{
		scheduleOnce(schedule_selector(CStar97GameLayer::autoStartSchedule), 1.0f);
	}

	return true;
}

bool CStar97GameLayer::OnSubBigprizeRecord(const CMD_S_BigPrizeRecord * pBigPrizeRecord)
{
	//更新大奖次数
	m_pPrizeLayer->updatePrizeCount(pBigPrizeRecord->bigPrizeCount);

	return true;
}

void CStar97GameLayer::PlaySound(int nResId)
{
	if (m_bEnableSound)
	{
		tagAudioCfg* pAudioCfg = m_pResManager->GetConfigHelper()->GetAudioCfg(nResId);
		if (pAudioCfg)
		{
			SimpleAudioEngine::sharedEngine()->playEffect(pAudioCfg->m_sPath.c_str(), false);
		}
	}
}

void CStar97GameLayer::PlayMusic(int nResId, bool bLoop)
{
	if (m_bEnableSound)
	{
		tagAudioCfg* pAudioCfg = m_pResManager->GetConfigHelper()->GetAudioCfg(nResId);
		if (pAudioCfg)
		{
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic(pAudioCfg->m_sPath.c_str(), bLoop);
		}
	}
}

bool CStar97GameLayer::IsLookon()
{
	tagLocalUser* pUser = m_pGameDirector->GetMeData();
	return pUser && pUser->cbUserStatus == US_LOOKON;
}

void CStar97GameLayer::SetGameStatus(int cbGameStatus)
{
	CCLOG("Game status Change: [%d]", cbGameStatus);
	m_nGameStatus = cbGameStatus;
}

LONGLONG CStar97GameLayer::GetUserScore()
{
	tagLocalUser* pUser = m_pGameDirector->GetMeData();

	return pUser->nUserScore;
}

BYTE CStar97GameLayer::GetUserStatus(WORD wChairID)
{
	tagLocalUser* pUser = m_pGameDirector->GetTableUserData(wChairID);
	if (pUser)
		return pUser->cbUserStatus;
	else
		return US_NULL;
}

void CStar97GameLayer::updateUserScore(LONGLONG lScore)
{
	m_pUILayer->setUserScore(lScore);
}

void CStar97GameLayer::touchAutoStart()
{
	if (m_bAutoPlay == true)
		return ;

	m_bAutoPlay = true;
	m_pResultLayer->setAutoTipsVisible(true);

	if (!IsLookon())
	{
		m_pUILayer->setBtnVisible(GBTag_AutoStart, false);
		m_pUILayer->setBtnVisible(GBTag_CancelAuto, true);

		m_pUILayer->setBtnEnable(GBTag_Start, false);
		m_pUILayer->setBtnEnable(GBTag_BetScore, false);
	}

	//自动开始判断
	if (!IsLookon() && m_nGameStatus == GS_WK_FREE && !m_bTouchBegin)
	{
		scheduleOnce(schedule_selector(CStar97GameLayer::autoStartSchedule), 1.0f);
	}
}

void CStar97GameLayer::touchCancelAuto()
{
	m_bAutoPlay = false;
	m_pResultLayer->setAutoTipsVisible(false);

	if (!IsLookon())
	{
		m_pUILayer->setBtnVisible(GBTag_AutoStart, true);
		m_pUILayer->setBtnVisible(GBTag_CancelAuto, false);

		if (GetGameStatus() == GS_WK_FREE && !m_bTouchBegin)
		{
			if (m_lCreditScore <= 0)
			{
				m_pUILayer->setBtnEnable(GBTag_Start, false);
				m_pUILayer->setBtnEnable(GBTag_BetScore, false);
			}
			else if (m_nBetScore <= 0)
			{
				m_pUILayer->setBtnEnable(GBTag_Start, false);
				if (m_lCreditScore >= g_nJettonLevel[0] * m_nCellScore)
				{
					m_pUILayer->setBtnEnable(GBTag_BetScore, true);
				}
				else
				{
					m_pUILayer->setBtnEnable(GBTag_BetScore, false);
				}
			}
			else
			{
				if (m_lCreditScore >= m_nBetScore)
				{
					m_pUILayer->setBtnEnable(GBTag_Start, true);
					m_pUILayer->setBtnEnable(GBTag_BetScore, true);
				}
				else
				{
					m_pUILayer->setBtnEnable(GBTag_Start, false);
					if (m_lCreditScore >= g_nJettonLevel[0] * m_nCellScore)
						m_pUILayer->setBtnEnable(GBTag_BetScore, true);
					else
						m_pUILayer->setBtnEnable(GBTag_BetScore, false);
				}
			}
		}
	}
}

void CStar97GameLayer::touchCreditScore()
{
	//计算可上分数
	LONGLONG lRemainScore = GetUserScore() - m_lCreditScore;

	if (lRemainScore / m_nCellScore <= 0)
	{
		return ;
	}
	else if (lRemainScore - 10000 * m_nCellScore >= 0)
	{
		m_lCreditScore += 10000 * m_nCellScore;
	}
	else
	{
		m_lCreditScore += lRemainScore;
	}

	//更新按钮状态
	if (!IsLookon() && !m_bAutoPlay && m_nGameStatus == GS_WK_FREE && !m_bTouchBegin)
	{
		if(m_nBetScore > 0)
		{
			if (m_lCreditScore - m_nBetScore >= 0)	//判断是否够下当前注码
			{
				m_pUILayer->setBtnEnable(GBTag_Start, true);
				m_pUILayer->setBtnEnable(GBTag_BetScore, true);
			}
			else if (m_lCreditScore - g_nJettonLevel[0] * m_nCellScore >= 0)	//判断是否够下最小注码
			{
				m_pUILayer->setBtnEnable(GBTag_Start, true);
				m_pUILayer->setBtnEnable(GBTag_BetScore, true);
				m_nBetScore = g_nJettonLevel[0] * m_nCellScore;
			}
		}
		else if (m_nBetScore == 0)
		{
			m_pUILayer->setBtnEnable(GBTag_Start, false);
			if (m_lCreditScore - g_nJettonLevel[0] * m_nCellScore >= 0)
				m_pUILayer->setBtnEnable(GBTag_BetScore, true);
		}
	}

	//更新上分
	m_pResultLayer->setCreditScore(m_lCreditScore / m_nCellScore);
	PlaySound(Star97_Sound_Bet_Score);
}

void CStar97GameLayer::touchStart()
{
	//分数校验
	if (m_nBetScore <= 0 || m_lCreditScore < m_nBetScore || GetUserScore() < m_nBetScore)
	{
		CC_ASSERT(false);
		return ;
	}

	//设置按钮状态
	m_pUILayer->setBtnEnable(GBTag_Start, false);
	m_pUILayer->setBtnEnable(GBTag_BetScore, false);
	SendUserReady();
	m_bTouchBegin = true;

	//发送消息
	CMD_C_GameStart gameStart;
	memset(&gameStart, 0, sizeof(gameStart));
	gameStart.lAddScore = m_nBetScore;

	SendGameMessage(SUB_C_GAME_START, &gameStart, sizeof(gameStart));

	m_pResultLayer->betScore(m_nBetScore / m_nCellScore, m_nBetScore / m_nCellScore / LINE_NUM);
	m_pResultLayer->updateStarCount();

	return ;
}

void CStar97GameLayer::touchBetScore()
{
	//计算最大可押等级
	int nMaxLevel = -1;
	int nIndex = CountArray(g_nJettonLevel) - 1;
	while (nIndex >= 0)
	{
		if (m_lCreditScore - g_nJettonLevel[nIndex] * m_nCellScore >= 0)
		{
			nMaxLevel = nIndex;
			break;
		}
		--nIndex;
	}
	if (nMaxLevel < 0)
		return ;

	//更新押注
	m_nBetLevel = (++m_nBetLevel) % (nMaxLevel + 1);
	m_nBetScore = g_nJettonLevel[m_nBetLevel] * m_nCellScore;
	m_pResultLayer->betScore(g_nJettonLevel[m_nBetLevel], g_nJettonLevel[m_nBetLevel] / LINE_NUM);
	PlaySound(Star97_Sound_Bet_Score);

	//更新开始按钮
	if (m_nGameStatus == GS_WK_FREE && !m_bTouchBegin)
	{
		m_pUILayer->setBtnEnable(GBTag_Start, true);
	}
}

void CStar97GameLayer::touchLookPrize()
{
	bool bIsVisible = m_pPrizeLayer->isVisible();
	m_pPrizeLayer->setVisible(!bIsVisible);
}

void CStar97GameLayer::touchIconStop(const bool bStop[CELL_NUM])
{
	if (m_nGameStatus != GS_WK_PLAYING)
		return ;

	bool bAllStop = true;
	for (int i = 0; i < CELL_NUM; ++i)
	{
		if (bStop[i] == false)
		{
			bAllStop = false;
			break;
		}
	}

	if (bAllStop)
		allStopRollSchedule(0.0f);
	else
		m_pResultLayer->stopIconRoll(bStop);
}

void CStar97GameLayer::touchExit()
{
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();

#ifndef ANDROID_GAME
	m_pGameDirector->ReturnFromGame(eLobbyType_Table, false);
#else
	m_pGameDirector->SendRoomData(MDM_GR_USER, SUB_GR_USER_LEFT_GAME_REQ, NULL, 0);
#endif
}

void CStar97GameLayer::touchSetting()
{
	if (!m_pSettingLayer->isVisible())
	{
		m_pSettingLayer->setVisible(true);
	}
}

void CStar97GameLayer::allIconStopRoll()
{
	//更新结果显示
	m_pResultLayer->setLinesVisible(m_bOddsLine);
	bool bFlashFrameShow[CELL_NUM] = {0};	//闪烁框显示与否
	checkFlashFrameShow(m_cbResult, bFlashFrameShow);
	m_pResultLayer->setFlashFrameVisible(bFlashFrameShow);
	unschedule(schedule_selector(CStar97GameLayer::rollIconSoundSchedule));
	unschedule(schedule_selector(CStar97GameLayer::allStopRollSchedule));

	if (m_winScore.lTotalPoints > 0)
	{
		m_pResultLayer->setStatusInfo(GSInfo_YouWin);

		if (m_winScore.lNormalScore > 0)
			m_pResultLayer->setNormalScore(m_winScore.lNormalScore / m_nCellScore);
		if (m_winScore.lHandselScore > 0)
			m_pResultLayer->setHandselScore(m_winScore.lHandselScore / m_nCellScore, m_cbHandselRatio);

		//播放中奖音效
		if (m_winScore.lExtraScore > 0)
			PlaySound(Star97_Sound_Win_Big_Prize);
		else
			PlaySound(Star97_Sound_Player_Win);
	}
	else
	{
		m_pResultLayer->setStatusInfo(GSInfo_GameOver);
		PlaySound(Star97_Sound_Player_Lose);
	}

	scheduleOnce(schedule_selector(CStar97GameLayer::showScoreInfoSchedule), 1.5f);

	return ;
}

void CStar97GameLayer::showScoreInfoSchedule(float dt)
{
	if (m_winScore.lTotalPoints > 0)
	{
		if (m_cbStarRatio > 0)
			m_pResultLayer->setStarWinScore(m_winScore.lNormalScore / m_nCellScore, m_cbStarRatio, STAR_VALID_COUNT);
		else
			m_pResultLayer->setNormalWinScore(m_winScore, m_nCellScore, m_cbHandselRatio);

		PlaySound(Star97_Sound_Player_GetScore);
		schedule(schedule_selector(CStar97GameLayer::getScoreSoundSchedule), 0.5f);
	}
	else
	{
		resultSetOver();
	}

	unschedule(schedule_selector(CStar97GameLayer::showScoreInfoSchedule));
}

void CStar97GameLayer::resultSetOver()
{
	//结束游戏
	SendGameMessage(SUB_C_GAME_END, NULL, 0);
	unschedule(schedule_selector(CStar97GameLayer::getScoreSoundSchedule));
}

void CStar97GameLayer::checkFlashFrameShow(const BYTE cbResult[CELL_NUM], bool bShow[CELL_NUM])
{
	//初始化返回值
	memset(bShow, 0, sizeof(bShow[0]) * CELL_NUM);

	if (CommonFunc::GetAllLineType(cbResult) != ALTypeNull)	//全盘中奖判断
	{
		memset(bShow, 1, sizeof(bShow[0]) * CELL_NUM);
	}
	else if (CommonFunc::ComputeNum7Count(cbResult) >= 2)
	{
		for (int i = 0; i < CELL_NUM; ++i)
		{
			if (cbResult[i] == OTypeNum7)
			{
				bShow[i] = true;
			}
		}
	}

	return ;
}

void CStar97GameLayer::autoStartSchedule(float dt)
{
	//游戏状态判断
	if (m_nGameStatus != GS_WK_FREE)
		return ;

	if (!m_bAutoPlay)
		return ;

	if (m_nBetScore<= 0 || m_lCreditScore < m_nBetScore || GetUserScore() < m_nBetScore)
		return ;

	unschedule(schedule_selector(CStar97GameLayer::autoStartSchedule));
	touchStart();
}

void CStar97GameLayer::allStopRollSchedule(float dt)
{
	//游戏状态判断
	if (m_nGameStatus != GS_WK_PLAYING)
		return ;

	if (m_cbStarRatio <= 0)
	{
		m_pResultLayer->stopAllIconRoll();
		unschedule(schedule_selector(CStar97GameLayer::allStopRollSchedule));
	}
}

void CStar97GameLayer::rollIconSoundSchedule(float dt)
{
	PlaySound(Star97_Sound_Fruit_Roll);
}

void CStar97GameLayer::getScoreSoundSchedule(float dt)
{
	PlaySound(Star97_Sound_Player_GetScore);
}

void CStar97GameLayer::stopRollSound()
{
	unschedule(schedule_selector(CStar97GameLayer::rollIconSoundSchedule));
}

bool CStar97GameLayer::isBetStarUseful()
{
	if (m_pResultLayer->getStarCount() <= 0)
		return true;
	else if (m_pResultLayer->getStarCount() == 1)
	{
		if (m_nGameStatus == GS_WK_FREE)
			return true;
		else
			return false;
	}
	else
		return false;
}

}
