#include "stdafx.h"
#include "PullMachResultLayer.h"
#include "PullMachResourceDef.h"
#include "PullMachResManager.h"
#include "GameLogic.h"
#include "PullMachRollNode.h"
#include "PullMachGameLayer.h"
#include "PullMachBgSceneryNode.h"
#include "PullMachPrizeRemainFontNode.h"
#include "ContinueUpdateLabelAtlas.h"
#include "GraduallyShowSprite.h"
#include <cmath>

namespace GamePullMach777
{

static const int sg_nMaxRollCount = 400;	//����������
static const int sg_nMaxScoreUpdateCount = 30;	//������������ʱ
static const int sg_nGirlMoveActionId = 100;	//Ů���ƶ�����ID
static const float sg_fGirlMoveSpeed = 120;	//Ů���ƶ��ٶ�(����/��)
static const int sg_nGMSpeedScaleY = 6;	//Ů���ƶ�y���ٶ�����

CPullMachResultLayer * CPullMachResultLayer::create(CPullMachGameLayer * pGameLayer, CPMResManager * pResManager)
{
	CPullMachResultLayer * pRet = new CPullMachResultLayer(pGameLayer, pResManager);

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

CPullMachResultLayer::CPullMachResultLayer(CPullMachGameLayer * pGameLayer, CPMResManager * pResManager)
	:m_pGameLayer(pGameLayer)
	,m_pResManager(pResManager)
	,m_pBgScenery(NULL)
	,m_pRemainPrizeTip(NULL)
	,m_pRollBgLight(NULL)
	,m_pPrizeTimesTip(NULL)
	,m_pBlueLightFrame(NULL)
	,m_pTreasureBox(NULL)
	,m_pFlyGirl(NULL)
	,m_pScroll(NULL)
	,m_pFontUserScore(NULL)
	,m_pFontAllWinLose(NULL)
	,m_pFontCreditScore(NULL)
	,m_pFontWinScore(NULL)
	,m_pPrizeRemainScore(NULL)
	,m_pFontPrizeTimes(NULL)
	,m_pUserFace(NULL)
	,m_FacePos()
	,m_pUserName(NULL)
	,m_pUserID(NULL)
	,m_bBigPrizeMode(false)
	,m_bOpenGivePrizeAni(false)
	,m_nLightFrameIndex(0)
{
	memset(m_pRollNode, 0, sizeof(m_pRollNode));
	memset(m_pBetCoin, 0, sizeof(m_pBetCoin));
	for (int i = 0; i < VER_LINE_NUM; ++i)
		m_nLogoCount[i] = -1;
	memset(m_nRollCount, 0, sizeof(m_nRollCount));
	memset(m_bStopRoll, 0, sizeof(m_bStopRoll));
	memset(m_pFontLogoNum, 0, sizeof(m_pFontLogoNum));
}

CPullMachResultLayer::~CPullMachResultLayer()
{

}

bool CPullMachResultLayer::init()
{
	if (!CCLayer::init())
		return false;

	CCSize size = this->getContentSize();
	float fMidX = size.width / 2;
	float fTopY = size.height;

	//��Ӿ�ɫ��ͼ
	m_pBgScenery = CPullMachBgSceneryNode::create(m_pResManager);
	m_pBgScenery->setPosition(ccp(fMidX - 385, fTopY - 601));
	this->addChild(m_pBgScenery, 1);

	CCLayerColor * pBgColor = CCLayerColor::create(ccc4(255, 255, 255, 255), 781, 430);
	pBgColor->setPosition(m_pBgScenery->getPosition());
	this->addChild(pBgColor, 0);

	CCSprite * pSprite = NULL;
	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, PullMach_Bg_Change, pSprite))
	{
		pSprite->setPosition(ccp(fMidX + 5, fTopY - 385));
		this->addChild(pSprite, 2);
	}

	//��ӷ���ͼ��
	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, PullMach_Bg_DangQianFenShu, pSprite))
	{
		pSprite->setAnchorPoint(ccp(0, 1));
		pSprite->setPosition(ccp(fMidX - 335, fTopY -195));
		this->addChild(pSprite, 10);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, PullMach_Bg_User_Score_Frame, pSprite))
	{
		pSprite->setAnchorPoint(CCPointZero);
		pSprite->setPosition(ccp(fMidX - 235, fTopY - 230));
		this->addChild(pSprite, 10);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, PullMach_Font_User_Score, m_pFontUserScore))
	{
		m_pFontUserScore->setAnchorPoint(ccp(1, 0));
		m_pFontUserScore->setPosition(ccp(fMidX - 12, fTopY - 219));
		m_pFontUserScore->setString("0");
		this->addChild(m_pFontUserScore, 11);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, PullMach_Bg_ZhongShuYing, pSprite))
	{
		pSprite->setAnchorPoint(ccp(0, 1));
		pSprite->setPosition(ccp(fMidX + 45, fTopY - 195));
		this->addChild(pSprite, 10);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, PullMach_Bg_User_Score_Frame, pSprite))
	{
		pSprite->setAnchorPoint(CCPointZero);
		pSprite->setPosition(ccp(fMidX + 120, fTopY - 230));
		this->addChild(pSprite, 10);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, PullMach_Font_User_Score, m_pFontAllWinLose))
	{
		m_pFontAllWinLose->setAnchorPoint(ccp(1, 0));
		m_pFontAllWinLose->setPosition(ccp(fMidX + 343, fTopY - 219));
		m_pFontAllWinLose->setString("0");
		this->addChild(m_pFontAllWinLose, 11);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, PullMach_Bg_Credit_Frame, pSprite))
	{
		pSprite->setAnchorPoint(CCPointZero);
		pSprite->setPosition(ccp(fMidX - 375, fTopY - 565));
		this->addChild(pSprite, 10);
	}

	m_pFontCreditScore = CContinueUpdateLabelAtlas::create(m_pResManager, eSpriteType_Cfg, PullMach_Font_Credit_Score);
	m_pFontCreditScore->setAnchorPoint(ccp(1, 0));
	m_pFontCreditScore->setPosition(ccp(fMidX - 283, fTopY - 557));
	this->addChild(m_pFontCreditScore, 11);

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, PullMach_Bg_Credit_Frame, pSprite))
	{
		pSprite->setAnchorPoint(CCPointZero);
		pSprite->setPosition(ccp(fMidX + 279, fTopY - 565));
		this->addChild(pSprite, 10);
	}

	m_pFontWinScore = CContinueUpdateLabelAtlas::create(m_pResManager, eSpriteType_Cfg, PullMach_Font_Win_Score);
	m_pFontWinScore->setAnchorPoint(ccp(1, 0));
	m_pFontWinScore->setPosition(ccp(fMidX + 371, fTopY - 557));
	this->addChild(m_pFontWinScore, 11);

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, PullMach_Font_Prize_Times, m_pFontPrizeTimes))
	{
		m_pFontPrizeTimes->setAnchorPoint(ccp(0.5, 1));
		m_pFontPrizeTimes->setPosition(ccp(fMidX + 348, fTopY - 475));
		m_pFontPrizeTimes->setVisible(false);
		this->addChild(m_pFontPrizeTimes, 10);
	}

	CCPoint logoNumPos[VER_LINE_NUM];
	logoNumPos[0].setPoint(fMidX - 170, fTopY - 330);
	logoNumPos[1].setPoint(fMidX, fTopY - 330);
	logoNumPos[2].setPoint(fMidX + 190, fTopY - 330);

	for (int i = 0; i < VER_LINE_NUM; ++i)
	{
		if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, PullMach_Font_Logo_Num, m_pFontLogoNum[i]))
		{
			m_pFontLogoNum[i]->setAnchorPoint(ccp(0.5, 0));
			m_pFontLogoNum[i]->setPosition(logoNumPos[i]);
			m_pFontLogoNum[i]->setString("");
			this->addChild(m_pFontLogoNum[i], 10);
		}
	}

	//��ӹ������ͼ��
	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, PullMach_Bg_Roll_Mach_Bg, m_pRollBgLight))
	{
		m_pRollBgLight->setPosition(ccp(fMidX + 8, fTopY - 430));
		m_pRollBgLight->setVisible(false);
		this->addChild(m_pRollBgLight, 20);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, PullMach_Bg_Roll_Mach, pSprite))
	{
		pSprite->setPosition(m_pRollBgLight->getPosition());
		this->addChild(pSprite, 21);
	}

	CCPoint rollPos[VER_LINE_NUM];
	rollPos[0].setPoint(fMidX - 232, fTopY - 516);
	rollPos[1].setPoint(fMidX - 51, fTopY - 516);
	rollPos[2].setPoint(fMidX + 131, fTopY - 516);
	for (int i = 0; i < VER_LINE_NUM; ++i)
	{
		m_pRollNode[i] = CPullMachRollNode::create(m_pResManager);
		m_pRollNode[i]->setPosition(rollPos[i]);
		this->addChild(m_pRollNode[i], 22);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, PullMach_Bg_Roll_Mach_Light, pSprite))
	{
		pSprite->setPosition(m_pRollBgLight->getPosition());
		this->addChild(pSprite, 23);
	}

	//��ʼ����ʾ���
	m_pRemainPrizeTip = CGraduallyShowSprite::create(m_pResManager, eSpriteType_Base, PullMach_Bg_ShengYuJiangLi);
	m_pRemainPrizeTip->setAnchorPoint(ccp(0, 0));
	m_pRemainPrizeTip->setPosition(ccp(fMidX - 155, fTopY - 320));
	m_pRemainPrizeTip->setVisible(false);
	this->addChild(m_pRemainPrizeTip, 30);

	m_pPrizeRemainScore = CPullMachPrizeRemainFontNode::create(m_pResManager);
	m_pPrizeRemainScore->setPosition(ccp(fMidX + 65, fTopY - 315));
	m_pPrizeRemainScore->setVisible(false);
	this->addChild(m_pPrizeRemainScore, 30);

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, PullMach_Bg_Box, m_pTreasureBox))
	{
		m_pTreasureBox->setPosition(ccp(fMidX - 347, fTopY - 514));
		m_pTreasureBox->setAnchorPoint(ccp(58.0f / 150, 30.0f / 150));
		this->addChild(m_pTreasureBox, 30);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, PullMach_Bg_Scroll, m_pScroll))
	{
		m_pScroll->setPosition(ccp(fMidX + 5, fTopY - 558));
		this->addChild(m_pScroll, 30);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, PullMach_Bg_Box, m_pFlyGirl))
	{
		CCSize bgSize = m_pScroll->getContentSize();	//�����ߴ�
		m_pFlyGirl->setPosition(ccp(bgSize.width / 2, bgSize.height / 2));
		m_pScroll->addChild(m_pFlyGirl);

		//�����ж���
		CCAnimation * pGirlFlyAni = CCAnimationCache::sharedAnimationCache()->
			animationByName(m_pResManager->getAnimationNameById(GameAni_GirlFly));
		m_pFlyGirl->runAction(CCAnimate::create(pGirlFlyAni));

		//�����ƶ�����
		CCPoint destPos(bgSize.width / 2, bgSize.height / 2);	//Ŀ��λ��
		CCAction * pMoveAction = CCSequence::create(
			CCMoveTo::create(10 / sg_fGirlMoveSpeed * sg_nGMSpeedScaleY, ccp(destPos.x, destPos.y + 10)),
			CCRepeat::create(
				CCSequence::create(
				CCMoveTo::create(20 / sg_fGirlMoveSpeed * sg_nGMSpeedScaleY, ccp(destPos.x, destPos.y - 10)),
				CCMoveTo::create(20 / sg_fGirlMoveSpeed * sg_nGMSpeedScaleY, ccp(destPos.x, destPos.y + 10)),
				NULL
				), -1
			),
			NULL
			);
		pMoveAction->setTag(sg_nGirlMoveActionId);
		m_pFlyGirl->runAction(pMoveAction);
	}

	for (int i = 0; i < CountArray(m_pBetCoin); ++i)
	{
		if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, PullMach_Bg_Silver_Coin, m_pBetCoin[i]))
		{
			m_pBetCoin[i]->setPosition(ccp(fMidX - 340 + i * 19, fTopY - 581));
			this->addChild(m_pBetCoin[i], 30);
		}
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, PullMach_Bg_Prize_Time_Tip, m_pPrizeTimesTip))
	{
		m_pPrizeTimesTip->setPosition(ccp(fMidX + 348, fTopY - 435));
		m_pPrizeTimesTip->setVisible(false);
		this->addChild(m_pPrizeTimesTip, 30);
	}

	//��ӽ��汳��ͼ��
	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, PullMach_Bg_Interface, pSprite))
	{
		pSprite->setAnchorPoint(ccp(0.5f, 1.0f));
		pSprite->setPosition(ccp(fMidX, fTopY));
		this->addChild(pSprite, 40);
	}

	//�����Ϣ
	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, PullMach_Bg_User_Info_Bg, pSprite))
	{
		pSprite->setPosition(ccp(fMidX - 13, fTopY - 82));
		this->addChild(pSprite, 41);
	}

	m_pUserFace = NULL;
	m_FacePos.setPoint(fMidX - 121, fTopY - 80);

	m_pUserID = CCLabelTTF::create("", "����", 25, CCSizeZero, kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
	//m_pUserName->setColor(ccc3(240, 170, 120));
	m_pUserID->setAnchorPoint(ccp(0, 0.5));
	m_pUserID->setPosition(ccp(fMidX - 45, fTopY - 62));
	this->addChild(m_pUserID, 42);

	m_pUserName = CCLabelTTF::create("", "����", 25, CCSizeZero, kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
	//m_pUserName->setColor(ccc3(240, 170, 120));
	m_pUserName->setAnchorPoint(ccp(0, 0.5));
	m_pUserName->setPosition(ccp(fMidX - 45, fTopY - 101));
	this->addChild(m_pUserName, 42);

	//�������߿�
	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, PullMach_Bg_Flash_Frame1, m_pBlueLightFrame))
	{
		m_pBlueLightFrame->setAnchorPoint(ccp(0.5f, 1.0f));
		m_pBlueLightFrame->setPosition(ccp(fMidX + 3, fTopY - 170));
		m_pBlueLightFrame->setVisible(false);
		this->addChild(m_pBlueLightFrame, 50);
	}

	return true;
}

void CPullMachResultLayer::setUserInfo(const tagLocalUser * pUserData)
{
	if (pUserData == NULL)
		return ;

	//����ͷ��
	if (m_pUserFace)
	{
		m_pUserFace->removeFromParent();
		m_pUserFace = NULL;
	}
	m_pUserFace = m_pGameLayer->GetGameDirector()->GetUserHeadSprite(pUserData->wFaceID);
	if (m_pUserFace)
	{
		m_pUserFace->setPosition(m_FacePos);
		m_pUserFace->setScale(0.7f);
		this->addChild(m_pUserFace, 40);
	}

	//����ID������
	char szID[64];
	snprintf(szID, sizeof(szID), "ID:%d", pUserData->nGameID);
	m_pUserID->setString(szID);
	m_pUserName->setString(pUserData->sUserName.c_str());
}

void CPullMachResultLayer::setRollResult(BYTE cbResult[CELL_NUM])
{
	//��ʼ����������
	memset(m_nRollCount, 0, sizeof(m_nRollCount));
	for (int i = 0; i < VER_LINE_NUM; ++i)
		setRollCount(i, sg_nMaxRollCount + i * 20);
	memset(m_bStopRoll, 0, sizeof(m_bStopRoll));

	//��������
	for (int i = 0; i < VER_LINE_NUM; ++i)
	{
		//�������й�������
		BYTE cbRollData[3] = {0};	//��������
		for (int j = 0; j < 3; ++j)
		{
			int nPos = UNIT_LINE * j + i;
			cbRollData[j] = cbResult[nPos];
		}
		m_pRollNode[i]->beginRollByResult(cbRollData);
	}

	//���ù���״̬У�鶨ʱ��
	schedule(schedule_selector(CPullMachResultLayer::rollSchedule));
}

void CPullMachResultLayer::setWinLineId(int nLineId)
{
	//����У��
	CC_ASSERT(nLineId >= 0 && nLineId < LINE_NUM);
	if (nLineId < 0 || nLineId >= LINE_NUM)
		return ;

	//ȡ���н�λ��
	int nWinPos[UNIT_LINE] = {0};
	memcpy(nWinPos, g_nLinesPosIndex[nLineId], sizeof(g_nLinesPosIndex[0]));

	for (int i = 0; i < UNIT_LINE; ++i)
	{
		int nPos = nWinPos[i];
		m_pRollNode[nPos % VER_LINE_NUM]->flashIcon(nPos / VER_LINE_NUM);
	}
}

void CPullMachResultLayer::setRollCount(int nIndex, int nCount)
{
	//����У��
	if (nIndex < 0 || nIndex >= VER_LINE_NUM)
	{
		CC_ASSERT(false);
		return ;
	}

	m_nRollCount[nIndex] = (nCount > 0) ? nCount : 0;
}

void CPullMachResultLayer::stopAllIconRoll()
{
	for (int i = 0; i < VER_LINE_NUM; ++i)
	{
		setRollCount(i, 0);
	}
}

void CPullMachResultLayer::stopIconRoll(int nIndex)
{
	CC_ASSERT(nIndex >= 0 && nIndex < VER_LINE_NUM);
	setRollCount(nIndex, 0);
}

void CPullMachResultLayer::setBigPrizeMode(bool bPrizeMode)
{
	if (m_bBigPrizeMode == bPrizeMode)
		return ;

	m_bBigPrizeMode = bPrizeMode;
	if (bPrizeMode)
	{
		m_pRollBgLight->setVisible(true);
		CCAction * pAction = CCRepeat::create(
			CCSequence::create(
				CCDelayTime::create(0.25f),
				CCHide::create(),
				CCDelayTime::create(0.25f),
				CCShow::create(),
				NULL
			), -1
			);
		m_pRollBgLight->runAction(pAction);
		m_pBgScenery->updateBgScenery(Scenery_BigPrize);
		for (int i = 0; i < VER_LINE_NUM; ++i)
		{
			m_pFontLogoNum[i]->setVisible(false);
		}
	}
	else
	{
		m_pRollBgLight->setVisible(false);
		m_pRollBgLight->stopAllActions();
		m_pBgScenery->updateBgScenery(Scenery_Normal);
		memset(m_nLogoCount, 0, sizeof(m_nLogoCount));
		for (int i = 0; i < VER_LINE_NUM; ++i)
		{
			m_pFontLogoNum[i]->setString("0");
			m_pFontLogoNum[i]->setVisible(true);
		}
	}

	m_pGameLayer->PlaySound(PullMach_Sound_Change_Scene);
}

bool CPullMachResultLayer::setLogoCount(const int nLogoCount[VER_LINE_NUM])
{
	//״̬�ж�
	if (m_bBigPrizeMode)
		return false;

	//����logo��Ŀ
	bool bUpdateCount = false;	//�Ƿ�����˸���
	for (int i = 0; i < VER_LINE_NUM; ++i)
	{
		//ȷ����Ŀ
		int nCount = (nLogoCount[i] > MAX_VER_LOGO_NUM) ? MAX_VER_LOGO_NUM : nLogoCount[i];
		if (nCount == m_nLogoCount[i])
			continue;

		//���¸���
		m_nLogoCount[i] = nCount;
		bUpdateCount = true;

		char szCount[32] = {0};
		snprintf(szCount, sizeof(szCount), "%d", m_nLogoCount[i]);
		m_pFontLogoNum[i]->setString(szCount);
	}

	return bUpdateCount;
}

void CPullMachResultLayer::setUserScore(LONGLONG lScore)
{
	if (lScore > 99999999999)
		lScore = 99999999999;
	else if (lScore < -9999999999)
		lScore = -9999999999;

	char szScore[64] = {0};
	snprintf(szScore, sizeof(szScore), "%lld", lScore);
	m_pFontUserScore->setString(szScore);
}

void CPullMachResultLayer::setAllWinLoseScore(LONGLONG lScore)
{
	if (lScore > 99999999999)
		lScore = 99999999999;
	else if (lScore < -9999999999)
		lScore = -9999999999;

	char szScore[64] = {0};
	snprintf(szScore, sizeof(szScore), "%lld", lScore);
	m_pFontAllWinLose->setString(szScore);
}

void CPullMachResultLayer::setCreditScore(LONGLONG lScore, bool bDirectShow)
{
	//����У��
	CC_ASSERT(lScore >= 0);
	if (lScore < 0)
		lScore = 0;

	//�����Ϸַ���
	lScore /= m_pGameLayer->getCellScore();
	if (lScore > 9999)
		lScore = 9999;
	if (bDirectShow)
	{
		//ֱ������
		m_pFontCreditScore->setScore(lScore);
	}
	else
	{
		//��������
		m_pFontCreditScore->continueUpdateScore(lScore, sg_nMaxScoreUpdateCount);
	}
}

void CPullMachResultLayer::setBetScore(int nBetScore)
{
	//������עӲ��
	for (int i = 0; i < CountArray(m_pBetCoin); ++i)
	{
		m_pBetCoin[i]->setVisible(false);
		CCAction * pAction = CCSequence::create(
			CCDelayTime::create(0.2f * (i + 1)),
			CCShow::create(),
			NULL
			);
		m_pBetCoin[i]->runAction(pAction);
	}

	m_pGameLayer->PlaySound(PullMach_Sound_Bet_Coin);
}

void CPullMachResultLayer::setWinScore(LONGLONG lScore, bool bDirectShow)
{
	//����У��
	CC_ASSERT(lScore >= 0);
	if (lScore < 0)
		lScore = 0;

	//����Ӯ��
	lScore /= m_pGameLayer->getCellScore();
	if (lScore > 9999)
		lScore = 9999;
	if (bDirectShow)
	{
		//ֱ������
		m_pFontWinScore->setScore(lScore);
	}
	else
	{
		//��������
		m_pFontWinScore->continueUpdateScore(lScore, sg_nMaxScoreUpdateCount);
	}
}

void CPullMachResultLayer::setBigPrizeRemainScore(LONGLONG lRemainScore, bool bDirectShow)
{
	//����У��
	CC_ASSERT(lRemainScore >= 0);
	if (lRemainScore < 0)
		lRemainScore = 0;

	//���´󽱵÷�
	lRemainScore /= m_pGameLayer->getCellScore();
	if (bDirectShow)
	{
		//ֱ������
		m_pPrizeRemainScore->setScore(lRemainScore);
	}
	else
	{
		//��������
		m_pPrizeRemainScore->continueUpdateScore(lRemainScore, sg_nMaxScoreUpdateCount);
	}
}

void CPullMachResultLayer::openGivePrizeAni(LONGLONG lPrizeScore, int nPrizeTimes)
{
	//����У��
	CC_ASSERT(lPrizeScore >= 0);
	if (lPrizeScore < 0)
		lPrizeScore = 0;

	//״̬�ж�
	if (m_bOpenGivePrizeAni == true)
		return ;
	m_bOpenGivePrizeAni = true;

	//��ʾʣ��󽱷�����ʾ
	float fUpdateRate = 0.02f;
	float fEachAddLegth = 10.0f;
	float fTipWidth = m_pRemainPrizeTip->getContentSize().width;
	m_pRemainPrizeTip->setVisible(true);
	m_pRemainPrizeTip->graduallyShow(fUpdateRate, fEachAddLegth, true);

	m_pPrizeRemainScore->setScore(lPrizeScore / m_pGameLayer->getCellScore());
	CCAction * pAction = CCSequence::create(
		CCHide::create(),
		CCDelayTime::create(fTipWidth / fEachAddLegth * __max(0.01f, fUpdateRate)),
		CCShow::create(),
		CCScaleTo::create(0.0f, 5.0f),
		CCScaleTo::create(1.0f, 0.5f),
		CCScaleTo::create(0.5f, 2.0f),
		CCScaleTo::create(0.2f, 1.0f),
		NULL
		);	//�������Ŷ���
	m_pPrizeRemainScore->runAction(pAction);

	m_pPrizeTimesTip->setVisible(true);
	char szTimes[32] = {0};
	snprintf(szTimes, sizeof(szTimes), "%d", nPrizeTimes);
	m_pFontPrizeTimes->setString(szTimes);
	m_pFontPrizeTimes->setVisible(true);

	//��������
	m_pBgScenery->updateBgScenery(Scenery_GivePrize);
	openBlue7Ani(true);	//������7����
	openBoxPrizeAni(true);	//�������䶯��
	openLogoAni(true);	//������Ů����
	m_pGameLayer->PlaySound(PullMach_Sound_Get_Prize);
}

void CPullMachResultLayer::closeGivePrizeAni()
{
	//��λ��״̬
	m_bOpenGivePrizeAni = false;
	m_pRemainPrizeTip->setVisible(false);
	m_pPrizeRemainScore->setVisible(false);
	m_pPrizeRemainScore->setScore(0);
	openBlue7Ani(false);
	openBoxPrizeAni(false);
	openLogoAni(false);
	m_pPrizeTimesTip->setVisible(false);
	m_pFontPrizeTimes->setVisible(false);
}

void CPullMachResultLayer::openBlue7Ani(bool bOpen)
{
	if (bOpen)	//�������⶯��
	{
		m_pBlueLightFrame->setVisible(true);
		m_nLightFrameIndex = 0;
		m_pResManager->resetSpriteTexture(m_pBlueLightFrame, PullMach_Bg_Flash_Frame1);
		schedule(schedule_selector(CPullMachResultLayer::flashBlue7FrameSchedule), 0.25f);
	}
	else	//�ر����⶯��
	{
		m_pBlueLightFrame->setVisible(false);
		unschedule(schedule_selector(CPullMachResultLayer::flashBlue7FrameSchedule));
	}
}

void CPullMachResultLayer::openRed7Ani(bool bOpen)
{
	static int n1 = 0, n2 = 0;
	if (bOpen)	//������7����
	{
		printf("red7 open count %d\n", ++n1);
		m_pTreasureBox->stopAllActions();
		CCAnimation * pRed7Ani = CCAnimationCache::sharedAnimationCache()->
			animationByName(m_pResManager->getAnimationNameById(GameAni_BoxRed7));
		pRed7Ani->setRestoreOriginalFrame(false);
		CCAnimation * pRed7Swing = CCAnimationCache::sharedAnimationCache()->
			animationByName(m_pResManager->getAnimationNameById(GameAni_Red7Swing));
		pRed7Ani->setRestoreOriginalFrame(false);

		CCAction * pAction = CCSequence::create(
			CCAnimate::create(pRed7Ani),
			CCAnimate::create(pRed7Swing),
			NULL
			);
		m_pTreasureBox->setAnchorPoint(ccp(88.0f / 200, 26.0f / 200));
		m_pResManager->resetSpriteFrameTexture(m_pTreasureBox, PullMach_AniBegin_Box_Open1);
		m_pTreasureBox->runAction(pAction);
	}
	else	//�رպ�7����
	{
		printf("red7 close count %d\n", ++n2);

		m_pTreasureBox->stopAllActions();
		m_pTreasureBox->setAnchorPoint(ccp(58.0f / 150, 30.0f / 150));
		m_pResManager->resetSpriteFrameTexture(m_pTreasureBox, PullMach_Bg_Box);
	}
}

void CPullMachResultLayer::openLogoAni(bool bOpen)
{
	CCSize bgSize = m_pScroll->getContentSize();	//�����ߴ�
	CCSize girlSize = m_pFlyGirl->getContentSize();	//Ů���ߴ�
	CCPoint destPos(bgSize.width / 2, bgSize.height / 2);	//Ŀ��λ��
	CCPoint srcPos = m_pFlyGirl->getPosition();	//��ǰλ��

	if (bOpen)	//����Ů�����ж���
	{
		m_pFlyGirl->stopActionByTag(sg_nGirlMoveActionId);
		float fReplaceMaxTime = __max(std::fabs(destPos.y - srcPos.y) / sg_fGirlMoveSpeed * sg_nGMSpeedScaleY,
			std::fabs(destPos.x - srcPos.x) / sg_fGirlMoveSpeed);	//��λ����ʱ
		CCAction * pMoveAction = CCSequence::create(
			CCMoveTo::create(fReplaceMaxTime, destPos),
			CCDelayTime::create(0.2f),
			CCMoveTo::create((destPos.x - girlSize.width / 2) / sg_fGirlMoveSpeed, ccp(girlSize.width / 2, destPos.y)),
			CCRepeat::create(
				CCSequence::create(
				CCMoveTo::create((bgSize.width - girlSize.width) / sg_fGirlMoveSpeed, ccp(bgSize.width - girlSize.width / 2, destPos.y)),
				CCMoveTo::create((bgSize.width - girlSize.width) / sg_fGirlMoveSpeed, ccp(girlSize.width / 2, destPos.y)),
				NULL
				), -1
			),
			NULL
			);
		pMoveAction->setTag(sg_nGirlMoveActionId);
		m_pFlyGirl->runAction(pMoveAction);
	}
	else	//�ر�Ů�����ж���
	{
		m_pFlyGirl->stopActionByTag(sg_nGirlMoveActionId);
		float fReplaceMaxTime = __max(std::fabs(destPos.y - srcPos.y) / sg_fGirlMoveSpeed * sg_nGMSpeedScaleY,
			std::fabs(destPos.x - srcPos.x) / sg_fGirlMoveSpeed);	//��λ����ʱ
		CCAction * pMoveAction = CCSequence::create(
			CCMoveTo::create(fReplaceMaxTime, destPos),
			CCDelayTime::create(0.2f),
			CCMoveTo::create(10 / sg_fGirlMoveSpeed * sg_nGMSpeedScaleY, ccp(destPos.x, destPos.y + 10)),
			CCRepeat::create(
				CCSequence::create(
				CCMoveTo::create(20 / sg_fGirlMoveSpeed * sg_nGMSpeedScaleY, ccp(destPos.x, destPos.y - 10)),
				CCMoveTo::create(20 / sg_fGirlMoveSpeed * sg_nGMSpeedScaleY, ccp(destPos.x, destPos.y + 10)),
				NULL
				), -1
			),
			NULL
			);
		pMoveAction->setTag(sg_nGirlMoveActionId);
		m_pFlyGirl->runAction(pMoveAction);
	}
}

void CPullMachResultLayer::openBoxPrizeAni(bool bOpen)
{
	if (bOpen)	//�������䶯��
	{
		m_pTreasureBox->stopAllActions();
		CCAnimation * pBoxPrizeAni = CCAnimationCache::sharedAnimationCache()->
			animationByName(m_pResManager->getAnimationNameById(GameAni_BoxLight));
		pBoxPrizeAni->setRestoreOriginalFrame(false);
		m_pTreasureBox->setAnchorPoint(ccp(58.0f / 150, 30.0f / 150));
		m_pTreasureBox->runAction(CCAnimate::create(pBoxPrizeAni));
	}
	else	//�رձ��䶯��
	{
		m_pTreasureBox->stopAllActions();
		m_pTreasureBox->setAnchorPoint(ccp(58.0f / 150, 30.0f / 150));
		m_pResManager->resetSpriteFrameTexture(m_pTreasureBox, PullMach_Bg_Box);
	}
}

void CPullMachResultLayer::rollSchedule(float dt)
{
	bool bAllRollingStop = true;	//�Ƿ�����ͼ���ֹͣת��

	//���¸�ͼ��ת��״̬
	for (int i = 0; i < VER_LINE_NUM; ++i)
	{
		if (m_nRollCount[i] <= 0)
		{
			if (m_bStopRoll[i] == false)
			{
				m_bStopRoll[i] = true;
				m_pRollNode[i]->endRollIconLayer();
				m_pGameLayer->PlaySound(PullMach_Sound_Stop);
			}
		}
		else
		{
			--m_nRollCount[i];
		}
	}

	//�ж��Ƿ�ȫ������ֹͣ
	for (int i = 0; i < VER_LINE_NUM; ++i)
	{
		if (m_bStopRoll[i] == false || (m_pRollNode[i]->isRolling() == true))
		{
			bAllRollingStop = false;
			break;
		}
	}

	if (bAllRollingStop)
	{
		unschedule(schedule_selector(CPullMachResultLayer::rollSchedule));
		m_pGameLayer->allIconStopRoll();
	}
}

void CPullMachResultLayer::flashBlue7FrameSchedule(float dt)
{
	m_nLightFrameIndex = (m_nLightFrameIndex + 1) % 2;
	m_pResManager->resetSpriteTexture(m_pBlueLightFrame, PullMach_Bg_Flash_Frame1 + m_nLightFrameIndex);
}


}
