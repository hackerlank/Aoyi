#include "stdafx.h"
#include "OxCJ4LoadingLayer.h"
#include "ResManager.h"
#include "ResourceDef.h"

namespace GameOxCJ4
{

COxCJ4LoadingLayer::COxCJ4LoadingLayer(CResManager * pResManager)
	:m_pResManager(pResManager)
	,m_AddCount(0)
{

}

COxCJ4LoadingLayer::~COxCJ4LoadingLayer()
{

}

void COxCJ4LoadingLayer::OnInit()
{
	screenSize = CCDirector::sharedDirector()->getWinSize();
	std::string sPath = m_pResManager->GetConfigHelper()->m_sLoadingPak+"images/scene/loading_layer.jpg";
	CCSprite *pBackView = CCSprite::create(sPath.c_str());
	if (pBackView!=NULL)
	{
		pBackView->setPosition(CCPointMake(screenSize.width/2,screenSize.height/2));
		addChild(pBackView);
	}

	CCSprite *pLoading1 = CCSprite::createWithSpriteFrameName("loading/loading1.png");
	pLoading1->setPosition(CCPointMake(screenSize.width/2-100,screenSize.height/2-310));
	addChild(pLoading1);
	pLoading1->runAction(CCRepeatForever::create((CCActionInterval *)CCSequence::create(CCJumpBy::create(0.5f,CCPointZero,20,1),CCDelayTime::create(1),NULL)));

	CCSprite *pLoading2 = CCSprite::createWithSpriteFrameName("loading/loading2.png");
	pLoading2->setPosition(CCPointMake(screenSize.width/2-60,screenSize.height/2-310));
	addChild(pLoading2);
	pLoading2->runAction(CCRepeatForever::create((CCActionInterval *)CCSequence::create(CCDelayTime::create(0.2f),CCJumpBy::create(0.5f,CCPointZero,20,1),CCDelayTime::create(0.8f),NULL)));

	CCSprite *pLoading3 = CCSprite::createWithSpriteFrameName("loading/loading3.png");
	pLoading3->setPosition(CCPointMake(screenSize.width/2-20,screenSize.height/2-310));
	addChild(pLoading3);
	pLoading3->runAction(CCRepeatForever::create((CCActionInterval *)CCSequence::create(CCDelayTime::create(0.4f),CCJumpBy::create(0.5f,CCPointZero,20,1),CCDelayTime::create(0.6f),NULL)));

	CCSprite *pLoading4 = CCSprite::createWithSpriteFrameName("loading/loading4.png");
	pLoading4->setPosition(CCPointMake(screenSize.width/2+20,screenSize.height/2-310));
	addChild(pLoading4);
	pLoading4->runAction(CCRepeatForever::create((CCActionInterval *)CCSequence::create(CCDelayTime::create(0.6f),CCJumpBy::create(0.5f,CCPointZero,20,1),CCDelayTime::create(0.4f),NULL)));

	CCSprite *pLoading5 = CCSprite::createWithSpriteFrameName("loading/loading5.png");
	pLoading5->setPosition(CCPointMake(screenSize.width/2+60,screenSize.height/2-310));
	addChild(pLoading5);
	pLoading5->runAction(CCRepeatForever::create((CCActionInterval *)CCSequence::create(CCDelayTime::create(0.8f),CCJumpBy::create(0.5f,CCPointZero,20,1),CCDelayTime::create(0.2f),NULL)));

	CCSprite *pLoading6 = CCSprite::createWithSpriteFrameName("loading/loading6.png");
	pLoading6->setPosition(CCPointMake(screenSize.width/2+100,screenSize.height/2-310));
	addChild(pLoading6);

	CCArray *animFrames = CCArray::create();
	char szPath[128];
	memset(szPath,0,sizeof(szPath));
	for (int i=6; i<=8; i++)
	{
		sprintf(szPath,"loading/loading%d.png",i);
		CCSpriteFrame *fram = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(szPath);
		animFrames->addObject(fram);
	}
	CCAnimation *pAnimation = CCAnimation::createWithSpriteFrames(animFrames,0.5f);

	pLoading6->runAction(CCRepeatForever::create((CCActionInterval *)CCSpawn::create(CCAnimate::create(pAnimation),
		CCSequence::create(CCDelayTime::create(1),CCJumpBy::create(0.5f,CCPointZero,20,1),NULL),NULL)));

	//╪сть
	if (m_pResManager)
	{
		m_pResManager->AsyncCacheRes(this, callfuncO_selector(COxCJ4LoadingLayer::OnLoadResFinish));

		if (m_pGameDirector->m_nCfgDebugMode > 0)
		{
			int nTotalCount = m_pResManager->m_nAsyncLoadCount;
			cocos2d::CCLog("SparrowXL async loading resource(%d)\r\n", nTotalCount);
		}
	}
}

void COxCJ4LoadingLayer::OnLoadResFinish(CCObject *pObj)
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

		removeAllChildren();
		CCDirector::sharedDirector()->replaceScene(m_pGameDirector->GetGameScene());
	}
}

void COxCJ4LoadingLayer::DisplaySelf(CCNode *pNode)
{
	pNode->setVisible(true);
}

void COxCJ4LoadingLayer::DisappearSelf(CCNode *pNode)
{
	pNode->setVisible(false);
	pNode->setPosition(CCPointMake(screenSize.width/2-40,screenSize.height/2+30));
}

}
