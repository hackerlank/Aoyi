#include "stdafx.h"
#include "OxCJ4UILayer.h"

namespace GameOxCJ4
{

COxCJ4UILayer * COxCJ4UILayer::create(COxCJ4GameLayer * pGameLayer, CResManager * pResManager)
{
	COxCJ4UILayer * pRet = new COxCJ4UILayer(pGameLayer, pResManager);

	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

COxCJ4UILayer::COxCJ4UILayer(COxCJ4GameLayer * pGameLayer, CResManager * pResManager)
	:CLayerBase(pResManager)
	,m_pGameLayer(pGameLayer)
	,m_pResManager(pResManager)
	,m_pBtnStart(NULL)
	,m_pBtnCallBanker(NULL)
	,m_pBtnNoCall(NULL)
	,m_nJetBeginTag(0)
	,m_pBtnSortCard(NULL)
	,m_pBtnShowCard(NULL)
	,m_pBtnTiShi(NULL)
	,m_pBtnTuoGuan(NULL)
	,m_pBtnNoTuoGuan(NULL)
	,m_pBtnOut(NULL)
	,m_pBtnOption(NULL)
{
	memset(m_pBtnJetton, 0, sizeof(m_pBtnJetton));
	m_MenuArray.clear();
}

COxCJ4UILayer::~COxCJ4UILayer()
{
}

bool COxCJ4UILayer::init()
{
	if (!CLayerBase::init())
		return false;

	CCSize size = this->getContentSize();
	CCPoint midPoint = ccp(size.width / 2, size.height / 2);
	CCPoint bkPoint[4];
	bkPoint[0] = ccp(midPoint.x - 475,midPoint.y - 288);
	bkPoint[1] = ccp(midPoint.x - 560, midPoint.y);
	bkPoint[2] = ccp(midPoint.x, midPoint.y + 280);
	bkPoint[3] = ccp(midPoint.x + 553, midPoint.y);

	InitMenu();
	RestAllMenu();

	return true;
}

void COxCJ4UILayer::InitMenu()
{
	m_MenuArray.clear();
	//CCPoint mPoint = m_pRootNode->getPosition();
	CCSize size = this->getContentSize();
	CCPoint mPoint = ccp(size.width / 2, size.height / 2);
	float fMenuY = mPoint.y - 100.0f;

	m_pBtnStart = createMenuItemSpriteByResID(Ox_Bt_Zhun_Bei_N, Ox_Bt_Zhun_Bei_C);
	if (m_pBtnStart)
	{
		m_pBtnStart->setTarget(this, menu_selector(COxCJ4UILayer::OnTouchStart));
		m_pBtnStart->setPosition(ccp(mPoint.x, fMenuY));
		m_MenuArray[Menu_Item_ID_Start] = m_pBtnStart;
	}

	m_pBtnCallBanker = createMenuItemSpriteByResID(Ox_Bt_Call_Banker_N, Ox_Bt_Call_Banker_C);
	if (m_pBtnCallBanker)
	{
		m_pBtnCallBanker->setTarget(this, menu_selector(COxCJ4UILayer::OnTouchCallBanker));
		m_pBtnCallBanker->setPosition(ccp(mPoint.x - 100, fMenuY));
		m_MenuArray[Menu_Item_ID_CallBanker] = m_pBtnCallBanker;
	}

	m_pBtnNoCall = createMenuItemSpriteByResID(Ox_Bt_Bu_Jiao_N, Ox_Bt_Bu_Jiao_C);
	if (m_pBtnNoCall)
	{
		m_pBtnNoCall->setTarget(this, menu_selector(COxCJ4UILayer::OnTouchNoCall));
		m_pBtnNoCall->setPosition(ccp(mPoint.x + 100, fMenuY));
		m_MenuArray[Menu_Item_ID_NoCall] = m_pBtnNoCall;
	}

	m_nJetBeginTag = 100;
	for (BYTE i = 0; i < CountArray(m_pBtnJetton); ++i)
	{
		m_pBtnJetton[i] = createMenuItemSpriteByResID(Ox_Bt_Jetton_Bk_N, Ox_Bt_Jetton_Bk_C, Ox_Bt_Jetton_Bk_D);
		if (m_pBtnJetton[i])
		{
			m_pBtnJetton[i]->setTarget(this, menu_selector(COxCJ4UILayer::OnTouchJetton));
			m_pBtnJetton[i]->setTag(m_nJetBeginTag + i);
			m_pBtnJetton[i]->setPosition(ccp(mPoint.x - 300 + i * 200, fMenuY));

			if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Ox_Font_Jetton_Value, m_pJettonValue[i]))
			{
				CCSize btnSize = m_pBtnJetton[i]->getContentSize();
				m_pJettonValue[i]->setAnchorPoint(ccp(0.5, 0.5));
				m_pJettonValue[i]->setPosition(btnSize.width / 2, btnSize.height / 2);
				m_pBtnJetton[i]->addChild(m_pJettonValue[i]);
			}
		}
	}
	m_MenuArray[Menu_Item_ID_Jetton0] = m_pBtnJetton[0];
	m_MenuArray[Menu_Item_ID_Jetton1] = m_pBtnJetton[1];
	m_MenuArray[Menu_Item_ID_Jetton2] = m_pBtnJetton[2];
	m_MenuArray[Menu_Item_ID_Jetton3] = m_pBtnJetton[3];

	m_pBtnSortCard = createMenuItemSpriteByResID(Ox_Bt_Pai_Xu_N, Ox_Bt_Pai_Xu_C);
	if (m_pBtnSortCard)
	{
		m_pBtnSortCard->setTarget(this, menu_selector(COxCJ4UILayer::OnTouchSortCard));
		m_pBtnSortCard->setPosition(ccp(mPoint.x - 190, fMenuY));
		m_MenuArray[Menu_Item_ID_SortCard] = m_pBtnSortCard;
	}

	m_pBtnShowCard = createMenuItemSpriteByResID(Ox_Bt_Show_Card_N, Ox_Bt_Show_Card_C, Ox_Bt_Show_Card_D);
	if (m_pBtnShowCard)
	{
		m_pBtnShowCard->setTarget(this, menu_selector(COxCJ4UILayer::OnTouchShowCard));
		m_pBtnShowCard->setPosition(ccp(mPoint.x, fMenuY));
		m_MenuArray[Menu_Item_ID_ShowCard] = m_pBtnShowCard;
	}

	m_pBtnTiShi = createMenuItemSpriteByResID(Ox_Bt_Ti_Shi_N, Ox_Bt_Ti_Shi_C);
	if (m_pBtnTiShi)
	{
		m_pBtnTiShi->setTarget(this, menu_selector(COxCJ4UILayer::OnTouchTiShi));
		m_pBtnTiShi->setPosition(ccp(mPoint.x + 190, fMenuY));
		m_MenuArray[Menu_Item_ID_TiShi] = m_pBtnTiShi;
	}

	m_pBtnTuoGuan = createMenuItemSpriteByResID(Ox_Bt_Tuo_Guan_N, Ox_Bt_Tuo_Guan_C);
	if (m_pBtnTuoGuan)
	{
		m_pBtnTuoGuan->setTarget(this, menu_selector(COxCJ4UILayer::OnTouchTuoGuan));
		m_pBtnTuoGuan->setPosition(ccp(mPoint.x + 555, mPoint.y - 320));
		m_MenuArray[Menu_Item_ID_TuoGuan] = m_pBtnTuoGuan;
	}

	m_pBtnNoTuoGuan = createMenuItemSpriteByResID(Ox_Bt_Qu_Xiao_TG_N, Ox_Bt_Qu_Xiao_TG_C);
	if (m_pBtnNoTuoGuan)
	{
		m_pBtnNoTuoGuan->setTarget(this, menu_selector(COxCJ4UILayer::OnTouchNoTuoGuan));
		m_pBtnNoTuoGuan->setPosition(ccp(mPoint.x + 555, mPoint.y - 320));
		m_MenuArray[Menu_Item_ID_NoTuoGuan] = m_pBtnNoTuoGuan;
	}

	m_pBtnOut = createMenuItemSpriteByResID(Ox_Bt_Out_N, Ox_Bt_Out_C);
	if (m_pBtnOut)
	{
		m_pBtnOut->setTarget(this, menu_selector(COxCJ4UILayer::OnTouchOut));
		m_pBtnOut->setAnchorPoint(ccp(0, 1));
		m_pBtnOut->setPosition(ccp(0, size.height));
		//m_MenuArray[Menu_Item_ID_Out] = m_pBtnOut;
	}

	m_pBtnOption = createMenuItemSpriteByResID(Ox_Bt_Option_N, Ox_Bt_Option_C);
	if (m_pBtnOption)
	{
		m_pBtnOption->setTarget(this, menu_selector(COxCJ4UILayer::OnTouchOption));
		m_pBtnOption->setAnchorPoint(ccp(0, 1));
		m_pBtnOption->setPosition(ccp(m_pBtnOut->getContentSize().width, size.height));
		//m_MenuArray[Menu_Item_ID_Out] = m_pBtnOut;
	}

	CCMenu * pMenu = CCMenu::create(m_pBtnStart, m_pBtnCallBanker, m_pBtnNoCall, m_pBtnSortCard,
		m_pBtnShowCard, m_pBtnTiShi, m_pBtnTuoGuan, m_pBtnNoTuoGuan, m_pBtnOut, m_pBtnOption, NULL);
	for (BYTE i = 0; i < CountArray(m_pBtnJetton); ++i)
		pMenu->addChild(m_pBtnJetton[i]);

	pMenu->setPosition(CCPointZero);
	addToRoot(pMenu);
}

void COxCJ4UILayer::onEnter()
{
	CLayerBase::onEnter();
}

void COxCJ4UILayer::onExit()
{
	CLayerBase::onExit();
}

bool COxCJ4UILayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return false;
}

CCMenuItem * COxCJ4UILayer::GetMItemById(int nItemId)
{
	std::map<int, CCMenuItem *>::const_iterator iter = m_MenuArray.find(nItemId);

	if (iter != m_MenuArray.end())
		return iter->second;
	else
	{
		CC_ASSERT(false);
		return NULL;
	}

	return NULL;
}

void COxCJ4UILayer::RestAllMenu()
{
	for (std::map<int, CCMenuItem *>::iterator iter = m_MenuArray.begin(); iter != m_MenuArray.end(); ++iter)
	{
		iter->second->setVisible(false);
		iter->second->setEnabled(true);
	}
}

void COxCJ4UILayer::SetJettonValue(int nCount, LONGLONG lJetton[], bool bEnable[])
{
	if (nCount != CountArray(m_pBtnJetton))
	{
		CC_ASSERT(false);
		return ;
	}

	for (BYTE i = 0; i < nCount; ++i)
	{
		if (m_pJettonValue[i])
		{
			char szValue[64] = {0};
			snprintf(szValue, 64, "%lld", lJetton[i]);

			m_pJettonValue[i]->setString(szValue);
		}
		if(m_pBtnJetton[i])
		{
			m_pBtnJetton[i]->setEnabled(bEnable[i]);
		}
	}
}

void COxCJ4UILayer::OnTouchStart(CCObject* pObject)
{
	m_pBtnStart->setVisible(false);
	
	if (m_pGameLayer->GetGameStatus() == GS_TK_FREE)
		m_pGameLayer->SendUserReady();
}

void COxCJ4UILayer::OnTouchCallBanker(CCObject* pObject)
{
	m_pBtnCallBanker->setVisible(false);
	m_pBtnNoCall->setVisible(false);

	if (m_pGameLayer->GetGameStatus() != GS_TK_CALL)
		return ;

	CMD_C_CallBanker CallBanker;
	memset(&CallBanker, 0, sizeof(CallBanker));
	CallBanker.cbBanker = 1;

	m_pGameLayer->SendGameMessage(SUB_C_CALL_BANKER, &CallBanker, sizeof(CallBanker));
}

void COxCJ4UILayer::OnTouchNoCall(CCObject* pObject)
{
	m_pBtnCallBanker->setVisible(false);
	m_pBtnNoCall->setVisible(false);

	if (m_pGameLayer->GetGameStatus() != GS_TK_CALL)
		return ;

	CMD_C_CallBanker CallBanker;
	memset(&CallBanker, 0, sizeof(CallBanker));
	CallBanker.cbBanker = 0;

	m_pGameLayer->SendGameMessage(SUB_C_CALL_BANKER, &CallBanker, sizeof(CallBanker));
}

void COxCJ4UILayer::OnTouchJetton(CCObject* pObject)
{
	CCMenuItem * pItem = dynamic_cast<CCMenuItem *>(pObject);
	if (pItem == NULL)
		return ;

	int nTag = pItem->getTag();
	if (nTag < m_nJetBeginTag || nTag >= (int)(m_nJetBeginTag + CountArray(m_pBtnJetton)))
	{
		CC_ASSERT(false);
		return ;
	}

	for (BYTE i = 0; i < CountArray(m_pBtnJetton); ++i)
	{
		if (m_pBtnJetton[i])
			m_pBtnJetton[i]->setVisible(false);
	}

	nTag -= m_nJetBeginTag;
	if (m_pJettonValue[nTag] == NULL)
		return ;

	if (m_pGameLayer->GetGameStatus() != GS_TK_SCORE)
		return ;

	LONGLONG lScore = 0;
	sscanf(m_pJettonValue[nTag]->getString(), "%lld", &lScore);

	m_pGameLayer->MePlaceJetton(lScore);
}

void COxCJ4UILayer::OnTouchSortCard(CCObject* pObject)
{
	m_pGameLayer->SortHandCard();
}

void COxCJ4UILayer::OnTouchShowCard(CCObject* pObject)
{
	if (m_pGameLayer->ShowMyCard(true) == false)
		return ;

	m_pBtnSortCard->setVisible(false);
	m_pBtnShowCard->setVisible(false);
	m_pBtnTiShi->setVisible(false);
}

void COxCJ4UILayer::OnTouchTiShi(CCObject* pObject)
{
	m_pGameLayer->TeachMeOut();
}

void COxCJ4UILayer::OnTouchTuoGuan(CCObject* pObject)
{
	m_pGameLayer->AutoPlay(true);
	m_pBtnTuoGuan->setVisible(false);
	m_pBtnNoTuoGuan->setVisible(true);
}

void COxCJ4UILayer::OnTouchNoTuoGuan(CCObject* pObject)
{
	m_pGameLayer->AutoPlay(false);
	m_pBtnTuoGuan->setVisible(true);
	m_pBtnNoTuoGuan->setVisible(false);
}

void COxCJ4UILayer::OnTouchOut(CCObject* pObject)
{
	if (m_pGameLayer->GetGameStatus() == GS_TK_FREE)
	{
		SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		m_pGameLayer->GetGameDirector()->ReturnFromGame(eLobbyType_Table);
	}
	else
	{
		m_pGameLayer->QuitGame();
	}
}

void COxCJ4UILayer::OnTouchOption(CCObject* pObject)
{
	m_pGameLayer->GetGameDirector()->ShowGameOption();
}

WORD COxCJ4UILayer::GetUserChairID(WORD wChairID)
{
	WORD wMyChairID = m_pGameLayer->GetMeChairID();

	return ServerTableIndexToLocal(wChairID, wMyChairID);
}

}
