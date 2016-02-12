#ifndef _STAR97_UI_LAYER_H_
#define _STAR97_UI_LAYER_H_

#include "LayerBase.h"

namespace GameStar97
{

class CStar97GameLayer;
class CResManager;
class CLayerBase;

//游戏按钮tag
enum enGameBtnTag
{
	GBTag_AutoStart = 10,	//自动开始
	GBTag_CancelAuto ,	//取消自动
	GBTag_CreditScore ,	//上分
	GBTag_Start ,	//开始
	GBTag_BetScore ,	//押注
	GBTag_LookPrize ,	//查看大奖
	GBTag_Exit ,	//退出
	GBTag_Setting,	//设置
};

class CStar97UILayer : public CCLayer, public CLayerBase
{
public:
	static CStar97UILayer * create(CStar97GameLayer * pGameLayer, CResManager * pResManager);

	CStar97UILayer(CStar97GameLayer * pGameLayer, CResManager * pResManager);
	~CStar97UILayer();

	virtual bool init();

public:
	//设置玩家分数
	void setUserScore(LONGLONG lUserScore);
	//设置按钮显示
	void setBtnVisible(enGameBtnTag eBtnTag, bool bVisible);
	//设置按钮状态
	void setBtnEnable(enGameBtnTag eBtnTag, bool bEnable);

	//按钮回调
public:
	void OnTouchAutoStart(CCObject* pObject);
	void OnTouchCancelAuto(CCObject* pObject);
	void OnTouchCreditScore(CCObject* pObject);
	void OnTouchStart(CCObject* pObject);
	void OnTouchBetScore(CCObject* pObject);
	void OnTouchLookPrize(CCObject* pObject);
	void OnTouchExit(CCObject* pObject);
	void OnTouchSetting(CCObject* pObject);

	//键盘按下
	void ccOnKeyDown(unsigned int keyCode, unsigned int keyFlag);
	//键盘抬起
	void ccOnKeyUp(unsigned int keyCode, unsigned int keyFlag);

private:
	CStar97GameLayer * m_pGameLayer;
	CResManager * m_pResManager;

	//按钮相关
	CCMenu * m_pMenuLayer;
	CCMenuItemSprite * m_pBtnAutoStart;	//自动开始按钮
	CCMenuItemSprite * m_pBtnCancelAuto;	//取消自动按钮
	CCMenuItemSprite * m_pBtnCreditScore;	//上分按钮
	CCMenuItemSprite * m_pBtnStart;	//开始按钮
	CCMenuItemSprite * m_pBtnBetScore;	//押分按钮
	CCMenuItemSprite * m_pBtnLookPrize;	//查看大奖按钮
	CCMenuItemSprite * m_pBtnExit;	//退出按钮
	CCMenuItemSprite * m_pBtnSetting;	//设置按钮

	//显示
	CCLabelAtlas * m_pFontUserScore;

};

}

#endif	//_STAR97_UI_LAYER_H_