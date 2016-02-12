#include "stdafx.h"
#include "OxCJ4QuitConfirm.h"
#include "LayerBase.h"
#include "ResManager.h"

namespace GameOxCJ4
{

COxCJ4QuitConfirm * COxCJ4QuitConfirm::create(COxCJ4GameLayer * pGameLayer, CResManager * pResManager)
{
	COxCJ4QuitConfirm * pRet = new COxCJ4QuitConfirm(pGameLayer, pResManager);

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

	return NULL;
}

COxCJ4QuitConfirm::COxCJ4QuitConfirm(COxCJ4GameLayer * pGameLayer, CResManager * pResManager)
	:CLayerBase(pResManager)
	,m_pGameLayer(pGameLayer)
	,m_pResManager(pResManager)
	,m_pBtnQuit(NULL)
	,m_pBtnCancel(NULL)
{

}

COxCJ4QuitConfirm::~COxCJ4QuitConfirm()
{

}

bool COxCJ4QuitConfirm::init()
{
	if (!CLayerBase::init())
		return false;

	CCSize size = this->getContentSize();
	CCPoint mPoint = ccp(size.width / 2, size.height / 2);

	CCSprite * pBg = NULL;
	if (!m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Ox_Bg_Confirm, pBg))
		return false;
	pBg->setPosition(ccp(mPoint.x, mPoint.y));
	addToRoot(pBg);

	m_pBtnQuit = createMenuItemSpriteByResID(Ox_Bt_Quit_N, Ox_Bt_Quit_C);
	if (m_pBtnQuit)
	{
		m_pBtnQuit->setTarget(this, menu_selector(COxCJ4QuitConfirm::OnTouchQuit));
		m_pBtnQuit->setPosition(ccp(mPoint.x- 100, mPoint.y - 70));
	}

	m_pBtnCancel = createMenuItemSpriteByResID(Ox_Bt_Cancel_N, Ox_Bt_Cancel_C);
	if (m_pBtnCancel)
	{
		m_pBtnCancel->setTarget(this, menu_selector(COxCJ4QuitConfirm::OnTouchCancel));
		m_pBtnCancel->setPosition(ccp(mPoint.x + 100, mPoint.y -70));
	}

	CCMenu * pMenu = CCMenu::create(m_pBtnQuit, m_pBtnCancel,  NULL);
	pMenu->setPosition(CCPointZero);
	addToRoot(pMenu);

	return true;
}

void COxCJ4QuitConfirm::OnTouchQuit(CCObject* pObject)
{
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	setVisible(false);

	if (!m_pGameLayer->GetGameDirector()->m_bUseIPC)
		m_pGameLayer->GetGameDirector()->SendRoomData(MDM_GR_USER, SUB_GR_USER_LEFT_GAME_REQ, NULL, 0);

	m_pGameLayer->GetGameDirector()->ReturnFromGame(eLobbyType_Table, false);
}

void COxCJ4QuitConfirm::OnTouchCancel(CCObject* pObject)
{
	this->setVisible(false);
}

}

