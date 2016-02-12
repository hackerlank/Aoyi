#include "stdafx.h"
#include "Star97LoadingLayer.h"
#include "ResManager.h"
#include "ResourceDef.h"

namespace GameStar97
{

CStar97LoadingLayer::CStar97LoadingLayer(CResManager * pResManager)
	:m_pResManager(pResManager)
	,m_AddCount(0)
	//,m_bResLoadReady(false)
	//,m_bAnimatReady(false)
{

}

CStar97LoadingLayer::~CStar97LoadingLayer()
{

}

void CStar97LoadingLayer::OnInit()
{
	screenSize = CCDirector::sharedDirector()->getWinSize();
	float fMidY = screenSize.height/2;

	//加载背景
	std::string sPath;
#ifndef ANDROID_GAME
	sPath = m_pResManager->GetConfigHelper()->m_sLoadingPak+"loading/loadingBg.jpg";
#else
	sPath = m_pResManager->GetConfigHelper()->m_sLoadingPak+"images/scene/phoneBg.jpg";
#endif

	CCSprite *pBackView = CCSprite::create(sPath.c_str());
	if (pBackView!=NULL)
	{
#ifndef ANDROID_GAME
		fMidY = screenSize.height/2 - TITLE_HEIGHT / 2 + BORDER_BOTTOM;
		pBackView->setPosition(CCPointMake(screenSize.width/2, fMidY));
#else
		pBackView->setPosition(CCPointMake(screenSize.width/2, fMidY));
#endif
		addChild(pBackView);
	}

//	m_bResLoadReady = false;
//#ifndef ANDROID_GAME
//	m_bAnimatReady = false;
//#else
//	m_bAnimatReady = true;
//#endif

#ifndef ANDROID_GAME
	//创建加载动画
	float fEndX[4] = {722.0f, 589.0f, 456.0f, 323.0f};
	float fVelocity = fEndX[0] / 100;
	//float fMoveTime[4] = {1.0f, fEndX[1] / fEndX[0], fEndX[2] / fEndX[0], fEndX[3] / fEndX[0]};	//匀速运动
	float fMoveTime[4] = {0.7f, 0.7f, 0.7f, 0.7f};

	CCSprite * pAnimate = NULL;
	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Star97_Load_Ming, pAnimate))
	{
		//文字“明”
		pAnimate->setAnchorPoint(ccp(0, 0.5f));
		pAnimate->setPosition(ccp(screenSize.width, fMidY));
		pBackView->addChild(pAnimate);

		//执行动画
		CCAction * moveAction = CCSequence::create(CCMoveBy::create(fMoveTime[0], ccp(-fEndX[0], 0)),
			CCDelayTime::create(fMoveTime[1] + fMoveTime[2] + fMoveTime[3] + 1.0f),
			CCMoveBy::create(fMoveTime[3], ccp(-fEndX[3], 0)), NULL);
		pAnimate->runAction(moveAction);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Star97_Load_Xing, pAnimate))
	{
		//文字“星”
		pAnimate->setAnchorPoint(ccp(0, 0.5f));
		pAnimate->setPosition(ccp(screenSize.width, fMidY));
		pBackView->addChild(pAnimate);

		//执行动画
		CCAction * moveAction = CCSequence::create(CCDelayTime::create(fMoveTime[0]),
			CCMoveBy::create(fMoveTime[1], ccp(-fEndX[1], 0)),
			CCDelayTime::create(fMoveTime[2] + fMoveTime[3] + 1.0f + fMoveTime[3]),
			CCMoveBy::create(fMoveTime[2], ccp(-fEndX[2], 0)), NULL);
		pAnimate->runAction(moveAction);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Star97_Load_9, pAnimate))
	{
		//文字“9”
		pAnimate->setAnchorPoint(ccp(0, 0.5f));
		pAnimate->setPosition(ccp(screenSize.width, fMidY));
		pBackView->addChild(pAnimate);

		//执行动画
		CCAction * moveAction = CCSequence::create(CCDelayTime::create(fMoveTime[0] + fMoveTime[1]),
			CCMoveBy::create(fMoveTime[2], ccp(-fEndX[2], 0)),
			CCDelayTime::create(fMoveTime[3] + 1.0f + fMoveTime[3] + fMoveTime[2]),
			CCMoveBy::create(fMoveTime[1], ccp(-fEndX[1], 0)), NULL);
		pAnimate->runAction(moveAction);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Star97_Load_7, pAnimate))
	{
		//文字“7”
		pAnimate->setAnchorPoint(ccp(0, 0.5f));
		pAnimate->setPosition(ccp(screenSize.width, fMidY));
		pBackView->addChild(pAnimate);

		//执行动画
		CCAction * moveAction = CCSequence::create(CCDelayTime::create(fMoveTime[0] + fMoveTime[1] + fMoveTime[2]),
			CCMoveBy::create(fMoveTime[3], ccp(-fEndX[3], 0)),
			CCDelayTime::create(1.0f + fMoveTime[3] + fMoveTime[2] + fMoveTime[1]),
			CCMoveBy::create(fMoveTime[0], ccp(-fEndX[0], 0)),
			CCCallFunc::create(this, callfunc_selector(CStar97LoadingLayer::animatReady)),NULL);
		pAnimate->runAction(moveAction);
	}
#else
	animatReady();
#endif
}

void CStar97LoadingLayer::OnLoadResFinish(CCObject *pObj)
{
	int nTotalCount = m_pResManager->m_nAsyncLoadCount;

	m_AddCount++;

	if (m_AddCount >= nTotalCount)
	{
		std::list<std::string> textures;

		for (int i = 0; i < (int)m_pResManager->GetConfigHelper()->m_vTextures.size(); i++)
		{
			tagTexture* pTexture = m_pResManager->GetConfigHelper()->m_vTextures[i];
			if (NULL == pTexture)
			{
				ERROR_CHECK;
				continue;
			}

			bool bHasLoad = false;
			for (std::list<std::string>::iterator iter = textures.begin(); iter != textures.end(); ++iter)
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

#ifdef ANDROID_GAME
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(pTexture->sPlist.c_str());
#else
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(pTexture->sPlist.c_str(), pTexture->sTexture.c_str());
#endif

			textures.push_back(pTexture->sTexture);
		}

		if (m_pGameDirector->m_nCfgDebugMode > 0)
		{
			cocos2d::CCLog("cache res animation\r\n");
		}
		m_pResManager->CacheResAinimation();
		if (m_pGameDirector->m_nCfgDebugMode > 0)
		{
			cocos2d::CCLog("load end\r\n");
			cocos2d::CCLog("... Loading Finish ... ...");
		}

		//m_bResLoadReady = true;
		//if (m_bAnimatReady)
		{
			removeAllChildren();
			CCDirector::sharedDirector()->replaceScene(m_pGameDirector->GetGameScene());
		}
	}
}

void CStar97LoadingLayer::animatReady()
{
	//m_bAnimatReady = true;
	//if (m_bResLoadReady)
	//{
	//	removeAllChildren();
	//	CCDirector::sharedDirector()->replaceScene(m_pGameDirector->GetGameScene());
	//}
	//加载
	if (m_pResManager)
	{
		m_pResManager->AsyncCacheRes(this, callfuncO_selector(CStar97LoadingLayer::OnLoadResFinish));

		if (m_pGameDirector->m_nCfgDebugMode > 0)
		{
			int nTotalCount = m_pResManager->m_nAsyncLoadCount;
			cocos2d::CCLog("SparrowXL async loading resource(%d)\r\n", nTotalCount);
		}
	}
}

}
