#include "stdafx.h"
#include "PullMachHelpLayer.h"
#include "PullMachResourceDef.h"
#include "PullMachResManager.h"

namespace GamePullMach777
{

CPullMachHelpLayer * CPullMachHelpLayer::create(CPMResManager * pResManager)
{
	CPullMachHelpLayer * pRet = new CPullMachHelpLayer(pResManager);
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

CPullMachHelpLayer::CPullMachHelpLayer(CPMResManager * pResManager)
	:m_pResManager(pResManager)
	,m_pScroll(NULL)
{

}

CPullMachHelpLayer::~CPullMachHelpLayer()
{

}

bool CPullMachHelpLayer::init()
{
	if (!CCLayer::init())
		return false;

	CCSize barDownSize;	//底面边框尺寸
	CCSize barUpSize;	//上面边框尺寸
	CCSize infoBgSize;	//信息背景尺寸
	CCSize infoViewSize;	//信息可视尺寸

	CCSprite * pSprite = NULL;
	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, PullMach_Bg_Help_Bar_Down, pSprite))
	{
		barDownSize = pSprite->getContentSize();
		pSprite->setAnchorPoint(CCPointZero);
		pSprite->setPosition(CCPointZero);
		this->addChild(pSprite);
	}

	{
		float fHeight = this->getContentSize().height - 200;
		infoBgSize.setSize(barDownSize.width, (fHeight > 600) ? 600 : fHeight);
		CCLayerColor * bgLayer = CCLayerColor::create(ccc4(0, 24, 85, 255), infoBgSize.width, infoBgSize.height);
		bgLayer->setPosition(ccp(0, barDownSize.height));
		this->addChild(bgLayer);
	}

	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, PullMach_Bg_Help_Bar_Up, pSprite))
	{
		barUpSize = pSprite->getContentSize();
		pSprite->setAnchorPoint(CCPointZero);
		pSprite->setPosition(ccp(0, barDownSize.height + infoBgSize.height));
		this->addChild(pSprite);
	}

	//添加滚动内容
	infoViewSize.setSize(infoBgSize.width - 20, infoBgSize.height);
	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, PullMach_Bg_Help_Info, pSprite))
	{
		pSprite->setAnchorPoint(ccp(0.5f, 0.0f));
		pSprite->setPosition(ccp(infoViewSize.width / 2, 0));
	}

	CCLayerColor * layerColor = CCLayerColor::create(ccc4(0, 13, 32, 255), infoViewSize.width, pSprite->getContentSize().height);
	layerColor->setPosition(CCPointZero);
	layerColor->addChild(pSprite);

	m_pScroll = CCScrollView::create();
	m_pScroll->setViewSize(infoViewSize);
	m_pScroll->setPosition(ccp(10, barDownSize.height));
	m_pScroll->setDirection(kCCScrollViewDirectionVertical);
	m_pScroll->addChild(layerColor);
	m_pScroll->setContentSize(layerColor->getContentSize());
	m_pScroll->setContentOffset(m_pScroll->minContainerOffset());
	m_pScroll->setTouchPriority(kCCMenuHandlerPriority - 1);
	this->addChild(m_pScroll);

	this->setContentSize(CCSize(infoBgSize.width, barDownSize.height + barUpSize.height + infoBgSize.height));

	//添加关闭按钮
	CCControlButton* pCloseButton = m_pResManager->CreateControlButton(PullMach_Bt_Help_Close_N, PullMach_Bt_Help_Close_C);
	pCloseButton->setAnchorPoint(ccp(1, 1));
	pCloseButton->setPosition(getContentSize().width - 10, getContentSize().height - 4);
	pCloseButton->addTargetWithActionForControlEvents(this, cccontrol_selector(CPullMachHelpLayer::OnTouchClose), CCControlEventTouchUpInside);
	pCloseButton->setScaleX(1.5f);
	this->addChild(pCloseButton);

	return true;
}

void CPullMachHelpLayer::onEnter()
{
	CCLayer::onEnter();
	dynamic_cast<CCTargetedTouchHandler *>(CCDirector::sharedDirector()->
		getTouchDispatcher()->findHandler(m_pScroll))->setSwallowsTouches(true);
}

void CPullMachHelpLayer::setVisible(bool visible)
{
	m_pScroll->setVisible(visible);
	CCLayer::setVisible(visible);
}

void CPullMachHelpLayer::OnTouchClose(CCObject *sender, CCControlEvent controlEvent)
{
	this->setVisible(false);
}

}
