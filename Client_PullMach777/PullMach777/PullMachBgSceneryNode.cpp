#include "stdafx.h"
#include "PullMachBgSceneryNode.h"
#include "PullMachResourceDef.h"
#include "PullMachResManager.h"

namespace GamePullMach777
{

static const BYTE g_cbMinOpacity = 100;	//最少可降低的透明度

CPullMachBgSceneryNode * CPullMachBgSceneryNode::create(CPMResManager * pResManager)
{
	CPullMachBgSceneryNode * pRet = new CPullMachBgSceneryNode(pResManager);
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

CPullMachBgSceneryNode::CPullMachBgSceneryNode(CPMResManager * pResManager)
	:m_pResManager(pResManager)
	,m_pBgScenery(NULL)
	,m_enBgType(Scenery_Normal)
	,m_bUpdating(false)
{

}

CPullMachBgSceneryNode::~CPullMachBgSceneryNode()
{

}

bool CPullMachBgSceneryNode::init()
{
	CC_ASSERT(g_cbMinOpacity >= 0);
	//初始化背景图
	m_enBgType = Scenery_Normal;
	if (!m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, getResIdBySceneryType(m_enBgType), m_pBgScenery))
	{
		CCLOG("bg scenery load faild, type:%d", m_enBgType);
		return false;
	}
	m_pBgScenery->setAnchorPoint(CCPointZero);
	m_pBgScenery->setPosition(CCPointZero);
	this->addChild(m_pBgScenery);

	return true;
}

void CPullMachBgSceneryNode::updateBgScenery(enSceneryType enType)
{
	//参数校验
	if (m_enBgType == enType)
		return ;
	if (m_bUpdating)
	{
		directSetBgScenery(enType);
		return ;
	}

	//更新背景
	m_enBgType = enType;
	m_bUpdating = true;
	schedule(schedule_selector(CPullMachBgSceneryNode::reduceOpacitySchedule));
}

void CPullMachBgSceneryNode::directSetBgScenery(enSceneryType enType)
{
	//参数校验
	if (m_enBgType == enType)
		return ;

	m_enBgType = enType;
	if (m_bUpdating)
	{
		unschedule(schedule_selector(CPullMachBgSceneryNode::reduceOpacitySchedule));
		unschedule(schedule_selector(CPullMachBgSceneryNode::increaseOpacitySchedule));
		m_pBgScenery->setOpacity(255);
		m_pResManager->resetSpriteTexture(m_pBgScenery, getResIdBySceneryType(enType));
		m_bUpdating = false;
	}
	else
	{
		m_pResManager->resetSpriteTexture(m_pBgScenery, getResIdBySceneryType(enType));
	}
}

int CPullMachBgSceneryNode::getResIdBySceneryType(enSceneryType enType)
{
	switch (enType)
	{
	case Scenery_Normal:
		return PullMach_Bg_Normal;
	case Scenery_BigPrize:
		return PullMach_Bg_Big_Prize;
	case Scenery_GivePrize:
		return PullMach_Bg_Give_Prize;
	default:
		CC_ASSERT(false);
		return 0;
	}
}

void CPullMachBgSceneryNode::reduceOpacitySchedule(float dt)
{
	BYTE cbOpacity = m_pBgScenery->getOpacity();
	cbOpacity = __max(cbOpacity - 3, g_cbMinOpacity);
	m_pBgScenery->setOpacity(cbOpacity);

	if (cbOpacity <= g_cbMinOpacity)
	{
		unschedule(schedule_selector(CPullMachBgSceneryNode::reduceOpacitySchedule));
		m_pResManager->resetSpriteTexture(m_pBgScenery, getResIdBySceneryType(m_enBgType));
		schedule(schedule_selector(CPullMachBgSceneryNode::increaseOpacitySchedule));
	}
}

void CPullMachBgSceneryNode::increaseOpacitySchedule(float dt)
{
	BYTE cbOpacity = m_pBgScenery->getOpacity();
	cbOpacity = __min(cbOpacity + 3, 255);
	m_pBgScenery->setOpacity(cbOpacity);

	if (cbOpacity >= 255)
	{
		unschedule(schedule_selector(CPullMachBgSceneryNode::increaseOpacitySchedule));
		m_bUpdating = false;
	}
}

}
