#include "stdafx.h"
#include "OxCJ4CardHeap.h"
#include "GameLogic.h"
#include "ResourceDef.h"
#include "CommonDef.h"

namespace GameOxCJ4
{

COxCJ4CardHeap * COxCJ4CardHeap::create(CResManager * pResManager)
{
	COxCJ4CardHeap * pRet = new COxCJ4CardHeap(pResManager);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return NULL;
}

COxCJ4CardHeap::COxCJ4CardHeap(CResManager * pResManager)
	:m_pResManager(pResManager)
	,m_pBatchNode(NULL)
{

}

COxCJ4CardHeap::~COxCJ4CardHeap()
{

}

bool COxCJ4CardHeap::init()
{
	tagTexture* pTexture = m_pResManager->GetConfigHelper()->m_vTextures[0];
	m_pBatchNode = CCSpriteBatchNode::create(pTexture->sTexture.c_str());
	if (m_pBatchNode == NULL)
	{
		CC_ASSERT(false);
		return false;
	}

	addChild(m_pBatchNode);

	return true;
}

void COxCJ4CardHeap::SetCardData(const BYTE cbCardData[], BYTE cbCardCount, bool bIsOxType, float fScale/* = 0.5f*/)
{
	CC_ASSERT(cbCardCount == MAX_COUNT);
	if (cbCardCount != MAX_COUNT)
		return ;

	ClearAllCard();

	CGameLogic GameLogic;

	CCPoint point[MAX_COUNT];
	if (bIsOxType)
	{
		point[0] = ccp(0, 0);
		for (BYTE i = 1; i < MAX_COUNT; ++i)
		{
			if (i != 3)
				point[i] = ccp(point[i - 1].x + Pos_NorCardSpace, 0);
			else
				point[i] = ccp(point[i - 1].x + Pos_OxCardSpace, 0);
		}
	}
	else
	{
		for (BYTE i = 0; i < MAX_COUNT; ++i)
			point[i] = ccp(i * Pos_NorCardSpace, 0);
	}

	for (BYTE i = 0; i < cbCardCount; ++i)
	{
		BYTE nIndex = GameLogic.GetCardIndex(cbCardData[i]);
		if (nIndex >= ALL_COUNT)
			continue;

		CCSprite * pSprite = NULL;
		if (!m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, g_vCardResList[nIndex], pSprite))
			continue;

		pSprite->setAnchorPoint(ccp(0, 0.5));
		pSprite->setPosition(point[i]);
		pSprite->setScale(fScale);
		m_pBatchNode->addChild(pSprite);
	}
}

void COxCJ4CardHeap::ClearAllCard()
{
	m_pBatchNode->removeAllChildren();
}

}

