#include "stdafx.h"
#include "OxCJ4TouchCard.h"
#include "OxCJ4GameLayer.h"
#include "CommonDef.h"
#include "ResManager.h"

namespace GameOxCJ4
{

COxCJ4TouchCard * COxCJ4TouchCard::createWithCfgID(COxCJ4GameLayer * pGameLayer, CResManager * pResManager, int nType, int nCfgId)
{
	COxCJ4TouchCard * pRet = new COxCJ4TouchCard();
	if (pRet && pRet->initWithCfgID(pGameLayer, pResManager, nType, nCfgId))
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

COxCJ4TouchCard::COxCJ4TouchCard()
	:m_pGameLayer(NULL)
	,m_bCanTouch(false)
	,m_bIsJump(false)
	,m_bTouchEnd(true)
{

}

COxCJ4TouchCard::~COxCJ4TouchCard()
{

}

bool COxCJ4TouchCard::initWithCfgID(COxCJ4GameLayer * pGameLayer, CResManager * pResManager, int nType, int nCfgId)
{
	m_pGameLayer = pGameLayer;

	CGameSpriteCfg * pCfg = pResManager->GetConfigHelper()->GetSpriteConfig(nType, nCfgId);
	if (pCfg == NULL)
		return false;

	if (pCfg->m_sPath.length() == 0)
		return CCSprite::init();
	else if (pCfg->m_bPack)
	{
		CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pCfg->m_sPath.c_str());
		if (!pFrame)
			return false;

		return CCSprite::initWithSpriteFrame(pFrame);
	}
	else
		return CCSprite::initWithFile(pCfg->m_sPath.c_str());

	return false;
}

void COxCJ4TouchCard::onEnter()
{
	CCSprite::onEnter();
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void COxCJ4TouchCard::onExit()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
	CCSprite::onExit();
}

bool COxCJ4TouchCard::ccTouchBegan(CCTouch * touch, CCEvent* event)
{
	if (m_bTouchEnd == false)
		return false;
	if (!containsTouchLocation(touch))
		return false;
	if (m_bCanTouch == false)
		return false;

	m_bTouchEnd = false;
	return true;
}

void COxCJ4TouchCard::ccTouchMoved(CCTouch* touch, CCEvent* event)
{

}

void COxCJ4TouchCard::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	m_bTouchEnd = true;

	if (!containsTouchLocation(touch))
		return;

	if (m_bIsJump)
		setPositionY(getPositionY() - Pos_TouchCardJump);
	else
		setPositionY(getPositionY() + Pos_TouchCardJump);

	m_bIsJump = !m_bIsJump;
	m_pGameLayer->UpdateOxType();
}

bool COxCJ4TouchCard::containsTouchLocation(CCTouch * touch)
{
	CCSize cardSize = this->getContentSize();
	CCRect cardRect(0.0f, 0.0f, cardSize.width, cardSize.height);

	return cardRect.containsPoint(convertTouchToNodeSpace(touch));
}

void COxCJ4TouchCard::SetJump(bool bCanJump)
{
	if (bCanJump == m_bIsJump || m_bTouchEnd == false)
		return ;

	if (bCanJump)
		setPositionY(getPositionY() + Pos_TouchCardJump);
	else
		setPositionY(getPositionY() - Pos_TouchCardJump);

	m_bIsJump = bCanJump;
	m_pGameLayer->UpdateOxType();
	return ;
}

}
