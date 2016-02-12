#include "stdafx.h"
#include "OxCJ4UserInfo.h"

namespace GameOxCJ4
{

COxCJ4UserInfo::CUserInfoNode::CUserInfoNode()
	:m_pFace(NULL)
	,m_pName(NULL)
	,m_pCoin(NULL)
	,m_pScore(NULL)
{
}
COxCJ4UserInfo::CUserInfoNode::~CUserInfoNode()
{
}

bool COxCJ4UserInfo::CUserInfoNode::init()
{

	return true;
}


COxCJ4UserInfo * COxCJ4UserInfo::create(COxCJ4GameLayer * pGameLayer, CResManager * pResManager)
{
	COxCJ4UserInfo * pRet = new COxCJ4UserInfo(pGameLayer, pResManager);

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

COxCJ4UserInfo::COxCJ4UserInfo(COxCJ4GameLayer * pGameLayer, CResManager * pResManager)
	:CLayerBase(pResManager)
	,m_pGameLayer(pGameLayer)
	,m_pResManager(pResManager)
	,m_pBanker(NULL)
{
	memset(m_pUserInfoBk, 0, sizeof(m_pUserInfoBk));
	memset(m_UserInfo, 0, sizeof(m_UserInfo));
}

COxCJ4UserInfo::~COxCJ4UserInfo()
{
}

bool COxCJ4UserInfo::init()
{
	if (!CLayerBase::init())
		return false;

	CCSize size = this->getContentSize();
	CCPoint midPoint = ccp(size.width / 2, size.height / 2);
	CCPoint bkPoint[4];
	bkPoint[0] = ccp(midPoint.x - 475,midPoint.y - 288);
	bkPoint[1] = ccp(midPoint.x - 560, midPoint.y);
	bkPoint[2] = ccp(midPoint.x, midPoint.y + 280);
	bkPoint[3] = ccp(midPoint.x + 553, midPoint.y);

	//…Ë÷√±≥æ∞
	for (WORD i = 0; i < 4; ++i)
	{
		int nBkId = (i == 0 || i== 2) ? Ox_Bg_Seatbk_h : Ox_Bg_Seatbk_v;

		if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, nBkId, m_pUserInfoBk[i]))
		{
			m_pUserInfoBk[i]->setPosition(bkPoint[i]);
			addToRoot(m_pUserInfoBk[i]);

			m_UserInfo[i] = CUserInfoNode::create();
			if (m_UserInfo[i])
			{
				m_UserInfo[i]->setPosition(CCPointZero);
				m_pUserInfoBk[i]->addChild(m_UserInfo[i]);
			}
		}
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Ox_Bg_Banker_Flag, m_pBanker))
	{
		m_pBanker->setPosition(CCPointZero);
		m_pBanker->setVisible(false);
		addToRoot(m_pBanker, 10);
	}

	return true;
}

void COxCJ4UserInfo::SetUserInfo(WORD wChairID, const tagLocalUser * pUserData)
{
	WORD wPos = GetUserChairID(wChairID);
	if (wPos == INVALID_CHAIR)
		return ;
	if (m_UserInfo[wPos] == NULL)
		return ;

	CUserInfoNode * pInfo = m_UserInfo[wPos];
	pInfo->removeAllChildren();

	if (pUserData == NULL)
		return ;

	CCPoint point[4];
	CCSize nameSize = CCSizeZero;

	if (wPos == 0 || wPos == 2)
	{
		point[0] = ccp(60,60);
		point[1] = ccp(113, 85);
		point[2] = ccp(127,45);
		point[3] = ccp(147, 45);
		nameSize = CCSizeMake(180, 40);
	}
	else
	{
		point[0] = ccp(70,130);
		point[1] = ccp(10, 60);
		point[2] = ccp(25,20);
		point[3] = ccp(43, 20);
		nameSize = CCSizeMake(125, 40);
	}

	pInfo->m_pFace = GetUserHeadSprite(pUserData->wFaceID, pUserData->bUserGender);
	if (pInfo->m_pFace)
	{
		pInfo->m_pFace->setPosition(point[0]);
		pInfo->addChild(pInfo->m_pFace);
	}

	pInfo->m_pName = CCLabelTTF::create(pUserData->sUserName.c_str(), "∫⁄ÃÂ", 30, nameSize, kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
	if (pInfo->m_pName)
	{
		pInfo->m_pName->setColor(ccc3(240, 170, 120));
		pInfo->m_pName->setAnchorPoint(ccp(0, 0.5));
		pInfo->m_pName->setPosition(point[1]);
		pInfo->addChild(pInfo->m_pName);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Ox_Bg_Jetton, pInfo->m_pCoin))
	{
		pInfo->m_pCoin->setPosition(point[2]);
		pInfo->addChild(pInfo->m_pCoin);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Ox_Font_User_Score, pInfo->m_pScore))
	{
		pInfo->m_pScore->setAnchorPoint(ccp(0, 0.5));
		pInfo->m_pScore->setPosition(point[3]);

		char szUserScore[64] = {0};
		snprintf(szUserScore, 64, "%lld", pUserData->nUserScore);
		pInfo->m_pScore->setString(szUserScore);
		pInfo->addChild(pInfo->m_pScore);
	}

}

void COxCJ4UserInfo::SetUserScore(WORD wChairID, LONGLONG lScore)
{
	WORD wPos = GetUserChairID(wChairID);
	if (wPos == INVALID_CHAIR)
		return ;

	if (m_UserInfo[wPos] == NULL || m_UserInfo[wPos]->m_pScore == NULL)
		return ;

	char szUserScore[64] = {0};
	snprintf(szUserScore, 64, "%lld", lScore);

	m_UserInfo[wPos]->m_pScore->setString(szUserScore);
}

void COxCJ4UserInfo::SetBanker(WORD wChairID)
{
	WORD wPos = GetUserChairID(wChairID);
	if (wPos == INVALID_CHAIR)
		return ;

	if (m_pBanker == NULL || m_pUserInfoBk[wPos] == NULL)
		return ;

	m_pBanker->setVisible(true);
	CCPoint point = m_pUserInfoBk[wPos]->getPosition();
	CCSize bkSize = m_pUserInfoBk[wPos]->getContentSize();
	m_pBanker->setPosition(ccp(point.x + bkSize.width / 2, point.y + bkSize.height / 2));
	m_pBanker->runAction(CCSequence::create(CCScaleTo::create(0.3f, 1.5f), CCScaleTo::create(0.3f, 1.0f), NULL));
}

CCPoint COxCJ4UserInfo::GetUserCoinWordPos(WORD wChairID)
{
	CC_ASSERT(wChairID < GAME_PLAYER);

	WORD wPos = wChairID;
	CCPoint CoinPoint;

	if (wPos == 0 || wPos == 2)
		CoinPoint = ccp(127,45);
	else
		CoinPoint = ccp(25,30);

	return m_UserInfo[wPos]->convertToWorldSpace(CoinPoint);
}

WORD COxCJ4UserInfo::GetUserChairID(WORD wChairID)
{
	WORD wMyChairID = m_pGameLayer->GetMeChairID();

	return ServerTableIndexToLocal(wChairID, wMyChairID);
}

CCSprite * COxCJ4UserInfo::GetUserHeadSprite(int faceid, int gender)
{
	CCSprite * pHeadSprite = NULL;

	CGameSpriteCfg * pCfg = m_pGameLayer->GetGameDirector()->GetUserHeadSpriteCfg(faceid, gender);

	if (pCfg != NULL)
		m_pResManager->GenerateNodeByCfgID(pCfg, pHeadSprite);

	return pHeadSprite;
}

}


