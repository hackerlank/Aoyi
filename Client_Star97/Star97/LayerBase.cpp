#include "stdafx.h"
#include "LayerBase.h"
#include "CMD_Game.h"
#include "ResourceManager.h"
#include "ResManager.h"

namespace GameStar97
{

CLayerBase::CLayerBase(CResourceManager* pResManager)
	: m_pResManager(pResManager)
{
}

CLayerBase::~CLayerBase()
{

}

CCSpriteFrame * CLayerBase::getSpriteFrameByCfgId(int cfgid)
{
	CGameSpriteCfg* pCfg = m_pResManager->GetConfigHelper()->GetSpriteConfig(eSpriteType_Base, cfgid);
	if (pCfg == NULL)
	{
		CC_ASSERT(false);
		return NULL;
	}

	return CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pCfg->m_sPath.c_str());
}

void CLayerBase::resetSpriteTexture(CCSprite * pSprite, int cfgid)
{
	//参数校验
	if (pSprite == NULL)
		return ;

	//替换纹理
	CCSpriteFrame * pSpriteFrame = getSpriteFrameByCfgId(cfgid);
	if (pSpriteFrame == NULL)
	{
		CC_ASSERT(false);
		return ;
	}

	pSprite->setDisplayFrame(pSpriteFrame);
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

}
