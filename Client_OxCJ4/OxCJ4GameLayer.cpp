#include "stdafx.h"
#include "OxCJ4GameLayer.h"
#include "OxCJ4UserInfo.h"
#include "OxCJ4UILayer.h"
#include "OxCJ4JettonControl.h"
#include "OxCJ4CardHeap.h"
#include "OxCJ4TouchCard.h"
#include "ResManager.h"
#include "ResourceDef.h"
#include "GameLogic.h"
#include "CommonDef.h"
#include "CMD_OxCJ4.h"
#include "LayerBase.h"
#include "OxCJ4CaiJinLayer.h"
#include "OxCJ4QuitConfirm.h"

#define TIME_WAIT_BEGIN				5									//开始时间
#define TIME_CALL_BANKER			5									//叫庄时间
#define TIME_PLACE_JETTON			5									//下注时间
#define TIME_OPENCARD				15									//开牌时间

namespace GameOxCJ4
{

COxCJ4GameLayer::COxCJ4GameLayer(CResManager * pResManager)
	:m_pResManager(pResManager)
	,m_pUserInfo(NULL)
	,m_pUILayer(NULL)
	,m_pJettonLayer(NULL)
	,m_pCardLayer(NULL)
	,m_pTipsLayer(NULL)
	,m_pCaiJinLayer(NULL)
	,m_pConfirmLayer(NULL)
	,m_pCallingBankerSprite(NULL)
	,m_pClockSprite(NULL)
	,m_pClockLabel(NULL)
	,m_bInit(false)
	,m_nGameStatus(GS_TK_FREE)
	,m_GameLogic()
{
	memset(m_pBackHeapBatch, 0, sizeof(m_pBackHeapBatch));
	memset(m_pOxCardHeap, 0, sizeof(m_pOxCardHeap));
	memset(m_pTouchCard, 0, sizeof(m_pTouchCard));
	memset(m_pReadySprite, 0, sizeof(m_pReadySprite));
	memset(m_pOverShowSprite, 0, sizeof(m_pOverShowSprite));
	memset(m_pCardTypeSprite, 0, sizeof(m_pCardTypeSprite));

	m_bEnableSound = true;
	m_bAutoPlay = false;
	m_bCanBegin = true;
	m_bHaveShow = false;
	m_bHaveJetton = false;
	memset(m_cbPlayStatus, 0, sizeof(m_cbPlayStatus));
	m_wCurCallUser = INVALID_CHAIR;
	m_wCurrentBanker = INVALID_CHAIR;
	m_nGameTime = 0;

	memset(m_lUserJetton, 0, sizeof(m_lUserJetton));
	m_lCellScore = 0;
	m_lTurnMaxScore = 0;
	m_lAutoJettonScore = 0;
	m_lJetUnitValue = 0;

	memset(m_cbHandCard, 0, sizeof(m_cbHandCard));
	memset(m_cbAllUserCard, 0, sizeof(m_cbAllUserCard));
	memset(m_cbOxType, 0, sizeof(m_cbOxType));
	memset(m_lGetScore, 0, sizeof(m_lGetScore));
	memset(m_bWinnings, 0, sizeof(m_bWinnings));
}

COxCJ4GameLayer::~COxCJ4GameLayer()
{

}

void COxCJ4GameLayer::OnInit()
{
	CCSize cSize = this->getContentSize();
	float fMidx = cSize.width / 2;
	float fMidy = cSize.height / 2;

	CCSprite *pBackView = NULL;
	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Ox_Bk_Begin, pBackView))
	{
		pBackView->setPosition(ccp(fMidx, fMidy));
		addChild(pBackView , 0);
	}

	m_pUserInfo = COxCJ4UserInfo::create(this, m_pResManager);
	if (m_pUserInfo)
	{
		addChild(m_pUserInfo, 10);

		for (WORD i = 0; i < CountArray(m_JettonIconPos); ++i)
		{
			CCPoint cPoint = m_pUserInfo->GetUserCoinWordPos(i);
			m_JettonIconPos[i] = this->convertToNodeSpace(cPoint);
		}
	}

	m_pCardLayer = CCLayer::create();
	if (m_pCardLayer)
	{
		addChild(m_pCardLayer, 20);

		tagTexture* pTexture = m_pResManager->GetConfigHelper()->m_vTextures[0];

		for (BYTE i = 0; i < CountArray(m_pBackHeapBatch); ++i)
		{
			m_pBackHeapBatch[i] = CCSpriteBatchNode::create(pTexture->sTexture.c_str());
			if (m_pBackHeapBatch[i])
				m_pCardLayer->addChild(m_pBackHeapBatch[i]);
		}

		CCPoint oxPoint[4];
		oxPoint[0] = ccp(fMidx - 92, fMidy - 165);
		oxPoint[1] = ccp(fMidx - 450, fMidy);
		oxPoint[2] = ccp(fMidx - 92, fMidy + 165);
		oxPoint[3] = ccp(fMidx + 253, fMidy);

		for (BYTE i = 0; i < CountArray(m_pOxCardHeap); ++i)
		{
			m_pOxCardHeap[i] = COxCJ4CardHeap::create(m_pResManager);
			if (m_pOxCardHeap[i])
			{
				m_pOxCardHeap[i]->setPosition(oxPoint[i]);
				m_pOxCardHeap[i]->ClearAllCard();
				m_pCardLayer->addChild(m_pOxCardHeap[i]);
			}
		}
	}

	m_pJettonLayer = COxCJ4JettonControl::create(this, m_pResManager);
	if (m_pJettonLayer)
		addChild(m_pJettonLayer, 30);

	m_pUILayer = COxCJ4UILayer::create(this, m_pResManager);
	if (m_pUILayer)
	{
		addChild(m_pUILayer, 40);
		m_pUILayer->RestAllMenu();
		CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_TuoGuan);
		if (item)
			item->setVisible(true);
	}

	m_pTipsLayer = CCLayer::create();
	if (m_pTipsLayer)
	{
		addChild(m_pTipsLayer, 50);

		CCPoint cPoint[4];
		cPoint[0] = CCPointMake(fMidx, fMidy - 180);
		cPoint[1] = CCPointMake(fMidx - 400, fMidy);
		cPoint[2] = CCPointMake(fMidx, fMidy + 180);
		cPoint[3] = CCPointMake(fMidx + 400, fMidy);

		for (BYTE i = 0; i < 4; ++i)
		{
			if (!m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Ox_Bg_User_Ready, m_pReadySprite[i]))
				continue;

			m_pReadySprite[i]->setPosition(cPoint[i]);
			m_pReadySprite[i]->setVisible(false);

			m_pTipsLayer->addChild(m_pReadySprite[i]);
		}

		if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Ox_Bg_Calling_Banker,m_pCallingBankerSprite))
		{
			m_pCallingBankerSprite->setPosition(CCPointZero);
			m_pCallingBankerSprite->setVisible(false);

			m_pTipsLayer->addChild(m_pCallingBankerSprite);
		}

		cPoint[0] = CCPointMake(fMidx, fMidy - 200);
		cPoint[1] = CCPointMake(fMidx - 360, fMidy - 50);
		cPoint[2] = CCPointMake(fMidx, fMidy + 120);
		cPoint[3] = CCPointMake(fMidx + 350, fMidy - 50);

		for (BYTE i = 0; i < 4; ++i)
		{
			m_OxTypePos[i] = cPoint[i];
			if (!m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Ox_Bg_Over_Show, m_pOverShowSprite[i]))
				continue;

			m_pOverShowSprite[i]->setPosition(cPoint[i]);
			m_pOverShowSprite[i]->setVisible(false);

			m_pTipsLayer->addChild(m_pOverShowSprite[i]);
		}
	}

	m_pCaiJinLayer = COxCJ4CaiJinLayer::create(this, m_pResManager);
	if (m_pCaiJinLayer)
	{
		m_pCaiJinLayer->setPosition(ccp(cSize.width, cSize.height));
		addChild(m_pCaiJinLayer, 60);
	}

	m_pConfirmLayer = COxCJ4QuitConfirm::create(this, m_pResManager);
	if (m_pConfirmLayer)
	{
		m_pConfirmLayer->setVisible(false);
		addChild(m_pConfirmLayer, 70);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Ox_Bg_Nao_Zhong, m_pClockSprite))
	{
		m_pClockSprite->setPosition(ccp(fMidx + 560, fMidy - 220));
		m_pClockSprite->setVisible(false);

		addChild(m_pClockSprite, 50);
	}

	if (m_pClockSprite && m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Ox_Font_Clock,m_pClockLabel))
	{
		CCSize cSize = m_pClockSprite->getContentSize();
		m_pClockLabel->setPosition(cSize.width /2, cSize.height / 2 - 4);
		m_pClockLabel->setAnchorPoint(ccp(0.5f, 0.5f));
		m_pClockSprite->addChild(m_pClockLabel);
	}

	m_CallingPos[0] = ccp(0,0);
	m_CallingPos[1] = ccp(fMidx - 380, fMidy);
	m_CallingPos[2] = ccp(fMidx, fMidy + 170);
	m_CallingPos[3] = ccp(fMidx + 380, fMidy);

	m_GetScorePos[0] = ccp(fMidx - 480, fMidy - 210);
	m_GetScorePos[1] = ccp(fMidx - 630, fMidy + 110);
	m_GetScorePos[2] = ccp(fMidx + 160, fMidy + 260);
	m_GetScorePos[3] = ccp(fMidx + 625, fMidy + 110);

	m_bInit = true;

	CMD_GF_Info Info;
	memset(&Info, 0, sizeof(Info));
	Info.bAllowLookon = FALSE;
	m_pGameDirector->SendRoomData(MDM_GF_FRAME, SUB_GF_INFO, &Info, sizeof(Info));

	PlayMusic(Ox_Sound_BackGround, true);

	return ;
}

void COxCJ4GameLayer::Clear()
{
	CGameLayer::Clear();
	removeAllChildren();
}


bool COxCJ4GameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return false;
}

void COxCJ4GameLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	return ;
}

void COxCJ4GameLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	return ;
}

bool COxCJ4GameLayer::OnEventGameScene(bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	if (!m_bInit)
		return true;

	switch (GetGameStatus())
	{
	case GS_TK_FREE:
		{
			if (wDataSize != sizeof(CMD_S_StatusFree))
				return false;

			return OnEventGameSceneFree(bLookonOther, static_cast<const CMD_S_StatusFree *>(pBuffer));
		}
	case GS_TK_CALL:
		{
			if (wDataSize != sizeof(CMD_S_StatusCall))
				return false;

			return OnEventGameSceneCall(bLookonOther, static_cast<const CMD_S_StatusCall *>(pBuffer));
		}
	case GS_TK_SCORE:
		{
			if (wDataSize != sizeof(CMD_S_StatusScore))
				return false;

			return OnEventGameSceneScore(bLookonOther, static_cast<const CMD_S_StatusScore *>(pBuffer));
		}
	case GS_TK_PLAYING:
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

bool COxCJ4GameLayer::OnGameMessage(WORD wSubCmd, const void * pBuffer, WORD wDataSize)
{
	if (!m_bInit)
		return true;

	switch (wSubCmd)
	{
	case SUB_S_GAME_START:
		OnSubGameStart(TO_MSG_DATA(CMD_S_GameStart));
		break;

	case SUB_S_ADD_SCORE:
		OnSubAddScore(TO_MSG_DATA(CMD_S_AddScore));
		break;

	case SUB_S_PLAYER_EXIT:
		break;

	case SUB_S_SEND_CARD:
		OnSubSendCard(TO_MSG_DATA(CMD_S_SendCard));
		break;

	case SUB_S_GAME_END:
		OnSubGameEnd(TO_MSG_DATA(CMD_S_GameEnd));
		break;

	case SUB_S_OPEN_CARD:
		OnSubOpenCard(TO_MSG_DATA(CMD_S_Open_Card));
		break;

	case SUB_S_CALL_BANKER:
		OnSubCallBanker(TO_MSG_DATA(CMD_S_CallBanker));
		break;

	case SUB_S_SUCCEED:
		break;

	case SUB_S_CHANGE_CARD:
		break;

	case SUB_S_WINNINGS:
		OnSubWinnings(TO_MSG_DATA(CMD_S_Winnings));
		break;

	case SUB_S_WINNINGS_WEEK:
		OnSubWinWeek(TO_MSG_DATA(DBR_GR_AddWinningsResult));
		break;
	}

	return true;
}

void COxCJ4GameLayer::SendUserReady()
{
	if (GetUserStatus(GetMeChairID()) != US_READY)
	{
		m_pGameDirector->SendRoomData(MDM_GF_FRAME, SUB_GF_USER_READY);
		m_pReadySprite[0]->setVisible(true);
	}
}

void COxCJ4GameLayer::SendGameMessage(WORD wSubCmd, const void* pData, WORD wDataSize)
{
	m_pGameDirector->SendRoomData(MDM_GF_GAME, wSubCmd, pData, wDataSize);
}

bool COxCJ4GameLayer::OnEventGameSceneFree(bool bLookon, const CMD_S_StatusFree * pStatusFree)
{
	SetGameStatus(GS_TK_FREE);
	UpdateUserInfo();
	if (!bLookon && m_pUILayer)
	{
		CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_Start);
		if (item)
			item->setVisible(true);
	}
	m_bCanBegin = true;
	SetGameTime(Time_Status_Begin, TIME_WAIT_BEGIN);

	return true;
}

bool COxCJ4GameLayer::OnEventGameSceneCall(bool bLookon, const CMD_S_StatusCall * pStatusCall)
{
	SetGameStatus(GS_TK_CALL);
	memcpy(m_cbPlayStatus, pStatusCall->cbPlayStatus, sizeof(pStatusCall->cbPlayStatus));
	m_wCurCallUser = pStatusCall->wCallBanker;

	UpdateUserInfo();
	if (!bLookon && m_pUILayer && m_wCurCallUser == GetMeChairID())
	{
		CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_CallBanker);
		if (item)
			item->setVisible(true);

		item = m_pUILayer->GetMItemById(Menu_Item_ID_NoCall);
		if (item)
			item->setVisible(true);
	}

	if (m_pCallingBankerSprite && m_wCurCallUser != GetMeChairID())
	{
		WORD wPos = GetUserChairID(m_wCurCallUser);
		m_pCallingBankerSprite->setVisible(true);
		m_pCallingBankerSprite->setPosition(m_CallingPos[wPos]);
	}

	m_bCanBegin = false;
	SetGameTime(Time_Status_Call, TIME_CALL_BANKER);

	return true;
}

bool COxCJ4GameLayer::OnEventGameSceneScore(bool bLookOn, const CMD_S_StatusScore * pStatusScore)
{
	SetGameStatus(GS_TK_SCORE);
	m_lTurnMaxScore = pStatusScore->lTurnMaxScore;
	memcpy(m_lUserJetton, pStatusScore->lTableScore, sizeof(pStatusScore->lTableScore));
	m_wCurrentBanker = pStatusScore->wBankerUser;
	memcpy(m_cbPlayStatus, pStatusScore->cbPlayStatus, sizeof(pStatusScore->cbPlayStatus));
	m_lCellScore = pStatusScore->lCellScore;

	//设置界面
	UpdateUserInfo();

	if (m_pUserInfo)
		m_pUserInfo->SetBanker(pStatusScore->wBankerUser);

	WORD wUserCount = 0;
	for (WORD i = 0; i < GAME_PLAYER; ++i)
	{
		if (m_cbPlayStatus[i])
			++wUserCount;
	}
	m_lJetUnitValue = MAX(1, ComputeUnitValue(pStatusScore->lCellScore, pStatusScore->bRealyCell, pStatusScore->nMaxMulti, wUserCount));

	if (m_pJettonLayer)
	{
		for (WORD i = 0; i < GAME_PLAYER; ++i)
		{
			if (m_lUserJetton[i] > 0)
			{
				LONGLONG lCoinCount = MAX(1, m_lUserJetton[i] / m_lJetUnitValue);
				if (lCoinCount > Max_Jetton_Count)
					lCoinCount = Max_Jetton_Count;

				m_pJettonLayer->UserPlaceJetton(i, static_cast<BYTE>(lCoinCount), m_lUserJetton[i]);
			}
		}
	}

	if (!bLookOn && m_lUserJetton[GetMeChairID()] == 0 && m_cbPlayStatus[GetMeChairID()] != 0)
	{
		//下注按钮
		if (m_pUILayer)
		{
			CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_Jetton0);
			if (item)
				item->setVisible(true);

			item = m_pUILayer->GetMItemById(Menu_Item_ID_Jetton1);
			if (item)
				item->setVisible(true);

			item = m_pUILayer->GetMItemById(Menu_Item_ID_Jetton2);
			if (item)
				item->setVisible(true);

			item = m_pUILayer->GetMItemById(Menu_Item_ID_Jetton3);
			if (item)
				item->setVisible(true);
		}

		SetCanUseJetton(pStatusScore->lTurnMaxScore, pStatusScore->lCellScore);
	}

	m_bCanBegin = false;
	SetGameTime(Time_Status_Jetton, TIME_PLACE_JETTON);

	return true;
}

bool COxCJ4GameLayer::OnEventGameScenePlay(bool bLookOn, const CMD_S_StatusPlay * pStatusPlay)
{
	SetGameStatus(GS_TK_PLAYING);
	m_lTurnMaxScore = pStatusPlay->lTurnMaxScore;
	memcpy(m_lUserJetton, pStatusPlay->lTableScore, sizeof(pStatusPlay->lTableScore));
	m_wCurrentBanker = pStatusPlay->wBankerUser;
	memcpy(m_cbPlayStatus, pStatusPlay->cbPlayStatus, sizeof(pStatusPlay->cbPlayStatus));
	m_lCellScore = pStatusPlay->lCellScore;
	memcpy(m_cbAllUserCard, pStatusPlay->cbHandCardData, sizeof(pStatusPlay->cbHandCardData));
	memcpy(m_cbHandCard, pStatusPlay->cbHandCardData[GetMeChairID()], sizeof(m_cbHandCard));

	//设置界面
	UpdateUserInfo();

	if (m_pUserInfo)
		m_pUserInfo->SetBanker(pStatusPlay->wBankerUser);

	WORD wUserCount = 0;
	for (WORD i = 0; i < GAME_PLAYER; ++i)
	{
		if (m_cbPlayStatus[i])
			++wUserCount;
	}
	m_lJetUnitValue = MAX(1, ComputeUnitValue(pStatusPlay->lCellScore, pStatusPlay->bRealyCell, pStatusPlay->nMaxMulti, wUserCount));

	if (m_pJettonLayer)
	{
		for (WORD i = 0; i < GAME_PLAYER; ++i)
		{
			if (m_lUserJetton[i] > 0)
			{
				LONGLONG lCoinCount = MAX(1, m_lUserJetton[i] / m_lJetUnitValue);
				if (lCoinCount > Max_Jetton_Count)
					lCoinCount = Max_Jetton_Count;

				m_pJettonLayer->UserPlaceJetton(i, static_cast<BYTE>(lCoinCount), m_lUserJetton[i]);
			}
		}
	}

	//设置发牌动画
	CCSize size = this->getContentSize();
	float fMidx = size.width / 2;
	float fMidy = size.height / 2;

	//自己扑克
	CCPoint fPoint = ccp(fMidx - 240, fMidy - 255);
	if (!bLookOn && m_pCardLayer && pStatusPlay->bOpenCardUser[GetMeChairID()] == false && m_cbPlayStatus[GetMeChairID()] != 0)
	{
		for (BYTE i = 0; i < MAX_COUNT; ++i)
		{
			BYTE nIndex = m_GameLogic.GetCardIndex(m_cbHandCard[i]);
			if (nIndex >= ALL_COUNT)
				continue;

			m_pTouchCard[i] = COxCJ4TouchCard::createWithCfgID(this, m_pResManager, eSpriteType_Base,g_vCardResList[nIndex]);
			if (m_pTouchCard[i] == NULL)
				continue;

			m_pTouchCard[i]->setPosition(ccp(fPoint.x + i * Pos_TouchCardSpace, fPoint.y));
			m_pTouchCard[i]->SetTouchEnable(true);
			m_pCardLayer->addChild(m_pTouchCard[i]);
		}

		//摊牌按钮
		if (m_pUILayer)
		{
			CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_SortCard);
			if (item)
				item->setVisible(true);

			item = m_pUILayer->GetMItemById(Menu_Item_ID_ShowCard);
			if (item)
				item->setVisible(true);

			item = m_pUILayer->GetMItemById(Menu_Item_ID_TiShi);
			if (item)
				item->setVisible(true);
		}

	}

	//自己摊牌扑克
	if (m_pBackHeapBatch[0]  && m_cbPlayStatus[GetMeChairID()] != 0)
	{
		m_pBackHeapBatch[0]->removeAllChildren();
		fPoint = ccp(fMidx - 55, fMidy - 165);
		for (BYTE i = 0; i < MAX_COUNT; ++i)
		{
			CCSprite * pBkCard = NULL;
			if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Ox_Card_0, pBkCard))
			{
				pBkCard->setScale(0.5f);
				pBkCard->setPosition(ccp(fPoint.x + i * Pos_NorCardSpace, fPoint.y));
				m_pBackHeapBatch[0]->addChild(pBkCard);
			}
		}
		m_pBackHeapBatch[0]->setVisible(pStatusPlay->bOpenCardUser[GetMeChairID()] == true);
	}

	if (pStatusPlay->bOpenCardUser[GetMeChairID()] == true && m_pOverShowSprite[0]  && m_cbPlayStatus[GetMeChairID()] != 0)
		m_pOverShowSprite[0]->setVisible(true);

	//其他玩家扑克
	for (BYTE index = 1; index <CountArray(m_pBackHeapBatch); ++index)
	{
		if (m_pBackHeapBatch[index] == NULL)
			continue;

		WORD wTableID = GetUserTableID(index);
		if (m_cbAllUserCard[wTableID][0] == 0)
			continue;

		if (index == 1)
			fPoint = ccp(fMidx - 413, fMidy);
		else if (index == 2)
			fPoint = ccp(fMidx - 55, fMidy + 165);
		else if (index == 3)
			fPoint = ccp(fMidx + 290, fMidy);
		else
		{
			CC_ASSERT(false);
			fPoint = CCPointZero;
		}

		CCSpriteBatchNode * pBatch = m_pBackHeapBatch[index];
		pBatch->removeAllChildren();
		for (BYTE i = 0; i < MAX_COUNT; ++i)
		{
			CCSprite * pBkCard = NULL;
			if (!m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Ox_Card_0, pBkCard))
				continue;

			pBkCard->setPosition(ccp(fPoint.x + i * Pos_NorCardSpace, fPoint.y));
			pBkCard->setScale(0.5f);
			pBatch->addChild(pBkCard);
		}

		if (pStatusPlay->bOpenCardUser[wTableID] == true && m_pOverShowSprite[index])
			m_pOverShowSprite[index]->setVisible(true);
	}

	m_bCanBegin = false;
	SetGameTime(Time_Status_Show, TIME_OPENCARD);

	return true;
}

void COxCJ4GameLayer::OnSubGameFree(float dt)
{
	//清理界面
	for (BYTE i = 0;i < CountArray(m_pCardTypeSprite); ++i)
	{
		if (m_pCardTypeSprite[i])
		{
			m_pCardTypeSprite[i]->removeFromParent();
			m_pCardTypeSprite[i] = NULL;
		}
	}

	for (BYTE i = 0; i < CountArray(m_pBackHeapBatch); ++i)
	{
		if (m_pBackHeapBatch[i])
			m_pBackHeapBatch[i]->removeAllChildren();
	}

	for (BYTE i = 0; i < CountArray(m_pOxCardHeap); ++i)
	{
		if (m_pOxCardHeap[i])
		{
			m_pOxCardHeap[i]->ClearAllCard();
		}
	}

	if (m_pUILayer)
	{
		m_pUILayer->RestAllMenu();

		if (m_bAutoPlay)
		{
			CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_NoTuoGuan);
			if (item)
				item->setVisible(true);
		}
		else
		{
			CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_TuoGuan);
			if (item)
				item->setVisible(true);
		}
	}

	if (m_pUserInfo)
	{
		//更新玩家信息
		for (WORD i = 0; i < GAME_PLAYER; ++i)
		{
			tagLocalUser* pUser = m_pGameDirector->GetTableUserData(i);
			if (pUser == NULL)
				m_pUserInfo->SetUserInfo(i, NULL);
			else if (pUser != NULL && m_cbPlayStatus[i] == 0)
				m_pUserInfo->SetUserInfo(i, pUser);
		}
	}

	if (m_pJettonLayer)
	{
		m_pJettonLayer->RemoveAllJetton();
	}

	if (m_pTipsLayer)
	{
		if (m_pCallingBankerSprite)
			m_pCallingBankerSprite->setVisible(false);

		for (BYTE i = 0; i < CountArray(m_pOverShowSprite); ++i)
		{
			if (m_pOverShowSprite[i])
				m_pOverShowSprite[i]->setVisible(false);
		}

		for (BYTE i = 0; i < CountArray(m_pCardTypeSprite); ++i)
		{
			if (m_pCardTypeSprite[i])
			{
				m_pCardTypeSprite[i]->removeFromParent();
				m_pCardTypeSprite[i] = NULL;
			}
		}
	}

	//初始化变量
	m_bCanBegin = true;
	m_bHaveShow = false;
	m_bHaveJetton = false;
	memset(m_cbPlayStatus, 0, sizeof(m_cbPlayStatus));
	m_wCurCallUser = INVALID_CHAIR;
	m_wCurrentBanker = INVALID_CHAIR;
	m_nGameTime = 0;

	memset(m_lUserJetton, 0, sizeof(m_lUserJetton));
	m_lCellScore = 0;
	m_lTurnMaxScore = 0;
	m_lAutoJettonScore = 0;
	m_lJetUnitValue = 0;

	memset(m_cbHandCard, 0, sizeof(m_cbHandCard));
	memset(m_cbAllUserCard, 0, sizeof(m_cbAllUserCard));
	memset(m_cbOxType, 0, sizeof(m_cbOxType));
	memset(m_lGetScore, 0, sizeof(m_lGetScore));
	memset(m_bWinnings, 0, sizeof(m_bWinnings));
	for (BYTE i = 0; i < CountArray(m_szWinsName); ++i)
	{
		m_szWinsName[i].clear();
	}

	//设置界面
	SetGameStatus(GS_TK_FREE);
	if (!IsLookon() && m_pUILayer && !m_bAutoPlay)
	{
		CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_Start);
		if (item)
			item->setVisible(true);
	}

	SetGameTime(Time_Status_Begin, TIME_WAIT_BEGIN);
}

bool COxCJ4GameLayer::OnSubCallBanker(const CMD_S_CallBanker * pCallBanker)
{
	m_bCanBegin = false;
	if (m_pUILayer)
	{
		CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_Start);
		if (item)
			item->setVisible(false);
	}

	for (BYTE i = 0; i < CountArray(m_pReadySprite); ++i)
	{
		if (m_pReadySprite[i])
			m_pReadySprite[i]->setVisible(false);
	}

	SetGameStatus(GS_TK_CALL);
	memcpy(m_cbPlayStatus, pCallBanker->cbPlayStatus, sizeof(pCallBanker->cbPlayStatus));
	m_wCurCallUser = pCallBanker->wCallBanker;

	//设置界面
	if (!IsLookon() && m_pUILayer && m_wCurCallUser == GetMeChairID()  && !m_bAutoPlay)
	{
		CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_CallBanker);
		if (item)
			item->setVisible(true);

		item = m_pUILayer->GetMItemById(Menu_Item_ID_NoCall);
		if (item)
			item->setVisible(true);
	}

	if (m_pCallingBankerSprite)
	{
		if (m_wCurCallUser == GetMeChairID())
			m_pCallingBankerSprite->setVisible(false);
		else
		{
			WORD wPos = GetUserChairID(m_wCurCallUser);
			m_pCallingBankerSprite->setVisible(true);
			m_pCallingBankerSprite->setPosition(m_CallingPos[wPos]);
		}
	}

	SetGameTime(Time_Status_Call, TIME_CALL_BANKER);

	return true;
}

bool COxCJ4GameLayer::OnSubGameStart(const CMD_S_GameStart * pGameStart)
{
	if (m_pUILayer)
	{
		CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_CallBanker);
		if (item)
			item->setVisible(false);

		item = m_pUILayer->GetMItemById(Menu_Item_ID_NoCall);
		if (item)
			item->setVisible(false);
	}

	if (m_pCallingBankerSprite)
		m_pCallingBankerSprite->setVisible(false);

	SetGameStatus(GS_TK_SCORE);
	m_wCurCallUser = INVALID_CHAIR;
	m_lCellScore = pGameStart->lCellScore;
	m_lTurnMaxScore = pGameStart->lTurnMaxScore;
	m_wCurrentBanker = pGameStart->wBankerUser;
	memcpy(m_cbPlayStatus, pGameStart->cbPlayStatus, sizeof(pGameStart->cbPlayStatus));

	//设置界面
	if (m_pUserInfo)
		m_pUserInfo->SetBanker(pGameStart->wBankerUser);

	WORD wUserCount = 0;
	for (WORD i = 0; i < GAME_PLAYER; ++i)
	{
		if (m_cbPlayStatus[i])
			++wUserCount;
	}
	m_lJetUnitValue = ComputeUnitValue(m_lCellScore, pGameStart->bRealyCell, pGameStart->nMaxMulti, wUserCount);

	//下注按钮
	if (m_wCurrentBanker != GetMeChairID() && m_cbPlayStatus[GetMeChairID()] != 0)
	{
		if (m_pUILayer)
		{
			CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_Jetton0);
			if (item)
				item->setVisible(true);

			item = m_pUILayer->GetMItemById(Menu_Item_ID_Jetton1);
			if (item)
				item->setVisible(true);

			item = m_pUILayer->GetMItemById(Menu_Item_ID_Jetton2);
			if (item)
				item->setVisible(true);

			item = m_pUILayer->GetMItemById(Menu_Item_ID_Jetton3);
			if (item)
				item->setVisible(true);
		}

		SetCanUseJetton(m_lTurnMaxScore, m_lCellScore);
	}

	////取得玩家性别
	//for (WORD i = 0; i < GAME_PLAYER; ++i)
	//{
	//	tagLocalUser* pUser = m_pGameDirector->GetTableUserData(i);
	//	if (pUser)
	//		m_cbUserGender[i] = pUser->bUserGender;
	//}

	SetGameTime(Time_Status_Jetton, TIME_PLACE_JETTON);

	return true;
}

bool COxCJ4GameLayer::OnSubAddScore(const CMD_S_AddScore * pAddScore)
{
	CCLOG("user %d, place jetton:[%lld]", pAddScore->wAddScoreUser, pAddScore->lAddScoreCount);
	CC_ASSERT(pAddScore->wAddScoreUser < GAME_PLAYER);

	m_lUserJetton[pAddScore->wAddScoreUser] = pAddScore->lAddScoreCount;
	if (pAddScore->wAddScoreUser == GetMeChairID())
		m_bHaveJetton = true;

	LONGLONG lCoinCount = MAX(1, pAddScore->lAddScoreCount / MAX(1, m_lJetUnitValue));
	if (lCoinCount > Max_Jetton_Count)
		lCoinCount = Max_Jetton_Count;

	if (m_pJettonLayer)
	{
		m_pJettonLayer->UserPlaceJetton(pAddScore->wAddScoreUser, static_cast<BYTE>(lCoinCount), pAddScore->lAddScoreCount);
		PlaySound(Ox_Sound_Add_Score);
	}

	return true;
}

bool COxCJ4GameLayer::OnSubSendCard(const CMD_S_SendCard * pSendCard)
{
	if (m_pUILayer)
	{
		CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_Jetton0);
		if (item)
			item->setVisible(false);

		item = m_pUILayer->GetMItemById(Menu_Item_ID_Jetton1);
		if (item)
			item->setVisible(false);

		item = m_pUILayer->GetMItemById(Menu_Item_ID_Jetton2);
		if (item)
			item->setVisible(false);

		item = m_pUILayer->GetMItemById(Menu_Item_ID_Jetton3);
		if (item)
			item->setVisible(false);
	}

	memset(m_cbHandCard, 0, sizeof(m_cbHandCard));
	memcpy(m_cbHandCard, pSendCard->cbCardData[GetMeChairID()], sizeof(pSendCard->cbCardData[0]));
	SetGameStatus(GS_TK_PLAYING);

	//设置发牌动画
	if (m_pCardLayer == NULL)
		return false;

	CCSize size = m_pCardLayer->getContentSize();
	float fMidx = size.width / 2;
	float fMidy = size.height / 2;

	//自己扑克
	CCPoint fPoint = ccp(fMidx - 240, fMidy - 255);
	if (m_cbPlayStatus[GetMeChairID()] != 0)
	{
		for (BYTE i = 0; i < MAX_COUNT; ++i)
		{
			if (m_pTouchCard[i])
			{
				m_pTouchCard[i]->removeFromParent();
				m_pTouchCard[i] = NULL;
			}

			BYTE nIndex = m_GameLogic.GetCardIndex(m_cbHandCard[i]);
			if (nIndex >= ALL_COUNT)
				continue;

			m_pTouchCard[i] = COxCJ4TouchCard::createWithCfgID(this, m_pResManager, eSpriteType_Base,g_vCardResList[nIndex]);
			if (m_pTouchCard[i] == NULL)
				continue;

			m_pTouchCard[i]->setPosition(ccp(fMidx, fMidy));
			m_pTouchCard[i]->SetTouchEnable(false);
			m_pTouchCard[i]->setScale(0.5f);
			m_pTouchCard[i]->setVisible(false);
			m_pCardLayer->addChild(m_pTouchCard[i]);

			CCAction * pAction = CCSequence::create(CCDelayTime::create(0.05f * i), CCShow::create(), CCSpawn::create(
				CCJumpTo::create(0.2f, ccp(fPoint.x + i * Pos_TouchCardSpace, fPoint.y), 50.0f, 1), CCScaleTo::create(0.2f, 1.0f), NULL), NULL);

			m_pTouchCard[i]->runAction(pAction);
		}
		PlaySound(Ox_Sound_Send_Card);
		scheduleOnce(schedule_selector(COxCJ4GameLayer::SetCardTouchCanUse), 0.5f);

		//自己摊牌扑克
		if (m_pBackHeapBatch[0])
		{
			m_pBackHeapBatch[0]->removeAllChildren();
			fPoint = ccp(fMidx - 55, fMidy - 165);
			for (BYTE i = 0; i < MAX_COUNT; ++i)
			{
				CCSprite * pBkCard = NULL;
				if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Ox_Card_0, pBkCard))
				{
					pBkCard->setScale(0.5f);
					pBkCard->setPosition(ccp(fPoint.x + i * Pos_NorCardSpace, fPoint.y));
					m_pBackHeapBatch[0]->addChild(pBkCard);
				}
			}
			m_pBackHeapBatch[0]->setVisible(false);
		}
	}
	//其他玩家扑克
	WORD wUsefulCount = 0;
	for (BYTE index = 1; index <CountArray(m_pBackHeapBatch); ++index)
	{
		if (m_pBackHeapBatch[index] == NULL)
			continue;

		WORD wTableID = GetUserTableID(index);
		if (m_cbPlayStatus[wTableID] == 0)
			continue;

		if (index == 1)
			fPoint = ccp(fMidx - 413, fMidy);
		else if (index == 2)
			fPoint = ccp(fMidx - 55, fMidy + 165);
		else if (index == 3)
			fPoint = ccp(fMidx + 290, fMidy);
		else
		{
			CC_ASSERT(false);
			fPoint = CCPointZero;
		}

		++wUsefulCount;
		CCSpriteBatchNode * pBatch = m_pBackHeapBatch[index];
		pBatch->setVisible(true);
		pBatch->removeAllChildren();
		for (BYTE i = 0; i < MAX_COUNT; ++i)
		{
			CCSprite * pBkCard = NULL;
			if (!m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Ox_Card_0, pBkCard))
				continue;

			pBkCard->setPosition(ccp(fMidx, fMidy));
			pBkCard->setScale(0.5f);
			pBkCard->setVisible(false);
			pBatch->addChild(pBkCard);

			float fDelayTime = 0.0f;
			if (index != 3)
				fDelayTime = 0.2f * wUsefulCount + 0.05f * i;
			else
				fDelayTime = 0.2f * wUsefulCount + 0.05f * (MAX_COUNT - i - 1);

			CCAction * pAction = CCSequence::create(CCDelayTime::create(fDelayTime), CCShow::create(),
				CCJumpTo::create(0.2f, ccp(fPoint.x + i * Pos_NorCardSpace, fPoint.y), 50, 1), NULL);

			pBkCard->runAction(pAction);
		}

		scheduleOnce(schedule_selector(COxCJ4GameLayer::PlaySendSound), 0.2f * wUsefulCount);
	}

	SetGameTime(Time_Status_Show, TIME_OPENCARD);

	return true;
}

bool COxCJ4GameLayer::OnSubOpenCard(const CMD_S_Open_Card * pOpenCard)
{
	memcpy(m_cbAllUserCard[pOpenCard->wPlayerID], pOpenCard->cbOpenOxCard, sizeof(pOpenCard->cbOpenOxCard));
	m_cbOxType[pOpenCard->wPlayerID] = m_GameLogic.CheckCardType(pOpenCard->cbOpenOxCard, MAX_COUNT);

	WORD wPos = GetUserChairID(pOpenCard->wPlayerID);
	if (m_pOverShowSprite[wPos] != NULL)
	{
		m_pOverShowSprite[wPos]->setVisible(true);
	}
	if (pOpenCard->wPlayerID == GetMeChairID())
	{
		for (BYTE i = 0; i < MAX_COUNT; ++i)
		{
			if (m_pTouchCard[i])
			{
				m_pTouchCard[i]->removeFromParent();
				m_pTouchCard[i] = NULL;
			}
		}
		unschedule(schedule_selector(COxCJ4GameLayer::SetCardTouchCanUse));
		if (m_pBackHeapBatch[0])
			m_pBackHeapBatch[0]->setVisible(true);
	}

	PlaySound(Ox_Sound_Open_Card);

	return true;
}

bool COxCJ4GameLayer::OnSubGameEnd(const CMD_S_GameEnd * pGameEnd)
{
	if (m_pUILayer)
	{
		CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_SortCard);
		if (item)
			item->setVisible(false);

		item = m_pUILayer->GetMItemById(Menu_Item_ID_ShowCard);
		if (item)
			item->setVisible(false);

		item = m_pUILayer->GetMItemById(Menu_Item_ID_TiShi);
		if (item)
			item->setVisible(false);
	}

	if (m_nGameStatus != GS_TK_PLAYING)
	{
		OnSubGameFree(0.0f);
		return true;
	}

	SetGameStatus(GS_TK_FREE);
	memcpy(m_lGetScore, pGameEnd->lGameScore, sizeof(pGameEnd->lGameScore));
	memcpy(m_bWinnings, pGameEnd->bWinnings, sizeof(pGameEnd->bWinnings));

	for (BYTE i = 0; i < GAME_PLAYER; ++i)
	{
		if (m_bWinnings[i] == false)
			continue;

		tagLocalUser* pUser = m_pGameDirector->GetTableUserData(i);
		if (!pUser)
		{
			m_bWinnings[i] = false;
			continue;
		}

		m_szWinsName[i] = CStringHelper::UTF8ToGB2312(pUser->sUserName.c_str());
	}

	//进行结束处理
	m_bCanBegin = false;
	ShowAllUserCard();

	unschedule(schedule_selector(COxCJ4GameLayer::OnTimeBegin));
	unschedule(schedule_selector(COxCJ4GameLayer::OnTimeCallBanker));
	unschedule(schedule_selector(COxCJ4GameLayer::OnTimeJetton));
	unschedule(schedule_selector(COxCJ4GameLayer::OnTimeOpenCard));

	if (m_pClockSprite)
	{
		m_pClockSprite->setVisible(false);
		SetClockTime(0);
	}

	return true;
}

bool COxCJ4GameLayer::OnSubWinnings(const CMD_S_Winnings * pWinnings)
{
	if (m_pCaiJinLayer)
		m_pCaiJinLayer->SetCaiJinChi(pWinnings);

	return true;
}

bool COxCJ4GameLayer::OnSubWinWeek(const DBR_GR_AddWinningsResult * pWinWeek)
{
	LONGLONG lScore = pWinWeek->lWinnings;
	if (m_pCaiJinLayer)
		m_pCaiJinLayer->SetMeWeekWin(lScore);

	return true;
}

void COxCJ4GameLayer::ShowGameMessage(const char* sMsg)
{

}

void COxCJ4GameLayer::OnUserScoreChange(WORD wChairID, LONGLONG lUserScore)
{
	if (m_pUserInfo)
		m_pUserInfo->SetUserScore(wChairID, lUserScore);
}

void COxCJ4GameLayer::OnUserSit(WORD wChairID, const tagLocalUser* pLocalUser)
{
	if (m_pUserInfo)
		m_pUserInfo->SetUserInfo(wChairID, pLocalUser);
}

void COxCJ4GameLayer::OnUserStateChange(WORD wChairID, int nUserState, bool bLookup)
{
	if (bLookup)
		return ;

	WORD wPos = GetUserChairID(wChairID);
	if (nUserState = US_READY && wChairID != GetMeChairID())
	{
		if (m_pReadySprite[wPos])
			m_pReadySprite[wPos]->setVisible(true);
	}
	else if (wChairID != GetMeChairID())
	{
		if (m_pReadySprite[wPos])
			m_pReadySprite[wPos]->setVisible(false);
	}
}

void COxCJ4GameLayer::OnUserLeave(WORD wChairID)
{
	m_cbPlayStatus[wChairID];
	switch(m_nGameStatus)
	{
	case GS_TK_FREE:
	case GS_TK_CALL:
		{
			if (m_pUserInfo)
				m_pUserInfo->SetUserInfo(wChairID, NULL);
		}
		break;
	case GS_TK_SCORE:
		{
			if (m_pUserInfo)
				m_pUserInfo->SetUserInfo(wChairID, NULL);

			m_lUserJetton[wChairID] = 0;
		}
		break;
	case GS_TK_PLAYING:
		{

		}
		break;
	}
}

bool COxCJ4GameLayer::IsLookon()
{
	tagLocalUser* pUser = m_pGameDirector->GetMeData();
	return pUser && pUser->cbUserStatus == US_LOOKON;
}


void COxCJ4GameLayer::SetGameStatus(int cbGameStatus)
{
	CCLOG("Game status Change: [%d]", cbGameStatus);
	m_nGameStatus = cbGameStatus;
}

WORD COxCJ4GameLayer::GetUserChairID(WORD wChairID)
{
	return CLayerBase::ServerTableIndexToLocal(wChairID, GetMeChairID());
}

WORD COxCJ4GameLayer::GetUserTableID(WORD wChairID)
{
	//参数校验
	if (wChairID < 0 || wChairID >= GAME_PLAYER)
	{
		CC_ASSERT(false);
		return INVALID_CHAIR;
	}

	return (wChairID + GetMeChairID()) % GAME_PLAYER;
}

BYTE COxCJ4GameLayer::GetUserStatus(WORD wChairID)
{
	tagLocalUser* pUser = m_pGameDirector->GetTableUserData(wChairID);
	if (pUser)
		return pUser->cbUserStatus;
	else
		return US_NULL;
}

void COxCJ4GameLayer::MePlaceJetton(LONGLONG lScore)
{
	if (m_bHaveJetton == true || m_nGameStatus != GS_TK_SCORE)
		return ;

	CMD_C_AddScore AddScore;
	memset(&AddScore, 0, sizeof(AddScore));
	AddScore.lScore = lScore;

	SendGameMessage(SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));

	m_bHaveJetton = true;
}

void COxCJ4GameLayer::SortHandCard()
{
	if (m_nGameStatus != GS_TK_PLAYING)
		return ;

	for (BYTE i = 0; i < MAX_COUNT; ++i)
	{
		if (m_pTouchCard[i])
			m_pTouchCard[i]->SetJump(false);
	}

	return ;
}

bool COxCJ4GameLayer::ShowMyCard(bool bIsSelfShow)
{
	if (m_nGameStatus != GS_TK_PLAYING || m_bHaveShow)
		return false;

	//发送消息
	CMD_C_OxCard ShowCard;
	memset(&ShowCard, 0, sizeof(ShowCard));
	ShowCard.bMyOpenCard = bIsSelfShow;

	if (bIsSelfShow)
	{
		BYTE cbJumpData[MAX_COUNT];
		BYTE cbNoJumpData[MAX_COUNT];
		BYTE cbJumpCount = 0;
		BYTE cbNoJumpCount = 0;
		memset(cbJumpData, 0, sizeof(cbJumpData));
		memset(cbNoJumpData, 0, sizeof(cbNoJumpData));

		for (BYTE i = 0; i < MAX_COUNT; ++i)
		{
			if (m_pTouchCard[i] && m_pTouchCard[i]->IsJump())
				cbJumpData[cbJumpCount++] = m_cbHandCard[i];
			else
				cbNoJumpData[cbNoJumpCount++] = m_cbHandCard[i];
		}

		if (cbJumpCount == 2)
			memcpy(ShowCard.cbMyOpenCard, cbJumpData, sizeof(cbJumpData[0]) * 2);
		else if (cbNoJumpCount == 2)
			memcpy(ShowCard.cbMyOpenCard, cbNoJumpData, sizeof(cbNoJumpData[0]) * 2);
		else
			ShowCard.bMyOpenCard = false;
	}

	SendGameMessage(SUB_C_OPEN_CARD, &ShowCard, sizeof(ShowCard));
	m_bHaveShow = true;

	//更新界面
	if (m_pUILayer)
	{
		CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_SortCard);
		if (item)
			item->setVisible(false);

		item = m_pUILayer->GetMItemById(Menu_Item_ID_ShowCard);
		if (item)
			item->setVisible(false);

		item = m_pUILayer->GetMItemById(Menu_Item_ID_TiShi);
		if (item)
			item->setVisible(false);
	}

	for (BYTE i = 0; i < MAX_COUNT; ++i)
	{
		if (m_pTouchCard[i])
		{
			m_pTouchCard[i]->removeFromParent();
			m_pTouchCard[i] = NULL;
		}
	}

	if (m_pBackHeapBatch[0])
		m_pBackHeapBatch[0]->setVisible(true);
	if (m_pOverShowSprite[0])
		m_pOverShowSprite[0]->setVisible(true);

	return true;
}

void COxCJ4GameLayer::TeachMeOut()
{
	if (m_nGameStatus != GS_TK_PLAYING)
		return ;

	BYTE cbMaxTypeData[MAX_COUNT];
	memset(cbMaxTypeData, 0, sizeof(cbMaxTypeData));
	memcpy(cbMaxTypeData, m_cbHandCard, sizeof(m_cbHandCard));
	BYTE cbOxType = m_GameLogic.GetMaxCardType(cbMaxTypeData, MAX_COUNT);

	SortHandCard();
	if (cbOxType != CT_NIU0)
	{
		for (BYTE i = 3; i < MAX_COUNT; ++i)
		{
			for (BYTE j = 0; j < MAX_COUNT; ++j)
			{
				if (cbMaxTypeData[i] == m_cbHandCard[j])
				{
					if (m_pTouchCard[j])
						m_pTouchCard[j]->SetJump(true);
					break;
				}
			}
		}
	}

	UpdateOxType();

	return ;
}

void COxCJ4GameLayer::AutoPlay(bool bCanAuto)
{
	m_bAutoPlay = bCanAuto;

	if (m_cbPlayStatus[GetMeChairID()] == 0)
		return ;

	//更新状态
	if (bCanAuto == true)
	{
		if (m_nGameStatus == GS_TK_FREE)
		{
			if (m_pUILayer)
			{
				CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_Start);
				if (item)
					item->setVisible(false);
			}
		}
		else if (m_nGameStatus == GS_TK_CALL)
		{
			if (m_pUILayer)
			{
				CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_CallBanker);
				if (item)
					item->setVisible(false);

				item = m_pUILayer->GetMItemById(Menu_Item_ID_NoCall);
				if (item)
					item->setVisible(false);
			}
		}
		else if (m_nGameStatus == GS_TK_SCORE)
		{
			if (m_pUILayer)
			{
				CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_Jetton0);
				if (item)
					item->setVisible(false);

				item = m_pUILayer->GetMItemById(Menu_Item_ID_Jetton1);
				if (item)
					item->setVisible(false);

				item = m_pUILayer->GetMItemById(Menu_Item_ID_Jetton2);
				if (item)
					item->setVisible(false);

				item = m_pUILayer->GetMItemById(Menu_Item_ID_Jetton3);
				if (item)
					item->setVisible(false);
			}
		}
		else if (m_nGameStatus == GS_TK_PLAYING)
		{
			for (BYTE i = 0; i < MAX_COUNT; ++i)
			{
				if (m_pTouchCard[i])
				{
					m_pTouchCard[i]->SetJump(false);
					m_pTouchCard[i]->SetTouchEnable(false);
				}
			}
			UpdateOxType();

			if (m_pUILayer)
			{
				CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_SortCard);
				if (item)
					item->setVisible(false);

				item = m_pUILayer->GetMItemById(Menu_Item_ID_ShowCard);
				if (item)
					item->setVisible(false);

				item = m_pUILayer->GetMItemById(Menu_Item_ID_TiShi);
				if (item)
					item->setVisible(false);
			}
		}
	}
	else
	{
		if (m_nGameStatus == GS_TK_FREE && m_bCanBegin == true)
		{
			//if (isScheduled(schedule_selector(COxCJ4GameLayer::OnTimeBegin)))
			{
				BYTE cbMeStatus = GetUserStatus(GetMeChairID());
				if (cbMeStatus != US_LOOKON && cbMeStatus != US_READY && m_pUILayer)
				{
					CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_Start);
					if (item)
						item->setVisible(true);
				}
			}
		}
		else if (m_nGameStatus == GS_TK_CALL)
		{
			//if (isScheduled(schedule_selector(COxCJ4GameLayer::OnTimeCallBanker)))
			{
				if (!IsLookon() && m_pUILayer && m_wCurCallUser == GetMeChairID() && m_wCurrentBanker == INVALID_CHAIR)
				{
					CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_CallBanker);
					if (item)
						item->setVisible(true);

					item = m_pUILayer->GetMItemById(Menu_Item_ID_NoCall);
					if (item)
						item->setVisible(true);
				}
			}
		}
		else if (m_nGameStatus == GS_TK_SCORE)
		{
			//if (isScheduled(schedule_selector(COxCJ4GameLayer::OnTimeJetton)))
			{
				if (!IsLookon() && m_pUILayer && m_wCurrentBanker != GetMeChairID()&& m_bHaveJetton == false)
				{
					if (m_pUILayer)
					{
						CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_Jetton0);
						if (item)
							item->setVisible(true);

						item = m_pUILayer->GetMItemById(Menu_Item_ID_Jetton1);
						if (item)
							item->setVisible(true);

						item = m_pUILayer->GetMItemById(Menu_Item_ID_Jetton2);
						if (item)
							item->setVisible(true);

						item = m_pUILayer->GetMItemById(Menu_Item_ID_Jetton3);
						if (item)
							item->setVisible(true);
					}

					SetCanUseJetton(m_lTurnMaxScore, m_lCellScore);
				}
			}
		}
		else if (m_nGameStatus == GS_TK_PLAYING)
		{
			//if (isScheduled(schedule_selector(COxCJ4GameLayer::OnTimeOpenCard)))
			{
				if (!IsLookon() && m_pUILayer && !m_bHaveShow)
				{
					for (BYTE i = 0; i < MAX_COUNT; ++i)
					{
						if (m_pTouchCard[i])
						{
							m_pTouchCard[i]->SetJump(false);
							m_pTouchCard[i]->SetTouchEnable(true);
						}
					}
					UpdateOxType();

					if (m_pUILayer)
					{
						CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_SortCard);
						if (item)
							item->setVisible(true);

						item = m_pUILayer->GetMItemById(Menu_Item_ID_ShowCard);
						if (item)
							item->setVisible(true);

						item = m_pUILayer->GetMItemById(Menu_Item_ID_TiShi);
						if (item)
							item->setVisible(true);
					}
				}
			}
		}
	}

	return ;
}

void COxCJ4GameLayer::QuitGame()
{
	if (m_pConfirmLayer)
		m_pConfirmLayer->setVisible(true);
}

void COxCJ4GameLayer::GetAllUserJetIconPos(CCPoint jettonPos[GAME_PLAYER])
{
	CC_ASSERT(CountArray(m_JettonIconPos) == GAME_PLAYER);
	memcpy(jettonPos, m_JettonIconPos, sizeof(m_JettonIconPos));
}

BYTE COxCJ4GameLayer::UpdateOxType()
{
	if (m_nGameStatus != GS_TK_PLAYING)
		return CT_NIU0;

	BYTE cbJumpData[MAX_COUNT];
	BYTE cbNoJumpData[MAX_COUNT];
	BYTE cbJumpCount = 0;
	BYTE cbNoJumpCount = 0;
	memset(cbJumpData, 0, sizeof(cbJumpData));
	memset(cbNoJumpData, 0, sizeof(cbNoJumpData));

	for (BYTE i = 0; i < MAX_COUNT; ++i)
	{
		if (m_pTouchCard[i] && m_pTouchCard[i]->IsJump())
			cbJumpData[cbJumpCount++] = m_cbHandCard[i];
		else
			cbNoJumpData[cbNoJumpCount++] = m_cbHandCard[i];
	}

	BYTE cbOxType = CT_NIU0;
	if ((cbJumpCount == 2 && cbNoJumpCount == 3) || (cbJumpCount == 3 && cbNoJumpCount == 2))
	{
		BYTE cbCheckData[MAX_COUNT];
		memset(cbCheckData, 0, sizeof(cbCheckData));

		if (cbJumpCount == 2)
		{
			memcpy(cbCheckData, cbNoJumpData, sizeof(cbNoJumpData[0]) * 3);
			memcpy(&cbCheckData[3], cbJumpData, sizeof(cbJumpData[0]) * 2);
		}
		else
		{
			memcpy(cbCheckData, cbJumpData, sizeof(cbJumpData[0]) * 3);
			memcpy(&cbCheckData[3], cbNoJumpData, sizeof(cbNoJumpData[0]) * 2);
		}

		cbOxType = m_GameLogic.CheckCardType(cbCheckData, MAX_COUNT);
	}

	//更新摊牌按钮状态
	if (m_pUILayer)
	{
		CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_ShowCard);
		if (item)
			item->setEnabled(cbJumpCount == 2 || cbJumpCount == 3 || cbJumpCount == 0);
	}

	return cbOxType;
}

void COxCJ4GameLayer::PlaySound(int nResId)
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

void COxCJ4GameLayer::PlayMusic(int nResId, bool bLoop)
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

void COxCJ4GameLayer::PlayOxSound(CCNode * pTarget, void * data)
{
	WORD wChairID = reinterpret_cast<int>(data);
	BYTE cbUserGender = 0;

	tagLocalUser* pUser = m_pGameDirector->GetTableUserData(wChairID);
	if (pUser)
		cbUserGender = pUser->bUserGender;

	int nOxSoundId = GetOxSoundResID(m_cbOxType[wChairID], cbUserGender);
	PlaySound(nOxSoundId);
}

void COxCJ4GameLayer::UpdateUserInfo(WORD wChairID /*= INVALID_CHAIR*/)
{
	if (m_pUserInfo == NULL)
		return ;

	if (wChairID == INVALID_CHAIR)
	{
		for (BYTE i = 0; i < GAME_PLAYER; ++i)
		{
			tagLocalUser * pUserData = m_pGameDirector->GetTableUserData(i);
			m_pUserInfo->SetUserInfo(i, pUserData);
		}
	}
	else
	{
		tagLocalUser * pUserData = m_pGameDirector->GetTableUserData(wChairID);
		m_pUserInfo->SetUserInfo(wChairID, pUserData);
	}
}

LONGLONG COxCJ4GameLayer::ComputeUnitValue(LONGLONG lCellScore, bool bIsRealCell, int nMaxMulti, WORD wUserCount)
{
	if ((m_nGameStatus != GS_TK_SCORE && m_nGameStatus != GS_TK_PLAYING) || m_wCurrentBanker == INVALID_CHAIR || nMaxMulti <= 0 || wUserCount <= 1)
	{
		CC_ASSERT(false);
		return 1;
	}

	if (bIsRealCell)
	{
		return g_nTimes[3] * lCellScore / Max_Jetton_Count;
	}
	else
	{
		tagLocalUser * pUserData = m_pGameDirector->GetTableUserData(m_wCurrentBanker);
		if (pUserData == NULL)
			return 1;

		LONGLONG lBankerScore = pUserData->nUserScore;
		LONGLONG lMaxJettonValue = 0;

		for (WORD i = 0; i < GAME_PLAYER; ++i)
		{
			if (i == m_wCurrentBanker)
				continue;

			pUserData = m_pGameDirector->GetTableUserData(i);
			if (pUserData == NULL)
				continue;

			LONGLONG lTempValue = MIN(lBankerScore / (wUserCount - 1) / nMaxMulti, pUserData->nUserScore / nMaxMulti);
			if (lTempValue > lMaxJettonValue)
				lMaxJettonValue = lTempValue;
		}

		return MAX(1, lMaxJettonValue / Max_Jetton_Count);
	}
}

void COxCJ4GameLayer::SetCanUseJetton(LONGLONG lMaxScore, LONGLONG lCellScore)
{
	CC_ASSERT(m_nGameStatus == GS_TK_SCORE);
	if (m_nGameStatus != GS_TK_SCORE  || m_cbPlayStatus[GetMeChairID()] == 0)
		return ;
	if (m_pUILayer == NULL)
		return ;

	LONGLONG lJettonValue[4];
	bool bEnable[4];
	memset(lJettonValue, 0, sizeof(lJettonValue));
	memset(bEnable, 0, sizeof(bEnable));

	for (BYTE i = 0; i < 4; ++i)
	{
		lJettonValue[i] = g_nTimes[i] * lCellScore;

		if (lJettonValue[i] > lMaxScore)
			bEnable[i] = false;
		else
			bEnable[i] = true;
	}
	m_lAutoJettonScore = lJettonValue[3];

	if (IsLookon() || m_bAutoPlay == true)
		memset(bEnable, 0, sizeof(bEnable));

	m_pUILayer->SetJettonValue(4, lJettonValue, bEnable);
}

void COxCJ4GameLayer::SetGameTime(int nType, int nTime)
{
	CCLOG("SetGameTime type:[%d], time[%d]", nType, nTime);

	unschedule(schedule_selector(COxCJ4GameLayer::OnTimeBegin));
	unschedule(schedule_selector(COxCJ4GameLayer::OnTimeCallBanker));
	unschedule(schedule_selector(COxCJ4GameLayer::OnTimeJetton));
	unschedule(schedule_selector(COxCJ4GameLayer::OnTimeOpenCard));

	m_nGameTime = nTime;
	if (m_pClockSprite)
	{
		m_pClockSprite->setVisible(true);
		SetClockTime(nTime);
	}

	switch(nType)
	{
	case Time_Status_Begin:
		CC_ASSERT(m_nGameStatus == GS_TK_FREE);
		schedule(schedule_selector(COxCJ4GameLayer::OnTimeBegin), 1.0f);
		break;
	case Time_Status_Call:
		CC_ASSERT(m_nGameStatus == GS_TK_CALL);
		schedule(schedule_selector(COxCJ4GameLayer::OnTimeCallBanker), 1.0f);
		break;
	case Time_Status_Jetton:
		CC_ASSERT(m_nGameStatus == GS_TK_SCORE);
		schedule(schedule_selector(COxCJ4GameLayer::OnTimeJetton), 1.0f);
		break;
	case Time_Status_Show:
		CC_ASSERT(m_nGameStatus == GS_TK_PLAYING);
		schedule(schedule_selector(COxCJ4GameLayer::OnTimeOpenCard), 1.0f);
		break;
	default:
		CC_ASSERT(false);
		break;
	}

	return ;
}

void COxCJ4GameLayer::SetClockTime(int nTime)
{
	if (nTime < 0 || m_pClockLabel == NULL)
		return ;

	char szValue[32];
	memset(szValue, 0, sizeof(szValue));
	snprintf(szValue, 32, "%d", nTime);
	m_pClockLabel->setString(szValue);
}

void COxCJ4GameLayer::OnTimeBegin(float dt)
{
	SetClockTime(--m_nGameTime);

	if (m_nGameTime <= 0 || (m_nGameTime <= (TIME_WAIT_BEGIN - 2) && m_bAutoPlay))
	{
		unschedule(schedule_selector(COxCJ4GameLayer::OnTimeBegin));
		m_pClockSprite->setVisible(false);

		if (m_pUILayer)
		{
			CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_Start);
			if (item)
				item->setVisible(false);
		}

		SendUserReady();

		return ;
	}

}

void COxCJ4GameLayer::OnTimeCallBanker(float dt)
{
	SetClockTime(--m_nGameTime);

	if (m_nGameTime <= 0 || (m_nGameTime <= (TIME_CALL_BANKER - 2) && m_bAutoPlay && m_wCurCallUser == GetMeChairID()))
	{
		unschedule(schedule_selector(COxCJ4GameLayer::OnTimeCallBanker));
		m_pClockSprite->setVisible(false);

		if (m_pUILayer)
		{
			CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_CallBanker);
			if (item)
				item->setVisible(false);

			item = m_pUILayer->GetMItemById(Menu_Item_ID_NoCall);
			if (item)
				item->setVisible(false);
		}

		if (m_wCurCallUser == GetMeChairID() && m_wCurrentBanker == INVALID_CHAIR)
		{
			CMD_C_CallBanker CallBanker;
			memset(&CallBanker, 0, sizeof(CallBanker));
			CallBanker.cbBanker = 0;

			SendGameMessage(SUB_C_CALL_BANKER, &CallBanker, sizeof(CallBanker));
		}

		return ;
	}

}

void COxCJ4GameLayer::OnTimeJetton(float dt)
{
	SetClockTime(--m_nGameTime);

	if (m_nGameTime <= 0 || (m_nGameTime <= (TIME_PLACE_JETTON - 2) && m_bAutoPlay && m_wCurrentBanker != GetMeChairID() && m_cbPlayStatus[GetMeChairID()] != 0))
	{
		unschedule(schedule_selector(COxCJ4GameLayer::OnTimeJetton));
		m_pClockSprite->setVisible(false);

		if (m_pUILayer)
		{
			CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_Jetton0);
			if (item)
				item->setVisible(false);

			item = m_pUILayer->GetMItemById(Menu_Item_ID_Jetton1);
			if (item)
				item->setVisible(false);

			item = m_pUILayer->GetMItemById(Menu_Item_ID_Jetton2);
			if (item)
				item->setVisible(false);

			item = m_pUILayer->GetMItemById(Menu_Item_ID_Jetton3);
			if (item)
				item->setVisible(false);
		}

		if (m_wCurrentBanker != GetMeChairID() && m_bHaveJetton == false)
		{
			CMD_C_AddScore AddScore;
			memset(&AddScore, 0, sizeof(AddScore));
			AddScore.lScore  = (m_lAutoJettonScore <= 0) ? m_lTurnMaxScore : m_lAutoJettonScore;

			SendGameMessage(SUB_C_ADD_SCORE, &AddScore, sizeof(AddScore));
			m_bHaveJetton = true;
		}

		return ;
	}

}

void COxCJ4GameLayer::OnTimeOpenCard(float dt)
{
	SetClockTime(--m_nGameTime);

	if (m_nGameTime <= 0 || (m_nGameTime <= (TIME_OPENCARD - 3) && m_bAutoPlay && m_cbPlayStatus[GetMeChairID()] != 0))
	{
		unschedule(schedule_selector(COxCJ4GameLayer::OnTimeOpenCard));
		m_pClockSprite->setVisible(false);

		if (m_bHaveShow == false)
			ShowMyCard(false);

		return ;
	}

	if (m_nGameTime <= 5)
		PlaySound(Ox_Sound_Game_Warn);
}

void COxCJ4GameLayer::SetCardTouchCanUse(float dt)
{
	if (m_nGameStatus != GS_TK_PLAYING || IsLookon() || m_bHaveShow || m_cbPlayStatus[GetMeChairID()] == 0)
		return ;

	for (BYTE i = 0;i < CountArray(m_pTouchCard); ++i)
	{
		if (m_pTouchCard[i])
		{
			m_pTouchCard[i]->SetTouchEnable(!IsLookon() && !m_bAutoPlay);
		}
	}

	if (!IsLookon() && m_pUILayer && !m_bAutoPlay)
	{
 		CCMenuItem * item = m_pUILayer->GetMItemById(Menu_Item_ID_SortCard);
		if (item)
			item->setVisible(true);

		item = m_pUILayer->GetMItemById(Menu_Item_ID_ShowCard);
		if (item)
			item->setVisible(true);

		item = m_pUILayer->GetMItemById(Menu_Item_ID_TiShi);
		if (item)
			item->setVisible(true);
	}
}

void COxCJ4GameLayer::ShowAllUserCard()
{
	if (m_nGameStatus != GS_TK_FREE)
		return ;

	//取得需要摊牌用户
	WORD wShowUser[GAME_PLAYER];
	WORD wShowCount = 0;
	memset(wShowUser, 0, sizeof(wShowUser));
	for (WORD i = 0;i < GAME_PLAYER; ++i)
	{
		if (m_cbAllUserCard[i][0] != 0)
		{
			wShowUser[wShowCount++] = i;
		}
	}
	if (wShowCount <= 0)
	{
		CC_ASSERT(false);
		m_bCanBegin = true;
		OnSubGameFree(0.0f);
		return ;
	}

	RandValue(wShowUser, wShowCount);
	//调整庄家开牌时间为最后
	for (WORD i = 0;i < wShowCount - 1; ++i)
	{
		if (wShowUser[i] == m_wCurrentBanker)
		{
			wShowUser[i] = wShowUser[wShowCount - 1];
			wShowUser[wShowCount - 1] = m_wCurrentBanker;
			break;
		}
	}

	float fDelay = 1.0f;
	for (WORD wIndex = 0; wIndex < wShowCount; ++wIndex)
	{
		WORD wChairID = wShowUser[wIndex];
		WORD wPos = GetUserChairID(wChairID);

		if (m_pBackHeapBatch[wPos])
			m_pBackHeapBatch[wPos]->runAction(CCSequence::create(CCDelayTime::create(wIndex * fDelay), CCHide::create(), NULL));
		if (m_pOverShowSprite[wPos])
			m_pOverShowSprite[wPos]->runAction(CCSequence::create(CCDelayTime::create(wIndex * fDelay), CCHide::create(), NULL));

		if (m_pOxCardHeap[wPos] == NULL)
			continue;


		m_pOxCardHeap[wPos]->ClearAllCard();
		m_pOxCardHeap[wPos]->SetCardData(m_cbAllUserCard[wChairID], MAX_COUNT, m_cbOxType[wChairID] > CT_NIU0);
		m_pOxCardHeap[wPos]->setVisible(false);

		m_pOxCardHeap[wPos]->runAction(CCSequence::create(CCDelayTime::create(wIndex * fDelay), CCShow::create(),
			CCCallFuncND::create(this, callfuncND_selector(COxCJ4GameLayer::PlayOxSound), reinterpret_cast<void *>(wChairID)),NULL));

		if (m_pCardTypeSprite[wPos])
		{
			m_pCardTypeSprite[wPos]->removeFromParent();
			m_pCardTypeSprite[wPos] = NULL;
		}

		if (!m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, GetOxTypeResID(m_cbOxType[wChairID]), m_pCardTypeSprite[wPos]))
			continue;

		m_pCardTypeSprite[wPos]->setPosition(m_OxTypePos[wPos]);
		m_pCardTypeSprite[wPos]->setVisible(false);
		if (m_pTipsLayer)
			m_pTipsLayer->addChild(m_pCardTypeSprite[wPos]);

		if (m_cbOxType[wChairID] == CT_NIU0)
		{
			m_pCardTypeSprite[wPos]->runAction(CCSequence::create(CCDelayTime::create(wIndex * fDelay), CCShow::create(),
				CCScaleTo::create(0.0f, 7.0f), CCSpawn::create(CCScaleTo::create(fDelay, 1.0f), CCFadeIn::create(fDelay), NULL), NULL));
		}
		else
		{
			float dt = fDelay / 3;
			m_pCardTypeSprite[wPos]->runAction(CCSequence::create(CCDelayTime::create(wIndex * fDelay), CCShow::create(),
				CCScaleTo::create(dt, 1.5f), CCScaleTo::create(dt, 0.5f), CCScaleTo::create(dt, 1.0f), NULL));
		}
	}

	//金币改变过程
	scheduleOnce(schedule_selector(COxCJ4GameLayer::PlaceJettonChangeStep), fDelay * wShowCount + 0.3f);

	return ;
}

void COxCJ4GameLayer::PlaceJettonChangeStep(float dt)
{
	if (m_nGameStatus != GS_TK_FREE)
		return ;

	if (m_pJettonLayer == NULL)
	{
		m_bCanBegin = true;
		OnSubGameFree(0.0f);
		return ;
	}

	//处理桌面筹码
	LONGLONG lMaxCount = 0;
	LONGLONG lJetUnitValue = MAX(1, m_lJetUnitValue);
	for (WORD i = 0; i < GAME_PLAYER; ++i)
	{
		if (i == m_wCurrentBanker || m_lUserJetton[i] <= 0)
			continue;

		LONGLONG lCount = MAX(1, m_lUserJetton[i] / lJetUnitValue);
		if (lCount > lMaxCount)
			lMaxCount = lCount;

		if (m_lGetScore[i] < 0)
		{
			m_pJettonLayer->GivePlaceJetton(i, m_wCurrentBanker);
			if (-m_lGetScore[i] - m_lUserJetton[i] > 0)
			{
				LONGLONG lGiveCount = MAX(1, (-m_lGetScore[i] - m_lUserJetton[i]) / lJetUnitValue);
				if (lGiveCount > Max_Jetton_Count)
					lGiveCount = Max_Jetton_Count;

				m_pJettonLayer->GiveHoldJetton(i, m_wCurrentBanker, static_cast<BYTE>(lGiveCount));
				if (lGiveCount > lMaxCount)
					lMaxCount = lGiveCount;
			}
		}
		else if (m_lGetScore[i] > 0)
		{
			m_pJettonLayer->GivePlaceJetton(i, i);
		}
	}

	lMaxCount = (lMaxCount > Max_Jetton_Count) ? Max_Jetton_Count : lMaxCount;

	scheduleOnce(schedule_selector(COxCJ4GameLayer::BankerJettonChangeStep), lMaxCount * 0.05f + 0.5f);

	//显示成功失败提示
	CCSize size = this->getContentSize();
	if (m_lGetScore[GetMeChairID()] > 0)
	{
		CCSprite * pWinInfo = NULL;
		if (m_pTipsLayer && m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Ox_Bg_Win_Flag, pWinInfo))
		{
			pWinInfo->setPosition(ccp(size.width /2 , size.height / 2));
			pWinInfo->setScale(0.5f);
			CCAction * pAction = CCEaseOut::create((CCActionInterval *)CCSequence::create(
				CCScaleTo::create(2.0f, 1.0f), CCDelayTime::create(0.5f),
				CCCallFunc::create(pWinInfo, callfunc_selector(CCSprite::removeFromParent)), NULL), 3.0f);
			pWinInfo->runAction(pAction);
			m_pTipsLayer->addChild(pWinInfo, 100);
		}
		PlaySound(Ox_Sound_Game_Win);
	}
	else
	{
		CCSprite * pLostInfo = NULL;
		if (m_pTipsLayer && m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Ox_Bg_Lose_Flag, pLostInfo))
		{
			pLostInfo->setPosition(ccp(size.width /2 , size.height / 2 + 100));
			pLostInfo->setScale(1.5f);
			CCAction * pAction = CCEaseOut::create((CCActionInterval *)CCSequence::create(CCSpawn::create(
				CCScaleTo::create(2.0f, 1.0f), CCMoveBy::create(2.0f, ccp(0, -100)), NULL), CCDelayTime::create(0.5f),
				CCCallFunc::create(pLostInfo, callfunc_selector(CCSprite::removeFromParent)), NULL), 3.0f);
			pLostInfo->runAction(pAction);
			m_pTipsLayer->addChild(pLostInfo, 100);
		}
		PlaySound(Ox_Sound_Game_Lost);
	}

	//显示彩金动画
	BYTE cbWinsUserCount = 0;
	for (BYTE i = 0; i < GAME_PLAYER; ++i)
	{
		if (!m_bWinnings[i])
			continue;

		if(m_pJettonLayer)
			m_pJettonLayer->GiveWinningsJetton(i, Max_Jetton_Count / 2);

		CCLabelTTF * pWinsInfo = NULL;
		if ((pWinsInfo = CCLabelTTF::create("", "黑体", 30)) == NULL)
			continue;

		pWinsInfo->setColor(ccc3(255, 0, 0));
		pWinsInfo->setAnchorPoint(ccp(0.5, 0.5));
		pWinsInfo->setPosition(ccp(size.width / 2 - 100, size.height / 2 + 100 - cbWinsUserCount * 45));
		pWinsInfo->setVisible(false);

		char szTemp[100];
		memset(szTemp, 0, sizeof(szTemp));
		snprintf(szTemp, 100, "恭喜玩家[ %s ]获得彩金！", m_szWinsName[i].c_str());
		std::string testSt = CStringHelper::GB2312ToUTF8(szTemp);
		pWinsInfo->setString(CStringHelper::GB2312ToUTF8(szTemp).c_str());

		CCAction * pAction = CCSequence::create(CCDelayTime::create(0.5f * cbWinsUserCount), CCShow::create(), CCMoveBy::create(1.0f, ccp(200, 0)),
			CCDelayTime::create(2.0f), CCCallFunc::create(pWinsInfo, callfunc_selector(CCSprite::removeFromParent)), NULL);
		pWinsInfo->runAction(pAction);

		if (m_pTipsLayer)
			m_pTipsLayer->addChild(pWinsInfo, 60);

		++cbWinsUserCount;
	}

	if (cbWinsUserCount > 0)
	{
		CCSprite * pQianDai = NULL;
		if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Ox_Bg_Qian_Dai, pQianDai))
		{
			pQianDai->setPosition(ccp(size.width / 2, size.height / 2 + 100));
			pQianDai->setScale(0.5f);
			CCAction * pAction = CCSequence::create(CCSpawn::create(CCScaleTo::create(1.5f, 1.0f), CCMoveBy::create(1.5f, ccp(0, -100)), NULL),
				CCDelayTime::create(2.0f), CCCallFunc::create(pQianDai, callfunc_selector(CCSprite::removeFromParent)), NULL);
			pQianDai->runAction(pAction);
			if (m_pTipsLayer)
				m_pTipsLayer->addChild(pQianDai, 50);
		}
	}

	return ;
}

void COxCJ4GameLayer::BankerJettonChangeStep(float dt)
{
	if (m_nGameStatus != GS_TK_FREE)
		return ;

	if (m_pJettonLayer == NULL)
	{
		m_bCanBegin = true;
		OnSubGameFree(0.0f);
		return ;
	}

	//处理庄家持有注码
	LONGLONG lMaxCount = 0;
	LONGLONG lJetUnitValue = MAX(1, m_lJetUnitValue);
	for (WORD i = 0; i < GAME_PLAYER; ++i)
	{
		if (i == m_wCurrentBanker || m_lUserJetton[i] <= 0 || m_lGetScore[i] <= 0)
			continue;

		LONGLONG lCount = MAX(1, m_lGetScore[i] / lJetUnitValue);
		if (lCount > Max_Jetton_Count)
			lCount = Max_Jetton_Count;

		m_pJettonLayer->GiveHoldJetton(m_wCurrentBanker, i, static_cast<BYTE>(lCount));

		if (lCount > lMaxCount)
			lMaxCount = lCount;
	}

	if (lMaxCount == 0)
		scheduleOnce(schedule_selector(COxCJ4GameLayer::ShowAllUserGetScore), 0.2f);
	else
		scheduleOnce(schedule_selector(COxCJ4GameLayer::ShowAllUserGetScore), lMaxCount * 0.05f + 0.5f);

	return ;
}

void COxCJ4GameLayer::ShowAllUserGetScore(float dt)
{
	if (m_nGameStatus != GS_TK_FREE)
		return ;

	if (m_pTipsLayer == NULL)
	{
		m_bCanBegin = true;
		OnSubGameFree(0.0f);
		return ;
	}

	CCPoint pAnchor[4];
	pAnchor[0] = ccp(0.5, 0.5);
	pAnchor[1] = ccp(0, 0.5);
	pAnchor[2] = ccp(0, 0.5);
	pAnchor[3] = ccp(1, 0.5);

	for (WORD i = 0; i < GAME_PLAYER; ++i)
	{
		if (m_lUserJetton[i] <= 0 && i != m_wCurrentBanker)
			continue;

		WORD wPos = GetUserChairID(i);
		int nNumId = (m_lGetScore[i] > 0) ? Ox_Font_Score_Num_Add : Ox_Font_Score_Num_Dec;

		CCLabelAtlas * labelScore = NULL;
		if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, nNumId, labelScore))
		{
			labelScore->setPosition(m_GetScorePos[wPos]);
			labelScore->setAnchorPoint(pAnchor[wPos]);

			char szUserScore[64] = {0};
			if (m_lGetScore[i] > 0)
				snprintf(szUserScore, 64, "+%lld", m_lGetScore[i]);
			else
				snprintf(szUserScore, 64, "%lld", m_lGetScore[i]);
			labelScore->setString(szUserScore);

			labelScore->runAction(CCSequence::create(CCMoveBy::create(1.0f, ccp(0, 60)),
				CCCallFunc::create(labelScore, callfunc_selector(CCNode::removeFromParent)), NULL));
			m_pTipsLayer->addChild(labelScore);
		}
	}

	scheduleOnce(schedule_selector(COxCJ4GameLayer::OnSubGameFree), 1.5f);

	return ;
}

void COxCJ4GameLayer::PlaySendSound(float dt)
{
	PlaySound(Ox_Sound_Send_Card);
}

int COxCJ4GameLayer::GetOxTypeResID(BYTE cbOxType)
{
	int nResID = 0;
	switch(cbOxType)
	{
	case CT_NIU0:
		nResID = Ox_Bg_Result_Ox0;
		break;
	case CT_NIU1:
		nResID = Ox_Bg_Result_Ox1;
		break;
	case CT_NIU2:
		nResID = Ox_Bg_Result_Ox2;
		break;
	case CT_NIU3:
		nResID = Ox_Bg_Result_Ox3;
		break;
	case CT_NIU4:
		nResID = Ox_Bg_Result_Ox4;
		break;
	case CT_NIU5:
		nResID = Ox_Bg_Result_Ox5;
		break;
	case CT_NIU6:
		nResID = Ox_Bg_Result_Ox6;
		break;
	case CT_NIU7:
		nResID = Ox_Bg_Result_Ox7;
		break;
	case CT_NIU8:
		nResID = Ox_Bg_Result_Ox8;
		break;
	case CT_NIU9:
		nResID = Ox_Bg_Result_Ox9;
		break;
	case CT_NIUNIU:
	case CT_NIUWH_TWODOUBLE:
	case CT_NIUWH_THREE:
	case CT_NIUWH_HULU:
	case CT_NIUWH_TWOGUI:
	case CT_NIUWH_FOUR:
		nResID = Ox_Bg_Result_Ox10;
		break;
	default:
		CC_ASSERT(false);
		return 0x0fffffff;
	}

	return nResID;
}

int COxCJ4GameLayer::GetOxSoundResID(BYTE cbOxType, BYTE cbUserGender)
{
	int nResID = 0;
	switch(cbOxType)
	{
	case CT_NIU0:
		nResID = (cbUserGender != 0) ? Ox_Sound_Wom_Niu0: Ox_Sound_Man_Niu0;
		break;
	case CT_NIU1:
		nResID = (cbUserGender != 0) ? Ox_Sound_Wom_Niu1: Ox_Sound_Man_Niu1;
		break;
	case CT_NIU2:
		nResID = (cbUserGender != 0) ? Ox_Sound_Wom_Niu2: Ox_Sound_Man_Niu2;
		break;
	case CT_NIU3:
		nResID = (cbUserGender != 0) ? Ox_Sound_Wom_Niu3: Ox_Sound_Man_Niu3;
		break;
	case CT_NIU4:
		nResID = (cbUserGender != 0) ? Ox_Sound_Wom_Niu4: Ox_Sound_Man_Niu4;
		break;
	case CT_NIU5:
		nResID = (cbUserGender != 0) ? Ox_Sound_Wom_Niu5: Ox_Sound_Man_Niu5;
		break;
	case CT_NIU6:
		nResID = (cbUserGender != 0) ? Ox_Sound_Wom_Niu6: Ox_Sound_Man_Niu6;
		break;
	case CT_NIU7:
		nResID = (cbUserGender != 0) ? Ox_Sound_Wom_Niu7: Ox_Sound_Man_Niu7;
		break;
	case CT_NIU8:
		nResID = (cbUserGender != 0) ? Ox_Sound_Wom_Niu8: Ox_Sound_Man_Niu8;
		break;
	case CT_NIU9:
		nResID = (cbUserGender != 0) ? Ox_Sound_Wom_Niu9: Ox_Sound_Man_Niu9;
		break;
	case CT_NIUNIU:
	case CT_NIUWH_TWODOUBLE:
	case CT_NIUWH_THREE:
	case CT_NIUWH_HULU:
	case CT_NIUWH_TWOGUI:
	case CT_NIUWH_FOUR:
		nResID = (cbUserGender != 0) ? Ox_Sound_Wom_NiuNiu: Ox_Sound_Man_NiuNiu;
		break;
	default:
		CC_ASSERT(false);
		return 0x0fffffff;
	}

	return nResID;
}

template<class T> void COxCJ4GameLayer::RandValue(T data[], int nCount)
{
	if (data == NULL || nCount <= 0)
	{
		CC_ASSERT(false);
		return ;
	}

	T * TempData = new T[nCount];
	memset(TempData, 0, sizeof(TempData[0]) * nCount);
	memcpy(TempData, data, sizeof(data[0]) * nCount);

	int nIndex = 0;
	while (nIndex < nCount)
	{
		int nPos = rand() % (nCount - nIndex);
		data[nIndex++] = TempData[nPos];
		TempData[nPos] = TempData[nCount - nIndex];
	}

	delete [] TempData;
	TempData = NULL;

	return ;
}

}
