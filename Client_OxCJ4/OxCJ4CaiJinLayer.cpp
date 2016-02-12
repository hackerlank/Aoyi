#include "stdafx.h"
#include "OxCJ4CaiJinLayer.h"
#include "OxCJ4GameLayer.h"
#include "ResManager.h"
#include "ResourceDef.h"

namespace GameOxCJ4
{

COxCJ4CaiJinLayer * COxCJ4CaiJinLayer::create(COxCJ4GameLayer * pGameLayer, CResManager * pResManger)
{
	COxCJ4CaiJinLayer * pRet = new COxCJ4CaiJinLayer(pGameLayer, pResManger);
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

COxCJ4CaiJinLayer::COxCJ4CaiJinLayer(COxCJ4GameLayer * pGameLayer, CResManager * pResManger)
	:CLayerBase(pResManger)
	,m_pGameLayer(pGameLayer)
	,m_pResManager(pResManger)
	,m_pCaiJinChi(NULL)
	,m_pJiLu(NULL)
	,m_pXiangQing(NULL)
	,m_pWinningsPool(NULL)
	,m_pMeWeekWin(NULL)
	,m_pBtnXiangQing(NULL)
	,m_pBtnJiLu(NULL)
{
	memset(m_pName, 0, sizeof(m_pName));
	memset(m_pCardType, 0, sizeof(m_pCardType));
	memset(m_pGetScore, 0, sizeof(m_pGetScore));
}

COxCJ4CaiJinLayer::~COxCJ4CaiJinLayer()
{

}

bool COxCJ4CaiJinLayer::init()
{
	if (!CLayerBase::init())
		return false;

	if (!m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Ox_Bg_Cai_Jinchi, m_pCaiJinChi))
		return false;
	m_pCaiJinChi->setAnchorPoint(ccp(1, 1));
	m_pCaiJinChi->setPosition(CCPointZero);
	addToRoot(m_pCaiJinChi);

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Ox_Font_Cai_Jin_Chi, m_pWinningsPool))
	{
		CCSize size = m_pCaiJinChi->getContentSize();
		m_pWinningsPool->setAnchorPoint(ccp(1, 0.5));
		m_pWinningsPool->setPosition(ccp(size.width - 10, size.height / 2));
		m_pWinningsPool->setString("0");
		m_pCaiJinChi->addChild(m_pWinningsPool);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Ox_Font_Week_Score, m_pMeWeekWin))
	{
		m_pMeWeekWin->setAnchorPoint(ccp(0, 0.5));
		m_pMeWeekWin->setPosition(ccp(130, 47));
		m_pMeWeekWin->setString("0");
		m_pCaiJinChi->addChild(m_pMeWeekWin);
	}

	if (!m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Ox_Bg_Xiang_Qing, m_pXiangQing))
		return false;
	m_pXiangQing->setAnchorPoint(ccp(1, 1));
	m_pXiangQing->setPosition(ccp(-85, -41));
	m_pXiangQing->setVisible(false);
	addToRoot(m_pXiangQing, 10);

	if (!m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Ox_Bg_Ji_Lu, m_pJiLu))
		return false;
	m_pJiLu->setAnchorPoint(ccp(1, 1));
	m_pJiLu->setPosition(ccp(0, -m_pCaiJinChi->getContentSize().height));
	m_pJiLu->setVisible(false);
	addToRoot(m_pJiLu);

	//彩金纪录数据
	CCSize size[3];
	size[0] = CCSizeMake(120, 33);
	size[1] = CCSizeMake(70, 33);
	size[2] = CCSizeMake(135, 33);
	float fHeight = m_pJiLu->getContentSize().height;
	for (BYTE i = 0; i < CountArray(m_pName); ++i)
	{
		if (m_pName[i] = CCLabelTTF::create("", "宋体", 20, size[0], kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter))
		{
			m_pName[i]->setAnchorPoint(ccp(0, 0.5));
			m_pName[i]->setPosition(ccp(10, fHeight - 50 - 26 * i));
			m_pJiLu->addChild(m_pName[i]);
		}

		if (m_pCardType[i] = CCLabelTTF::create("", "Arial", 20, size[1], kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter))
		{
			m_pCardType[i]->setAnchorPoint(ccp(0.5, 0.5));
			m_pCardType[i]->setPosition(ccp(165, fHeight - 50 - 26 * i));
			m_pJiLu->addChild(m_pCardType[i]);
		}

		if (m_pGetScore[i] = CCLabelTTF::create("", "Arial", 20, size[2], kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter))
		{
			m_pGetScore[i]->setAnchorPoint(ccp(0.5, 0.5));
			m_pGetScore[i]->setPosition(ccp(270, fHeight - 50 - 26 * i));
			m_pJiLu->addChild(m_pGetScore[i]);
		}
	}

	m_pBtnXiangQing = createMenuItemSpriteByResID(Ox_Bg_Null_Bk, Ox_Bg_Null_Bk);
	if (m_pBtnXiangQing)
	{
		m_pBtnXiangQing->setTarget(this, menu_selector(COxCJ4CaiJinLayer::OnTouchXiangQing));
		m_pBtnXiangQing->setAnchorPoint(ccp(1, 1));
		m_pBtnXiangQing->setPosition(CCPointZero);
	}

	m_pBtnJiLu = createMenuItemSpriteByResID(Ox_Bg_Null_Bk, Ox_Bg_Null_Bk);
	if (m_pBtnJiLu)
	{
		m_pBtnJiLu->setTarget(this, menu_selector(COxCJ4CaiJinLayer::OnTouchJiLu));
		m_pBtnJiLu->setAnchorPoint(ccp(0.5, 0.5));
		m_pBtnJiLu->setPosition(ccp(-63, -152));
	}

	CCMenu * pMenu = CCMenu::create(m_pBtnXiangQing, m_pBtnJiLu, NULL);
	pMenu->setPosition(CCPointZero);
	addToRoot(pMenu);

	return true;
}

void COxCJ4CaiJinLayer::SetCaiJinChi(const CMD_S_Winnings * pWinnings)
{
	if (pWinnings == NULL)
		return ;

	char szScore[64];
	memset(szScore, 0, sizeof (szScore));
	snprintf(szScore, 64, "%lld", pWinnings->lWinningsPool);
	m_pWinningsPool->setString(szScore);

	tagLocalUser * pUser = m_pGameLayer->GetGameDirector()->GetMeData();
	std::string szMeName = CStringHelper::UTF8ToGB2312(pUser->sUserName.c_str());

	for (BYTE i = 0; i < pWinnings->nRecordCount; ++i)
	{
		const tagWinningsRecord & record = pWinnings->record[i];
		if (record.lScore == 0)
			continue;

		m_pName[i]->setString(CStringHelper::GB2312ToUTF8(record.szName).c_str());
		m_pCardType[i]->setString(record.szCard);

		memset(szScore, 0, sizeof(szScore));
		snprintf(szScore, 64, "%lld", record.lScore);
		m_pGetScore[i]->setString(szScore);

		if (szMeName == record.szName)
		{
			m_pName[i]->setColor(ccc3(250, 100, 100));
			m_pCardType[i]->setColor(ccc3(250, 100, 100));
			m_pGetScore[i]->setColor(ccc3(250, 100, 100));
		}
		else
		{
			m_pName[i]->setColor(ccc3(255, 255, 255));
			m_pCardType[i]->setColor(ccc3(255, 255, 255));
			m_pGetScore[i]->setColor(ccc3(255, 255, 255));
		}
	}

	return ;
}

void COxCJ4CaiJinLayer::SetMeWeekWin(LONGLONG lScore)
{
	char szTemp[64];
	memset(szTemp, 0, sizeof(szTemp));
	snprintf(szTemp, 64, "%lld", lScore);
	if (m_pMeWeekWin)
		m_pMeWeekWin->setString(szTemp);
}

void COxCJ4CaiJinLayer::OnTouchXiangQing(CCObject* pObject)
{
	if (m_pXiangQing->isVisible())
		m_pXiangQing->setVisible(false);
	else
		m_pXiangQing->setVisible(true);
}

void COxCJ4CaiJinLayer::OnTouchJiLu(CCObject* pObject)
{
	if (m_pJiLu->isVisible())
		m_pJiLu->setVisible(false);
	else
		m_pJiLu->setVisible(true);
}

}
