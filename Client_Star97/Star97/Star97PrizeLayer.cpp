#include "stdafx.h"
#include "ResManager.h"
#include "ResourceDef.h"
#include "Star97PrizeLayer.h"

namespace GameStar97
{

CStar97PrizeLayer * CStar97PrizeLayer::create(CResManager * pResManager)
{
	CStar97PrizeLayer * pRet = new CStar97PrizeLayer(pResManager);

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

CStar97PrizeLayer::CStar97PrizeLayer(CResManager * pResManager)
	:m_pResManager(pResManager)
	,m_pPrize(NULL)
	,m_pFontAnyBar(NULL)
	,m_pFontOrange(NULL)
	,m_pFontPawpaw(NULL)
	,m_pFontBell(NULL)
	,m_pFontTsama(NULL)
	,m_pFontBarY(NULL)
	,m_pFontLeech(NULL)
	,m_pFontBarR(NULL)
	,m_pFontBarB(NULL)
	,m_pFont9Num7(NULL)
	,m_pFont8Num7(NULL)
	,m_pFont7Num7(NULL)
	,m_pFontExtraAllHalf(NULL)
	,m_pFontExtraAllWhole(NULL)
	,m_pFontExtra9Num7(NULL)
	,m_pFontExtra8Num7(NULL)
	,m_pFontExtra7Num7(NULL)
	,m_pFontExtraAnyBar(NULL)
	,m_pFontAllJetton(NULL)
	,m_pFontAllWin(NULL)
{

}

CStar97PrizeLayer::~CStar97PrizeLayer()
{

}

bool CStar97PrizeLayer::init()
{
	if (!CCLayer::init())
		return false;

	CCLayerColor * pBgLayer = CCLayerColor::create(ccc4(0,0,0,128), WINDOW_WIDTH, CONTENTS_HEIGHT);
	this->addChild(pBgLayer);

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Star97_Bg_BigPrize_Bg, m_pPrize))
	{
		m_pPrize->setAnchorPoint(CCPointZero);
#ifndef ANDROID_GAME
		m_pPrize->setPosition(ccp(35, 78));
#else
		m_pPrize->setPosition(ccp(220, 20));
#endif
		this->addChild(m_pPrize);
	}

	//添加中奖个数
	CCPoint basePos(383.0f, 150.0f);
	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Record, m_pFontAnyBar))
	{
		m_pFontAnyBar->setAnchorPoint(ccp(0, 0.5f));
		m_pFontAnyBar->setPosition(basePos);
		m_pFontAnyBar->setString("0");
		m_pPrize->addChild(m_pFontAnyBar);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Record, m_pFontOrange))
	{
		m_pFontOrange->setAnchorPoint(ccp(0, 0.5f));
		m_pFontOrange->setPosition(ccp(basePos.x, basePos.y + 48 * 1));
		m_pFontOrange->setString("0");
		m_pPrize->addChild(m_pFontOrange);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Record, m_pFontPawpaw))
	{
		m_pFontPawpaw->setAnchorPoint(ccp(0, 0.5f));
		m_pFontPawpaw->setPosition(ccp(basePos.x, basePos.y + 48 * 2));
		m_pFontPawpaw->setString("0");
		m_pPrize->addChild(m_pFontPawpaw);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Record, m_pFontBell))
	{
		m_pFontBell->setAnchorPoint(ccp(0, 0.5f));
		m_pFontBell->setPosition(ccp(basePos.x, basePos.y + 48 * 3));
		m_pFontBell->setString("0");
		m_pPrize->addChild(m_pFontBell);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Record, m_pFontTsama))
	{
		m_pFontTsama->setAnchorPoint(ccp(0, 0.5f));
		m_pFontTsama->setPosition(ccp(basePos.x, basePos.y + 48 * 4));
		m_pFontTsama->setString("0");
		m_pPrize->addChild(m_pFontTsama);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Record, m_pFontBarY))
	{
		m_pFontBarY->setAnchorPoint(ccp(0, 0.5f));
		m_pFontBarY->setPosition(ccp(basePos.x, basePos.y + 48 * 5));
		m_pFontBarY->setString("0");
		m_pPrize->addChild(m_pFontBarY);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Record, m_pFontLeech))
	{
		m_pFontLeech->setAnchorPoint(ccp(0, 0.5f));
		m_pFontLeech->setPosition(ccp(basePos.x, basePos.y + 48 * 6));
		m_pFontLeech->setString("0");
		m_pPrize->addChild(m_pFontLeech);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Record, m_pFontBarR))
	{
		m_pFontBarR->setAnchorPoint(ccp(0, 0.5f));
		m_pFontBarR->setPosition(ccp(basePos.x, basePos.y + 48 * 7));
		m_pFontBarR->setString("0");
		m_pPrize->addChild(m_pFontBarR);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Record, m_pFontBarB))
	{
		m_pFontBarB->setAnchorPoint(ccp(0, 0.5f));
		m_pFontBarB->setPosition(ccp(basePos.x, basePos.y + 48 * 8));
		m_pFontBarB->setString("0");
		m_pPrize->addChild(m_pFontBarB);
	}

	basePos.setPoint(730.0f, 438.0f);
	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Record, m_pFont7Num7))
	{
		m_pFont7Num7->setAnchorPoint(ccp(0, 0.5f));
		m_pFont7Num7->setPosition(basePos);
		m_pFont7Num7->setString("0");
		m_pPrize->addChild(m_pFont7Num7);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Record, m_pFont8Num7))
	{
		m_pFont8Num7->setAnchorPoint(ccp(0, 0.5f));
		m_pFont8Num7->setPosition(ccp(basePos.x, basePos.y + 48 * 1));
		m_pFont8Num7->setString("0");
		m_pPrize->addChild(m_pFont8Num7);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Record, m_pFont9Num7))
	{
		m_pFont9Num7->setAnchorPoint(ccp(0, 0.5f));
		m_pFont9Num7->setPosition(ccp(basePos.x, basePos.y + 48 * 2));
		m_pFont9Num7->setString("0");
		m_pPrize->addChild(m_pFont9Num7);
	}

	//添加额外奖励分数
	basePos.setPoint(693.0f, 355.0f);
	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Extra_Info, m_pFontExtraAllHalf))
	{
		m_pFontExtraAllHalf->setAnchorPoint(ccp(0.5f, 0.5f));
		m_pFontExtraAllHalf->setPosition(basePos);
		m_pFontExtraAllHalf->setString("0");
		m_pPrize->addChild(m_pFontExtraAllHalf);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Extra_Info, m_pFontExtraAllWhole))
	{
		m_pFontExtraAllWhole->setAnchorPoint(ccp(0.5f, 0.5f));
		m_pFontExtraAllWhole->setPosition(ccp(basePos.x, basePos.y - 31 * 1));
		m_pFontExtraAllWhole->setString("0");
		m_pPrize->addChild(m_pFontExtraAllWhole);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Extra_Info, m_pFontExtra9Num7))
	{
		m_pFontExtra9Num7->setAnchorPoint(ccp(0.5f, 0.5f));
		m_pFontExtra9Num7->setPosition(ccp(basePos.x, basePos.y - 31 * 2));
		m_pFontExtra9Num7->setString("0");
		m_pPrize->addChild(m_pFontExtra9Num7);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Extra_Info, m_pFontExtra8Num7))
	{
		m_pFontExtra8Num7->setAnchorPoint(ccp(0.5f, 0.5f));
		m_pFontExtra8Num7->setPosition(ccp(basePos.x, basePos.y - 31 * 3));
		m_pFontExtra8Num7->setString("0");
		m_pPrize->addChild(m_pFontExtra8Num7);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Extra_Info, m_pFontExtra7Num7))
	{
		m_pFontExtra7Num7->setAnchorPoint(ccp(0.5f, 0.5f));
		m_pFontExtra7Num7->setPosition(ccp(basePos.x, basePos.y - 31 * 4));
		m_pFontExtra7Num7->setString("0");
		m_pPrize->addChild(m_pFontExtra7Num7);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Extra_Info, m_pFontExtraAnyBar))
	{
		m_pFontExtraAnyBar->setAnchorPoint(ccp(0.5f, 0.5f));
		m_pFontExtraAnyBar->setPosition(ccp(basePos.x, basePos.y - 31 * 5));
		m_pFontExtraAnyBar->setString("0");
		m_pPrize->addChild(m_pFontExtraAnyBar);
	}

	//总下注、总赢分
	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Table_Winlose, m_pFontAllJetton))
	{
		m_pFontAllJetton->setAnchorPoint(ccp(0, 0.5f));
		m_pFontAllJetton->setPosition(ccp(192, 77));
		m_pFontAllJetton->setString("0");
		m_pPrize->addChild(m_pFontAllJetton);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Table_Winlose, m_pFontAllWin))
	{
		m_pFontAllWin->setAnchorPoint(ccp(0, 0.5f));
		m_pFontAllWin->setPosition(ccp(544, 77));
		m_pFontAllWin->setString("0");
		m_pPrize->addChild(m_pFontAllWin);
	}

	//使用触摸
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);

	return true;
}

bool CStar97PrizeLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if (isVisible() == false)
		return false;

	CCSize size = m_pPrize->getContentSize();
	CCPoint pos = m_pPrize->getPosition();
	CCRect rect(pos.x, pos.y, size.width, size.height);
	if (rect.containsPoint(convertTouchToNodeSpace(pTouch)))
	{
		this->setVisible(false);
		return true;
	}
	else
	{
		return false;
	}
}

void CStar97PrizeLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{

}

void CStar97PrizeLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

}

void CStar97PrizeLayer::updatePrizeCount(const tagBigPrizeCount & prizeCount)
{
	char szScore[256] = {0};
	snprintf(szScore, sizeof(szScore), "%d", prizeCount.nAnyBar);
	m_pFontAnyBar->setString(szScore);

	snprintf(szScore, sizeof(szScore), "%d", prizeCount.nAllOrange);
	m_pFontOrange->setString(szScore);

	snprintf(szScore, sizeof(szScore), "%d", prizeCount.nAllPawpaw);
	m_pFontPawpaw->setString(szScore);

	snprintf(szScore, sizeof(szScore), "%d", prizeCount.nAllBell);
	m_pFontBell->setString(szScore);

	snprintf(szScore, sizeof(szScore), "%d", prizeCount.nAllTsama);
	m_pFontTsama->setString(szScore);

	snprintf(szScore, sizeof(szScore), "%d", prizeCount.nAllBarY);
	m_pFontBarY->setString(szScore);

	snprintf(szScore, sizeof(szScore), "%d", prizeCount.nAllLeech);
	m_pFontLeech->setString(szScore);

	snprintf(szScore, sizeof(szScore), "%d", prizeCount.nAllBarR);
	m_pFontBarR->setString(szScore);

	snprintf(szScore, sizeof(szScore), "%d", prizeCount.nAllBarB);
	m_pFontBarB->setString(szScore);

	snprintf(szScore, sizeof(szScore), "%d", prizeCount.nAllNum7);
	m_pFont9Num7->setString(szScore);

	snprintf(szScore, sizeof(szScore), "%d", prizeCount.n8Num7);
	m_pFont8Num7->setString(szScore);

	snprintf(szScore, sizeof(szScore), "%d", prizeCount.n7Num7);
	m_pFont7Num7->setString(szScore);
}

void CStar97PrizeLayer::updateAllJettonWin(LONGLONG lJetton, LONGLONG lWin)
{
	char szScore[256] = {0};
	snprintf(szScore, sizeof(szScore), "%lld", lJetton);
	m_pFontAllJetton->setString(szScore);

	snprintf(szScore, sizeof(szScore), "%lld", lWin);
	m_pFontAllWin->setString(szScore);
}

void CStar97PrizeLayer::setExtraScoreInfo(const tagExtraScoreInfo & extraInfo)
{
	char szScore[256] = {0};
	snprintf(szScore, sizeof(szScore), "%d", extraInfo.nAllHalf);
	m_pFontExtraAllHalf->setString(szScore);

	snprintf(szScore, sizeof(szScore), "%d", extraInfo.nAllWhole);
	m_pFontExtraAllWhole->setString(szScore);

	snprintf(szScore, sizeof(szScore), "%d", extraInfo.n9Num7);
	m_pFontExtra9Num7->setString(szScore);

	snprintf(szScore, sizeof(szScore), "%d", extraInfo.n8Num7);
	m_pFontExtra8Num7->setString(szScore);

	snprintf(szScore, sizeof(szScore), "%d", extraInfo.n7Num7);
	m_pFontExtra7Num7->setString(szScore);

	snprintf(szScore, sizeof(szScore), "%d", extraInfo.nAnyBar);
	m_pFontExtraAnyBar->setString(szScore);
}

}
