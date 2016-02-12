#include "stdafx.h"
#include "Star97ResultLayer.h"
#include "ResourceDef.h"
#include "ResManager.h"
#include "Star97RollNode.h"
#include "Star97GameLayer.h"
#include "LayerBase.h"
#include <cmath>

namespace GameStar97
{

static const int g_nMaxRollCount = 400;	//最大滚动次数
static const int g_nMinRollCount = 200;	//最小滚动次数
static const int g_nMaxScoreUpdateCount = 180;	//分数更新最多耗时

CStar97ResultLayer * CStar97ResultLayer::create(CStar97GameLayer * pGameLayer, CResManager * pResManager)
{
	CStar97ResultLayer * pRet = new CStar97ResultLayer(pGameLayer, pResManager);

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

CStar97ResultLayer::CStar97ResultLayer(CStar97GameLayer * pGameLayer, CResManager * pResManager)
	:CLayerBase(pResManager)
	,m_pGameLayer(pGameLayer)
	,m_pResManager(pResManager)
	,m_pAutoTips(NULL)
	,m_pHaleSprite(NULL)
	,m_pBonusSprite(NULL)
	,m_pExtraBg(NULL)
	,m_pStatusTips(NULL)
	,m_pFontHandselPool(NULL)
	,m_pFontHandselScore(NULL)
	,m_pFontNormalScore(NULL)
	,m_pFontCreditScore(NULL)
	,m_pFontBetScore(NULL)
	,m_pFontStarRatio(NULL)
	,m_pFontStarCount(NULL)
	,m_lHandselPool(0)
	,m_lExtraScore(0)
	,m_lHandselScore(0)
	,m_lNormalScore(0)
	,m_lCreditScore(0)
	,m_nBetScore(0)
	,m_lUpdateRate(0)
	,m_nStarLevel(0)
	,m_nStarCount(0)
	,m_eRollType(RollType_Normal)
{
	memset(m_pRollNode, 0, sizeof(m_pRollNode));
	memset(m_pFlashFrame, 0, sizeof(m_pFlashFrame));
	memset(m_pWinLine, 0, sizeof(m_pWinLine));
	memset(m_pFontExtraScore, 0, sizeof(m_pFontExtraScore));
	memset(m_pFontLineScore, 0, sizeof(m_pFontLineScore));
	memset(m_nRollCount, 0, sizeof(m_nRollCount));
	memset(m_bStopRoll, 0, sizeof(m_bStopRoll));
}

CStar97ResultLayer::~CStar97ResultLayer()
{

}

bool CStar97ResultLayer::init()
{
	if (!CCLayer::init())
		return false;

	//初始化图标
	float fRollPosX[CELL_NUM] = {
		138.0f, 362.0f, 583.0f, 138.0f, 362.0f, 583.0f, 138.0f, 362.0f, 583.0f, 
	};
	float fRollPosY[CELL_NUM] = {
		411.0f, 411.0f, 411.0f, 237.0f, 237.0f, 237.0f, 62.0f, 62.0f, 62.0f,
	};
	for (int i = 0; i < 9; ++i)
	{
		m_pRollNode[i] = CStar97RollNode::create(m_pResManager);
		if (i < 9)m_pRollNode[i]->setPosition(ccp(fRollPosX[i], fRollPosY[i]));
		this->addChild(m_pRollNode[i]);
	}

	//初始化闪烁框
	for (int i = 0; i < CELL_NUM; ++i)
	{
		if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Star97_Bg_Flash_Frame, m_pFlashFrame[i]))
		{
			m_pFlashFrame[i]->setAnchorPoint(CCPointZero);
			m_pFlashFrame[i]->setPosition(ccp(fRollPosX[i], fRollPosY[i]));
			this->addChild(m_pFlashFrame[i], 20);
			m_pFlashFrame[i]->setVisible(false);
		}
	}

	//初始化中奖线
	float fLinePosX[LINE_NUM] = {
		123.0f, 123.0f, 123.0f, 123.0f, 123.0f, 236.0f, 459.0f, 683.0f,
	};
	float fLinePosY[LINE_NUM] = {
		310.0f, 484.0f, 136.0f, 64.0f, 62.0f, 64.0f, 64.0f, 64.0f,
	};
	for (int i = 0; i < LINE_NUM; ++i)
	{
		if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, g_vLineResList[i], m_pWinLine[i]))
		{
			m_pWinLine[i]->setAnchorPoint(CCPointZero);
			m_pWinLine[i]->setPosition(ccp(fLinePosX[i], fLinePosY[i]));
			this->addChild(m_pWinLine[i], 10);
			m_pWinLine[i]->setVisible(false);
		}
	}

	//初始化界面其他精灵
	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Star97_Bg_Auto_Tips, m_pAutoTips))
	{
		m_pAutoTips->setPosition(ccp(400, 400));
		this->addChild(m_pAutoTips, 30);
		m_pAutoTips->setVisible(false);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Star97_Bg_Half_Tag, m_pHaleSprite))
	{
		m_pHaleSprite->setPosition(ccp(69, 435));
		this->addChild(m_pHaleSprite, 10);
		m_pHaleSprite->setVisible(false);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Star97_Bg_Bonus_Tag, m_pBonusSprite))
	{
		m_pBonusSprite->setPosition(ccp(69, 400));
		this->addChild(m_pBonusSprite, 10);
		m_pBonusSprite->setVisible(false);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Star97_Bg_Extra_Reward, m_pExtraBg))
	{
		m_pExtraBg->setPosition(ccp(463, 313));
		this->addChild(m_pExtraBg, 25);
		m_pExtraBg->setVisible(false);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Star97_Bg_Game_Over, m_pStatusTips))
	{
		m_pStatusTips->setPosition(ccp(413.0f, 26.0f));
		this->addChild(m_pStatusTips, 10);
	}

	//创建上分提示动画
	{
		CCAnimation * animation = CCAnimation::create();
		animation->addSpriteFrame(getSpriteFrameByCfgId(Star97_Bg_Push_Bet));
		animation->addSpriteFrame(getSpriteFrameByCfgId(Star97_Bg_Push_Start));
		animation->setDelayPerUnit(0.5f);
		animation->setLoops(-1);
		CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "BetScore");

		m_pStatusTips->runAction(CCAnimate::create(animation));
	}

	//初始化各分数信息
	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Handsel, m_pFontHandselPool))
	{
		m_pFontHandselPool->setAnchorPoint(ccp(0.5f, 0.5f));
		m_pFontHandselPool->setPosition(ccp(93, 587));
		m_pFontHandselPool->setString("0");
		this->addChild(m_pFontHandselPool, 10);
	}

	for (int i = 0; i < CountArray(m_pFontExtraScore); ++i)
	{
		if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Extra, m_pFontExtraScore[i]))
		{
			m_pFontExtraScore[i]->setAnchorPoint(ccp(0.5f, 0.5f));
			m_pFontExtraScore[i]->setPosition(ccp(78 + i * 59, 101));
			m_pFontExtraScore[i]->setString("");
			m_pExtraBg->addChild(m_pFontExtraScore[i], 10);
		}
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Common, m_pFontHandselScore))
	{
		m_pFontHandselScore->setAnchorPoint(ccp(0.5f, 0.5f));
		m_pFontHandselScore->setPosition(ccp(69, 364));
		m_pFontHandselScore->setString("");
		this->addChild(m_pFontHandselScore, 10);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Common, m_pFontNormalScore))
	{
		m_pFontNormalScore->setAnchorPoint(ccp(0.0f, 0.5f));
		m_pFontNormalScore->setPosition(ccp(470, 40));
		m_pFontNormalScore->setString("");
		this->addChild(m_pFontNormalScore, 10);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Push_Score, m_pFontCreditScore))
	{
		m_pFontCreditScore->setAnchorPoint(ccp(0.5f, 0.5f));
		m_pFontCreditScore->setPosition(ccp(86, 22));
		m_pFontCreditScore->setString("0");
		this->addChild(m_pFontCreditScore, 10);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Push_Score, m_pFontBetScore))
	{
		m_pFontBetScore->setAnchorPoint(ccp(0.5f, 0.5f));
		m_pFontBetScore->setPosition(ccp(725, 22));
		m_pFontBetScore->setString("0");
		this->addChild(m_pFontBetScore, 10);
	}

	float fLineBetPosX[LINE_NUM] = {
		105.0f, 105.0f, 105.0f, 105.0f, 105.0f, 275.0f, 500.0f, 725.0f,
	};
	float fLineBetPosY[LINE_NUM] = {
		313.0f, 485.0f, 139.0f, 553.0f, 72.0f, 587.0f, 587.0f, 587.0f,
	};
	for (int i = 0; i < LINE_NUM; ++i)
	{
		if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Common, m_pFontLineScore[i]))
		{
			m_pFontLineScore[i]->setAnchorPoint(ccp(1.0f, 0.5f));
			m_pFontLineScore[i]->setPosition(ccp(fLineBetPosX[i], fLineBetPosY[i]));
			m_pFontLineScore[i]->setString("0");
			this->addChild(m_pFontLineScore[i], 10);
		}
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Common, m_pFontStarRatio))
	{
		m_pFontStarRatio->setAnchorPoint(ccp(1.0f, 0.5f));
		m_pFontStarRatio->setPosition(ccp(105, 205));
		m_pFontStarRatio->setString("");
		this->addChild(m_pFontStarRatio, 10);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_Handsel, m_pFontStarCount))
	{
		m_pFontStarCount->setAnchorPoint(ccp(1.0f, 0.5f));
		m_pFontStarCount->setPosition(ccp(105, 250));
		m_pFontStarCount->setString("");
		this->addChild(m_pFontStarCount, 10);
	}

	return true;
}

void CStar97ResultLayer::setCreditScore(LONGLONG lScore)
{
	//参数校验
	CC_ASSERT(lScore >= 0);
	if (lScore < 0)
		return ;

	//更新上分
	m_lCreditScore = lScore;

	char szScore[256] = {0};
	snprintf(szScore, sizeof(szScore), "%lld", m_lCreditScore);
	m_pFontCreditScore->setString(szScore);
}

void CStar97ResultLayer::betScore(int nBetScore, int nLineBet)
{
	//参数校验
	CC_ASSERT(nBetScore >= 0 && nLineBet >= 0);
	if (nBetScore < 0 || nLineBet < 0)
		return ;

	m_nBetScore = nBetScore;

	//更新分数
	char szScore[256] = {0};
	snprintf(szScore, sizeof(szScore), "%d", m_nBetScore);
	m_pFontBetScore->setString(szScore);

	snprintf(szScore, sizeof(szScore), "%lld", m_lCreditScore - m_nBetScore);
	m_pFontCreditScore->setString(szScore);

	for (int i = 0; i < LINE_NUM; ++i)
	{
		snprintf(szScore, sizeof(szScore), "%d", nLineBet);
		m_pFontLineScore[i]->setString(szScore);
	}
}

void CStar97ResultLayer::setNormalScore(LONGLONG lScore)
{
	//参数校验
	CC_ASSERT(lScore >= 0);
	if (lScore < 0)
		return ;

	//更新得分
	m_lNormalScore = lScore;

	char szScore[256] = {0};
	snprintf(szScore, sizeof(szScore), "%lld", m_lNormalScore);
	m_pFontNormalScore->setString(szScore);
}

void CStar97ResultLayer::setHandselScore(LONGLONG lScore, BYTE cbHandselRatio)
{
	//参数校验
	CC_ASSERT(lScore >= 0);
	if (lScore < 0)
		return ;

	//更新得分
	m_lHandselScore = lScore;

	if (lScore == 0)
	{
		m_pFontHandselScore->setString("");
	}
	else
	{
		char szScore[256] = {0};
		snprintf(szScore, sizeof(szScore), "%lld", m_lHandselScore);
		m_pFontHandselScore->setString(szScore);

		if (cbHandselRatio == 1)
			m_pHaleSprite->setVisible(true);
		m_pBonusSprite->setVisible(true);
	}
}

void CStar97ResultLayer::setRollResult(BYTE cbResult[CELL_NUM], BYTE cbStarRatio)
{
	//更新押分
	m_lCreditScore -= m_nBetScore;
	char szScore[256] = {0};
	snprintf(szScore, sizeof(szScore), "%lld", m_lCreditScore);
	m_pFontCreditScore->setString(szScore);

	//设置转动
	if (cbStarRatio == 0)
	{
		m_eRollType = RollType_Normal;
		memset(m_nRollCount, 0, sizeof(m_nRollCount));
		int nRollRegion = g_nMaxRollCount - g_nMinRollCount + 1;
		for (int i = 0; i < CELL_NUM; ++i)
		{
			setRollCount(i, rand() % nRollRegion + g_nMinRollCount);
		}
		memset(m_bStopRoll, 0, sizeof(m_bStopRoll));

		//开始正常滚动
		for (int i = 0; i < CELL_NUM; ++i)
		{
			m_pRollNode[i]->beginRollIconLayer(cbResult[i] - 1);
		}
	}
	else
	{
		m_eRollType = RollType_Star;
		memset(m_nRollCount, 0, sizeof(m_nRollCount));
		memset(m_bStopRoll, 0, sizeof(m_bStopRoll));

		//开始明星滚动
		for (int i = 0; i < CELL_NUM; ++i)
		{
			m_pRollNode[i]->setStarPrize(cbStarRatio, i);
		}
	}

	//设置滚动状态校验定时器
	schedule(schedule_selector(CStar97ResultLayer::rollSchedule));
}

void CStar97ResultLayer::setRollCount(int nIndex, int nCount)
{
	//参数校验
	if (nIndex < 0 || nIndex >= CELL_NUM)
	{
		CC_ASSERT(false);
		return ;
	}

	if (m_eRollType == RollType_Star)
		return ;

	m_nRollCount[nIndex] = (nCount > 0) ? nCount : 0;
}

void CStar97ResultLayer::stopAllIconRoll()
{
	for (int i = 0; i < CELL_NUM; ++i)
	{
		setRollCount(i, 0);
	}
}

void CStar97ResultLayer::stopIconRoll(const bool bStop[CELL_NUM])
{
	for (int i = 0; i < CELL_NUM; ++i)
	{
		if (bStop[i])
		{
			setRollCount(i, 0);
		}
	}
}

void CStar97ResultLayer::setFlashFrameVisible(bool bFlashFrameVisible[CELL_NUM])
{
	for (int i = 0; i < CELL_NUM; ++i)
	{
		m_pFlashFrame[i]->setVisible(bFlashFrameVisible[i]);
	}
}

void CStar97ResultLayer::setLinesVisible(bool bLinesVisible[LINE_NUM])
{
	for (int i = 0; i < LINE_NUM; ++i)
	{
		m_pWinLine[i]->setVisible(bLinesVisible[i]);
	}
}

void CStar97ResultLayer::setStatusInfo(enGameStatusInfo eStatus)
{
	switch(eStatus)
	{
	case GSInfo_Bet:
		{
			CCAnimation * pAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("BetScore");
			m_pStatusTips->runAction(CCAnimate::create(pAnimation));
			break;
		}
	case GSInfo_Roll:
		{
			m_pStatusTips->stopAllActions();
			resetSpriteTexture(m_pStatusTips, Star97_Bg_Insert_Coin);
			break;
		}
	case GSInfo_GameOver:
		{
			resetSpriteTexture(m_pStatusTips, Star97_Bg_Game_Over);
			break;
		}
	case GSInfo_YouWin:
		{
			resetSpriteTexture(m_pStatusTips, Star97_Bg_You_Win);
			break;
		}
	default:
		CC_ASSERT(false);
		return ;
	}
}

void CStar97ResultLayer::setHandselPoolScore(LONGLONG lScore)
{
	CC_ASSERT(lScore >= 0);
	if (lScore < 0)
		return ;

	m_lHandselPool = lScore;
	char szScore[256] = {0};
	snprintf(szScore, sizeof(szScore), "%lld", lScore);
	m_pFontHandselPool->setString(szScore);
}

void CStar97ResultLayer::resetScoreInfo()
{
	//游戏状态判断
	if (m_pGameLayer->GetGameStatus() != GS_WK_FREE)
	{
		CC_ASSERT(false);
		return ;
	}

	//初始化变量
	m_lExtraScore = 0;
	m_lHandselScore = 0;
	m_lNormalScore = 0;
	m_lUpdateRate = 0;
	m_eRollType = RollType_Normal;
	memset(m_nRollCount, 0, sizeof(m_nRollCount));
	memset(m_bStopRoll, 0, sizeof(m_bStopRoll));

	m_pFontHandselScore->setString("");
	m_pFontNormalScore->setString("");

	//更新彩金标识显示
	if (m_pHaleSprite->isVisible())
		m_pHaleSprite->setVisible(false);
	if (m_pBonusSprite->isVisible())
		m_pBonusSprite->setVisible(false);
}

void CStar97ResultLayer::updateStarCount()
{
	//更新明星中奖次数
	if (m_nStarCount > 0)
	{
		--m_nStarCount;
		if (m_nStarCount == 0)
		{
			m_pFontStarRatio->setString("");
			m_pFontStarCount->setString("");
			m_nStarLevel = 0;
		}
		else
		{
			char szNum[256] = {0};
			snprintf(szNum, sizeof(szNum), "%d", m_nStarCount);
			m_pFontStarCount->setString(szNum);
		}
	}
}

void CStar97ResultLayer::setAutoTipsVisible(bool bVisible)
{
	m_pAutoTips->setVisible(bVisible);
}

void CStar97ResultLayer::setStarWinScore(LONGLONG lWinScore, int nStarLevel, int nStarCount)
{
	//参数校验
	CC_ASSERT(lWinScore > 0);
	CC_ASSERT(nStarLevel >= 2 && nStarLevel <= 4);
	CC_ASSERT(nStarCount > 0);
	if (lWinScore <= 0 || nStarLevel < 2 || nStarLevel > 4 || nStarCount <= 0)
		return ;

	//更新分数
	m_lExtraScore = 0;
	m_lHandselScore = 0;
	m_lNormalScore = lWinScore;
	m_nStarLevel = nStarLevel;
	m_nStarCount = nStarCount;

	char szNum[256] = {0};
	snprintf(szNum, sizeof(szNum), "%lld", lWinScore);
	m_pFontNormalScore->setString(szNum);

	snprintf(szNum, sizeof(szNum), "*%d", nStarLevel);
	m_pFontStarRatio->setString(szNum);

	snprintf(szNum, sizeof(szNum), "%d", nStarCount);
	m_pFontStarCount->setString(szNum);

	//更新自然得分
	m_lUpdateRate = 0;
	computeUpdateRate(lWinScore, m_lUpdateRate);
	schedule(schedule_selector(CStar97ResultLayer::normalScoreSchedule));
}

void CStar97ResultLayer::setNormalWinScore(const tagWinScore & winScore, int nCellScore, BYTE cbHandselRatio)
{
	//更新分数
	m_lExtraScore = winScore.lExtraScore / nCellScore;
	m_lHandselScore = winScore.lHandselScore / nCellScore;
	m_lNormalScore = winScore.lNormalScore / nCellScore;
	m_lUpdateRate = 0;
	printf("begin handselpool:%lld, handselScore:%lld, shenyu:%lld\n", m_lHandselPool, m_lHandselScore, m_lHandselPool - m_lHandselScore);

	if (m_lHandselScore > 0)
	{
		char szScore[256] = {0};
		snprintf(szScore, sizeof(szScore), "%lld", m_lHandselScore);
		m_pFontHandselScore->setString(szScore);
	}
	else
	{
		m_pFontHandselScore->setString("");
	}

	if (m_lNormalScore > 0)
	{
		char szScore[256] = {0};
		snprintf(szScore, sizeof(szScore), "%lld", m_lNormalScore);
		m_pFontNormalScore->setString(szScore);
	}
	else
	{
		m_pFontNormalScore->setString("");
	}

	//设置更新分数定时器
	if (m_lHandselScore > 0)
	{
		if (cbHandselRatio == 1)
			m_pHaleSprite->setVisible(true);
		m_pBonusSprite->setVisible(true);

		computeUpdateRate(m_lHandselScore, m_lUpdateRate);
		schedule(schedule_selector(CStar97ResultLayer::handelScoreSchedule));
	}
	else if (m_lNormalScore > 0)
	{
		computeUpdateRate(m_lNormalScore, m_lUpdateRate);
		schedule(schedule_selector(CStar97ResultLayer::normalScoreSchedule));
	}
	else if (m_lExtraScore > 0)
	{
		m_pExtraBg->setVisible(true);
		setExtraFontScore(m_lExtraScore);

		computeUpdateRate(m_lExtraScore, m_lUpdateRate);
		schedule(schedule_selector(CStar97ResultLayer::extraScoreSchedule));
	}
	else
	{
		m_pGameLayer->resultSetOver();
	}
}

void CStar97ResultLayer::rollSchedule(float dt)
{
	bool bAllRollingStop = true;	//是否所有图标均停止转动

	if (m_eRollType == RollType_Normal)
	{
		//更新各图标转动状态
		bool bCanStopSound = true;	//是否可停止音效
		for (int i = 0; i < CELL_NUM; ++i)
		{
			if (m_nRollCount[i] <= 0)
			{
				if (m_bStopRoll[i] == false)
				{
					m_bStopRoll[i] = true;
					m_pRollNode[i]->endRollIconLayer();
				}
			}
			else
			{
				--m_nRollCount[i];
			}

			if (bCanStopSound && !m_pRollNode[i]->canStopSound())
			{
				bCanStopSound = false;
			}
		}

		if (bCanStopSound)
			m_pGameLayer->stopRollSound();

		//判断是否全都真正停止
		for (int i = 0; i < CELL_NUM; ++i)
		{
			if (m_bStopRoll[i] == false || (m_pRollNode[i]->isRolling() == true))
			{
				bAllRollingStop = false;
				break;
			}
		}
	}
	else if (m_eRollType == RollType_Star)
	{
		//判断是否全都真正停止
		for (int i = 0; i < CELL_NUM; ++i)
		{
			if (m_pRollNode[i]->isRolling() == true)
			{
				bAllRollingStop = false;
				break;
			}
		}
	}

	if (bAllRollingStop)
	{
		unschedule(schedule_selector(CStar97ResultLayer::rollSchedule));
		m_pGameLayer->allIconStopRoll();
	}
}

void CStar97ResultLayer::handelScoreSchedule(float dt)
{
	CC_ASSERT(m_lUpdateRate > 0);

	//更新分数数值
	LONGLONG lRealUpdate = m_lUpdateRate;	//真正更新分数
	if (m_lHandselScore - lRealUpdate < 0)
		lRealUpdate = m_lHandselScore;

	m_lHandselScore -= lRealUpdate;
	m_lCreditScore += lRealUpdate;
	m_lHandselPool -= lRealUpdate;

	//更新分数显示
	char szScore[256] = {0};
	snprintf(szScore, sizeof(szScore), "%lld", m_lHandselScore);
	m_pFontHandselScore->setString(szScore);

	snprintf(szScore, sizeof(szScore), "%lld", m_lCreditScore);
	m_pFontCreditScore->setString(szScore);

	snprintf(szScore, sizeof(szScore), "%lld", m_lHandselPool);
	m_pFontHandselPool->setString(szScore);

	//启动自然得分更新
	if (m_lHandselScore <= 0)
	{
		unschedule(schedule_selector(CStar97ResultLayer::handelScoreSchedule));

		if (m_lNormalScore > 0)
		{
			computeUpdateRate(m_lNormalScore, m_lUpdateRate);
			schedule(schedule_selector(CStar97ResultLayer::normalScoreSchedule));
		}
		else
		{
			m_pGameLayer->resultSetOver();
		}
	}
}

void CStar97ResultLayer::normalScoreSchedule(float dt)
{
	CC_ASSERT(m_lUpdateRate > 0);

	//更新分数数值
	LONGLONG lRealUpdate = m_lUpdateRate;
	if (m_lNormalScore - lRealUpdate < 0)
		lRealUpdate = m_lNormalScore;

	m_lNormalScore -= lRealUpdate;
	m_lCreditScore += lRealUpdate;

	//更新分数显示
	char szScore[256] = {0};
	snprintf(szScore, sizeof(szScore), "%lld", m_lNormalScore);
	m_pFontNormalScore->setString(szScore);

	snprintf(szScore, sizeof(szScore), "%lld", m_lCreditScore);
	m_pFontCreditScore->setString(szScore);

	//启动额外得分更新
	if (m_lNormalScore <= 0)
	{
		unschedule(schedule_selector(CStar97ResultLayer::normalScoreSchedule));

		if (m_lExtraScore > 0)
		{
			m_pExtraBg->setVisible(true);
			computeUpdateRate(m_lExtraScore, m_lUpdateRate);
			schedule(schedule_selector(CStar97ResultLayer::extraScoreSchedule));
		}
		else
		{
			m_pGameLayer->resultSetOver();
		}
	}
}

void CStar97ResultLayer::extraScoreSchedule(float dt)
{
	CC_ASSERT(m_lUpdateRate > 0);

	//更新分数数值
	LONGLONG lRealUpdate = m_lUpdateRate;
	if (m_lExtraScore - lRealUpdate < 0)
		lRealUpdate = m_lExtraScore;

	m_lExtraScore -= lRealUpdate;
	m_lCreditScore += lRealUpdate;

	//更新分数显示
	setExtraFontScore(m_lExtraScore);

	char szScore[256] = {0};
	snprintf(szScore, sizeof(szScore), "%lld", m_lCreditScore);
	m_pFontCreditScore->setString(szScore);

	//游戏结果更新完毕
	if (m_lExtraScore <= 0)
	{
		unschedule(schedule_selector(CStar97ResultLayer::extraScoreSchedule));
		m_pExtraBg->setVisible(false);

		m_pGameLayer->resultSetOver();
	}
}

void CStar97ResultLayer::computeUpdateRate(const LONGLONG lDestScore, LONGLONG & lRateScore)
{
	CC_ASSERT(lDestScore > 0);
	if (lDestScore <= 0)
	{
		lRateScore = 0;
		return ;
	}

	//设置更新频率分数
	if (g_nMaxScoreUpdateCount >= lDestScore)
	{
		lRateScore = 1;
	}
	else if (g_nMaxScoreUpdateCount * 10 >= lDestScore)
	{
		//至少达到最大次数的1半
		if (lDestScore  > g_nMaxScoreUpdateCount * 5)
			lRateScore = 10;
		else
			lRateScore = lDestScore / g_nMaxScoreUpdateCount;
	}
	else
	{
		lRateScore = lDestScore / g_nMaxScoreUpdateCount;
	}

	return ;
}

void CStar97ResultLayer::setExtraFontScore(LONGLONG lScore)
{
	CC_ASSERT(lScore >= 0);
	if (lScore < 0)
		return ;

	//设置额外奖励各数字
	int nNum[5] = {0};
	computeScoreNum(lScore, nNum, CountArray(nNum));

	bool bSetNull = true;
	for (int i = 0; i < 5; ++i)
	{
		int nShow = nNum[4 - i];
		if (bSetNull)
		{
			if (nShow == 0)
			{
				m_pFontExtraScore[i]->setString("");
			}
			else
			{
				bSetNull = false;
				char szScore[256] = {0};
				snprintf(szScore, sizeof(szScore), "%d", nShow);
				m_pFontExtraScore[i]->setString(szScore);
			}
		}
		else
		{
			char szScore[256] = {0};
			snprintf(szScore, sizeof(szScore), "%d", nShow);
			m_pFontExtraScore[i]->setString(szScore);
		}
	}
}

void CStar97ResultLayer::computeScoreNum(LONGLONG lScore, int nNum[], int nCout)
{
	//参数校验
	CC_ASSERT(nCout > 0);
	CC_ASSERT(lScore >= 0);
	if (nCout <= 0 || lScore < 0)
		return ;

	//范围判断
	memset(nNum, 0, sizeof(nNum[0]) * nCout);
	if (lScore >= std::pow(10.0, nCout))
	{
		CC_ASSERT(false);
		return ;
	}

	int nQuYu = 10;
	int nYuShu = 0;
	for (int i = 0; i < nCout; ++i)
	{
		int nTemp = lScore % nQuYu;
		nNum[i] = (nTemp - nYuShu) * 10 / nQuYu;
		nYuShu = nTemp;
		nQuYu *= 10;
	}

	return ;
}

}
