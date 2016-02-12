#include "stdafx.h"
#include "PullMachResManager.h"
#include "PullMachResourceDef.h"
#include "CMD_Game.h"
#include "GameLogic.h"
#include "PullMachGameLayer.h"
#include "PullMachUILayer.h"
#include "PullMachResultLayer.h"
#include "PullMachRollNode.h"
#include "PullMachHelpLayer.h"

namespace GamePullMach777
{

static const int sg_nMaxCreditScore = 5000;	//最大上分单位分数

CPullMachGameLayer::CPullMachGameLayer(CPullMachGame * pGame, CPMResManager * pResManager)
	:m_pChatViewLayer(NULL)
	,m_pBoxLayer(NULL)
	,m_pGamePrizeLayer(NULL)
	,m_pGame(pGame)
	,m_pResManager(pResManager)
	,m_pResultLayer(NULL)
	,m_pUILayer(NULL)
	,m_bEnableSound(true)
	,m_nGameStatus(0)
	,m_nCellScore(1)
	,m_lCreditScore(0)
	,m_nBetScore(0)
	,m_lTotalPoints(0)
	,m_cbWinLineId(-1)
	,m_cbWinType(SLTypeNull)
	,m_cbNextType(SLTypeNull)
	,m_bIsBigPrizeMode(false)
	,m_lRemainBigPrizeScore(0)
	,m_nPrizeTimes(0)
	,m_bAutoPlay(false)
	,m_bTouchBegin(false)
	,m_bSendEnd(false)
{
	memset(m_cbResult, 0, sizeof(m_cbResult));
	memset(m_nVerLogoCount, 0, sizeof(m_nVerLogoCount));
}

CPullMachGameLayer::~CPullMachGameLayer()
{
	//释放游戏动画缓存
	enGameAnimationId enGameAniId[4] =
	{
		GameAni_BoxRed7, GameAni_Red7Swing, GameAni_BoxLight, GameAni_GirlFly,
	};
	CCAnimationCache * pAniCache = CCAnimationCache::sharedAnimationCache();
	for (int i = 0; i < CountArray(enGameAniId); ++i)
	{
		pAniCache->removeAnimationByName(m_pResManager->getAnimationNameById(enGameAniId[i]));
	}
}

void CPullMachGameLayer::OnInit()
{
	ResizeToViewSize();
	setTouchEnabled(true);

	//加载游戏动画
	addGameAnimationCatch();

	CCSize size = this->getContentSize();

	AddTiledBackground(m_pResManager->GetConfigHelper()->GetSpriteConfig(eSpriteType_Base, PullMach_Bg_Split));

	//界面层
	m_pResultLayer = CPullMachResultLayer::create(this, m_pResManager);
	m_pResultLayer->setPosition(CCPointZero);
	this->addChild(m_pResultLayer, 10);

	//UI层
	m_pUILayer = CPullMachUILayer::create(this, m_pResManager);
	m_pUILayer->setPosition(CCPointZero);
	this->addChild(m_pUILayer, 20);

	m_pHelpLayer = CPullMachHelpLayer::create(m_pResManager);
	m_pHelpLayer->ignoreAnchorPointForPosition(false);
	m_pHelpLayer->setAnchorPoint(ccp(0.5f, 0.5f));
	m_pHelpLayer->setPosition(ccp(size.width / 2, size.height / 2));
	this->addChild(m_pHelpLayer, 40);
	m_pHelpLayer->setVisible(false);

	//chat view
	{
		CChatViewLayer* pLayer = new CChatViewLayer();
		pLayer->autorelease();

		pLayer->SetTypeID(eLayerType_ChatView);
		pLayer->setContentSize(CCSizeMake(316, 611));
		pLayer->SetResManager(m_pGameDirector->m_pResManager);
		pLayer->SetGameDirector(m_pGameDirector);

		addChild(pLayer,30);
		pLayer->initView(eChatTop,ccp(170, size.height - 535));

		//pLayer->initView(eChatBottom,ccp(170, 30));
		m_vModalModeTouchChildren.push_back(pLayer);

		m_pChatViewLayer = pLayer;

		setChildLayout(m_pChatViewLayer, "ChatView");
	}

	//box view
	{
		CBoxLayer* pLayer = new CBoxLayer();
		pLayer->autorelease();

		pLayer->SetTypeID(eLayerType_Box);
		pLayer->setContentSize(CCSizeMake(100, 100));
		pLayer->SetResManager(m_pGameDirector->m_pResManager);
		pLayer->SetGameDirector(m_pGameDirector);

		addChild(pLayer, 30);
		pLayer->initView(ccp(size.width - 170, size .height - 200));

		m_pBoxLayer = pLayer;

		setChildLayout(m_pBoxLayer, "BoxLayer");

		pLayer->updateBoxInfo(m_pGameDirector->m_dwBoxCountDown, m_pGameDirector->m_cbBoxRecelive);
	}

	//prize view
	{
		CGamePrizeLayer* pLayer = new CGamePrizeLayer();
		pLayer->autorelease();

		pLayer->SetTypeID(eLayerType_GamePrize);
		pLayer->setContentSize(getContentSize());
		pLayer->SetResManager(m_pGameDirector->m_pResManager);
		pLayer->SetGameDirector(m_pGameDirector);
		addChild(pLayer, 200);

		pLayer->setVisible(false);

		m_pGamePrizeLayer = pLayer;

		setChildLayout(m_pGamePrizeLayer, "GamePrizeLayer");
	}

	//准备完毕
	CMD_GF_Info Info;
	memset(&Info, 0, sizeof(Info));
	Info.bAllowLookon = FALSE;
	m_pGameDirector->SendRoomData(MDM_GF_FRAME, SUB_GF_INFO, &Info, sizeof(Info));

#if (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
	SetCursor(LoadCursor(NULL, IDC_ARROW));
#endif

	//PlayMusic(PullMach_Sound_Back_Music, true);

	return ;
}

void CPullMachGameLayer::Clear()
{
	CGameLayer::Clear();
	removeAllChildren();
}

void CPullMachGameLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch *pTouch = (CCTouch*)pTouches->anyObject();

	CGameLayer::ccTouchBegan(pTouch,pEvent);
}

void CPullMachGameLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch *pTouch = (CCTouch*)pTouches->anyObject();

	CGameLayer::ccTouchEnded(pTouch,pEvent);
}

bool CPullMachGameLayer::OnEventGameScene(bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
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

bool CPullMachGameLayer::OnGameMessage(WORD wSubCmd, const void * pBuffer, WORD wDataSize)
{
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

	case SUB_S_CLOSE:
		break;

	case SUB_GF_BIGPRIZE:
		break;
	}

	return true;
}

void CPullMachGameLayer::SendUserReady()
{
	if (GetUserStatus(0) != US_READY)
	{
		m_pGameDirector->SendRoomData(MDM_GF_FRAME, SUB_GF_USER_READY);
	}
}

void CPullMachGameLayer::SendGameMessage(WORD wSubCmd, const void* pData, WORD wDataSize)
{
	m_pGameDirector->SendRoomData(MDM_GF_GAME, wSubCmd, pData, wDataSize);
}

bool CPullMachGameLayer::OnEventGameSceneFree(bool bLookon, const CMD_S_StatusFree * pStatusFree)
{
	//更新信息
	SetGameStatus(GS_WK_FREE);
	m_nCellScore = pStatusFree->lCellScore;
	m_lRemainBigPrizeScore = pStatusFree->lRemainBigPrizeScore;
	m_nPrizeTimes = pStatusFree->nPrizeTimes;
	m_bIsBigPrizeMode = pStatusFree->bIsBigPrizeMode;
	memcpy(m_nVerLogoCount, pStatusFree->nVerLogoCount, sizeof(m_nVerLogoCount));

	//更新界面
	m_pResultLayer->setUserInfo(m_pGameDirector->GetMeData());
	m_pResultLayer->setBigPrizeMode(m_bIsBigPrizeMode);
	if (!m_bIsBigPrizeMode)
		m_pResultLayer->setLogoCount(m_nVerLogoCount);
	if (m_lRemainBigPrizeScore > 0)
	{
		m_pResultLayer->openGivePrizeAni(m_lRemainBigPrizeScore, m_nPrizeTimes);
	}
	else
	{
		m_pResultLayer->closeGivePrizeAni();
	}

	m_pUILayer->setBtnEnable(GBTag_Start, false);
	m_pResultLayer->setUserScore(GetUserScore());
	m_bTouchBegin = false;
	m_bSendEnd = false;

	if (bLookon)
	{
		m_pUILayer->setBtnEnable(GBTag_AutoStart, false);
		m_pUILayer->setBtnEnable(GBTag_CancelAuto, false);
		m_pUILayer->setBtnEnable(GBTag_CreditScore, false);
		m_pUILayer->setBtnEnable(GBTag_BetScore, false);
	}

	return true;
}

bool CPullMachGameLayer::OnEventGameScenePlay(bool bLookOn, const CMD_S_StatusPlay * pStatusPlay)
{
	//更新信息
	SetGameStatus(GS_WK_FREE);

	m_nCellScore = pStatusPlay->lCellScore;
	m_lRemainBigPrizeScore = pStatusPlay->lRemainBigPrizeScore;
	m_nPrizeTimes = pStatusPlay->nPrizeTimes;
	m_bIsBigPrizeMode = pStatusPlay->bIsBigPrizeMode;
	memcpy(m_nVerLogoCount, pStatusPlay->nVerLogoCount, sizeof(m_nVerLogoCount));

	//更新界面
	m_pResultLayer->setUserInfo(m_pGameDirector->GetMeData());
	m_pResultLayer->setBigPrizeMode(m_bIsBigPrizeMode);
	if (!m_bIsBigPrizeMode)
		m_pResultLayer->setLogoCount(m_nVerLogoCount);
	if (m_lRemainBigPrizeScore > 0)
	{
		m_pResultLayer->openGivePrizeAni(m_lRemainBigPrizeScore, m_nPrizeTimes);
	}
	else
	{
		m_pResultLayer->closeGivePrizeAni();
	}

	m_pUILayer->setBtnEnable(GBTag_Start, false);
	m_pResultLayer->setUserScore(GetUserScore());
	m_bTouchBegin = false;
	m_bSendEnd = false;

	if (bLookOn)
	{
		m_pUILayer->setBtnEnable(GBTag_AutoStart, false);
		m_pUILayer->setBtnEnable(GBTag_CancelAuto, false);
		m_pUILayer->setBtnEnable(GBTag_CreditScore, false);
		m_pUILayer->setBtnEnable(GBTag_BetScore, false);
	}

	return true;
}

bool CPullMachGameLayer::OnSubGameStart(const CMD_S_GameStart * pGameStart)
{
	SetGameStatus(GS_PLAYING);
	m_nCellScore = pGameStart->lCellScore;

	//更新开奖模式
	if (m_bIsBigPrizeMode != pGameStart->bIsBigPrizeMode)
	{
		m_pResultLayer->setBigPrizeMode(pGameStart->bIsBigPrizeMode);
	}
	m_bIsBigPrizeMode = pGameStart->bIsBigPrizeMode;


	//初始化变量
	memset(m_cbResult, 0, sizeof(m_cbResult));
	m_lTotalPoints = 0;
	m_cbWinLineId = 0xff;
	m_cbWinType = SLTypeNull;
	m_bSendEnd = false;

	//更新按钮状态
	m_pUILayer->setBtnEnable(GBTag_Start, false);
	m_pUILayer->setBtnEnable(GBTag_CreditScore, false);
	m_pUILayer->setBtnEnable(GBTag_BetScore, false);

	return true;
}

bool CPullMachGameLayer::OnSubBeginRoll(const CMD_S_BeginGame * pBeginGame)
{
	//更新游戏结果
	SetGameStatus(GS_WK_PLAYING);
	m_nBetScore = pBeginGame->lJettonScore;
	m_lTotalPoints = pBeginGame->lTotalPoints;
	memcpy(m_cbResult, pBeginGame->cbResult, sizeof(m_cbResult));
	m_cbWinLineId = pBeginGame->cbWinLineId;
	m_cbWinType = pBeginGame->cbWinType;
	m_lRemainBigPrizeScore = pBeginGame->lRemainBigPrizeScore;
	m_nPrizeTimes = pBeginGame->nPrizeTimes;
	memcpy(m_nVerLogoCount, pBeginGame->nVerLogoCount, sizeof(m_nVerLogoCount));

	//启动滚动
	if (IsLookon())
	{
		m_pResultLayer->setBetScore(m_nBetScore);
		m_pResultLayer->setCreditScore(m_lCreditScore - m_nBetScore, false);
	}
	m_pResultLayer->setRollResult(m_cbResult);
	schedule(schedule_selector(CPullMachGameLayer::rollIconSoundSchedule), 0.45f);
	PlaySound(PullMach_Sound_Roll);

	//自动停止滚动判断
	if (!IsLookon() && m_bAutoPlay)
	{
		scheduleOnce(schedule_selector(CPullMachGameLayer::allStopRollSchedule), 2.0f);
	}

	return true;
}

bool CPullMachGameLayer::OnSubGameEnd(const CMD_S_GameEnd * pGameEnd)
{
	//更新分数
	SetGameStatus(GS_WK_FREE);
	m_lCreditScore += pGameEnd->lUserScore;
	m_cbNextType = pGameEnd->cbNextType;
	m_bTouchBegin = false;
	m_nBetScore = 0;

	//设置大奖提示
	if (m_cbNextType != SLTypeNull)
	{
		switch(m_cbNextType)
		{
		case SLTypeBlue7:
			m_pResultLayer->openBlue7Ani(true);
			break;
		case SLTypeRed7:
			m_pResultLayer->openRed7Ani(true);
			break;
		case SLTypeLogo:
			m_pResultLayer->openLogoAni(true);
			break;
		default:
			CC_ASSERT(false);
			break;
		}
	}

	//判断大奖状态是否更新为无效
	if (m_lRemainBigPrizeScore == 0 && m_pResultLayer->isOpenGivePrizeAni())
	{
		m_pResultLayer->setBigPrizeMode(false);
		m_pResultLayer->closeGivePrizeAni();
	}

	//更新按钮状态
	if (!IsLookon() && !m_bAutoPlay)
	{
		m_pUILayer->setBtnEnable(GBTag_CreditScore, true);
		m_pUILayer->setBtnEnable(GBTag_BetScore, true);
	}

	//更新界面
	m_pResultLayer->setCreditScore(m_lCreditScore, true);
	m_pResultLayer->setAllWinLoseScore(pGameEnd->lAllWinLoseScore);

	//自动开始判断
	if (!IsLookon() && m_bAutoPlay)
	{
		scheduleOnce(schedule_selector(CPullMachGameLayer::autoStartSchedule), 1.0f);
	}

	return true;
}

void CPullMachGameLayer::PlaySound(enSoundResDef nResId)
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

void CPullMachGameLayer::PlayMusic(enSoundResDef nResId, bool bLoop)
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

bool CPullMachGameLayer::IsLookon()
{
	tagLocalUser* pUser = m_pGameDirector->GetMeData();
	return pUser && pUser->cbUserStatus == US_LOOKON;
}

void CPullMachGameLayer::SetGameStatus(int cbGameStatus)
{
	CCLOG("Game status Change: [%d]", cbGameStatus);
	m_nGameStatus = cbGameStatus;
}

LONGLONG CPullMachGameLayer::GetUserScore()
{
	tagLocalUser* pUser = m_pGameDirector->GetMeData();

	return pUser->nUserScore;
}

BYTE CPullMachGameLayer::GetUserStatus(WORD wChairID)
{
	tagLocalUser* pUser = m_pGameDirector->GetTableUserData(wChairID);
	if (pUser)
		return pUser->cbUserStatus;
	else
		return US_NULL;
}

void CPullMachGameLayer::updateUserScore(LONGLONG lScore)
{
	m_pResultLayer->setUserScore(lScore);
}

void CPullMachGameLayer::touchAutoStart()
{
	if (IsLookon() || m_bAutoPlay == true)
		return ;

	m_bAutoPlay = true;

	//更新按钮状态
	m_pUILayer->setBtnVisible(GBTag_AutoStart, false);
	m_pUILayer->setBtnVisible(GBTag_CancelAuto, true);
	m_pUILayer->setBtnEnable(GBTag_Start, false);
	m_pUILayer->setBtnEnable(GBTag_CreditScore, false);
	m_pUILayer->setBtnEnable(GBTag_BetScore, false);

	//自动开始判断
	if (m_nGameStatus == GS_WK_FREE && !m_bTouchBegin)
	{
		scheduleOnce(schedule_selector(CPullMachGameLayer::autoStartSchedule), 1.0f);
	}
}

void CPullMachGameLayer::touchCancelAuto()
{
	if (IsLookon() || m_bAutoPlay == false)
		return ;

	m_bAutoPlay = false;

	m_pUILayer->setBtnVisible(GBTag_AutoStart, true);
	m_pUILayer->setBtnVisible(GBTag_CancelAuto, false);

	if (GetGameStatus() == GS_WK_FREE && !m_bTouchBegin)
	{
		m_pUILayer->setBtnEnable(GBTag_CreditScore, true);
		if (m_lCreditScore <= 0)
		{
			m_pUILayer->setBtnEnable(GBTag_Start, false);
			m_pUILayer->setBtnEnable(GBTag_BetScore, false);
		}
		else if (m_nBetScore <= 0)
		{
			m_pUILayer->setBtnEnable(GBTag_Start, false);
			m_pUILayer->setBtnEnable(GBTag_BetScore, true);
		}
		else
		{
			m_pUILayer->setBtnEnable(GBTag_Start, true);
			m_pUILayer->setBtnEnable(GBTag_BetScore, false);
		}
	}
}

void CPullMachGameLayer::touchCreditScore()
{
	if (IsLookon() || m_nGameStatus != GS_WK_FREE)
		return ;

	//判断当前上分数目
	if (m_lCreditScore / m_nCellScore >= sg_nMaxCreditScore)
		return ;

	//计算可上分数
	LONGLONG lRemainScore = GetUserScore() - m_lCreditScore;

	if (lRemainScore / m_nCellScore <= 0)
	{
		return ;
	}
	else if (lRemainScore - 100 * m_nCellScore >= 0)
	{
		m_lCreditScore += 100 * m_nCellScore;
	}
	else
	{
		m_lCreditScore += lRemainScore;
	}

	if (m_lCreditScore / m_nCellScore >= sg_nMaxCreditScore)
		m_lCreditScore = sg_nMaxCreditScore * m_nCellScore;

	//更新上分
	m_pResultLayer->setCreditScore(m_lCreditScore, true);
	PlaySound(PullMach_Sound_Credit_Score);
}

void CPullMachGameLayer::touchBetScore()
{
	//状态校验
	if (IsLookon() || m_nGameStatus != GS_WK_FREE || m_bTouchBegin)
		return ;

	//分数校验
	if (m_lCreditScore < g_nJettonLevel[0] * m_nCellScore)
		return ;

	//更新押注
	m_nBetScore = g_nJettonLevel[0] * m_nCellScore;
	m_pUILayer->setBtnEnable(GBTag_BetScore, false);
	m_pResultLayer->setBetScore(m_nBetScore);
	m_pResultLayer->setCreditScore(m_lCreditScore - m_nBetScore, false);

	//更新开始按钮
	if (!m_bAutoPlay)
	{
		m_pUILayer->setBtnEnable(GBTag_Start, true);
	}
}

void CPullMachGameLayer::touchStart()
{
	//状态校验
	if (IsLookon() || m_nGameStatus != GS_WK_FREE || m_bTouchBegin)
		return ;
	//分数校验
	if (m_nBetScore <= 0 || m_lCreditScore < m_nBetScore || GetUserScore() < m_nBetScore)
	{
		CC_ASSERT(false);
		return ;
	}

	//设置按钮状态
	m_pUILayer->setBtnEnable(GBTag_Start, false);
	m_pUILayer->setBtnEnable(GBTag_CreditScore, false);
	m_pUILayer->setBtnEnable(GBTag_BetScore, false);
	SendUserReady();
	m_bTouchBegin = true;

	//发送消息
	CMD_C_GameStart gameStart;
	memset(&gameStart, 0, sizeof(gameStart));
	gameStart.lAddScore = m_nBetScore;

	SendGameMessage(SUB_C_GAME_START, &gameStart, sizeof(gameStart));

	m_pResultLayer->setWinScore(0, true);
	unschedule(schedule_selector(CPullMachGameLayer::autoStartSchedule));

	return ;
}

void CPullMachGameLayer::touchStopLeft()
{
	if (IsLookon() || m_nGameStatus != GS_WK_PLAYING)
		return ;
	m_pResultLayer->stopIconRoll(0);
}

void CPullMachGameLayer::touchStopMid()
{
	if (IsLookon() || m_nGameStatus != GS_WK_PLAYING)
		return ;
	m_pResultLayer->stopIconRoll(1);
}

void CPullMachGameLayer::touchStopRight()
{
	if (IsLookon() || m_nGameStatus != GS_WK_PLAYING)
		return ;
	m_pResultLayer->stopIconRoll(2);
}

void CPullMachGameLayer::touchHelp()
{
	if (m_pHelpLayer)
	{
		m_pHelpLayer->setVisible(true);
		m_pHelpLayer->setScale(0.1f);
		m_pHelpLayer->runAction(CCScaleTo::create(0.3f, 1.0f));
	}
}

void CPullMachGameLayer::touchExit()
{
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();

#ifndef ANDROID_GAME
	m_pGameDirector->ReturnFromGame(eLobbyType_Table, false);
#else
	m_pGameDirector->SendRoomData(MDM_GR_USER, SUB_GR_USER_LEFT_GAME_REQ, NULL, 0);
#endif
}

void CPullMachGameLayer::allIconStopRoll()
{
	//更新结果显示
	unschedule(schedule_selector(CPullMachGameLayer::rollIconSoundSchedule));
	unschedule(schedule_selector(CPullMachGameLayer::allStopRollSchedule));

	//更新界面
	float fDelayEndTime = 0.5f;	//延迟结束时间
	if (!m_bIsBigPrizeMode)
		m_pResultLayer->setLogoCount(m_nVerLogoCount);
	if (m_lRemainBigPrizeScore > 0)
	{
		if (m_pResultLayer->isOpenGivePrizeAni() == false)
		{
			m_pResultLayer->openGivePrizeAni(m_lRemainBigPrizeScore, m_nPrizeTimes);
			fDelayEndTime = 4.0f;
		}
		else
		{
			m_pResultLayer->setBigPrizeRemainScore(m_lRemainBigPrizeScore, false);
		}
	}
	else if (m_lRemainBigPrizeScore == 0)
	{
		if (m_pResultLayer->isOpenGivePrizeAni())
		{
			m_pResultLayer->setBigPrizeRemainScore(0, false);
		}
	}

	//复位中奖提示
	if (m_cbNextType != SLTypeNull && m_lRemainBigPrizeScore == 0)
	{
		switch(m_cbNextType)
		{
		case SLTypeBlue7:
			m_pResultLayer->openBlue7Ani(false);
			break;
		case SLTypeRed7:
			m_pResultLayer->openRed7Ani(false);
			break;
		case SLTypeLogo:
			m_pResultLayer->openLogoAni(false);
			break;
		default:
			CC_ASSERT(false);
			break;
		}
		m_cbNextType = SLTypeNull;
	}

	//处理中奖
	if (m_cbWinType != SLTypeNull)
	{
		if (m_cbWinType != SLTypeBlue7 && m_cbWinType != SLTypeRed7 && m_cbWinType != SLTypeLogo)
		{
			m_pResultLayer->setWinScore(m_lTotalPoints, false);
			m_pResultLayer->setCreditScore(m_lCreditScore - m_nBetScore + m_lTotalPoints, false);
			fDelayEndTime = 2.0f;
		}

		m_pResultLayer->setWinLineId(m_cbWinLineId);
		PlaySound(PullMach_Sound_Win_Tip);
	}

	//结束游戏
	scheduleOnce(schedule_selector(CPullMachGameLayer::resultSetOverSchedule), fDelayEndTime);

	return ;
}

void CPullMachGameLayer::resultSetOverSchedule(float dt)
{
	//结束游戏
	if (m_bSendEnd == false)
	{
		SendGameMessage(SUB_C_GAME_END, NULL, 0);
		m_bSendEnd = true;
	}
	unschedule(schedule_selector(CPullMachGameLayer::resultSetOverSchedule));
}

void CPullMachGameLayer::autoStartSchedule(float dt)
{
	//游戏状态判断
	if (m_nGameStatus != GS_WK_FREE || !m_bAutoPlay)
		return ;

	LONGLONG lMinScore = g_nJettonLevel[0] * m_nCellScore;
	LONGLONG lUserScore = GetUserScore();
	if (lUserScore < lMinScore)
	{
		unschedule(schedule_selector(CPullMachGameLayer::autoStartSchedule));
		touchCancelAuto();
		return;
	}
	if (m_lCreditScore < lMinScore)
	{
		if (lUserScore >= sg_nMaxCreditScore * m_nCellScore)
			m_lCreditScore = sg_nMaxCreditScore * m_nCellScore;
		else
			m_lCreditScore = lUserScore;
		PlaySound(PullMach_Sound_Credit_Score);
	}

	if (m_nBetScore <= 0)
	{
		m_nBetScore = lMinScore;
		m_pResultLayer->setBetScore(m_nBetScore);
		m_pResultLayer->setCreditScore(m_lCreditScore - m_nBetScore, false);
	}
	unschedule(schedule_selector(CPullMachGameLayer::autoStartSchedule));
	touchStart();
}

void CPullMachGameLayer::allStopRollSchedule(float dt)
{
	//游戏状态判断
	if (m_nGameStatus != GS_WK_PLAYING)
		return ;

	m_pResultLayer->stopAllIconRoll();
	unschedule(schedule_selector(CPullMachGameLayer::allStopRollSchedule));
}

void CPullMachGameLayer::rollIconSoundSchedule(float dt)
{
	PlaySound(PullMach_Sound_Roll);
}

void CPullMachGameLayer::addGameAnimationCatch()
{
	//正常动画
	{
		enGameAnimationId enGameAniId[3] =
		{
			GameAni_BoxRed7, GameAni_BoxLight, GameAni_GirlFly,
		};
		int nAniBeginFrameId[3] =
		{
			PullMach_AniBegin_Box_Open1, PullMach_AniBegin_Box_Light1, PullMach_AniBegin_Girl_Fly,
		};
		int nFrameCount[3] =
		{
			29, 46, 5,
		};
		float fDelay[3] =
		{
			0.03f, 0.02f, 0.2f,
		};
		unsigned int nLoops[3] =
		{
			1, -1, -1,
		};

		CCAnimationCache * pAniCache = CCAnimationCache::sharedAnimationCache();
		for (int i = 0; i < CountArray(nAniBeginFrameId); ++i)
		{
			CCAnimation * pAnimation = createAniBySpriteFrame(nAniBeginFrameId[i], nFrameCount[i],
				fDelay[i], nLoops[i]);
			pAniCache->addAnimation(pAnimation, m_pResManager->getAnimationNameById(enGameAniId[i]));
		}
	}

	//反转动画
	{
		enGameAnimationId enGameAniId[1] =
		{
			GameAni_Red7Swing,
		};
		int nAniBeginFrameId[1] =
		{
			PullMach_AniBegin_Red7_Swing1,
		};
		int nFrameCount[1] =
		{
			15,
		};
		float fDelay[1] =
		{
			0.05f
		};
		unsigned int nLoops[1] =
		{
			-1,
		};

		CCAnimationCache * pAniCache = CCAnimationCache::sharedAnimationCache();
		for (int i = 0; i < CountArray(nAniBeginFrameId); ++i)
		{
			CCAnimation * pAnimation = createAniBySpriteFrameWithSwing(nAniBeginFrameId[i], nFrameCount[i],
				fDelay[i], nLoops[i]);
			pAniCache->addAnimation(pAnimation, m_pResManager->getAnimationNameById(enGameAniId[i]));
		}
	}

}

CCAnimation * CPullMachGameLayer::createAniBySpriteFrame(int nBeginFrameId, int nFrameCount,
	float fDelay, unsigned int nLoops/* = 1*/)
{
	CCArray * pArray = CCArray::create();
	std::string szBeginFrameName = m_pResManager->GetConfigHelper()->GetSpriteConfig(eSpriteType_Base, nBeginFrameId)->m_sPath;
	szBeginFrameName.erase(szBeginFrameName.rfind("1.png"));
	CCSpriteFrameCache * pCache = CCSpriteFrameCache::sharedSpriteFrameCache();

	for (int i = 1; i <= nFrameCount; ++i)
	{
		char szTempName[260] = {0};
		snprintf(szTempName, sizeof(szTempName), "%s%d.png", szBeginFrameName.c_str(), i);
		CCSpriteFrame * pFrame = pCache->spriteFrameByName(szTempName);
		pArray->addObject(pFrame);
	}

	CCAnimation * pAnimation = CCAnimation::createWithSpriteFrames(pArray, fDelay);
	pAnimation->setLoops(nLoops);
	return pAnimation;
}

CCAnimation * CPullMachGameLayer::createAniBySpriteFrameWithSwing(int nBeginFrameId, int nFrameCount,
	float fDelay, unsigned int nLoops/* = 1*/)
{
	CCArray * pArray = CCArray::create();
	std::string szBeginFrameName = m_pResManager->GetConfigHelper()->GetSpriteConfig(eSpriteType_Base, nBeginFrameId)->m_sPath;
	szBeginFrameName.erase(szBeginFrameName.rfind("1.png"));
	CCSpriteFrameCache * pCache = CCSpriteFrameCache::sharedSpriteFrameCache();

	for (int i = 1; i <= nFrameCount; ++i)
	{
		char szTempName[260] = {0};
		snprintf(szTempName, sizeof(szTempName), "%s%d.png", szBeginFrameName.c_str(), i);
		CCSpriteFrame * pFrame = pCache->spriteFrameByName(szTempName);
		pArray->addObject(pFrame);
	}
	for (int i = nFrameCount - 1; i > 0; --i)
	{
		char szTempName[260] = {0};
		snprintf(szTempName, sizeof(szTempName), "%s%d.png", szBeginFrameName.c_str(), i);
		CCSpriteFrame * pFrame = pCache->spriteFrameByName(szTempName);
		pArray->addObject(pFrame);
	}

	CCAnimation * pAnimation = CCAnimation::createWithSpriteFrames(pArray, fDelay);
	pAnimation->setLoops(nLoops);
	return pAnimation;
}



}
