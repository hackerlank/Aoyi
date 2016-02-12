#include "stdafx.h"
#include "PullMachResManager.h"

namespace GamePullMach777
{
CPMResManager::CPMResManager(CGameDirector* pDirector)
	: CResourceManager(pDirector)
	, m_nSpriteIDSeed(1)
{

}

CPMResManager::~CPMResManager()
{

}

CConfigHelper* CPMResManager::CreateConfigHelper()
{
	if (NULL == m_pConfigHelp)
	{
		m_pConfigHelp = new CConfigHelper();
	}
	else
	{
		ERROR_CHECK;
	}

	return m_pConfigHelp;
}

CConfigHelper* CPMResManager::GetConfigHelper()
{
	return m_pConfigHelp;
}

void CPMResManager::Clear(bool bClearLobby)
{
	CResourceManager::Clear(bClearLobby);
}

void CPMResManager::LoadConfigFromFile()
{
	CResourceManager::LoadConfigFromFile();

}

CGameSprite* CPMResManager::GenerateGameSprite(unsigned int nTypeID)
{
	unsigned int nUserID = 0;
	nUserID = m_pGameDirector->m_nMeUserID;

	unsigned int nSpriteID = m_nSpriteIDSeed++;
	return GenerateGameSprite(nTypeID, nSpriteID, nUserID);
}


CGameSprite* CPMResManager::GenerateGameSprite(unsigned int nTypeID, unsigned nSpriteID, unsigned nCreatorID)
{
	if (0 == nSpriteID)
	{
		return NULL;
	}

	CGameSprite* pRet = NULL;

	switch (nTypeID)
	{
	case eSpriteType_Base:
		{
			pRet = new CGameSprite(nTypeID, this);
		}
		break;
	case eSpriteType_Cfg:
		{
			pRet = NULL;
		}
		break;
	default:
		{
			ERROR_CHECK;
			pRet = NULL;
		}
		break;
	}

	if (NULL == pRet)
	{
		return NULL;
	}

	//pRet->SetResourceManager(this);
	//pRet->m_nSpriteTypeID = nTypeID;

	//设置ID
	pRet->SetSpriteID(nSpriteID);
	pRet->SetCreatorID(nCreatorID);

	return pRet;
}


CCControlButton* CPMResManager::CreateControlButton(int nNormalSpriteID, int nHighlightedSpriteID, int nDisabledSpriteID)
{
	CCControlButton* pButton = NULL;

	do
	{
		CCScale9Sprite *backgroundButton = NULL;
		if (!GenerateNodeByCfgID(eSpriteType_Base, nNormalSpriteID, backgroundButton))
		{
			ERROR_CHECK;
			break;
		}

		pButton = CCControlButton::create(backgroundButton);
		if (NULL == pButton)
		{
			ERROR_CHECK;
			break;
		}

		CCScale9Sprite *pHighlightedSprite = NULL;
		if (GenerateNodeByCfgID(eSpriteType_Base, nHighlightedSpriteID, pHighlightedSprite))
		{
			pButton->setBackgroundSpriteForState(pHighlightedSprite, CCControlStateHighlighted);
		}

		if (nDisabledSpriteID != -1)
		{
			CCScale9Sprite *pDisabledSprite = NULL;
			if (GenerateNodeByCfgID(eSpriteType_Base, nDisabledSpriteID, pDisabledSprite))
			{
				pButton->setBackgroundSpriteForState(pDisabledSprite, CCControlStateDisabled);
			}
		}

		pButton->setAdjustBackgroundImage(false);
		pButton->setZoomOnTouchDown(false);
	}while(false);

	return pButton;
}

const std::vector<tagAudioCfg*>& CPMResManager::GetAudioCfgs()
{
	return m_pConfigHelp->m_vAudioCfgs;
}

const char * CPMResManager::getAnimationNameById(enGameAnimationId id)
{
	switch (id)
	{
	case GameAni_BoxRed7:
		return "BoxRed7";
	case GameAni_Red7Swing:
		return "Red7Swing";
	case GameAni_BoxLight:
		return "BlueLight";
	case GameAni_GirlFly:
		return "GirlFly";

	default:
		CC_ASSERT(false);
		return "";
	}
}

CCSpriteFrame * CPMResManager::getSpriteFrameByCfgId(int cfgid)
{
	CGameSpriteCfg* pCfg = GetConfigHelper()->GetSpriteConfig(eSpriteType_Base, cfgid);
	if (pCfg == NULL)
	{
		CC_ASSERT(false);
		return NULL;
	}

	return CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pCfg->m_sPath.c_str());
}

void CPMResManager::resetSpriteFrameTexture(CCSprite * pSprite, int cfgid)
{
	//参数校验
	CC_ASSERT(pSprite != NULL);
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

void CPMResManager::resetSpriteTexture(CCSprite * pSprite, int cfgid)
{
	//参数校验
	CC_ASSERT(pSprite != NULL);
	if (pSprite == NULL)
		return ;

	//取得替换纹理
	CGameSpriteCfg* pCfg = GetConfigHelper()->GetSpriteConfig(eSpriteType_Base, cfgid);
	int nCfgType = 1;
	if (pCfg == NULL)
	{
		pCfg = GetConfigHelper()->GetSpriteConfig(eSpriteType_Cfg, cfgid);
		nCfgType = 2;
		if (pCfg == NULL)
		{
			CC_ASSERT(false);
			return ;
		}
	}

	CCTexture2D * pTexture = CCTextureCache::sharedTextureCache()->addImage(pCfg->m_sPath.c_str());
	if (pTexture == NULL)
	{
		if (nCfgType == 1)
		{
			pCfg = GetConfigHelper()->GetSpriteConfig(eSpriteType_Cfg, cfgid);
			if (pCfg != NULL)
				pTexture = CCTextureCache::sharedTextureCache()->addImage(pCfg->m_sPath.c_str());
		}
		if (pTexture == NULL)
		{
			CC_ASSERT(false);
			return ;
		}
	}

	pSprite->setTexture(pTexture);
	CCSize size = pTexture->getContentSize();
	CCRect rect(0.0f, 0.0f, size.width, size.height);
	pSprite->setTextureRect(rect);
	return ;
}

}