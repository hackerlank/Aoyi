#include "stdafx.h"
#include "ContinueUpdateLabelAtlas.h"
#include "PullMachResourceDef.h"
#include "PullMachResManager.h"

namespace GamePullMach777
{

CContinueUpdateLabelAtlas * CContinueUpdateLabelAtlas::create(CPMResManager * pResManager, int type, int cfgid)
{
	CGameSpriteCfg* pCfg = pResManager->GetConfigHelper()->GetSpriteConfig(type, cfgid);
	if (pCfg == NULL)
	{
		ERROR_CHECK;
		return NULL;
	}

	float texturewidth = 0;
	float textureheight = 0;
	float nstartchar = 0;

	if (!pCfg->GetCfgParamValue(eSpriteParamType_TextureWidth, texturewidth) ||
		!pCfg->GetCfgParamValue(eSpriteParamType_TextureHeight, textureheight) ||
		!pCfg->GetCfgParamValue(eSpriteParamType_FontMapStartChar, nstartchar))
	{
		ERROR_CHECK;
		return NULL;
	}

	CContinueUpdateLabelAtlas * pLabel = new CContinueUpdateLabelAtlas();
	if (pLabel && pLabel->initWithString("0", pCfg->m_sPath.c_str(), (unsigned int)texturewidth,
		(unsigned int)textureheight, (unsigned int)nstartchar))
	{
		pLabel->autorelease();
		return pLabel;
	}
	else
	{
		CC_SAFE_DELETE(pLabel);
		return NULL;
	}
}

CContinueUpdateLabelAtlas::CContinueUpdateLabelAtlas()
	:m_bUpdating(false)
	,m_lNowScore(0)
	,m_lDestScore(0)
	,m_lEachUpdateScore(0)
	,m_bAddScore(true)
{

}

CContinueUpdateLabelAtlas::~CContinueUpdateLabelAtlas()
{

}

void CContinueUpdateLabelAtlas::setScore(LONGLONG lScore)
{
	if (m_bUpdating)
	{
		unschedule(schedule_selector(CContinueUpdateLabelAtlas::updateScoreSchedule));
		m_bUpdating = false;
	}

	m_lNowScore = lScore;
	m_lDestScore = lScore;
	char szScore[64] = {0};
	snprintf(szScore, sizeof(szScore), "%lld", lScore);
	setString(szScore);
}

void CContinueUpdateLabelAtlas::continueUpdateScore(LONGLONG lShowScore, int nMaxUpdateCount)
{
	//参数校验
	CC_ASSERT(nMaxUpdateCount >= 1);
	if (nMaxUpdateCount < 1)
		nMaxUpdateCount = 1;
	if (lShowScore == m_lDestScore)
		return ;

	//状态校验
	if (m_bUpdating)
	{
		unschedule(schedule_selector(CContinueUpdateLabelAtlas::updateScoreSchedule));
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

	schedule(schedule_selector(CContinueUpdateLabelAtlas::updateScoreSchedule), 0.05f);
}

void CContinueUpdateLabelAtlas::updateScoreSchedule(float dt)
{
	//更新分数
	if (m_bAddScore)
	{
		m_lNowScore += m_lEachUpdateScore;
		if (m_lNowScore >= m_lDestScore)
		{
			m_lNowScore = m_lDestScore;
			m_bUpdating = false;
			unschedule(schedule_selector(CContinueUpdateLabelAtlas::updateScoreSchedule));
		}
	}
	else
	{
		m_lNowScore -= m_lEachUpdateScore;
		if (m_lNowScore <= m_lDestScore)
		{
			m_lNowScore = m_lDestScore;
			m_bUpdating = false;
			unschedule(schedule_selector(CContinueUpdateLabelAtlas::updateScoreSchedule));
		}
	}

	//显示分数
	char szScore[64] = {0};
	snprintf(szScore, sizeof(szScore), "%lld", m_lNowScore);
	setString(szScore);
}

void CContinueUpdateLabelAtlas::setString(const char *label)
{
	CCLabelAtlas::setString(label);
}

}

