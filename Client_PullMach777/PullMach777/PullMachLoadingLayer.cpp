#include "stdafx.h"
#include "PullMachLoadingLayer.h"
#include "PullMachResManager.h"
#include "PullMachResourceDef.h"

namespace GamePullMach777
{

#define GetEglSize() CCSizeMake(WINDOW_WIDTH, WINDOW_HEIGHT);

CPullMachLoadingLayer::CPullMachLoadingLayer(CPMResManager * pResManager)
	:m_pGameResManager(pResManager)
	,m_AddCount(0)
{
	csize = GetEglSize();
}

CPullMachLoadingLayer::~CPullMachLoadingLayer()
{

}


void CPullMachLoadingLayer::OnLayerIn(CGameLayer* pOutLayer, ELayerAni enAniType, float fDuration)
{
	CGameLayer::OnLayerIn(pOutLayer, enAniType, fDuration);

	OnLayerInInit();
}

void CPullMachLoadingLayer::OnLayerOut(CGameLayer* pInLayer, ELayerAni enAniType, float fDuration)
{
	setTouchEnabled(false);
	CGameLayer::OnLayerOut(pInLayer, enAniType, fDuration);
}

void CPullMachLoadingLayer::OnLayerIn(float fDuration)
{
	CGameLayer::OnLayerIn(fDuration);

	OnLayerInInit();
}

void CPullMachLoadingLayer::OnLayerOut(float fDuration)
{
	setTouchEnabled(false);
	CGameLayer::OnLayerOut(fDuration);
}

void CPullMachLoadingLayer::OnLayerInInit()
{
	m_pLayer = CCLayerColor::create(ccc4(0,0,0,255));
	addChild(m_pLayer);

	//旋转
	{
		CCSprite *pSprite = CCSprite::createWithSpriteFrameName("images/lobby/splash/loading.png");
		pSprite->setPosition(ccp(csize.width/2, csize.height/2));
		m_pLayer->addChild(pSprite, 0);
		//pSprite->runAction(CCRepeatForever::create(CCRotateBy::create(1.f, 360.f)));
	}
	//字
	{
		CCSprite *pSprite = CCSprite::createWithSpriteFrameName("images/lobby/splash/loadingword.png");
		pSprite->setPosition(ccp(csize.width/2, csize.height/2));
		m_pLayer->addChild(pSprite, 0);
		//pSprite->runAction(CCRepeatForever::create(CCBlink::create(0.5f, 1)));
	}

	//加载
	if (m_pGameResManager)
	{
		//CPlatCommon::sharedPlatCommon()->ShowIOSMsg("fp ------- loading res -------------");

		m_pGameResManager->AsyncCacheRes(this, callfuncO_selector(CPullMachLoadingLayer::OnLoadResFinish));

		if (m_pGameDirector->m_nCfgDebugMode > 0)
		{
			int nTotalCount = m_pGameResManager->m_nAsyncLoadCount;
			cocos2d::CCLog("async loading resource(%d)", nTotalCount);
		}
	}
}

void CPullMachLoadingLayer::OnLayerRemove()
{
	//CCTextureCache::sharedTextureCache()->CloseAsyncThread();
}

void CPullMachLoadingLayer::OnInit()
{

}

void CPullMachLoadingLayer::OnLoadResFinish(CCObject* pObj)
{
	if (NULL == pObj)
	{
		ERROR_CHECK;
		return;
	}

	int nTotalCount = m_pGameResManager->m_nAsyncLoadCount;

	m_AddCount++;

	if (0 != nTotalCount)
	{

		//如果进度条小于100%，设置进度条的百分比
		if (m_AddCount < nTotalCount)
		{

		}
		else
		{
			std::list<std::string> textures;

			for (int i = 0; i < (int)m_pGameResManager->GetConfigHelper()->m_vTextures.size(); i++)
			{
				tagTexture* pTexture = m_pGameResManager->GetConfigHelper()->m_vTextures[i];
				if (NULL == pTexture)
				{
					ERROR_CHECK;
					continue;
				}

				bool bHasLoad = false;
				for (std::list<std::string>::iterator iter = textures.begin(); iter != textures.end(); iter++)
				{
					if (0 == pTexture->sTexture.compare((*iter).c_str()))
					{
						bHasLoad = true;
						break;
					}
				}

				if (bHasLoad)
				{
					continue;
				}

				CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(pTexture->sPlist.c_str(), pTexture->sTexture.c_str());

				textures.push_back(pTexture->sTexture);
			}

			m_pGameResManager->CacheResAinimation();

			//m_pLayer->stopAllActions();
			m_pGameDirector->OnLoadingFinish();
			m_AddCount = 0;
			//removeAllChildrenWithCleanup(true);
		}
	}
}

}
