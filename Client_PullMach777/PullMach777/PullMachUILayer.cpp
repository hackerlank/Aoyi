#include "stdafx.h"
#include "PullMachResourceDef.h"
#include "PullMachResManager.h"
#include "PullMachUILayer.h"
#include "PullMachMenu.h"
#include "LayerBase.h"
#include "PullMachGameLayer.h"


namespace GamePullMach777
{

CPullMachUILayer * CPullMachUILayer::create(CPullMachGameLayer * pGameLayer, CPMResManager * pResManager)
{
	CPullMachUILayer * pRet = new CPullMachUILayer(pGameLayer, pResManager);

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

CPullMachUILayer::CPullMachUILayer(CPullMachGameLayer * pGameLayer, CPMResManager * pResManager)
	:CLayerBase(pResManager)
	,m_pGameLayer(pGameLayer)
	,m_pResManager(pResManager)
	,m_pMenuLayer(NULL)
	,m_pBtnStart(NULL)
	,m_pBtnAutoStart(NULL)
	,m_pBtnCancelAuto(NULL)
	,m_pBtnCreditScore(NULL)
	,m_pBtnBetScore(NULL)
	,m_pBtnStopLeft(NULL)
	,m_pBtnStopMid(NULL)
	,m_pBtnStopRight(NULL)
	,m_pBtnStorage(NULL)
	,m_pBtnQuitGame(NULL)
	,m_pBtnHelp(NULL)
{

}

CPullMachUILayer::~CPullMachUILayer()
{

}

bool CPullMachUILayer::init()
{
	if (!CCLayer::init())
		return false;

	CCSize size = this->getContentSize();
	float fMidX = size.width / 2;
	float fTopY = size.height;

	//初始化按钮
	m_pBtnCreditScore = createMenuItemSpriteByResID(PullMach_Bt_Shang_Fen_N, PullMach_Bt_Shang_Fen_C, PullMach_Bt_Shang_Fen_D);
	m_pBtnCreditScore->setPosition(ccp(fMidX - 370, fTopY - 670));

	m_pBtnBetScore = createMenuItemSpriteByResID(PullMach_Bt_Ya_Fen_N, PullMach_Bt_Ya_Fen_C, PullMach_Bt_Ya_Fen_D);
	m_pBtnBetScore->setPosition(ccp(fMidX - 248, fTopY - 670));

	m_pBtnStopLeft = createMenuItemSpriteByResID(PullMach_Bt_Stop_Left_N, PullMach_Bt_Stop_Left_C, PullMach_Bt_Stop_Left_D);
	m_pBtnStopLeft->setPosition(ccp(fMidX - 129, fTopY - 670));

	m_pBtnStopMid = createMenuItemSpriteByResID(PullMach_Bt_Stop_Mid_N, PullMach_Bt_Stop_Mid_C, PullMach_Bt_Stop_Mid_D);
	m_pBtnStopMid->setPosition(ccp(fMidX, fTopY - 670));

	m_pBtnStopRight = createMenuItemSpriteByResID(PullMach_Bt_Stop_Right_N, PullMach_Bt_Stop_Right_C, PullMach_Bt_Stop_Right_D);
	m_pBtnStopRight->setPosition(ccp(fMidX + 128, fTopY - 670));

	m_pBtnAutoStart = createMenuItemSpriteByResID(PullMach_Bt_Zi_Dong_N, PullMach_Bt_Zi_Dong_C, PullMach_Bt_Zi_Dong_D);
	m_pBtnAutoStart->setPosition(ccp(fMidX + 238, fTopY - 670));

	m_pBtnCancelAuto = createMenuItemSpriteByResID(PullMach_Bt_Qu_Xiao_ZD_N, PullMach_Bt_Qu_Xiao_ZD_C, PullMach_Bt_Qu_Xiao_ZD_D);
	m_pBtnCancelAuto->setPosition(ccp(fMidX + 238, fTopY - 670));
	m_pBtnCancelAuto->setVisible(false);

	m_pBtnStart = createMenuItemSpriteByResID(PullMach_Bt_Start_N, PullMach_Bt_Start_C, PullMach_Bt_Start_D);
	m_pBtnStart->setPosition(ccp(fMidX + 360, fTopY - 670));

	//设置回调
	m_pBtnCreditScore->setTarget(this, menu_selector(CPullMachUILayer::OnTouchCreditScore));
	m_pBtnBetScore->setTarget(this, menu_selector(CPullMachUILayer::OnTouchBetScore));
	m_pBtnStopLeft->setTarget(this, menu_selector(CPullMachUILayer::OnTouchStopLeft));
	m_pBtnStopMid->setTarget(this, menu_selector(CPullMachUILayer::OnTouchStopMid));
	m_pBtnStopRight->setTarget(this, menu_selector(CPullMachUILayer::OnTouchStopRight));
	m_pBtnAutoStart->setTarget(this, menu_selector(CPullMachUILayer::OnTouchAutoStart));
	m_pBtnCancelAuto->setTarget(this, menu_selector(CPullMachUILayer::OnTouchCancelAuto));
	m_pBtnStart->setTarget(this, menu_selector(CPullMachUILayer::OnTouchStart));

	//创建按钮
	m_pMenuLayer = CPullMachMenu::create();
	m_pMenuLayer->addChild(m_pBtnCreditScore, 1, GBTag_CreditScore);
	m_pMenuLayer->addChild(m_pBtnBetScore, 1, GBTag_BetScore);
	m_pMenuLayer->addChild(m_pBtnStopLeft, 1, GBTag_StopLeft);
	m_pMenuLayer->addChild(m_pBtnStopMid, 1, GBTag_StopMid);
	m_pMenuLayer->addChild(m_pBtnStopRight, 1, GBTag_StopRight);
	m_pMenuLayer->addChild(m_pBtnAutoStart, 1, GBTag_AutoStart);
	m_pMenuLayer->addChild(m_pBtnCancelAuto, 1, GBTag_CancelAuto);
	m_pMenuLayer->addChild(m_pBtnStart, 1, GBTag_Start);

	m_pMenuLayer->ignoreAnchorPointForPosition(false);
	m_pMenuLayer->setAnchorPoint(CCPointZero);
	m_pMenuLayer->setPosition(CCPointZero);
	//m_pMenuLayer->setTouchPriority(10);
	this->addChild(m_pMenuLayer);

	//创建其他按钮
	m_pBtnHelp = createControlButtonByResID(PullMach_Bt_Help_N, PullMach_Bt_Help_N);
	m_pBtnHelp->addTargetWithActionForControlEvents(this, cccontrol_selector(CPullMachUILayer::OnTouchHelp), CCControlEventTouchUpInside);
	m_pBtnHelp->setPosition(ccp(size.width - 180, fTopY - 60));
	m_pBtnHelp->setAdjustBackgroundImage(false);
	addChild(m_pBtnHelp);

	m_pBtnQuitGame = createControlButtonByResID(PullMach_Bt_Quit_N, PullMach_Bt_Quit_N);
	m_pBtnQuitGame->addTargetWithActionForControlEvents(this, cccontrol_selector(CPullMachUILayer::OnTouchQuit), CCControlEventTouchUpInside);
	m_pBtnQuitGame->setPosition(ccp(size.width - 80, fTopY - 60));
	m_pBtnQuitGame->setAdjustBackgroundImage(false);
	addChild(m_pBtnQuitGame);

	m_pBtnStorage = createControlButtonByResID(PullMach_Bt_Bank_Storage_N, PullMach_Bt_Bank_Storage_N);
	m_pBtnStorage->addTargetWithActionForControlEvents(this, cccontrol_selector(CPullMachUILayer::OnTouchStorage), CCControlEventTouchUpInside);
	m_pBtnStorage->setPosition(ccp(size.width - 100, fTopY - 530));
	m_pBtnStorage->setAdjustBackgroundImage(false);
	addChild(m_pBtnStorage);

	//设置键盘可用
	setTouchEnabled(true);

	return true;
}

//设置按钮显示
void CPullMachUILayer::setBtnVisible(enGameBtnTag eBtnTag, bool bVisible)
{
	CCMenuItem * pItem = dynamic_cast<CCMenuItem *>(m_pMenuLayer->getChildByTag(eBtnTag));
	CCAssert(pItem != NULL, "this tag button don't have");
	if (pItem->isVisible() != bVisible)
		pItem->setVisible(bVisible);
}

//设置按钮状态
void CPullMachUILayer::setBtnEnable(enGameBtnTag eBtnTag, bool bEnabled)
{
	CCMenuItem * pItem = dynamic_cast<CCMenuItem *>(m_pMenuLayer->getChildByTag(eBtnTag));
	CCAssert(pItem != NULL, "this tag button don't have");
	if (pItem->isEnabled() != bEnabled)
		pItem->setEnabled(bEnabled);
}

void CPullMachUILayer::OnTouchStart(CCObject* pObject)
{
	m_pGameLayer->touchStart();
	m_pBtnStart->setEnabled(false);
}

void CPullMachUILayer::OnTouchAutoStart(CCObject* pObject)
{
	m_pGameLayer->touchAutoStart();

	m_pBtnAutoStart->setVisible(false);
	m_pBtnCancelAuto->setVisible(true);
	m_pBtnStart->setEnabled(false);
}

void CPullMachUILayer::OnTouchCancelAuto(CCObject* pObject)
{
	m_pGameLayer->touchCancelAuto();

	m_pBtnCancelAuto->setVisible(false);
	m_pBtnAutoStart->setVisible(true);
}

void CPullMachUILayer::OnTouchCreditScore(CCObject* pObject)
{
	m_pGameLayer->touchCreditScore();
}

void CPullMachUILayer::OnTouchBetScore(CCObject* pObject)
{
	m_pGameLayer->touchBetScore();
}

void CPullMachUILayer::OnTouchStopLeft(CCObject* pObject)
{
	m_pGameLayer->touchStopLeft();
}

void CPullMachUILayer::OnTouchStopMid(CCObject* pObject)
{
	m_pGameLayer->touchStopMid();
}

void CPullMachUILayer::OnTouchStopRight(CCObject* pObject)
{
	m_pGameLayer->touchStopRight();
}

void CPullMachUILayer::OnTouchQuit(CCObject *sender, CCControlEvent controlEvent)
{
	m_pGameLayer->touchExit();
}

void CPullMachUILayer::OnTouchHelp(CCObject *sender, CCControlEvent controlEvent)
{
	m_pGameLayer->touchHelp();
}

void CPullMachUILayer::OnTouchStorage(CCObject *sender, CCControlEvent controlEvent)
{
	//m_pGameLayer->GetGameDirector()->ShowStorage();
}

void CPullMachUILayer::ccOnKeyDown(unsigned int keyCode, unsigned int keyFlag)
{

}

void CPullMachUILayer::ccOnKeyUp(unsigned int keyCode, unsigned int keyFlag)
{
#if (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
	if (keyCode == VK_SPACE)	//空格按下
	{
		m_pGameLayer->allStopRollSchedule(0.0f);
	}
#endif
}

}
