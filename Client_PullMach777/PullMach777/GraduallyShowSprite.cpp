#include "stdafx.h"
#include "GraduallyShowSprite.h"
#include "PullMachResourceDef.h"
#include "PullMachResManager.h"

namespace GamePullMach777
{

CGraduallyShowSprite * CGraduallyShowSprite::create(CPMResManager * pResManager, int type, int cfgid)
{
	CGameSpriteCfg* pCfg = pResManager->GetConfigHelper()->GetSpriteConfig(type, cfgid);

	if (NULL == pCfg)
	{
		CCLog("error generate node by cfg id %d", cfgid);
		return NULL;
	}

	CGraduallyShowSprite * pShowSprite = new CGraduallyShowSprite();
	if (pShowSprite)
	{
		if (pCfg->m_sPath.length() == 0)
		{
			if (pShowSprite->init())
			{
				pShowSprite->autorelease();
				return pShowSprite;
			}
		}
		else if (pCfg->m_bPack)
		{
			if (pShowSprite->initWithSpriteFrameName(pCfg->m_sPath.c_str()))
			{
				pShowSprite->autorelease();
				return pShowSprite;
			}
		}
		else
		{
			if (pShowSprite->initWithFile(pCfg->m_sPath.c_str()))
			{
				pShowSprite->autorelease();
				return pShowSprite;
			}
		}
	}

	CC_SAFE_DELETE(pShowSprite);
	return NULL;
}

CGraduallyShowSprite::CGraduallyShowSprite()
	:m_bShowing(false)
	,m_ShowAllSize()
	,m_fEachAddLength(0.0f)
	,m_bHoriShow(true)
{

}

CGraduallyShowSprite::~CGraduallyShowSprite()
{

}

void CGraduallyShowSprite::graduallyShow(float fUpdateRate, float fEachAddLength, bool bHoriShow /*= true*/)
{
	//判断是否正在显示
	if (m_bShowing)
		return ;
	//参数校验
	CC_ASSERT(fUpdateRate >= 0.0f && fEachAddLength >= 1.0f);
	if (fUpdateRate < 0.0f || fEachAddLength < 1.0f)
		return ;

	//更新变量
	m_bShowing = true;
	m_ShowAllSize = this->getTextureRect().size;
	m_fEachAddLength = fEachAddLength;
	m_bHoriShow = bHoriShow;

	//设置显示
	CCRect rect = this->getTextureRect();
	if (bHoriShow)
	{
		rect.size.width = 0.0f;
	}
	else
	{
		rect.size.height = 0.0f;
	}
	this->setTextureRect(rect, isTextureRectRotated(), rect.size);
	schedule(schedule_selector(CGraduallyShowSprite::showSchedule), fUpdateRate);
}

void CGraduallyShowSprite::showSchedule(float dt)
{
	//计算新的显示矩形
	CCRect newRect = this->getTextureRect();
	bool bStopUpdate = false;

	if (m_bHoriShow)
	{
		newRect.size.width += m_fEachAddLength;
		if (newRect.size.width >= m_ShowAllSize.width)
		{
			newRect.size.width = m_ShowAllSize.width;
			bStopUpdate = true;
		}
	}
	else
	{
		newRect.size.height += m_fEachAddLength;
		if (newRect.size.height >= m_ShowAllSize.height)
		{
			newRect.size.height = m_ShowAllSize.height;
			bStopUpdate = true;
		}
	}

	//更新显示
	this->setTextureRect(newRect, isTextureRectRotated(), newRect.size);
	if (bStopUpdate)
	{
		unschedule(schedule_selector(CGraduallyShowSprite::showSchedule));
		m_bShowing = false;
	}
}

}