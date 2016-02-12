#include "stdafx.h"
#include "PullMachPrizeRemainFontNode.h"
#include "PullMachResourceDef.h"
#include "PullMachResManager.h"

namespace GamePullMach777
{

const int CPullMachPrizeRemainFontNode::sm_nFontResId[10] = {
	PullMach_Bg_Specil_Num0, PullMach_Bg_Specil_Num1, PullMach_Bg_Specil_Num2,
	PullMach_Bg_Specil_Num3, PullMach_Bg_Specil_Num4, PullMach_Bg_Specil_Num5,
	PullMach_Bg_Specil_Num6, PullMach_Bg_Specil_Num7, PullMach_Bg_Specil_Num8,
	PullMach_Bg_Specil_Num9,
};

CPullMachPrizeRemainFontNode * CPullMachPrizeRemainFontNode::create(CPMResManager * pResManager)
{
	CPullMachPrizeRemainFontNode * pRet = new CPullMachPrizeRemainFontNode(pResManager);
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

CPullMachPrizeRemainFontNode::CPullMachPrizeRemainFontNode(CPMResManager * pResManager)
	:m_pResManager(pResManager)
	,m_pFontBatchNode(NULL)
	,m_bUpdating(false)
	,m_lNowScore(0)
	,m_lDestScore(0)
	,m_lEachUpdateScore(0)
	,m_bAddScore(false)
{

}

CPullMachPrizeRemainFontNode::~CPullMachPrizeRemainFontNode()
{

}

bool CPullMachPrizeRemainFontNode::init()
{
	//加载分数资源
	std::string sPath = m_pResManager->GetConfigHelper()->m_sLoadingPak+"images/pak/common.pvr.ccz";
	m_pFontBatchNode = CCSpriteBatchNode::create(sPath.c_str(), 5);
	this->addChild(m_pFontBatchNode);

	CCSprite * pSprite = NULL;
	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, sm_nFontResId[0], pSprite))
	{
		pSprite->setAnchorPoint(CCPointZero);
		pSprite->setPosition(CCPointZero);
		m_pFontBatchNode->addChild(pSprite, 0, 1);
	}
	else
	{
		ERROR_CHECK;
		return false;
	}

	return true;
}

void CPullMachPrizeRemainFontNode::setScore(LONGLONG lScore)
{
	//参数校验
	CC_ASSERT(lScore >= 0);
	if (lScore < 0)
		lScore = 0;

	if (m_bUpdating)
	{
		unschedule(schedule_selector(CPullMachPrizeRemainFontNode::updateScoreSchedule));
		m_bUpdating = false;
	}

	m_lNowScore = lScore;
	m_lDestScore = lScore;
	showScore(lScore);
}

void CPullMachPrizeRemainFontNode::continueUpdateScore(LONGLONG lShowScore, int nMaxUpdateCount)
{
	//参数校验
	CC_ASSERT(nMaxUpdateCount >= 1);
	CC_ASSERT(lShowScore >= 0);
	if (nMaxUpdateCount < 1)
		nMaxUpdateCount = 1;
	if (lShowScore < 0)
		lShowScore = 0;
	if (lShowScore == m_lDestScore)
		return ;

	//状态校验
	if (m_bUpdating)
	{
		unschedule(schedule_selector(CPullMachPrizeRemainFontNode::updateScoreSchedule));
		m_lNowScore = m_lDestScore;
	}

	//计算更新参数
	m_lDestScore = lShowScore;
	m_bUpdating = true;
	if (lShowScore > m_lNowScore)
	{
		m_bAddScore = true;
		m_lEachUpdateScore = (LONGLONG)ceil(static_cast<double>(lShowScore - m_lNowScore) / nMaxUpdateCount);
	}
	else
	{
		m_bAddScore = false;
		m_lEachUpdateScore = (LONGLONG)ceil(static_cast<double>(m_lNowScore - lShowScore) / nMaxUpdateCount);
	}

	schedule(schedule_selector(CPullMachPrizeRemainFontNode::updateScoreSchedule), 0.05f);
}

void CPullMachPrizeRemainFontNode::updateScoreSchedule(float dt)
{
	//更新分数
	if (m_bAddScore)
	{
		m_lNowScore += m_lEachUpdateScore;
		if (m_lNowScore >= m_lDestScore)
		{
			m_lNowScore = m_lDestScore;
			m_bUpdating = false;
			unschedule(schedule_selector(CPullMachPrizeRemainFontNode::updateScoreSchedule));
		}
	}
	else
	{
		m_lNowScore -= m_lEachUpdateScore;
		if (m_lNowScore <= m_lDestScore)
		{
			m_lNowScore = m_lDestScore;
			m_bUpdating = false;
			unschedule(schedule_selector(CPullMachPrizeRemainFontNode::updateScoreSchedule));
		}
	}

	//显示分数
	showScore(m_lNowScore);
}

void CPullMachPrizeRemainFontNode::showScore(LONGLONG lScore)
{
	//计算显示分数的各位数字、位数
	int nBitNum[20] = {0};
	int nBitCount = 0;
	computeBitNumOfScore(lScore, nBitNum, nBitCount);
	int nNowBitCount = m_pFontBatchNode->getChildrenCount();	//当前分数位数

	//更新显示
	if (nNowBitCount == nBitCount)
	{
		//位数相同，直接替换纹理
		for (int i = 0; i < nBitCount; ++i)
		{
			m_pResManager->resetSpriteFrameTexture(dynamic_cast<CCSprite *>(m_pFontBatchNode->getChildByTag(i + 1)),
				sm_nFontResId[nBitNum[i]]);
		}
	}
	else if (nNowBitCount > nBitCount)
	{
		//当前位数过多，删除多余的
		for (int i = 0; i < nBitCount; ++i)
		{
			m_pResManager->resetSpriteFrameTexture(dynamic_cast<CCSprite *>(m_pFontBatchNode->getChildByTag(i + 1)),
				sm_nFontResId[nBitNum[i]]);
		}
		for (int i = nBitCount; i < nNowBitCount; ++i)
		{
			m_pFontBatchNode->removeChildByTag(i + 1, true);
		}
	}
	else
	{
		//当位数不足，添加子节点
		for (int i = 0; i < nNowBitCount; ++i)
		{
			m_pResManager->resetSpriteFrameTexture(dynamic_cast<CCSprite *>(m_pFontBatchNode->getChildByTag(i + 1)),
				sm_nFontResId[nBitNum[i]]);
		}
		for (int i = nNowBitCount; i < nBitCount; ++i)
		{
			CCSprite * pSprite = NULL;
			if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, sm_nFontResId[nBitNum[i]], pSprite))
			{
				pSprite->setAnchorPoint(CCPointZero);
				pSprite->setPosition(CCPointZero);
				m_pFontBatchNode->addChild(pSprite, 0, i + 1);
			}
		}
	}

	//调整数字位置
	float fAllWidth = 0.0f;
	for (int i = 0; i < nBitCount; ++i)
	{
		CCNode * pNode = m_pFontBatchNode->getChildByTag(i + 1);
		pNode->setPositionX(fAllWidth);
		fAllWidth += pNode->getContentSize().width;
	}

	return ;
}

void CPullMachPrizeRemainFontNode::computeBitNumOfScore(LONGLONG lScore, int nBitNum[20], int & nBitCount)
{
	//参数校验
	CC_ASSERT(lScore >= 0);
	if (lScore < 0)
		lScore = -lScore;

	//初始化结果值
	memset(nBitNum, 0, sizeof(nBitNum[0]) * 20);
	nBitCount = 0;

	//取得各位对应数字
	if (lScore == 0)
	{
		nBitCount = 1;
		return ;
	}

	LONGLONG lTempScore = lScore;
	while(lTempScore > 0)
	{
		nBitNum[nBitCount++] = lTempScore % 10;
		lTempScore /= 10;
	}

	//转置各位数字
	for (int i = 0, j = nBitCount; i < --j; ++i)
	{
		int nTemp = nBitNum[i];
		nBitNum[i] = nBitNum[j];
		nBitNum[j] = nTemp;
	}

	return ;
}

}
