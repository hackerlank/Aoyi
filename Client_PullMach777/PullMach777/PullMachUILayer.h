#ifndef _PullMach_UI_LAYER_H_
#define _PullMach_UI_LAYER_H_

#include "LayerBase.h"

namespace GamePullMach777
{

class CPullMachGameLayer;
class CPMResManager;
class CLayerBase;
class CPullMachMenu;

//游戏按钮tag
enum enGameBtnTag
{
	GBTag_Start = 10,	//开始
	GBTag_AutoStart,	//自动开始
	GBTag_CancelAuto,	//取消自动
	GBTag_CreditScore,	//上分
	GBTag_BetScore,	//押注
	GBTag_StopLeft,	//停左
	GBTag_StopMid,	//停中
	GBTag_StopRight,	//停右
};

class CPullMachUILayer : public CCLayer, public CLayerBase
{
public:
	static CPullMachUILayer * create(CPullMachGameLayer * pGameLayer, CPMResManager * pResManager);

	CPullMachUILayer(CPullMachGameLayer * pGameLayer, CPMResManager * pResManager);
	~CPullMachUILayer();

	virtual bool init();

public:
	//设置按钮显示
	void setBtnVisible(enGameBtnTag eBtnTag, bool bVisible);
	//设置按钮状态
	void setBtnEnable(enGameBtnTag eBtnTag, bool bEnable);

	//按钮回调
public:
	//开始按钮
	void OnTouchStart(CCObject* pObject);
	//自动开始按钮
	void OnTouchAutoStart(CCObject* pObject);
	//取消自动按钮
	void OnTouchCancelAuto(CCObject* pObject);
	//上分按钮
	void OnTouchCreditScore(CCObject* pObject);
	//押注按钮
	void OnTouchBetScore(CCObject* pObject);
	//左轮停止按钮
	void OnTouchStopLeft(CCObject* pObject);
	//中轮停止按钮
	void OnTouchStopMid(CCObject* pObject);
	//右轮停止按钮
	void OnTouchStopRight(CCObject* pObject);
	//点击退出
	void OnTouchQuit(CCObject *sender, CCControlEvent controlEvent);
	//点击帮助
	void OnTouchHelp(CCObject *sender, CCControlEvent controlEvent);
	//点击银行
	void OnTouchStorage(CCObject *sender, CCControlEvent controlEvent);

	//键盘按下
	void ccOnKeyDown(unsigned int keyCode, unsigned int keyFlag);
	//键盘抬起
	void ccOnKeyUp(unsigned int keyCode, unsigned int keyFlag);

private:
	CPullMachGameLayer * m_pGameLayer;
	CPMResManager * m_pResManager;

	//按钮相关
	CPullMachMenu * m_pMenuLayer;
	CCMenuItemSprite * m_pBtnStart;	//开始按钮
	CCMenuItemSprite * m_pBtnAutoStart;	//自动开始按钮
	CCMenuItemSprite * m_pBtnCancelAuto;	//取消自动
	CCMenuItemSprite * m_pBtnCreditScore;	//上分按钮
	CCMenuItemSprite * m_pBtnBetScore;	//下注按钮
	CCMenuItemSprite * m_pBtnStopLeft;	//停左轮按钮
	CCMenuItemSprite * m_pBtnStopMid;	//停中轮按钮
	CCMenuItemSprite * m_pBtnStopRight;	//停右轮按钮
	CCControlButton * m_pBtnStorage;	//银行按钮
	CCControlButton * m_pBtnQuitGame;	//退出游戏
	CCControlButton * m_pBtnHelp;		//帮助按钮
};

}

#endif	//_PullMach_UI_LAYER_H_
