#include "stdafx.h"
#include "LayerBase.h"
#include "CMD_OxCJ4.h"

namespace GameOxCJ4
{

CLayerBase::CLayerBase(CResourceManager* m_pResManager)
	: m_pResManager(m_pResManager)
	, m_pRootNode(NULL)
{
}

CLayerBase::~CLayerBase()
{

}

bool CLayerBase::init()
{
	if (!CCLayer::init())
		return false;

	m_pRootNode = CCNode::create();
	CCSize contentSize = this->getContentSize();
	m_pRootNode->setPosition(CCPointZero);
	addChild(m_pRootNode);

	return true;
}

WORD CLayerBase::ServerTableIndexToLocal(WORD wDesChair, WORD wMyChair)
{
	//参数校验
	if (wDesChair < 0 || wDesChair >= GAME_PLAYER || wMyChair < 0 || wMyChair >= GAME_PLAYER)
	{
		ERROR_CHECK;
		CC_ASSERT(false);
		return INVALID_CHAIR;
	}

	return (wDesChair + GAME_PLAYER - wMyChair) % GAME_PLAYER;
}

CCSprite* CLayerBase::createSpriteByResID(int nResID)
{
	CCSprite* pSprite = NULL;
	m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, nResID, pSprite);
	return pSprite;
}

CCControlButton* CLayerBase::createControlButtonByResID(int nNormalResID, int nSelectResID)
{
	CCControlButton* pButton = NULL;

	do
	{
		CCLabelTTF *titleButton = CCLabelTTF::create("", "Arial", 30);
		if (NULL == titleButton)
		{
			ERROR_CHECK;
			break;
		}

		CCScale9Sprite *backgroundButton = NULL;

		if (!m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, nNormalResID, backgroundButton))
		{
			ERROR_CHECK;
			break;
		}
		pButton = CCControlButton::create(titleButton, backgroundButton);
		if (NULL == pButton)
		{
			ERROR_CHECK;
			break;
		}

		CCScale9Sprite *backgroundHighlightedButton = NULL;

		if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, nSelectResID, backgroundHighlightedButton))
		{
			pButton->setBackgroundSpriteForState(backgroundHighlightedButton, CCControlStateHighlighted);
		}
	}while(false);

	return pButton;
}

CCMenuItemSprite* CLayerBase::createMenuItemSpriteByResID(int nNormalResID, int nSelectResID)
{
	CCSprite* pNormalSprite = createSpriteByResID(nNormalResID);
	CCSprite* pSelectSprite = createSpriteByResID(nSelectResID);

	return CCMenuItemSprite::create(pNormalSprite, pSelectSprite);
}

CCMenuItemSprite* CLayerBase::createMenuItemSpriteByResID(int nNormalResID, int nSelectResID, int nDisabledResID)
{
	CCSprite* pNormalSprite = createSpriteByResID(nNormalResID);
	CCSprite* pSelectSprite = createSpriteByResID(nSelectResID);
	CCSprite* pDisabledSprite = createSpriteByResID(nDisabledResID);

	return CCMenuItemSprite::create(pNormalSprite, pSelectSprite, pDisabledSprite, NULL, NULL);
}

void CLayerBase::addToRoot(CCNode* pNode, int zOrder)
{
	if (pNode == NULL)
		return;

	m_pRootNode->addChild(pNode, zOrder);
}

void CLayerBase::removeFromRoot(CCNode* pNode)
{
	m_pRootNode->removeChild(pNode);
}

}
