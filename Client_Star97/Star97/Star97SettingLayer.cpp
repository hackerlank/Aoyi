#include "stdafx.h"
#include "Star97SettingLayer.h"
#include "ResourceDef.h"
#include "ResManager.h"

namespace GameStar97
{

CStar97SettingLayer * CStar97SettingLayer::create(CResManager * pResManager)
{
	CStar97SettingLayer * pRet = new CStar97SettingLayer(pResManager);

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

CStar97SettingLayer::CStar97SettingLayer(CResManager * pResManager)
	:CLayerBase(pResManager)
	,m_pResManager(pResManager)
	,m_pMusicSlider(NULL)
	,m_pEffectSlider(NULL)
	,m_fMusicValue(0.0f)
	,m_fEffectValue(0.0f)
{

}

CStar97SettingLayer::~CStar97SettingLayer()
{

}

bool CStar97SettingLayer::init()
{
	if (!CCLayer::init())
		return false;

	//创建背景界面
	CCLayerColor * pBgLayer = CCLayerColor::create(ccc4(0,0,0,128), WINDOW_WIDTH, CONTENTS_HEIGHT);
	this->addChild(pBgLayer);

	CCSprite * pBgSprite = NULL;
	if (!m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Star97_Set_Setting_Bg, pBgSprite))
	{
		ERROR_CHECK;
		return false;
	}
	pBgSprite->setPosition(ccp(WINDOW_WIDTH / 2, CONTENTS_HEIGHT / 2));
	this->addChild(pBgSprite);
	CCSize bgSize = pBgSprite->getContentSize();

#ifndef ANDROID_GAME
	pBgSprite->setScale(0.8f);
#endif

	//创建界面布局
	CCSprite * pTitle = NULL;
	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Star97_Set_Title, pTitle))
	{
		pTitle->setPosition(ccp(bgSize.width / 2, bgSize.height - 80));
		pBgSprite->addChild(pTitle);
	}

	CCMenuItemSprite * pBtnClose = createMenuItemSpriteByResID(Star97_Set_Close, Star97_Set_Close);
	pBtnClose->setPosition(ccp(bgSize.width - 50, bgSize.height - 50));
	pBtnClose->setTarget(this, menu_selector(CStar97SettingLayer::OnTouchClose));

	CCMenu * pMenuLayer = CCMenu::create(pBtnClose, NULL);
	pMenuLayer->ignoreAnchorPointForPosition(false);
	pMenuLayer->setAnchorPoint(CCPointZero);
	pMenuLayer->setPosition(CCPointZero);
	pBgSprite->addChild(pMenuLayer);

	CCLabelTTF * pFont = CCLabelTTF::create(CStringHelper::GB2312ToUTF8("背景音乐").c_str(), "黑体", 36,
		CCSizeZero, kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
	pFont->setColor(ccc3(230, 150, 15));
	pFont->setAnchorPoint(ccp(0, 0.5f));
	pFont->setPosition(ccp(80, bgSize.height - 250));
	pBgSprite->addChild(pFont);

	pFont = CCLabelTTF::create(CStringHelper::GB2312ToUTF8("游戏音效").c_str(), "黑体", 36,
		CCSizeZero, kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
	pFont->setColor(ccc3(230, 150, 15));
	pFont->setAnchorPoint(ccp(0, 0.5f));
	pFont->setPosition(ccp(80, bgSize.height - 400));
	pBgSprite->addChild(pFont);

	//pFont = CCLabelTTF::create(CStringHelper::GB2312ToUTF8("音量控制").c_str(), "黑体", 36,
	//	CCSizeZero, kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
	//pFont->setColor(ccc3(230, 150, 15));
	//pFont->setAnchorPoint(ccp(0, 0.5f));
	//pFont->setPosition(ccp(80, bgSize.height - 405));
	//pBgSprite->addChild(pFont);

	//创建背景音乐控制
	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Star97_Set_Tiao_Bg, Star97_Set_Tiao, Star97_Set_Dian, m_pMusicSlider))
	{
		m_pMusicSlider->setMinimumValue(0.0f);
		m_pMusicSlider->setMaximumValue(MAX_VOLUME_VALUE);
		m_pMusicSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(CStar97SettingLayer::SliderChangeMusic), CCControlEventValueChanged);

		m_pMusicSlider->setAnchorPoint(ccp(0, 0.5f));
		m_pMusicSlider->setPosition(ccp(280, bgSize.height - 250));
		pBgSprite->addChild(m_pMusicSlider);
		m_pMusicSlider->setValue(MAX_VOLUME_VALUE / 2);
		SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(MAX_VOLUME_VALUE / 2);
	}

	//创建背景音效控制
	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Star97_Set_Tiao_Bg, Star97_Set_Tiao, Star97_Set_Dian, m_pEffectSlider))
	{
		m_pEffectSlider->setMinimumValue(0.0f);
		m_pEffectSlider->setMaximumValue(MAX_VOLUME_VALUE);
		m_pEffectSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(CStar97SettingLayer::SliderChangeEffect), CCControlEventValueChanged);

		m_pEffectSlider->setAnchorPoint(ccp(0, 0.5f));
		m_pEffectSlider->setPosition(ccp(280, bgSize.height - 400));
		pBgSprite->addChild(m_pEffectSlider);
		m_pEffectSlider->setValue(MAX_VOLUME_VALUE / 2);
		SimpleAudioEngine::sharedEngine()->setEffectsVolume(MAX_VOLUME_VALUE / 2);
	}

	////创建背景静音控制
	//CCControlSlider * pSlider = NULL;
	//if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Base, Star97_Set_TiaoS_Bg, Star97_Set_TiaoS, Star97_Set_Dian, pSlider))
	//{
	//	pSlider->setMinimumValue(0.0f);
	//	pSlider->setMaximumValue(1.0f);
	//	pSlider->setValue(1.0f);
	//	pSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(CStar97SettingLayer::SliderChangeQuiet), CCControlEventValueChanged);

	//	pSlider->setAnchorPoint(ccp(0, 0.5f));
	//	pSlider->setPosition(ccp(280, bgSize.height - 405));
	//	pBgSprite->addChild(pSlider);
	//}

	return true;
}

void CStar97SettingLayer::OnTouchClose(CCObject* pObject)
{
	this->setVisible(false);
}

void CStar97SettingLayer::SliderChangeMusic(CCObject *sender, CCControlEvent controlEvent)
{
	CCControlSlider * pSilder = dynamic_cast<CCControlSlider *>(sender);
	m_fMusicValue = pSilder->getValue();
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(m_fMusicValue);
}

void CStar97SettingLayer::SliderChangeEffect(CCObject *sender, CCControlEvent controlEvent)
{
	CCControlSlider * pSilder = dynamic_cast<CCControlSlider *>(sender);
	m_fEffectValue = pSilder->getValue();
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(m_fEffectValue);
}

void CStar97SettingLayer::SliderChangeQuiet(CCObject *sender, CCControlEvent controlEvent)
{
	//CCControlSlider * pSilder = dynamic_cast<CCControlSlider *>(sender);
	//if (pSilder->getValue() > 0.0f)
	//{
	//	pSilder->setValue(1.0f);
	//	m_pMusicSlider->setValue(m_fMusicValue);
	//	m_pEffectSlider->setValue(m_fEffectValue);
	//	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(m_fMusicValue);
	//	SimpleAudioEngine::sharedEngine()->setEffectsVolume(m_fEffectValue);
	//}
	//else
	//{
	//	pSilder->setValue(0.0f);
	//	m_pMusicSlider->setValue(0.0f);
	//	m_pEffectSlider->setValue(0.0f);
	//	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.0f);
	//	SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.0f);
	//}
}

}
