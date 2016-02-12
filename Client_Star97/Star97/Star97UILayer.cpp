#include "stdafx.h"
#include "ResourceDef.h"
#include "ResManager.h"
#include "Star97UILayer.h"
#include "Star97Game.h"
#include "Star97GameLayer.h"
#include "LayerBase.h"

namespace GameStar97
{

CStar97UILayer * CStar97UILayer::create(CStar97GameLayer * pGameLayer, CResManager * pResManager)
{
	CStar97UILayer * pRet = new CStar97UILayer(pGameLayer, pResManager);

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

CStar97UILayer::CStar97UILayer(CStar97GameLayer * pGameLayer, CResManager * pResManager)
	:CLayerBase(pResManager)
	,m_pGameLayer(pGameLayer)
	,m_pResManager(pResManager)
	,m_pMenuLayer(NULL)
	,m_pBtnStart(NULL)
	,m_pBtnCreditScore(NULL)
	,m_pBtnLookPrize(NULL)
	,m_pBtnExit(NULL)
	,m_pBtnBetScore(NULL)
	,m_pBtnAutoStart(NULL)
	,m_pBtnCancelAuto(NULL)
	,m_pFontUserScore(NULL)
{

}

CStar97UILayer::~CStar97UILayer()
{

}

bool CStar97UILayer::init()
{
	if (!CCLayer::init())
		return false;

	//初始化按钮
#ifndef ANDROID_GAME
	m_pBtnCancelAuto = createMenuItemSpriteByResID(Star97_Bt_PC_Qu_Xiao_ZD_N, Star97_Bt_PC_Qu_Xiao_ZD_C, Star97_Bt_PC_Qu_Xiao_ZD_D);
	m_pBtnCancelAuto->setPosition(ccp(299, 34));

	m_pBtnAutoStart = createMenuItemSpriteByResID(Star97_Bt_PC_Zi_Dong_N, Star97_Bt_PC_Zi_Dong_C, Star97_Bt_PC_Zi_Dong_D);
	m_pBtnAutoStart->setPosition(ccp(299, 34));

	m_pBtnCreditScore = createMenuItemSpriteByResID(Star97_Bt_PC_Shang_Fen_N, Star97_Bt_PC_Shang_Fen_C, Star97_Bt_PC_Shang_Fen_D);
	m_pBtnCreditScore->setPosition(ccp(378, 34));

	m_pBtnStart = createMenuItemSpriteByResID(Star97_Bt_PC_Start_N, Star97_Bt_PC_Start_C, Star97_Bt_PC_Start_D);
	m_pBtnStart->setPosition(ccp(456, 34));

	m_pBtnBetScore = createMenuItemSpriteByResID(Star97_Bt_PC_Ya_Fen_N, Star97_Bt_PC_Ya_Fen_C, Star97_Bt_PC_Ya_Fen_D);
	m_pBtnBetScore->setPosition(ccp(535, 34));

	m_pBtnLookPrize = createMenuItemSpriteByResID(Star97_Bt_PC_Da_Jiang_N, Star97_Bt_PC_Da_Jiang_C, Star97_Bt_PC_Da_Jiang_D);
	m_pBtnLookPrize->setPosition(ccp(613, 34));
#else
	m_pBtnCreditScore = createMenuItemSpriteByResID(Star97_Bt_PH_Shang_Fen_N, Star97_Bt_PH_Shang_Fen_C, Star97_Bt_PH_Shang_Fen_D);
	m_pBtnCreditScore->setPosition(ccp(144, 523));

	m_pBtnLookPrize = createMenuItemSpriteByResID(Star97_Bt_PH_Da_Jiang_N, Star97_Bt_PH_Da_Jiang_C, Star97_Bt_PH_Da_Jiang_D);
	m_pBtnLookPrize->setPosition(ccp(144, 378));

	m_pBtnExit = createMenuItemSpriteByResID(Star97_Bt_PH_Quit_N, Star97_Bt_PH_Quit_C, Star97_Bt_PH_Quit_D);
	m_pBtnExit->setPosition(ccp(144, 232));

	m_pBtnBetScore = createMenuItemSpriteByResID(Star97_Bt_PH_Ya_Fen_N, Star97_Bt_PH_Ya_Fen_C, Star97_Bt_PH_Ya_Fen_D);
	m_pBtnBetScore->setPosition(ccp(1136, 523));

	m_pBtnAutoStart = createMenuItemSpriteByResID(Star97_Bt_PH_Zi_Dong_N, Star97_Bt_PH_Zi_Dong_C, Star97_Bt_PH_Zi_Dong_D);
	m_pBtnAutoStart->setPosition(ccp(1136, 378));

	m_pBtnCancelAuto = createMenuItemSpriteByResID(Star97_Bt_PH_Qu_Xiao_ZD_N, Star97_Bt_PH_Qu_Xiao_ZD_C, Star97_Bt_PH_Qu_Xiao_ZD_D);
	m_pBtnCancelAuto->setPosition(ccp(1136, 378));

	m_pBtnStart = createMenuItemSpriteByResID(Star97_Bt_PH_Start_N, Star97_Bt_PH_Start_C, Star97_Bt_PH_Start_D);
	m_pBtnStart->setPosition(ccp(1136, 232));

	m_pBtnSetting = createMenuItemSpriteByResID(Star97_Bt_PH_Setting, Star97_Bt_PH_Setting);
	m_pBtnSetting->setPosition(ccp(1220, 677));
#endif

	//设置回调
	m_pBtnAutoStart->setTarget(this, menu_selector(CStar97UILayer::OnTouchAutoStart));
	m_pBtnCancelAuto->setTarget(this, menu_selector(CStar97UILayer::OnTouchCancelAuto));
	m_pBtnCreditScore->setTarget(this, menu_selector(CStar97UILayer::OnTouchCreditScore));
	m_pBtnStart->setTarget(this, menu_selector(CStar97UILayer::OnTouchStart));
	m_pBtnBetScore->setTarget(this, menu_selector(CStar97UILayer::OnTouchBetScore));
	m_pBtnLookPrize->setTarget(this, menu_selector(CStar97UILayer::OnTouchLookPrize));
	if (m_pBtnExit && m_pBtnSetting)
	{
		m_pBtnExit->setTarget(this, menu_selector(CStar97UILayer::OnTouchExit));
		m_pBtnSetting->setTarget(this, menu_selector(CStar97UILayer::OnTouchSetting));
	}
	m_pBtnCancelAuto->setVisible(false);

	//创建按钮
	m_pMenuLayer = CCMenu::create();
	m_pMenuLayer->addChild(m_pBtnAutoStart, 1, GBTag_AutoStart);
	m_pMenuLayer->addChild(m_pBtnCancelAuto, 1, GBTag_CancelAuto);
	m_pMenuLayer->addChild(m_pBtnCreditScore, 1, GBTag_CreditScore);
	m_pMenuLayer->addChild(m_pBtnStart, 1, GBTag_Start);
	m_pMenuLayer->addChild(m_pBtnBetScore, 1, GBTag_BetScore);
	m_pMenuLayer->addChild(m_pBtnLookPrize, 1, GBTag_LookPrize);
	if (m_pBtnExit && m_pBtnSetting)
	{
		m_pMenuLayer->addChild(m_pBtnExit, 1, GBTag_Exit);
		m_pMenuLayer->addChild(m_pBtnSetting, 1, GBTag_Setting);
	}

	m_pMenuLayer->ignoreAnchorPointForPosition(false);
	m_pMenuLayer->setAnchorPoint(CCPointZero);
	m_pMenuLayer->setPosition(CCPointZero);
	this->addChild(m_pMenuLayer);

	//设置其他界面
	if (m_pResManager->GenerateNodeByCfgID(eSpriteType_Cfg, Star97_Font_User_Score, m_pFontUserScore))
	{
		m_pFontUserScore->setAnchorPoint(ccp(0.0f, 0.5f));
		m_pFontUserScore->setPosition(ccp(84, 679));
		m_pFontUserScore->setString("0");
		this->addChild(m_pFontUserScore, 10);

#ifndef ANDROID_GAME
		m_pFontUserScore->setPositionY(-679.0f);
#endif
	}

	//添加房间信息
	CGameDirector* pGameDirector = m_pGameLayer->getGame()->GetGameDirector();
	tagLocalUser* pUserData = pGameDirector->GetMeData();

	if (pGameDirector && pUserData)
	{
#ifndef ANDROID_GAME
		char szTitle[256] = {0};
		string servername =  CStringHelper::UTF8ToGB2312(pGameDirector->m_sMeUTFServerName.c_str());
		snprintf(szTitle , sizeof(szTitle), "%s - [%d]号游戏桌", servername.c_str(), pUserData->nUserTable + 1);
		string szInfo =  CStringHelper::GB2312ToUTF8(szTitle);

		CCLabelTTF * pRoomInfo = CCLabelTTF::create(szInfo.c_str(), "宋体", 16,
			CCSizeMake(200, 50), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
		pRoomInfo->setColor(ccc3(237, 203, 76));
		pRoomInfo->setAnchorPoint(ccp(0.0f, 0.5f));
		pRoomInfo->setPosition(ccp(705, 45));
		this->addChild(pRoomInfo, 20);
#else
		char szTitle[256] = {0};
		std::vector<tagLocalGameServer> & gameServers = pGameDirector->m_vGameServers;
		for (std::vector<tagLocalGameServer>::iterator iter = gameServers.begin(); iter != gameServers.end(); ++iter)
		{
			if (pGameDirector->m_nCurServerID == iter->wServerID)
			{
				snprintf(szTitle , sizeof(szTitle), "%s", iter->sServerName.c_str());
				break;
			}
		}

		CCLabelTTF * pRoomName = CCLabelTTF::create(szTitle, "黑体", 30,
			CCSizeMake(200, 70), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
		pRoomName->setColor(ccc3(237, 203, 76));
		pRoomName->setAnchorPoint(ccp(0.5f, 0.5f));
		pRoomName->setPosition(ccp(140, 50));
		this->addChild(pRoomName, 20);

		char szTableNum[256] = {0};
		snprintf(szTableNum , sizeof(szTableNum), "%d",pUserData->nUserTable + 1);

		CCLabelTTF * pTableNum = CCLabelTTF::create(szTableNum, "黑体", 30, CCSizeZero,
			kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
		pTableNum->setColor(ccc3(237, 203, 76));
		pTableNum->setAnchorPoint(ccp(0.5f, 0.5f));
		pTableNum->setPosition(ccp(1136, 98));
		this->addChild(pTableNum, 20);
#endif
	}

	//设置键盘可用
	//setKeypadEnabled(true);
	setTouchEnabled(true);

	return true;
}

//设置玩家分数
void CStar97UILayer::setUserScore(LONGLONG lUserScore)
{
	if (lUserScore < 0)
	{
		CC_ASSERT(false);
		return ;
	}

	char szScore[256] = {0};
	snprintf(szScore, sizeof(szScore), "%lld", lUserScore);
	m_pFontUserScore->setString(szScore);
}

//设置按钮显示
void CStar97UILayer::setBtnVisible(enGameBtnTag eBtnTag, bool bVisible)
{
	CCMenuItem * pItem = dynamic_cast<CCMenuItem *>(m_pMenuLayer->getChildByTag(eBtnTag));
	CCAssert(pItem != NULL, "this tag button don't have");
	if (pItem->isVisible() != bVisible)
		pItem->setVisible(bVisible);
}

//设置按钮状态
void CStar97UILayer::setBtnEnable(enGameBtnTag eBtnTag, bool bEnabled)
{
	CCMenuItem * pItem = dynamic_cast<CCMenuItem *>(m_pMenuLayer->getChildByTag(eBtnTag));
	CCAssert(pItem != NULL, "this tag button don't have");
	if (eBtnTag == GBTag_BetScore && bEnabled)
	{
		//判断是否为明星局
		if (m_pGameLayer->isBetStarUseful())
			pItem->setEnabled(bEnabled);
		else
			return;
	}
	else if (pItem->isEnabled() != bEnabled)
		pItem->setEnabled(bEnabled);
}

void CStar97UILayer::OnTouchAutoStart(CCObject* pObject)
{
	m_pGameLayer->touchAutoStart();

	m_pBtnAutoStart->setVisible(false);
	m_pBtnCancelAuto->setVisible(true);
	m_pBtnStart->setEnabled(false);
	m_pBtnBetScore->setEnabled(false);
}

void CStar97UILayer::OnTouchCancelAuto(CCObject* pObject)
{
	m_pGameLayer->touchCancelAuto();

	m_pBtnCancelAuto->setVisible(false);
	m_pBtnAutoStart->setVisible(true);
}

void CStar97UILayer::OnTouchCreditScore(CCObject* pObject)
{
	m_pGameLayer->touchCreditScore();
}

void CStar97UILayer::OnTouchStart(CCObject* pObject)
{
	m_pGameLayer->touchStart();
}

void CStar97UILayer::OnTouchBetScore(CCObject* pObject)
{
	m_pGameLayer->touchBetScore();
}

void CStar97UILayer::OnTouchLookPrize(CCObject* pObject)
{
	m_pGameLayer->touchLookPrize();
}

void CStar97UILayer::OnTouchExit(CCObject* pObject)
{
	m_pGameLayer->touchExit();
}

void CStar97UILayer::OnTouchSetting(CCObject* pObject)
{
	m_pGameLayer->touchSetting();
}

void CStar97UILayer::ccOnKeyDown(unsigned int keyCode, unsigned int keyFlag)
{
	//static int times = 0;
#if (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
	if (keyCode == VK_F1)	//F1按下
	{
		if (m_pBtnCreditScore->isVisible() && m_pBtnCreditScore->isEnabled())
		{
			//if ((++times) % 3 == 0)
			{
				OnTouchCreditScore(NULL);
			}

			//if (times > 10000)
			//times -= 10000;
		}
	}
#endif
}

void CStar97UILayer::ccOnKeyUp(unsigned int keyCode, unsigned int keyFlag)
{
#if (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
#define VK_Q		0x51
#define VK_W		0x57
#define VK_E		0x45

	if (keyCode == VK_SPACE)	//空格按下
	{
		if (m_pBtnStart->isVisible() && m_pBtnStart->isEnabled())
		{
			OnTouchStart(NULL);
		}
		else if (m_pGameLayer->GetGameStatus() == GS_WK_PLAYING)
		{
			bool bStop[CELL_NUM] = {0};
			memset(bStop, 1, sizeof(bStop));
			m_pGameLayer->touchIconStop(bStop);
		}
	}
	else if (keyCode == VK_Q)	//Q键按下
	{
		bool bStop[CELL_NUM] = {0};
		bStop[0] = true;
		bStop[3] = true;
		bStop[6] = true;

		m_pGameLayer->touchIconStop(bStop);
	}
	else if (keyCode == VK_W)	//W键按下
	{
		bool bStop[CELL_NUM] = {0};
		bStop[1] = true;
		bStop[4] = true;
		bStop[7] = true;

		m_pGameLayer->touchIconStop(bStop);
	}
	else if (keyCode == VK_E)	//E键按下
	{
		bool bStop[CELL_NUM] = {0};
		bStop[2] = true;
		bStop[5] = true;
		bStop[8] = true;

		m_pGameLayer->touchIconStop(bStop);
	}
	else if (keyCode == VK_F2)	//F2按下
	{
		if (m_pBtnBetScore->isVisible() && m_pBtnBetScore->isEnabled())
		{
			OnTouchBetScore(NULL);
		}
	}
	else if (keyCode == VK_F3)	//F3按下
	{
		if (m_pBtnAutoStart->isVisible() && m_pBtnAutoStart->isEnabled())
		{
			OnTouchAutoStart(NULL);
		}
		else if (m_pBtnCancelAuto->isVisible() && m_pBtnCancelAuto->isEnabled())
		{
			OnTouchCancelAuto(NULL);
		}
	}
#endif
}

}
