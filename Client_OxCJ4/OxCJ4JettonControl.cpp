#include "stdafx.h"
#include "ResManager.h"
#include "OxCJ4JettonControl.h"
#include "OxCJ4GameLayer.h"
#include "LayerBase.h"

namespace GameOxCJ4
{

COxCJ4JettonControl * COxCJ4JettonControl::create(COxCJ4GameLayer * pGameLayer, CResManager * pResManager)
{
	COxCJ4JettonControl * pRet = new COxCJ4JettonControl(pGameLayer, pResManager);
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

COxCJ4JettonControl::COxCJ4JettonControl(COxCJ4GameLayer * pGameLayer, CResManager * pResManager)
	:m_pGameLayer(pGameLayer)
	,m_pResManager(pResManager)
	,m_pHoldBatchNode(NULL)
	,m_nJetAreaRadius(0)
{
	memset(m_pPlaceBatchNode, 0, sizeof(m_pPlaceBatchNode));
	memset(m_pLabelValue, 0, sizeof(m_pLabelValue));
}

COxCJ4JettonControl::~COxCJ4JettonControl()
{

}

bool COxCJ4JettonControl::init()
{
	if (!CCLayer::init())
		return false;

	CCSize contentSize = this->getContentSize();
	float fMidx = contentSize.width / 2;
	float fMidy = contentSize.height / 2;

	m_nJetAreaRadius = 30;
	m_PlaceJetPoint[0] = ccp(fMidx, fMidy - 75);
	m_PlaceJetPoint[1] = ccp(fMidx - 200, fMidy);
	m_PlaceJetPoint[2] = ccp(fMidx, fMidy + 75);
	m_PlaceJetPoint[3] = ccp(fMidx + 200, fMidy);

	m_pGameLayer->GetAllUserJetIconPos(m_HoldJetPoint);

	tagTexture * pTexture = m_pResManager->GetConfigHelper()->m_vTextures[0];
	for (BYTE i = 0; i < CountArray(m_pPlaceBatchNode); ++i)
	{
		m_pPlaceBatchNode[i] = CCSpriteBatchNode::create(pTexture->sTexture.c_str());
		if (m_pPlaceBatchNode[i] == NULL)
			return false;

		addChild(m_pPlaceBatchNode[i]);
	}

	m_pHoldBatchNode = CCSpriteBatchNode::create(pTexture->sTexture.c_str());
	if (m_pHoldBatchNode == NULL)
		return false;
	addChild(m_pHoldBatchNode);

	for (BYTE i = 0; i < CountArray(m_pLabelValue); ++i)
	{
		if (!m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Ox_Font_Ui, m_pLabelValue[i]))
			return false;

		m_pLabelValue[i]->setAnchorPoint(ccp(0.5, 0.5));
		if (i != 2)
			m_pLabelValue[i]->setPosition(m_PlaceJetPoint[i].x, m_PlaceJetPoint[i].y + m_nJetAreaRadius + 30);
		else
			m_pLabelValue[i]->setPosition(m_PlaceJetPoint[i].x, m_PlaceJetPoint[i].y - m_nJetAreaRadius - 30);

		m_pLabelValue[i]->setVisible(false);
		addChild(m_pLabelValue[i]);
	}

	return true;
}

void COxCJ4JettonControl::RemoveAllJetton()
{
	for (BYTE i = 0; i < CountArray(m_pPlaceBatchNode); ++i)
		m_pPlaceBatchNode[i]->removeAllChildren();

	m_pHoldBatchNode->removeAllChildren();
}

//玩家下注
void COxCJ4JettonControl::UserPlaceJetton(WORD wChairID, BYTE cbJettonCount, LONGLONG lJettonValue)
{
	if (cbJettonCount > Max_Jetton_Count)
		cbJettonCount = Max_Jetton_Count;

	WORD wPos = GetUserChairID(wChairID);
	const CCPoint & sPoint = m_HoldJetPoint[wPos];
	m_pPlaceBatchNode[wPos]->removeAllChildren();

	for (BYTE i = 0; i < cbJettonCount; ++i)
	{
		CCSprite * pCoin = NULL;
		if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Ox_Bg_Jetton, pCoin))
		{
			pCoin->setPosition(sPoint);
			pCoin->setVisible(false);
			m_pPlaceBatchNode[wPos]->addChild(pCoin);

			float fEndx = m_PlaceJetPoint[wPos].x + ((rand() % 2) ? 1 : -1) * (rand() % m_nJetAreaRadius);
			float fEndy = m_PlaceJetPoint[wPos].y + ((rand() % 2) ? 1 : -1) * (rand() % m_nJetAreaRadius);

			CCAction * pAction = CCSequence::create(CCDelayTime::create(0.05 * i), CCShow::create(),
				CCJumpTo::create(0.3f, ccp(fEndx, fEndy), 50, 1), NULL);
			pCoin->runAction(pAction);
		}
	}

	if (m_pLabelValue[wPos])
	{
		char szValue[64];
		snprintf(szValue, 64, "%lld", lJettonValue);
		m_pLabelValue[wPos]->setString(szValue);
		m_pLabelValue[wPos]->setVisible(true);
	}

	return ;
}

//处理注码
void COxCJ4JettonControl::GivePlaceJetton(WORD wGiveUser, WORD wAcceptUser)
{
	WORD wGivePos = GetUserChairID(wGiveUser);
	WORD wAcceptPos = GetUserChairID(wAcceptUser);

	const CCPoint & ePoint = m_HoldJetPoint[wAcceptPos];
	CCArray * pCoins = m_pPlaceBatchNode[wGivePos]->getChildren();

	if (pCoins && pCoins->count() > 0)
	{
		CCObject * child = NULL;
		int i = 0;
		CCARRAY_FOREACH_REVERSE(pCoins, child)
		{
			CCNode * pNode = (CCNode *)child;
			if (pNode)
			{
				CCAction * pAction = CCSequence::create(CCDelayTime::create(0.05f * i),
					CCJumpTo::create(0.3f, ePoint, 50, 1), CCCallFuncN::create(this, callfuncN_selector(COxCJ4JettonControl::RemoveSelf)), NULL);
				pNode->runAction(pAction);
				++i;
			}
		}
	}

	if (m_pLabelValue[wGivePos])
	{
		m_pLabelValue[wGivePos]->setString("0");
		m_pLabelValue[wGivePos]->setVisible(false);
	}

	return ;
}

//分数转移动画
void COxCJ4JettonControl::GiveHoldJetton(WORD wGiveUser, WORD wAcceptUser, BYTE cbJettonCount)
{
	WORD wGivePos = GetUserChairID(wGiveUser);
	WORD wAcceptPos = GetUserChairID(wAcceptUser);
	if (cbJettonCount > Max_Jetton_Count)
		cbJettonCount = Max_Jetton_Count;

	const CCPoint & sPoint = m_HoldJetPoint[wGivePos];
	CCPoint ePoint = m_HoldJetPoint[wAcceptPos];
	int nRadius = 25;

	if (wAcceptPos == 0 || wAcceptPos == 2)
		ePoint = ccp(ePoint.x - 70, ePoint.y + 20);
	else
		ePoint = ccp(ePoint.x + 40, ePoint.y + 105);

	for (BYTE i = 0; i < cbJettonCount; ++i)
	{
		CCSprite * pCoin = NULL;
		if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Ox_Bg_Jetton, pCoin))
		{
			pCoin->setPosition(sPoint);
			pCoin->setVisible(false);
			m_pHoldBatchNode->addChild(pCoin);
			CCPoint tPoint = ccp(ePoint.x + ((rand() % 2) ? 1 : -1) * (rand() % nRadius),
				ePoint.y + ((rand() % 2) ? 1 : -1) * (rand() % nRadius));

			CCAction * pAction = CCSequence::create(CCDelayTime::create(0.05f * i), CCShow::create(), CCJumpTo::create(0.3f, tPoint, rand() % 20 + 60, 1),
				CCCallFunc::create(pCoin, callfunc_selector(CCSprite::removeFromParent)), NULL);
			pCoin->runAction(pAction);
		}
	}

	return ;
}

void COxCJ4JettonControl::GiveWinningsJetton(WORD wChairID, BYTE cbJettonCount)
{
	WORD wPos = GetUserChairID(wChairID);
	if (cbJettonCount > Max_Jetton_Count)
		cbJettonCount = Max_Jetton_Count;

	CCPoint ePoint = m_HoldJetPoint[wPos];
	int nRadius = 25;

	if (wPos == 0 || wPos == 2)
		ePoint = ccp(ePoint.x - 70, ePoint.y + 20);
	else
		ePoint = ccp(ePoint.x + 40, ePoint.y + 105);

	for (BYTE i = 0; i < cbJettonCount; ++i)
	{
		CCSprite * pCoin = NULL;
		if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Ox_Bg_Jetton, pCoin))
		{
			pCoin->setPosition(ccp(this->getContentSize().width / 2, this->getContentSize().height / 2));
			pCoin->setVisible(false);
			m_pHoldBatchNode->addChild(pCoin);
			CCPoint tPoint = ccp(ePoint.x + ((rand() % 2) ? 1 : -1) * (rand() % nRadius),
				ePoint.y + ((rand() % 2) ? 1 : -1) * (rand() % nRadius));

			CCAction * pAction = CCSequence::create(CCDelayTime::create(0.05f * i), CCShow::create(), CCJumpTo::create(0.3f, tPoint, rand() % 50 + 50, 1),
				CCCallFunc::create(pCoin, callfunc_selector(CCSprite::removeFromParent)), NULL);
			pCoin->runAction(pAction);
		}
	}

	return ;
}

WORD COxCJ4JettonControl::GetUserChairID(WORD wChairID)
{
	WORD wMyChairID = m_pGameLayer->GetMeChairID();
	return CLayerBase::ServerTableIndexToLocal(wChairID, wMyChairID);
}

void COxCJ4JettonControl::RemoveSelf(CCNode * pRemNode)
{
	pRemNode->removeFromParentAndCleanup(true);
}

void COxCJ4JettonControl::SetNodeVertexZ(CCNode * pNode)
{
	pNode->setVertexZ(100.0f);
}

}
